/*
 * instructions_micro.h
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#pragma once

/* 6-bit instructions */
enum cpu_cat_instruction_micro
{
    CPU_CAT_6_HALT,
    CPU_CAT_6_COUNT
};

void cpu_cat_6_halt(struct cpu_s *cpu, struct cpu_execution_state *state);
