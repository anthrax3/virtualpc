#include "cpu.h"
#include "mem.h"
#include "pc.h"
#include "bus.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
0 - 1 - byte - &register
1 - 1 - byte - &(*register)
2 - 5 - byte + dword - &(*register + offset:dword)
3 - 6 - byte + byte + dword - &(*register * factor:byte + offset:dword)
4 - 7 - byte + byte + byte + dword - &(*register * factor:byte + *register + offset:dword)
5 - 4 - dword - &absolute
6 - 4 - dword - &(*absolute)
7 - 4 - dword - &(pc + offset:dword)
 */

static uint8_t cpu_addressing_offsets[] = {
	1, 1, 5, 6,
	7, 4, 4, 4
};

void cpu_fetch(struct cpu_s *cpu);

void
cpu_init(struct cpu_s *cpu, struct pc_s *pc)
{
    cpu->pc = pc;
    cpu_reset(cpu);
}

void
cpu_reset(struct cpu_s *cpu)
{
    memset(&cpu->state, 0, sizeof(struct cpu_state_s));
}

void
cpu_start(struct cpu_s *cpu)
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

void
cpu_step(struct cpu_s *cpu)
{
    if (cpu->state.halt == true)
        return;

    cpu_fetch(cpu);

    if (cpu_flag_isset(cpu, CF_DEBUGF))
    {
    	printf("pc = %08x | ", cpu->state.regs.pc);
    	uint8_t i = 0;
    	uint8_t l = cpu_instruction_length(cpu->state.instruction[0]);
    	for (; i < l && i < 16; ++i)
    	{
    		printf("%02x ", cpu->state.instruction[0]);
    	}
    	printf("\n");
    }

}

void
cpu_dump_information(struct cpu_s *cpu)
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
	uint8_t length = cpu_instruction_length(cpu->state.instruction[0]);
	for (i = 0; i < length && i < 16; ++i)
	{
		printf("%02x ", cpu->state.instruction[0]);
	}
	printf("\n");

    printf("=-= END OF CPU INFORMATION =-=\n");
}

void
cpu_flag_set(struct cpu_s *in, enum cpu_flags flag)
{
    in->state.regs.flags |= flag;
}

void
cpu_flag_unset(struct cpu_s *in, enum cpu_flags flag)
{
    in->state.regs.flags &= ~flag;
}

bool
cpu_flag_isset(struct cpu_s *in, enum cpu_flags flag)
{
    return !!(in->state.regs.flags & flag);
}

uint8_t
cpu_instruction_length(uint8_t head)
{
	uint8_t size = ((head & 0xc0) >> 6) & 0x3;
	uint8_t addr1 = ((head & 0x38) >> 3) & 0x7;
	uint8_t addr2 = (head & 0x07);
	uint8_t final_length = 1;

	if (size == 3)
		return 0;

	final_length += (1 << size);
	final_length += cpu_addressing_offsets[addr1];
	final_length += cpu_addressing_offsets[addr2];

	return final_length;
}

void cpu_fetch(struct cpu_s *cpu)
{
	uint8_t head;
	uint8_t length;

	if (bus_read(&cpu->pc->bus, cpu->state.regs.pc, 1, &head) != BER_SUCCESS)
	{
		/* TODO ... */
	}

	length = cpu_instruction_length(head);

	/* Short instruction with no operands */
	if (length == 0)
	{
		cpu->state.instruction[0] = head;
		cpu->state.regs.pc++;
		return;
	}

	bus_read(&cpu->pc->bus, cpu->state.regs.pc, length, cpu->state.instruction);
	cpu->state.regs.pc += length;
}
