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
    
    I_SHR,
    I_SHL,
    
    I_NOT,
    I_AND,
    I_OR,
    I_XOR,
    
    I_MOV,
    
    I_JMP,
    I_JZ,
    I_JNZ,
    
    I_TEST,
    I_CMP,
    
    I_PUSH,
    I_POP
};

enum instr_opt
{
    IO_REG_0,
    IO_REG_1,
    IO_REG_2,
    IO_REG_3,
    
    IO_IMM,
    IO_MEM
};

struct instr_opt_s
{
    enum instr_opt op1 : 4;
    enum instr_opt op2 : 4;
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