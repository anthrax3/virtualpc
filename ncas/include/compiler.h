/*
  Created by nullifiedcat on 21.02.18.
*/

#pragma once

#include "array.h"
#include "lexer.h"
#include <stdint.h>

struct compiler_label_s
{
    char name[32];
    uint32_t counter;
};

struct compiler_state_s
{
    struct array_s *labels;
    struct lexer_context_s *lexer;
    uint32_t counter;
    uint32_t token_counter;
};

struct compiler_state_s *compiler_init();
int compiler_process_file(struct compiler_state_s *state, const char *path);
void compiler_destroy(struct compiler_state_s *state);

int compiler_next_token(struct compiler_state_s *state);
