/*
  Created by nullifiedcat on 21.02.18.
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <lexer.h>

struct compiler_state_s *compiler_init()
{
    struct compiler_state_s *result =
        calloc(1, sizeof(struct compiler_state_s));
    result->labels = array_init(sizeof(struct compiler_label_s), NULL);
    result->lexer  = lexer_init();
    return result;
}

int compiler_process_file(struct compiler_state_s *state, const char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
        return 1;

    char c;
    while ((c = (char) fgetc(file)) != EOF)
    {
        lexer_push_char(state->lexer, c);
    }
    lexer_split(state->lexer);

    while (compiler_step(state) == EXIT_SUCCESS);

    return 0;
}

struct token_s *compiler_consume_token(struct compiler_state_s *state)
{
    struct token_s *result = compiler_peek_token(state);

    if (result != NULL)
        ++state->token_counter;

    return result;
}

struct token_s *compiler_peek_token(struct compiler_state_s *state)
{
    if (state->token_counter >= state->lexer->tokens->length)
        return NULL;

    return array_get(state->lexer->tokens, state->token_counter);
}

int compiler_step(struct compiler_state_s *state)
{
    struct token_s *token = compiler_consume_token(state);

    if (token == NULL)
        return EXIT_FAILURE;

    printf("Token '%s', type %d\n", token->contents, token->type);

    switch (token->type)
    {
    case TOKEN_OPERATOR:
        if (token->contents[0] == ':')
        {
            struct token_s *name = compiler_consume_token(state);
            if (name == NULL)
                return EXIT_FAILURE;

            compiler_add_label_here(state, name->contents);
        }
        else
        {
            printf("Excepted ':' operator, got %c\n", token->contents[0]);
            return EXIT_FAILURE;
        }
        break;
    case TOKEN_KEYWORD:
        switch (token->data.keyword)
        {
        case KW_ORIGIN:
            /* modify state->counter; */
        default:
            printf("Unexpected keyword %d\n", token->data.keyword);
        }
        break;
    default:
        printf("Unexpected token of type %d: '%s'\n", token->type, token->contents);
    }

    return EXIT_SUCCESS;
}

void compiler_add_label_here(struct compiler_state_s *state, const char *label)
{
    struct compiler_label_s created;

    created.counter = state->counter;
    strncpy(created.name, label, COMPILER_LABEL_MAX_LENGTH);
    created.name[COMPILER_LABEL_MAX_LENGTH - 1] = 0;

    array_push(state->labels, &created, 1);
}

uint32_t compiler_find_label(struct compiler_state_s *state, const char *label)
{
    size_t i = 0;
    for (; i < state->labels->length; ++i)
    {
        struct compiler_label_s *current = array_get(state->labels, i);
        if (strncmp(current->name, label, COMPILER_LABEL_MAX_LENGTH) == 0)
        {
            return current->counter;
        }
    }
    /* No label was found */
    return UINT32_MAX;
}

void compiler_destroy(struct compiler_state_s *state)
{
    array_destroy(state->labels);
    lexer_destroy(state->lexer);
    free(state);
}