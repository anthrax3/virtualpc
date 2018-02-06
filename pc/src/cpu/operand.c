/*
 * operand.c
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#include "cpu/operand.h"

uint32_t operand_size_mask[] = { 0x000000FF, 0x0000FFFF, 0xFFFFFFFF, 0x0 };

uint32_t operand_read(struct cpu_execution_state *state, uint8_t index)
{
    struct cpu_operand_s operand = state->operands[index];

    switch (operand.type)
    {
    case CPU_OPT_ADDRESS:
    {
        uint32_t temp = 0;
        bus_read(&state->cpu->pc->bus, operand.value, (1 << state->instruction_size), &temp);
        return temp;
    }
    case CPU_OPT_CONSTANT:
        return operand.value;
    case CPU_OPT_REGISTER:
    {
        uint32_t *reg = cpu_register(state->cpu, operand.value);
        if (reg)
            return *reg & operand_size_mask[state->instruction_size];
        else /* ERROR */
            return 0;
    }
    case CPU_OPT_NONE:
        /* ERROR */
        break;
    }
    /* ERROR */
    return 0;
}

void operand_write(struct cpu_execution_state *state, uint8_t index, uint32_t value)
{
    struct cpu_operand_s operand = state->operands[index];

    switch (operand.type)
    {
    case CPU_OPT_ADDRESS:
        bus_write(&state->cpu->pc->bus, operand.value, (1 << state->instruction_size), &value);
        return;
    case CPU_OPT_REGISTER:
    {
        uint32_t *reg = cpu_register(state->cpu, operand.value);
        if (reg) /* else error! */
            *reg = value;
        break;
    }
    default:
        /* ERROR */
        break;
    }
}
