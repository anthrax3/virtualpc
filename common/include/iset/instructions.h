/*
 * instructions.h
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#pragma once

/* 8-bit instructions */
enum iset_cat
{
    /* Basic operations */
    ISET_CAT_MOVE,
    ISET_CAT_LOAD_EFFECTIVE_ADDRESS,
    ISET_CAT_EXCHANGE,

    /* Logic? */
    ISET_CAT_COMPARE,

    /* Arithmetic */
    ISET_CAT_ADD,
    ISET_CAT_SUBTRACT,
    ISET_CAT_MULTIPLY,
    ISET_CAT_DIVIDE,
    ISET_CAT_NEGATE,
    ISET_CAT_INCREMENT,
    ISET_CAT_DECREMENT,

    /* Jumps */
    ISET_CAT_JUMP,
    ISET_CAT_JUMP_IF_ZERO,
    ISET_CAT_JUMP_IF_NOT_ZERO,
    ISET_CAT_JUMP_IF_GREATER,
    ISET_CAT_JUMP_IF_GREATER_OR_EQUAL,
    ISET_CAT_JUMP_IF_LESS,
    ISET_CAT_JUMP_IF_LESS_OR_EQUAL,

    /* Bit logic operators */
    ISET_CAT_NOT,
    ISET_CAT_AND,
    ISET_CAT_OR,
    ISET_CAT_XOR,

    /* Bit shifting */
    ISET_CAT_SHL,
    ISET_CAT_SHR,

    /* Amount of instructions (must be less than 64!) */
    ISET_CAT_COUNT
};