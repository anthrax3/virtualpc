/*
 * instructions_byte.h
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#pragma once

/* 8-bit instructions */
enum cpu_cat_instruction_byte
{
    /* Basic operations */
    CPU_CAT_8_MOVE,
    CPU_CAT_8_EXCHANGE,
    /* Logic? */
    CPU_CAT_8_COMPARE,
    /* Arithmetic */
    CPU_CAT_8_ADD,
    CPU_CAT_8_SUBTRACT,
    CPU_CAT_8_MULTIPLY,
    CPU_CAT_8_DIVIDE,
    CPU_CAT_8_NEGATE,
    CPU_CAT_8_INCREMENT,
    CPU_CAT_8_DECREMENT,
    /* Jumps */
    CPU_CAT_8_JUMP,
    CPU_CAT_8_JUMP_IF_EQUAL,
    CPU_CAT_8_JUMP_IF_NOT_EQUAL,
    CPU_CAT_8_JUMP_IF_GREATER,
    CPU_CAT_8_JUMP_IF_GREATER_OR_EQUAL,
    CPU_CAT_8_JUMP_IF_LESS,
    CPU_CAT_8_JUMP_IF_LESS_OR_EQUAL,
    /* Bit logic operators */
    CPU_CAT_8_NOT,
    CPU_CAT_8_AND,
    CPU_CAT_8_OR,
    CPU_CAT_8_XOR,
    /* Bit shifting */
    CPU_CAT_8_SHL,
    CPU_CAT_8_SHR,
    /* Amount of instructions (must be less than 64!) */
    CPU_CAT_8_COUNT
};

void cpu_cat_8_move(struct cpu_execution_state *state);
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
