#pragma once

#include <stdint.h>
#include <stdbool.h>

struct cpu_s;
struct cpu_execution_state;

typedef void(*cpu_instruction)(struct cpu_s *cpu, struct cpu_execution_state *state);
typedef cpu_instruction(*cpu_instruction_lookup)(uint8_t size, uint32_t instruction);

enum cpu_flags
{
    CF_ZF,
    CF_JF,
    CF_DEBUGF,
};

struct cpu_registers_s
{
    uint32_t rx[8]; /* r[abcdefgh] */
    uint32_t pc;
    uint32_t sp;
    uint32_t flags;
};

enum cpu_operand_type
{
    CPU_OPT_REGISTER,
    CPU_OPT_ADDRESS,
    CPU_OPT_CONSTANT
};

struct cpu_operand_s
{
    enum cpu_operand_type type;
    uint32_t value;
};

struct cpu_execution_state
{
    uint8_t instruction[16];
    struct cpu_operand_s operands[2];
    cpu_instruction execute;
};

struct cpu_state_s
{
    struct cpu_registers_s regs;
    struct cpu_execution_state execution;
    bool halt;
};

struct cpu_s
{
    struct pc_s *pc;
    cpu_instruction_lookup implementation;
    struct cpu_state_s state;
};

void cpu_init(struct cpu_s *cpu, struct pc_s *pc);

void cpu_start(struct cpu_s *cpu);

void cpu_reset(struct cpu_s *cpu);

void cpu_step(struct cpu_s *cpu);

void cpu_dump_information(struct cpu_s *cpu);

void cpu_flag_set(struct cpu_s *cpu, enum cpu_flags flag);

void cpu_flag_unset(struct cpu_s *cpu, enum cpu_flags flag);

bool cpu_flag_isset(struct cpu_s *cpu, enum cpu_flags flag);

uint8_t cpu_instruction_length(uint8_t head);

uint32_t *cpu_register(struct cpu_s *cpu, uint8_t name);

void cpu_fetch(struct cpu_s *cpu, struct cpu_execution_state *state);
void cpu_decode(struct cpu_s *cpu, struct cpu_execution_state *state);
