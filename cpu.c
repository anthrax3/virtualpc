#include "cpu.h"

#include <string.h>

void
cpu_init(struct cpu_s *out)
{
    cpu_reset(out);
}

void
cpu_reset(struct cpu_s *out)
{
    memset(out, 0, sizeof(struct cpu_s));
}

void
cpu_step(struct cpu_s *in)
{
    
}