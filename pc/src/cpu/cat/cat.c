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

cpu_instruction cat_instructions_micro[] = { NULL };
cpu_instruction cat_instructions_byte[]  = { NULL };
cpu_instruction cat_instructions_word[]  = { NULL };
cpu_instruction cat_instructions_dword[] = { NULL };

cpu_instruction cat_cpu_instruction_lookup(uint8_t size, uint32_t instruction)
{
    return NULL;
}
