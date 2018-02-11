/*
 * instruction.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

enum instruction_length
{
    INSTRUCTION_LENGTH_BYTE,
    INSTRUCTION_LENGTH_WORD,
    INSTRUCTION_LENGTH_DWORD,
    INSTRUCTION_LENGTH_MICRO
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

struct instruction_head_s
{

};

struct instruction_description_s
{
    struct
    {
        char operand_1 : 4;
        char operand_2 : 4;
    } operand_flags;
};
