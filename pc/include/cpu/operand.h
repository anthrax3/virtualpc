/*
 * operand.h
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#pragma once

/*
 * This file must ONLY be included by instruction set implementation files!
 */

#include "cpu.h"
#include "pc.h"

#define OPERAND(index) state->operands[index]
#define OPERAND_IS_WRITABLE(i) (OPERAND(i).type != CPU_OPT_CONSTANT)
#define OPERAND_IS_ADDRESSABLE(i) (OPERAND(i).type == CPU_OPT_ADDRESS)
#define OPERAND_IS_READABLE(i) (OPERAND(i).type != CPU_OPT_NONE)
#define CPU_ASSERT(expression, error)                                          \
    if (!(expression))                                                         \
    {                                                                          \
        pc_raise_exception(state->cpu->pc, PCEXCAT_CPU, error);                \
        return;                                                                \
    }

uint32_t operand_read(struct cpu_execution_state *state, uint8_t index);
void operand_write(struct cpu_execution_state *state, uint8_t index,
                   uint32_t value);
