/*
 * lexer.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <lexer.h>
#include <stdio.h>

#include "lexer.h"
#include "registers.h"
#include "mnemonics.h"

void token_delete_fn(void *token)
{
    struct token_s *tk = token;

    free(tk->contents);
}

struct lexer_context_s *lexer_init()
{
    struct lexer_context_s *result = calloc(1, sizeof(struct lexer_context_s));

    result->buffer = array_init(sizeof(char), NULL);
    result->tokens = array_init(sizeof(struct token_s), token_delete_fn);

    return result;
}

enum token_type lexer_identify(struct token_s *token)
{
    size_t length = strlen(token->contents);

    if (length == 0)
        return TOKEN_ILLEGAL;

    enum keyword_type kw = lexer_identify_keyword(token->contents);

    if (kw != KW_LAST)
    {
        token->data.keyword = kw;
        return TOKEN_KEYWORD;
    }

    switch (token->contents[0])
    {
    case '[':
    case ']':
    case '+':
    case '-':
    case ':':
    case ',':
        return TOKEN_OPERATOR;
    case '"':
        return TOKEN_STRING_LITERAL;
    default:
        break;
    }

    uint32_t i = 0;

    for (; i < REGISTER_COUNT; ++i)
    {
        if (!register_get(i))
            continue;

        if (!strcmp(register_get(i)->name, token->contents))
        {
            token->data.register_name = i;
            return TOKEN_REGISTER;
        }
    }

    uint32_t instruction, width;
    if (instruction_from_mnemonic(token->contents, &instruction, &width) == EXIT_SUCCESS)
    {
        token->data.instruction.instruction = instruction;
        token->data.instruction.width = width;
        return TOKEN_INSTRUCTION;
    }

    uint32_t number;
    if (lexer_parse_number(token->contents, &number) == EXIT_SUCCESS)
    {
        token->data.number = number;
        return TOKEN_NUMBER_LITERAL;
    }

    return TOKEN_IDENTIFIER;
}

const char *keywords[] = {
        [ KW_ORIGIN ] = "origin",
        [ KW_CONST ] = "const",
        [ KW_BYTE ] = "byte",
        [ KW_WORD ] = "word",
        [ KW_DWORD ] = "dword",
        [ KW_LAST ] = NULL
};
enum keyword_type lexer_identify_keyword(const char *token)
{
    enum keyword_type i = KW_FIRST;

    for (; i < KW_LAST; ++i)
    {
        if (strcmp(token, keywords[i]) == 0)
            return i;
    }

    return KW_LAST;
}

int lexer_parse_number(const char *token, uint32_t *out)
{
    size_t length = strlen(token);
    int base;

    switch (token[length - 1])
    {
    case 'h':
        base = 16;
        break;
    case 'o':
        base = 8;
        break;
    case 'd':
        base = 10;
        break;
    case 'b':
        base = 2;
        break;
    default:
        return EXIT_FAILURE;
    }

    if (out)
        *out = (uint32_t) strtoul(token, NULL, base);

    return EXIT_SUCCESS;
}

void lexer_split(struct lexer_context_s *context)
{
    if (!context->buffer->length)
        return;

    char *buffer = calloc(context->buffer->length + 1, sizeof(char));
    strncpy(buffer, context->buffer->memory, context->buffer->length);

    /* Empty the buffer */
    array_resize(context->buffer, 0);
    array_reserve(context->buffer, 32);

    struct token_s token;
    token.contents = buffer;
    lexer_identify(&token);

    array_push(context->tokens, &token, 1);
}

void lexer_push_char(struct lexer_context_s *context, char c)
{
    if (context->flags.reading_comment)
    {
        if (c == '\n')
            context->flags.reading_comment = 0;
        else
            return;
    }

    if (context->flags.split)
    {
        lexer_split(context);
        context->flags.split = 0;
    }

    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
        if (!context->flags.reading_string)
        {
            lexer_split(context);
            return;
        }
        break;
    case '"':
        if (context->flags.reading_string)
            context->flags.split      = 1;
        context->flags.reading_string = !context->flags.reading_string;
        break;
    case ';':
        if (!context->flags.reading_string)
        {
            lexer_split(context);
            context->flags.reading_comment = 1;
            return;
        }
        break;
    case '[':
    case ']':
    case '+':
    case '-':
    case ':':
    case ',':
        if (!context->flags.reading_string)
        {
            lexer_split(context);
            context->flags.split = 1;
        }
        break;
    }

    array_push(context->buffer, &c, 1);
}

void lexer_destroy(struct lexer_context_s *context)
{
    array_destroy(context->buffer);
    array_destroy(context->tokens);

    free(context);
}
