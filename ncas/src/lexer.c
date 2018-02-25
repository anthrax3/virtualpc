/*
 * lexer.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "registers.h"

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

enum token_type lexer_identify(const char *token)
{
    size_t length = strlen(token);

    if (length == 0)
        return TOKEN_ILLEGAL;

    switch (token[0])
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

        if (!strcmp(register_get(i)->name, token))
            return TOKEN_REGISTER;
    }


    char hex = (token[length - 1] == 'h');
    for (i = 0; i < length; ++i)
    {
        if (!(hex ? isxdigit(token[i]) : isdigit(token[i])))
            break;
    }
    if (i == length || (hex && i == length - 1))
    {
        return TOKEN_NUMBER_LITERAL;
    }
    else if (i > 0)
        return TOKEN_ILLEGAL;

    return TOKEN_IDENTIFIER;
}

uint32_t lexer_parse_number(const char *token)
{
    uint32_t length = strlen(token);
    int base = 10;

    if (token[length - 1] == 'h')
        base = 16;

    return strtoul(token, NULL, base);
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
    token.type = lexer_identify(buffer);
    if (token.type == TOKEN_NUMBER_LITERAL)
        token.number = lexer_parse_number(buffer);
    else
        token.number = 0;

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
            context->flags.split = 1;
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
