/*
  Created by nullifiedcat on 04.03.18.
*/


#pragma once

/*
 *  Parser processes sentences into compiler-ready code
 */

#include "model.h"
#include "instruction.h"
#include "compiler.h"

/*
 * Either a const value, offset or label
 * Is (byte)-1 same as (dword)-1 for the compiler?
 * Maybe there's a need to store the size too. TODO
 * Max size is 32bit anyway
 */
struct parser_constant_s
{
    char name[32];
    uint32_t value;
};

typedef struct parser_s
{
    compiler_t compiler;

    array_t constants;
    sentence_t current_sentence;
    size_t word_index;
} *parser_t;

parser_t parser_create(compiler_t compiler);
void parser_destroy(parser_t parser);

void parser_add_const(parser_t parser, const char *name, uint32_t value);
int parser_lookup_const(parser_t parser, const char *name, uint32_t *out);

void parser_process_sentences(parser_t parser, array_t sentences);
void parser_process_sentence(parser_t parser, sentence_t sentence);

void parser_process_hint(parser_t parser);
void parser_process_instruction(parser_t parser);

int parser_consume_identifier(parser_t parser, const char **out);
int parser_consume_number(parser_t parser, uint32_t *out);
int parser_consume_operand(parser_t parser, struct parsed_operand_s *output);