/*
 * instructions_byte.c
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#include "cpu.h"
#include "cpu/cat/instructions_micro.h"
#include "cpu/operand.h"

void cpu_cat_8_move(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);
    operand_write(state, 0, operand_read(state, 1));
}
void cpu_cat_8_load_effective_address(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_ADDRESSABLE(1), CPU_ERROR_ILLEGAL_OPERAND);
    /* Operand 1 is always ADDRESS (asserted above), value is the pointer */
    operand_write(state, 0, OPERAND(1).value);
}
void cpu_cat_8_exchange(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_WRITABLE(1), CPU_ERROR_ILLEGAL_OPERAND);
    uint32_t temp = operand_read(state, 0);
    operand_write(state, 0, operand_read(state, 1));
    operand_write(state, 1, temp);
}
void cpu_cat_8_compare(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);
    /* TODO (signed/unsigned???) */
}
void cpu_cat_8_add(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);
    uint32_t result = operand_read(state, 0) + operand_read(state, 1);
    operand_write(state, 0, result);
}
void cpu_cat_8_subtract(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);
    uint32_t result = operand_read(state, 0) - operand_read(state, 1);
    operand_write(state, 0, result);
}
void cpu_cat_8_multiply(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    uint32_t op0 = operand_read(state, 0);
    uint32_t op1 = operand_read(state, 1);

    int32_t result = *(int32_t *)&op0 * *(int32_t *)&op1;

    operand_write(state, 0, *(uint32_t *)&result);
}
void cpu_cat_8_divide(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    uint32_t op0 = operand_read(state, 0);
    uint32_t op1 = operand_read(state, 1);

    CPU_ASSERT(op1 != 0, CPU_ERROR_DIVIDE_BY_ZERO);

    int32_t result = *(int32_t *)&op0 / *(int32_t *)&op1;

    operand_write(state, 0, *(uint32_t *)&result);
}
void cpu_cat_8_negate(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    uint32_t op = operand_read(state, 0);
    int opn = -*(int *)&op;
    op = *(uint32_t *)&opn;

    operand_write(state, 0, op);
}
void cpu_cat_8_increment(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) + 1);
}
void cpu_cat_8_decrement(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) - 1);
}
void cpu_cat_8_jump(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_equal(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (state->cpu->state.regs.flags.zero)
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_not_equal(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (!state->cpu->state.regs.flags.zero)
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_greater(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (!state->cpu->state.regs.flags.zero && (state->cpu->state.regs.flags.sign == state->cpu->state.regs.flags.overflow))
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_greater_or_equal(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (state->cpu->state.regs.flags.zero || (state->cpu->state.regs.flags.sign == state->cpu->state.regs.flags.overflow))
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_less(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (!state->cpu->state.regs.flags.zero && (state->cpu->state.regs.flags.sign ^ state->cpu->state.regs.flags.overflow))
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_jump_if_less_or_equal(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_READABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    if (state->cpu->state.regs.flags.zero || (state->cpu->state.regs.flags.sign ^ state->cpu->state.regs.flags.overflow))
        state->cpu->state.regs.pc = operand_read(state, 0);
}
void cpu_cat_8_not(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, ~operand_read(state, 0));
}
void cpu_cat_8_and(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) & operand_read(state, 1));
}
void cpu_cat_8_or(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) | operand_read(state, 1));
}
void cpu_cat_8_xor(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) ^ operand_read(state, 1));
}
void cpu_cat_8_shl(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) << operand_read(state, 1));
}
void cpu_cat_8_shr(struct cpu_execution_state *state)
{
    CPU_ASSERT(OPERAND_IS_WRITABLE(0) && OPERAND_IS_READABLE(1), CPU_ERROR_ILLEGAL_OPERAND);

    operand_write(state, 0, operand_read(state, 0) >> operand_read(state, 1));
}
