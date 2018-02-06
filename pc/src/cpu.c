#include "cpu.h"
#include "mem.h"
#include "pc.h"
#include "bus.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    while (!cpu->state.halt)
    {
        cpu_step(cpu);
        if (cpu->state.regs.flags & CF_JF)
        {
            cpu->state.regs.flags &= ~CF_JF;
        }
        else
        {
            ++cpu->state.regs.pc;
        }
    }
}

void cpu_step(struct cpu_s *cpu)
{
    if (cpu->state.halt == true)
        return;

    cpu_fetch(cpu, &cpu->state.execution);
    cpu_decode(cpu, &cpu->state.execution);

    if (cpu->state.execution.implementation)
        cpu->state.execution.implementation(cpu, &cpu->state.execution);
    else
    {
        /* TODO error... */
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
    printf("\t$flags = 0x%08x\n", cpu->state.regs.flags);

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

void cpu_flag_set(struct cpu_s *in, enum cpu_flags flag)
{
    in->state.regs.flags |= flag;
}

void cpu_flag_unset(struct cpu_s *in, enum cpu_flags flag)
{
    in->state.regs.flags &= ~flag;
}

bool cpu_flag_isset(struct cpu_s *in, enum cpu_flags flag)
{
    return !!(in->state.regs.flags & flag);
}

void cpu_fetch(struct cpu_s *cpu, struct cpu_execution_state *state)
{
    uint8_t head;
    uint8_t length = 1;

    if (bus_read(&cpu->pc->bus, cpu->state.regs.pc, 1, &head) != BER_SUCCESS)
    {
        /* TODO ... */
    }

    uint8_t size = ((head & 0xc0) >> 6) & 0x3;

    /* 6-bit instruction with no operands */
    if (size == 3)
    {
        state->instruction[0]     = head;
        state->instruction_length = 1;
        state->instruction_size   = 3;
        cpu->state.regs.pc++;
        return;
    }

    uint8_t addr1 = ((head & 0x38) >> 3) & 0x7;
    uint8_t addr2 = (head & 0x07);

    length += (1 << size);

    uint32_t first_byte;

    if (bus_read(&cpu->pc->bus, cpu->state.regs.pc + 1, 1, &first_byte) !=
        BER_SUCCESS)
    {
        /* TODO ... */
    }

    state->instruction_size = (first_byte & 0xc0) >> 6;

    if (state->instruction_size != 3)
    {
        if (addr1 != 0)
            length += cpu_addressing_offsets[addr1];
        else
            length += (1 << state->instruction_size);

        if (addr2 != 0)
            length += cpu_addressing_offsets[addr2];
        else
            length += (1 << state->instruction_size);
    }

    if (length > 16)
    {
        /* TODO */
        length = 16;
    }

    state->instruction_length = length;

    bus_read(&cpu->pc->bus, cpu->state.regs.pc, length, state->instruction);
    cpu->state.regs.pc += length;
}

uint32_t *cpu_register(struct cpu_s *cpu, uint8_t name)
{
    if (name < 8)
        return &cpu->state.regs.rx[name];
    if (name == 9)
        return &cpu->state.regs.sp;

    return NULL;
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
            /* TODO error */
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
            /* TODO error */
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
            /* TODO error */
            result.value = 0xFFFFFFFF;
        }
        break;
    }
    case 7:
        break;
    default:
        break;
        /* TODO error */
    }

    return result;
}

void cpu_decode(struct cpu_s *cpu, struct cpu_execution_state *state)
{
    if (cpu_flag_isset(cpu, CF_DEBUGF))
    {
        printf("pc = %08x | ", cpu->state.regs.pc);
        uint8_t i = 0;
        for (; i < cpu->state.execution.instruction_length && i < 16; ++i)
        {
            printf("%02x ", cpu->state.execution.instruction[i]);
        }
        printf("\n");
    }

    uint8_t head = cpu->state.execution.instruction[0];

    uint8_t size  = ((head & 0xc0) >> 6) & 0x3;
    uint8_t addr1 = ((head & 0x38) >> 3) & 0x7;
    uint8_t addr2 = (head & 0x07);

    uint8_t i = 2;

    if (size != 3)
    {
        if (addr1 != 7)
        {
            state->operands[0] = cpu_decode_operand(
                cpu, addr1, cpu->state.execution.instruction + i);
            i += cpu_addressing_offsets[addr1];

            if (addr2 != 7)
                state->operands[1] = cpu_decode_operand(
                    cpu, addr2, cpu->state.execution.instruction + i);
        }
    }

    state->implementation = cpu->implementation(
        size, *(uint32_t *) (&cpu->state.execution.instruction[1]));
}
