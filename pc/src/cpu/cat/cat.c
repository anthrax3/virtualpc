/*
 * cat.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "cpu/cat/cat.h"
#include "cpu/cat/instructions_micro.h"
#include "cpu/cat/instructions_byte.h"
#include "cpu/cat/instructions_word.h"
#include "cpu/cat/instructions_dword.h"

#include <stdlib.h>

cpu_instruction cat_instructions_micro[CPU_CAT_6_COUNT]  = { cpu_cat_6_halt };
cpu_instruction cat_instructions_byte[CPU_CAT_8_COUNT]   = {
        cpu_cat_8_move,
        cpu_cat_8_exchange,
        cpu_cat_8_compare,
        cpu_cat_8_add,
        cpu_cat_8_subtract,
        cpu_cat_8_multiply,
        cpu_cat_8_divide,
        cpu_cat_8_negate,
        cpu_cat_8_increment,
        cpu_cat_8_decrement,
        cpu_cat_8_jump,
        cpu_cat_8_jump_if_equal,
        cpu_cat_8_jump_if_not_equal,
        cpu_cat_8_jump_if_greater,
        cpu_cat_8_jump_if_greater_or_equal,
        cpu_cat_8_jump_if_less,
        cpu_cat_8_jump_if_less_or_equal,
        cpu_cat_8_not,
        cpu_cat_8_and,
        cpu_cat_8_or,
        cpu_cat_8_xor,
        cpu_cat_8_shl,
        cpu_cat_8_shr };

cpu_instruction cat_instructions_word[CPU_CAT_16_COUNT]  = { };
cpu_instruction cat_instructions_dword[CPU_CAT_32_COUNT] = { };

cpu_instruction *cat_instruction_sets[] = {
        cat_instructions_byte,
        cat_instructions_word,
        cat_instructions_dword,
        cat_instructions_micro
};

uint32_t cat_instruction_count[] = {
        CPU_CAT_8_COUNT,
        CPU_CAT_16_COUNT,
        CPU_CAT_32_COUNT,
        CPU_CAT_6_COUNT
};

cpu_instruction cat_cpu_instruction_lookup(uint8_t size, uint32_t instruction)
{
    if (size > 3)
        return NULL;

    if (size == 3)
        instruction &= 0x3f;

    if (instruction >= cat_instruction_count[size])
        return NULL;

    return cat_instruction_sets[size][instruction];
}
