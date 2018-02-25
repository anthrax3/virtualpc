#include "cpu.h"
#include "mem.h"
#include "pc.h"
#include "bus.h"
#include "registers.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <instruction.h>

/*
    read /misc
 */

static uint8_t cpu_addressing_offsets[] = { 0, 4, 4, 1, 1, 5, 4, 0 };

void cpu_init(struct cpu_s *cpu, struct pc_s *pc)
{
    cpu->pc = pc;
    cpu_reset(cpu);
}

void cpu_reset(struct cpu_s *cpu)
{
    memset(&cpu->state, 0, sizeof(struct cpu_state_s));
}

void cpu_start(struct cpu_s *cpu)
{
    if (cpu->implementation == NULL)
        pc_raise_exception(cpu->pc, PCEXCAT_CPU, CPU_ERROR_NO_INSTRUCTION_SET);

    while (!cpu->state.halt)
    {
        cpu_step(cpu);
    }
}

void cpu_step(struct cpu_s *cpu)
{
    if (cpu->state.halt == true)
        return;

    cpu_fetch(cpu, &cpu->state.execution);
    cpu_decode(cpu, &cpu->state.execution);

    if (cpu->state.execution.implementation)
        cpu->state.execution.implementation(&cpu->state.execution);
    else
    {
        pc_raise_exception(cpu->pc, PCEXCAT_CPU, CPU_ERROR_UNKNOWN_INSTRUCTION);
    }
}

void cpu_dump_information(struct cpu_s *cpu)
{
    int i;
    printf("=-= CPU INFORMATION =-=\n");
    printf("Halted: %d\n", cpu->state.halt);
    printf("General Purpose Registers:\n");
    for (i = 0; i < 8; ++i)
    {
        printf("\t$r%c = 0x%08x\n", 'a' + i, cpu->state.regs.rx[i]);
    }
    printf("Special Registers:\n");
    printf("\t$pc = 0x%08x\n", cpu->state.regs.pc);
    printf("\t$sp = 0x%08x\n", cpu->state.regs.sp);
    printf("\t$flags = 0x%08x\n", cpu->state.regs.flags.flags);

    printf("Instruction cache:\n");

    printf("pc = %08x | ", cpu->state.regs.pc);

    for (i = 0; i < cpu->state.execution.instruction_length && i < 16; ++i)
    {
        printf("%02x ", cpu->state.execution.instruction[i]);
    }
    printf("\n");
    printf("operands:\n");
    printf("\t1: as %d: %08x\n", cpu->state.execution.operands[0].type,
           cpu->state.execution.operands[0].value);
    printf("\t2: as %d: %08x\n", cpu->state.execution.operands[1].type,
           cpu->state.execution.operands[1].value);

    printf("=-= END OF CPU INFORMATION =-=\n");
}

void cpu_fetch(struct cpu_s *cpu, struct cpu_execution_state *state)
{
    union instruction_head_s head;
    uint8_t length = 0;

    enum bus_error error;

    if ((error = bus_read(&cpu->pc->bus, cpu->state.regs.pc, 1, &head)) !=
        BER_SUCCESS)
    {
        pc_raise_exception(cpu->pc, PCEXCAT_BUS, error);
    }

    state->head = head;

    /* 6-bit instruction with no operands */
    if (head.size == CPU_WIDTH_VOID)
    {
        state->instruction_length = 1;
        state->instruction_width  = CPU_WIDTH_VOID;
        cpu->state.regs.pc++;
        return;
    }

    length += (1 << head.size);

    uint8_t first_byte;

    if ((error = bus_read(&cpu->pc->bus, cpu->state.regs.pc + 1, 1,
                          &first_byte)) != BER_SUCCESS)
    {
        pc_raise_exception(cpu->pc, PCEXCAT_BUS, error);
    }

    state->instruction_width = (first_byte & 0xc0) >> 6;

    if (state->instruction_width != CPU_WIDTH_VOID)
    {
        if (head.addressing.first != OPERAND_MODE_IMMEDIATE)
            length += cpu_addressing_offsets[head.addressing.first];
        else
            length += (1 << state->instruction_width);

        if (head.addressing.second != 0)
            length += cpu_addressing_offsets[head.addressing.second];
        else
            length += (1 << state->instruction_width);
    }

    if (length > 16)
    {
        pc_raise_exception(cpu->pc, PCEXCAT_CPU,
                           CPU_ERROR_INSTRUCTION_TOO_LONG);
        length = 16;
    }

    state->instruction_length = length;

    bus_read(&cpu->pc->bus, cpu->state.regs.pc + 1, length, state->instruction);
    cpu->state.regs.pc += length;
}

uint32_t *cpu_register(struct cpu_s *cpu, uint32_t name)
{
    const struct register_description_s *reg = register_get(name);

    if (reg == NULL)
        return NULL;

    return (uint32_t *) &cpu->state.regs + reg->offset;
}

/*
    read /misc
 */
struct cpu_operand_s cpu_decode_operand(struct cpu_s *cpu, uint8_t mode,
                                        uint8_t *data)
{
    struct cpu_operand_s result;

    switch (mode)
    {
    case 0:
        result.type  = CPU_OPT_CONSTANT;
        result.value = ((uint32_t *) data)[0];
        break;
    case 1:
        result.type  = CPU_OPT_ADDRESS;
        result.value = ((uint32_t *) data)[0];
        break;
    case 2:
        result.type  = CPU_OPT_ADDRESS;
        result.value = cpu->state.regs.pc + ((uint32_t *) data)[0];
        break;
    case 3:
        result.type  = CPU_OPT_REGISTER;
        result.value = data[0];
        break;
    case 4:
    {
        result.type   = CPU_OPT_ADDRESS;
        uint32_t *reg = cpu_register(cpu, data[0]);
        if (reg)
            result.value = *reg;
        else
        {
            pc_raise_exception(cpu->pc, PCEXCAT_CPU,
                               CPU_ERROR_UNKNOWN_REGISTER);
            result.value = 0xFFFFFFFF;
        }
        break;
    }
    case 5:
    {
        result.type   = CPU_OPT_ADDRESS;
        uint32_t *reg = cpu_register(cpu, data[0]);
        if (reg)
            result.value = *reg + *(uint32_t *) (&data[1]);
        else
        {
            pc_raise_exception(cpu->pc, PCEXCAT_CPU,
                               CPU_ERROR_UNKNOWN_REGISTER);
            result.value = 0xFFFFFFFF;
        }
        break;
    }
    case 6:
    {
        result.type    = CPU_OPT_ADDRESS;
        uint32_t *regA = cpu_register(cpu, data[0]);
        uint32_t *regB = cpu_register(cpu, data[1]);
        if (regA && regB)
            result.value = *regA + *regB * (*(uint16_t *) (&data[2]));
        else
        {
            pc_raise_exception(cpu->pc, PCEXCAT_CPU,
                               CPU_ERROR_UNKNOWN_REGISTER);
            result.value = 0xFFFFFFFF;
        }
        break;
    }
    case 7:
        result.type  = CPU_OPT_NONE;
        result.value = 0x0;
        break;
    default:
        /* why, cpu? */
        pc_raise_exception(cpu->pc, PCEXCAT_CPU, CPU_ERROR_ILLEGAL_STATE);
        break;
    }

    return result;
}

void cpu_decode(struct cpu_s *cpu, struct cpu_execution_state *state)
{
    if (cpu->state.regs.flags.debug)
    {
        printf("pc = %08x | ", cpu->state.regs.pc);
        uint8_t i = 0;
        for (; i < cpu->state.execution.instruction_length && i < 16; ++i)
        {
            printf("%02x ", cpu->state.execution.instruction[i]);
        }
        printf("\n");
    }

    uint8_t i = 0;

    if (cpu->state.execution.head.size != CPU_WIDTH_VOID)
    {
        i += (1 << cpu->state.execution.head.size);
        if (cpu->state.execution.head.addressing.first != OPERAND_MODE_NONE)
        {
            state->operands[0] = cpu_decode_operand(
                cpu, cpu->state.execution.head.addressing.first,
                cpu->state.execution.instruction + i);
            i += cpu_addressing_offsets[cpu->state.execution.head.addressing
                                            .first];

            if (cpu->state.execution.head.addressing.second !=
                OPERAND_MODE_NONE)
                state->operands[1] = cpu_decode_operand(
                    cpu, cpu->state.execution.head.addressing.second,
                    cpu->state.execution.instruction + i);
        }
    }

    state->implementation =
        cpu->implementation(cpu->state.execution.head.size,
                            *(uint32_t *) &cpu->state.execution.instruction[0]);
    state->cpu = cpu;
}
