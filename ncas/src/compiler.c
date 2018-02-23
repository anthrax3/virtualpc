/*
  Created by nullifiedcat on 21.02.18.
*/

#include "compiler.h"

#include <stdio.h>

struct compiler_state_s *compiler_init()
{
    struct compiler_state_s *result = calloc(1, sizeof(struct compiler_state_s));
    result->labels = array_init(sizeof(struct compiler_label_s), NULL);
    result->lexer = lexer_init();
    return result;
}

int compiler_process_file(struct compiler_state_s *state, const char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
        return 1;

    char c;
    while ((c = (char)fgetc(file)) != EOF)
    {
        lexer_push_char(state->lexer, c);
    }

    return 0;
}

void compiler_destroy(struct compiler_state_s *state)
{
    array_destroy(state->labels);
    lexer_destroy(state->lexer);
    free(state);
}