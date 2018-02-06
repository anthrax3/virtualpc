/*
 * instructions_micro.c
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#include "cpu.h"
#include "cpu/cat/instructions_micro.h"

void cpu_cat_6_halt(struct cpu_execution_state *state)
{
    state->cpu->state.halt = true;
}
