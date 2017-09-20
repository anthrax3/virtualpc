#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct cpu_registers_s
{
    uint32_t rx[4]; /* ra, rb, rc, rd */
    uint32_t pc;
    
    uint32_t flags;
};

struct cpu_s
{
    struct cpu_registers_s regs;
    
    uint8_t halt;
};

#endif /* CPU_H */