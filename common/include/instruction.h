/*
 * instruction.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>

enum cpu_width
{
    CPU_WIDTH_BYTE,
    CPU_WIDTH_WORD,
    CPU_WIDTH_DWORD,
    CPU_WIDTH_VOID
};

enum operand_mode
{
    OPERAND_MODE_IMMEDIATE,
    OPERAND_MODE_ADDRESS,
    OPERAND_MODE_OFFSET,
    OPERAND_MODE_REGISTER,
    OPERAND_MODE_REGISTER_INDIRECT,
    OPERAND_MODE_REGISTER_INDIRECT_WITH_OFFSET,
    OPERAND_MODE_REGISTER_INDIRECT_WITH_FACTOR,
    OPERAND_MODE_NONE
};

enum operand_flags
{
    OPERAND_MUST_BE_READABLE = (1 << 0),
    OPERAND_MUST_BE_ADDRESSABLE = (1 << 1),
    OPERAND_MUST_BE_WRITABLE = (1 << 2),
    OPERAND_MUST_BE_EMPTY = (1 << 3)
};

union instruction_head_s
{
    struct
    {
        uint8_t size : 2;
        union
        {
            struct
            {
                uint8_t first : 3;
                uint8_t second : 3;
            } addressing;
            struct
            {
                uint8_t instruction : 6;
            } micro;
        };
    };
    uint8_t byte;
};
