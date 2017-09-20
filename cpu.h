#ifndef CPU_H
#define CPU_H

#include <stdint.h>

enum instructions
{
    I_NOP = 0,
    
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    
    I_MOV,
    
    I_JMP,
    I_JZ,
    I_JNZ,
    
    I_TEST,
    I_CMP
};

struct cpu_registers_s
{
    uint32_t rx[4]; /* ra, rb, rc, rd */
    uint32_t pc;
    uint32_t sp;
    
    uint32_t flags;
};

struct cpu_s
{
    struct cpu_registers_s regs;
    
    uint8_t halt;
};

void
cpu_init(struct cpu_s *out);

void
cpu_reset(struct cpu_s *out);

void
cpu_step(struct cpu_s *in);

#endif /* CPU_H */