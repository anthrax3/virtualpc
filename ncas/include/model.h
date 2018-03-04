/*
  Created by nullifiedcat on 04.03.18.
*/

#pragma once

#include <instruction.h>
#include <registers.h>
#include "array.h"

enum sentence_meaning
{
    SM_UNKNOWN,
    /*
     * Compiler hint: origin 100h
     */
    SM_HINT,
    /*
     * Instruction: mov ra , [ rb ]
     */
    SM_INSTRUCTION
};

enum keyword
{
    KW_UNKNOWN,
    /*
     * Defines a label
     * : loop
     */
    KW_LABEL,
    /*
     * Defines a constant
     * const begin 100000h
     */
    KW_CONST,
    /*
     * Sets the address of where next instruction will be written to
     * origin 100000h
     */
    KW_ORIGIN,
    /*
     * Writes the values into resulting binary
     * store "Testing", byte 0h
     */
    KW_STORE,
    /*
     * (for operands)
     * Indicates that the following number is actually offset from current location
     */
    KW_OFFSET,
    KW_COUNT
};

enum operator_name
{
    OPERATOR_UNKNOWN,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_COUNT
};

enum word_meaning
{
    WM_UNKNOWN,
    /*
     * Words like const, origin...
     */
    WM_KEYWORD,
    /*
     * Width specifiers: byte, word, dword
     */
    WM_WIDTH,
    /*
     * Operators like +, -, *...
     */
    WM_OPERATOR,
    /*
     * Comma between operands
     */
    WM_COMMA,
    /*
     * [ and ]
     */
    WM_ADDRESSING_START,
    WM_ADDRESSING_END,
    /*
     * Number literal, like 1000h
     */
    WM_NUMBER,
    /*
     * String literal: "Testing"
     */
    WM_STRING,
    /*
     * Instruction mnemonic: mov, add, sub...
     */
    WM_MNEMONIC,
    /*
     * Register: ra, rb, ...
     */
    WM_REGISTER,
    /*
     * Custom identifier, label or constant value: basically everything else
     */
    WM_IDENTIFIER,
    WM_COUNT
};

typedef struct sentence_s
{
    array_t words;
    enum sentence_meaning meaning;
} * sentence_t;

typedef struct word_s
{
    char *content;
    enum word_meaning meaning;
    union {
        enum operator_name operator;
        enum keyword keyword;
        enum cpu_width width;
        struct
        {
            uint32_t code;
            enum cpu_width width;
        } instruction;
        enum register_name rname;
        uint32_t number;
    } data;
} * word_t;

typedef struct model_s
{
    array_t sentences;
} * model_t;

word_t word_create(const char *content, size_t length);
sentence_t sentence_create(array_t words);

model_t model_create();
void model_destroy(model_t model);