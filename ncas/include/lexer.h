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
    TOKEN_INSTRUCTION,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_NUMBER_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_ILLEGAL
};

enum keyword_type
{
    KW_FIRST,
    KW_ORIGIN = KW_FIRST,
    KW_CONST,

    /* Width control */
    KW_BYTE,
    KW_WORD,
    KW_DWORD,

    KW_LAST
};

struct token_s
{
    char *contents;
    enum token_type type;
    union
    {
        /* For number literal */
        uint32_t number;
        /* For keyword */
        enum keyword_type keyword;
        /* For instruction */
        struct
        {
            uint32_t width;
            uint32_t instruction;
        } instruction;
        /* For register */
        uint32_t register_name;
    } data;

};

/*
 * buffer = array of chars
 * tokens = array of buffers
 * statements = array of tokens (array of array of array of chars)
 */
struct lexer_context_s
{
    struct array_s *buffer;
    struct array_s *tokens;
    struct array_s *statements;

    struct
    {
        char reading_comment : 1;
        char reading_string : 1;
        char split : 1;
    } flags;
};

struct lexer_context_s *lexer_init();
enum token_type lexer_identify(struct token_s *token);
enum keyword_type lexer_identify_keyword(const char *token);
int lexer_parse_number(const char *token, uint32_t *out);
void lexer_split(struct lexer_context_s *context);
void lexer_split_statement(struct lexer_context_s *context);
void lexer_push_char(struct lexer_context_s *context, char c);
void lexer_destroy(struct lexer_context_s *context);
void lexer_finish(struct lexer_context_s *context);
