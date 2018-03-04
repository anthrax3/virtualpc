/*
  Created by nullifiedcat on 04.03.18.
*/

#include <stdio.h>
#include <memory.h>
#include <model.h>
#include "parser.h"

parser_t parser_create(compiler_t compiler)
{
    parser_t result = calloc(1, sizeof(struct parser_s));
    result->compiler = compiler;
    return result;
}

void parser_destroy(parser_t parser)
{
    free(parser);
}

void parser_add_const(parser_t parser, const char *name, uint32_t value)
{
    if (strlen(name) > 31)
    {
        printf("WARNING: Max name length is 31, name will be truncated. (%s)\n", name);
    }

    struct parser_constant_s result;
    strncpy(result.name, name, 32);
    result.name[31] = 0;
    result.value = value;

    array_push(parser->constants, &result, 1);
}

int parser_lookup_const(parser_t parser, const char *name, uint32_t *out)
{
    size_t i = 0;
    for (; i < parser->constants->length; ++i)
    {
        struct parser_constant_s constant = *(struct parser_constant_s *)array_get(parser->constants, i);
        if (!strcmp(constant.name, name))
        {
            if (out)
                *out = constant.value;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

void parser_process_sentences(parser_t parser, array_t sentences)
{
    size_t i = 0;
    for (; i < sentences->length; ++i)
    {
        parser_process_sentence(parser, *(sentence_t *)array_get(sentences, i));
    }
}

void parser_process_sentence(parser_t parser, sentence_t sentence)
{
    struct parsed_statement_s statement;
    statement.meaning = sentence->meaning;
    parser->current_sentence = sentence;
    parser->word_index = 0;

    switch (sentence->meaning)
    {
    case SM_HINT:
        parser_process_hint(parser);
        break;
    case SM_INSTRUCTION:
        parser_process_instruction(parser);
        break;
    default:
        printf("Unexpected sentence meaning: %d\n", sentence->meaning);
        break;
    }
}

void parser_process_hint(parser_t parser)
{
    word_t keyword = *(word_t *)array_get(parser->current_sentence->words, 0);

    if (keyword->meaning != WM_KEYWORD)
    {
        printf("parser_process_hint: excepted keyword\n");
        return;
    }

    switch (keyword->data.keyword)
    {
    case KW_CONST:
        /* Add const with name and value... */
        break;
    case KW_LABEL:
        /* Add const with current address... */
        break;
    case KW_ORIGIN:
        /* Set current address... */

        break;
    case KW_STORE:
        /* Write bytes directly to current address... */
        break;
    default:
        printf("Unknown keyword: %s\n", keyword->content);
        break;
    }
}

void parser_process_instruction(parser_t parser)
{
    struct parsed_instruction_s instruction;

    memset(&instruction, 0, sizeof(struct parsed_instruction_s));

    instruction.operands[0].mode = OPERAND_MODE_NONE;
    instruction.operands[1].mode = OPERAND_MODE_NONE;

    instruction.width = CPU_WIDTH_DWORD;
    instruction.operands[0].width = CPU_WIDTH_DWORD;
    instruction.operands[1].width = CPU_WIDTH_DWORD;


}

int parser_consume_operand(parser_t parser, struct parsed_operand_s *output)
{
    word_t first = *(word_t *)array_get(parser->current_sentence->words, parser->word_index);

    switch (first->meaning)
    {
    case WM_NUMBER:
        
    }

    return EXIT_FAILURE;
}

int parser_consume_instruction(parser_t parser, struct parsed_instruction_s *output)
{
    /* TODO */
    return EXIT_FAILURE;
}