#pragma once

#include <stdint.h>
#include <stdbool.h>

struct cpu_s;

typedef void(*instruction_fn)(struct cpu_s *in);

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

struct cpu_state_s
{
    struct cpu_registers_s regs;
    uint8_t instruction[16];
    bool halt;
};

struct cpu_s
{
	struct pc_s *pc;
    struct cpu_state_s state;
};

void
cpu_init(struct cpu_s *cpu, struct pc_s *pc);

void
cpu_start(struct cpu_s *cpu);

void
cpu_reset(struct cpu_s *cpu);

void
cpu_step(struct cpu_s *cpu);

void
cpu_dump_information(struct cpu_s *cpu);

void
cpu_flag_set(struct cpu_s *cpu, enum cpu_flags flag);

void
cpu_flag_unset(struct cpu_s *cpu, enum cpu_flags flag);

bool
cpu_flag_isset(struct cpu_s *cpu, enum cpu_flags flag);

uint8_t
cpu_instruction_length(uint8_t head);
