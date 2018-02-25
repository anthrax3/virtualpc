/*
  Created by nullifiedcat on 21.02.18.
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

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


void compiler_step(struct compiler_state_s *state)
{
    (void) state;
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