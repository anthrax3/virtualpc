/*
  Created by nullifiedcat on 04.03.18.
*/


#pragma once

/*
 *  Parser processes sentences into compiler-ready code
 */

#include "model.h"
#include "instruction.h"

typedef struct parser_s
{

} *parser_t;

parser_t parser_create();
void parser_destroy(parser_t parser);

/*
 * Hints won't be actually parsed, but passed straight to compiler instead.
 */
struct parsed_hint
{
    sentence_t sentence;
};

struct parsed_operand_s
{
    enum operand_mode mode;
    /*
     * Value:
     *  0: immediate value
     *  1: address
     *  2: offset
     *  3: -
     *  4: -
     *  5: offset
     *  6: -
     *  7: -
     */
    uint32_t value;
    /*
     * Only used in mode 6
     */
    uint16_t factor;
    /*
     * Modes 3..6
     */
    enum register_name rname1;
    /*
     * Mode 6 only
     */
    enum register_name rname2;
};

struct parsed_instruction_s
{
    uint32_t instruction;
    enum cpu_width width;

    struct parsed_operand_s operands[2];
};

struct parsed_statement_s
{
    enum sentence_meaning meaning;

    union
    {
        struct parsed_instruction_s instruction;
        struct parsed_hint hint;
    };
};

void parser_emit_statement(struct parsed_statement_s statement);


int parser_consume_operand(parser_t parser, struct parsed_operand_s *output);
int parser_consume_instruction(parser_t parser, struct parsed_instruction_s *output);

void parser_expect_number(parser_t parser);