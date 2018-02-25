/*
  Created by nullifiedcat on 25.02.18.
*/

#pragma once

#include <stdint.h>

struct cpu_execution_state;

typedef void (*cpu_instruction)(struct cpu_execution_state *state);

void cpu_cat_6_halt(struct cpu_execution_state *state);

void cpu_cat_8_move(struct cpu_execution_state *state);
void cpu_cat_8_load_effective_address(struct cpu_execution_state *state);
void cpu_cat_8_exchange(struct cpu_execution_state *state);
void cpu_cat_8_compare(struct cpu_execution_state *state);
void cpu_cat_8_add(struct cpu_execution_state *state);
void cpu_cat_8_subtract(struct cpu_execution_state *state);
void cpu_cat_8_multiply(struct cpu_execution_state *state);
void cpu_cat_8_divide(struct cpu_execution_state *state);
void cpu_cat_8_negate(struct cpu_execution_state *state);
void cpu_cat_8_increment(struct cpu_execution_state *state);
void cpu_cat_8_decrement(struct cpu_execution_state *state);
void cpu_cat_8_jump(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_equal(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_not_equal(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_greater(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_greater_or_equal(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_less(struct cpu_execution_state *state);
void cpu_cat_8_jump_if_less_or_equal(struct cpu_execution_state *state);
void cpu_cat_8_not(struct cpu_execution_state *state);
void cpu_cat_8_and(struct cpu_execution_state *state);
void cpu_cat_8_or(struct cpu_execution_state *state);
void cpu_cat_8_xor(struct cpu_execution_state *state);
void cpu_cat_8_shl(struct cpu_execution_state *state);
void cpu_cat_8_shr(struct cpu_execution_state *state);

cpu_instruction cat_cpu_instruction_lookup(uint8_t size, uint32_t instruction);