/*
 * cat.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "iset/instructions.h"
#include "iset/micro.h"
#include "cpu/implcat.h"

#include <stdlib.h>

cpu_instruction cat_instructions_micro[ISET_CAT_MICRO_COUNT] = {
    cpu_cat_6_halt
};
cpu_instruction cat_instructions[ISET_CAT_COUNT] = {
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
    cpu_cat_8_shr
};

cpu_instruction cat_cpu_instruction_lookup(uint8_t size, uint32_t instruction)
{
    if (size > 3)
        return NULL;

    if (size == 3)
    {
        instruction &= 0x3f;
        if (instruction >= ISET_CAT_MICRO_COUNT)
            return NULL;

        return cat_instructions_micro[instruction];
    }
    else
    {
        if (instruction >= ISET_CAT_COUNT)
            return NULL;

        return cat_instructions[instruction];
    }
}
