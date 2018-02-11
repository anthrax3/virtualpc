/*
 * tokenizer.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include "array.h"

struct tokenizer_context_s
{
    struct array_s *buffer;
    struct array_s *tokens;

    struct
    {
        char reading_comment : 1;
        char reading_string : 1;
        char split : 1;
    } flags;
};

struct tokenizer_context_s *tokenizer_init();
void tokenizer_split(struct tokenizer_context_s *context);
void tokenizer_push_char(struct tokenizer_context_s *context, char c);
void tokenizer_destroy(struct tokenizer_context_s *context);
