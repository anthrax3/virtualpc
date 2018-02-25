/*
  Created by nullifiedcat on 21.02.18.
*/

#pragma once

#include "array.h"
#include "lexer.h"
#include <stdint.h>

#define COMPILER_LABEL_MAX_LENGTH 32

struct compiler_label_s
{
    char name[COMPILER_LABEL_MAX_LENGTH];
    uint32_t counter;
};

struct compiler_state_s
{
    struct array_s *labels;
    struct lexer_context_s *lexer;
    uint32_t counter;
    uint32_t token_counter;
};

struct compiler_operand_s
{
    uint8_t mode;
    uint8_t size;

    struct
    {
        /* For immediate values, pointers */
        uint32_t value;
        /* For offsets, register + offsets */
        int32_t offset;
        /* For reg, [reg], [reg + off] */
        uint32_t rbase;
        /* For [rbase + rfactor * factor] */
        uint32_t rfactor;
        /* For [rbase + rfactor * factor] */
        uint16_t factor;
    } addressing;
};

struct compiler_instruction_s
{
    uint32_t instruction;
    uint8_t  width;

    struct compiler_operand_s operands[2];
};

struct compiler_state_s *compiler_init();
int compiler_process_file(struct compiler_state_s *state, const char *path);
void compiler_destroy(struct compiler_state_s *state);

void compiler_step(struct compiler_state_s *state);
int compiler_read_instruction(struct compiler_state_s *state);
int compiler_read_operand(struct compiler_state_s *state, struct compiler_operand_s* out);

uint32_t compiler_instruction_write(struct compiler_instruction_s instruction, uint8_t *out, uint32_t space);

void compiler_add_label_here(struct compiler_state_s *state, const char *label);
uint32_t compiler_find_label(struct compiler_state_s *state, const char *label);