/*
  Created by nullifiedcat on 25.02.18.
*/

#include "mnemonics.h"

#include "iset/instructions.h"
#include "iset/micro.h"

#include <string.h>
#include <instruction.h>

const char *imm_micro[] = {[ISET_CAT_MICRO_HALT] = "halt" };

const char *imm_normal[] = {
        /* Basic operations */
        [ISET_CAT_MOVE] = "mov", [ISET_CAT_LOAD_EFFECTIVE_ADDRESS] = "lea",
        [ISET_CAT_EXCHANGE] = "xchg",

        /* Logic? */
        [ISET_CAT_COMPARE] = "cmp",

        /* Arithmetic */
        [ISET_CAT_ADD] = "add", [ISET_CAT_SUBTRACT] = "sub",
        [ISET_CAT_MULTIPLY] = "mul", [ISET_CAT_DIVIDE] = "div",
        [ISET_CAT_NEGATE] = "neg", [ISET_CAT_INCREMENT] = "inc",
        [ISET_CAT_DECREMENT] = "dec",

        /* Jumps */
        [ISET_CAT_JUMP] = "jmp", [ISET_CAT_JUMP_IF_EQUAL] = "je",
        [ISET_CAT_JUMP_IF_NOT_EQUAL] = "jne", [ISET_CAT_JUMP_IF_GREATER] = "jg",
        [ISET_CAT_JUMP_IF_GREATER_OR_EQUAL] = "jge",
        [ISET_CAT_JUMP_IF_LESS]             = "jl",
        [ISET_CAT_JUMP_IF_LESS_OR_EQUAL]    = "jle",

        /* Bit logic operators */
        [ISET_CAT_NOT] = "not", [ISET_CAT_AND] = "and", [ISET_CAT_OR] = "or",
        [ISET_CAT_XOR] = "xor",

        /* Bit shifting */
        [ISET_CAT_SHL] = "shl", [ISET_CAT_SHR] = "shr"
};

int instruction_from_mnemonic(const char *mnemonic, uint32_t *out,
                              uint32_t *size)
{
    size_t i;
    for (i = 0; i < sizeof(imm_micro) / sizeof(imm_micro[0]); ++i)
    {
        if (!strcmp(imm_micro[i], mnemonic))
        {
            *size = CPU_WIDTH_VOID;
            *out = i;
            return 0;
        }
    }

    for (i = 0; i < sizeof(imm_normal) / sizeof(imm_normal[0]); ++i)
    {
        if (!strcmp(imm_normal[i], mnemonic))
        {
            if (i > 255)
                *size = CPU_WIDTH_WORD;
            else
                *size = CPU_WIDTH_BYTE;

            *out = i;
            return 0;
        }
    }

    /* NOT FOUND */
    return 1;
}