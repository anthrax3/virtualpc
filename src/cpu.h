#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct cpu_s;

typedef void(*instruction_fn)(struct cpu_s *in);

enum instructions
{
    I_NOP = 0,
    I_RESET,
    I_CPUID,
    
    I_SIZE8,
    I_SIZE16,
    I_SIZE32,

    I_LOAD,     /* LOAD R0, 100  ; Loads address into register */
    I_STORE,    /* STORE 100, R0 ; Stores register into address */

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

enum cpu_flags
{
    CF_ZF
};

struct cpu_registers_s
{
    uint32_t rx[4]; /* ra, rb, rc, rd */
    uint32_t pc;
    uint32_t sp;
    
    uint32_t flags;
};

struct cpu_state_s
{
    struct cpu_registers_s regs;
    uint8_t halt;
};

struct cpu_s
{
    struct cpu_state_s state;
    struct memory_s *mem;
    instruction_fn *iset;
};

void
cpu_init(struct cpu_s *out, struct memory_s *mem);

void
cpu_run(struct cpu_s *in);

void
cpu_reset(struct cpu_s *out);

void
cpu_step(struct cpu_s *in);

#endif /* CPU_H */
