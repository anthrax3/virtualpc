#include "cpu.h"
#include "mem.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void
cpu_init(struct cpu_s *out, struct memory_s *mem)
{
    cpu_reset(out);
    out->iset = calloc(256, sizeof(instruction_fn));
    out->mem = mem;
}

void
cpu_reset(struct cpu_s *out)
{
    memset(&out->state, 0, sizeof(struct cpu_state_s));
}

void
cpu_run(struct cpu_s *in)
{
    while (!in->state.halt)
    {
        cpu_step(in);
        if (in->state.regs.flags & CF_JF)
        {
            in->state.regs.flags &= ~CF_JF;
        }
        else
        {
            ++in->state.regs.pc;
        }
    }
}

void
cpu_step(struct cpu_s *in)
{
    if (in->state.halt != 0)
    {
        return;
    }
    uint8_t instr = cpu_mem_read_remote_byte(in, in->state.regs.pc);
    if (cpu_flag_isset(in, CF_DEBUGF))
        printf("pc=%08x\t[pc]=%02x\n", in->state.regs.pc, instr);
    if (in->iset[instr] != NULL)
    {
        in->iset[instr](in);
    }
    else
    {
        cpu_raise_exception(in, CPUE_CANNOT_EXECUTE);
    }
}

void
cpu_raise_exception(struct cpu_s *in, enum cpu_exception ex)
{
    printf("[CPU] Exception: %d\n", ex);
    in->state.halt = 1;
    cpu_dump_information(in);
}

void
cpu_dump_information(struct cpu_s *in)
{
    int i;
    printf("=-= CPU INFORMATION =-=\n");
    printf("Halted: %d\n", in->state.halt);
    printf("General Purpose Registers:\n");
    for (i = 0; i < 4; ++i)
    {
        printf("\t$r%d = 0x%08x\n", i, in->state.regs.rx[i]);
    }
    printf("Special Registers:\n");
    printf("\t$pc = 0x%08x\n", in->state.regs.pc);
    printf("\t$sp = 0x%08x\n", in->state.regs.sp);
    printf("\t$flags = 0x%08x\n", in->state.regs.flags);
    if (cpu_mem_is_readable(in, in->state.regs.pc, 1))
    {

        printf("Instruction at [pc]: %02x\n", cpu_mem_read_remote_byte(in, in->state.regs.pc));
    }
    else
    {
        printf("Can't access instruction at [pc]");
    }
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
