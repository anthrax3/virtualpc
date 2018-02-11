/*
 * tokenizer.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

void token_delete_fn(void *token)
{
    free(*(char **)token);
}

struct tokenizer_context_s *tokenizer_init()
{
    struct tokenizer_context_s *result = calloc(1, sizeof(struct tokenizer_context_s));

    result->buffer = array_init(sizeof(char), NULL);
    result->tokens = array_init(sizeof(char *), token_delete_fn);

    return result;
}

void tokenizer_split(struct tokenizer_context_s *context)
{
    if (!context->buffer->length)
        return;

    char *buffer = calloc(context->buffer->length + 1, sizeof(char));
    strncpy(buffer, context->buffer->memory, context->buffer->length);

    /* Empty the buffer */
    array_resize(context->buffer, 0);
    array_reserve(context->buffer, 32);

    array_push(context->tokens, &buffer, 1);
}

void tokenizer_push_char(struct tokenizer_context_s *context, char c)
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
        tokenizer_split(context);
        context->flags.split = 0;
    }

    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
        tokenizer_split(context);
        return;
    case '"':
        tokenizer_split(context);
        context->flags.reading_string = !context->flags.reading_string;
        context->flags.split = 1;
        break;
    case ';':
        if (!context->flags.reading_string)
        {
            tokenizer_split(context);
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
            tokenizer_split(context);
            context->flags.split = 1;
        }
        break;
    }

    array_push(context->buffer, &c, 1);
}

void tokenizer_destroy(struct tokenizer_context_s *context)
{
    array_destroy(context->buffer);
    array_destroy(context->tokens);

    free(context);
}
