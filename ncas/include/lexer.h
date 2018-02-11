/*
 * lexer.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include "array.h"

#include <stdint.h>

enum token_type
{
    TOKEN_REGISTER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_ILLEGAL
};

struct token_s
{
    char *contents;
    enum token_type type;
    /* For number literal */
    uint32_t number;
};

struct lexer_context_s
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

struct lexer_context_s *lexer_init();
enum token_type lexer_identify(const char *token);
uint32_t lexer_parse_number(const char *token);
void lexer_split(struct lexer_context_s *context);
void lexer_push_char(struct lexer_context_s *context, char c);
void lexer_destroy(struct lexer_context_s *context);
