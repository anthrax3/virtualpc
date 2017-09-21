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
        ++in->state.regs.pc;
    }
}

void
cpu_step(struct cpu_s *in)
{
    if (in->state.halt != 0)
    {
        return;
    }
    uint8_t instr = memory_read(in->mem, in->state.regs.pc);
    if (in->iset[instr] != NULL)
    {
        in->iset[instr](in);
    }
    else
    {
        printf("ERROR: Undefined instruction: %hhx\n", instr);
        in->state.halt = 1;
    }
}
