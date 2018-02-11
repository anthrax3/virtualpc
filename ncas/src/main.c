/*
 * main.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"
#include <stdio.h>
#include "lexer.h"

int main(int argc, const char **argv)
{
    if (argc < 2)
    {
        printf("ncas <file>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("failed to open file");
        return 1;
    }

    struct lexer_context_s *ctx = lexer_init();

    char c;

    while (1)
    {
        c = fgetc(file);

        if (c == EOF)
            break;

        lexer_push_char(ctx, c);
    }
    lexer_split(ctx);

    size_t i = 0;
    for (; i < ctx->tokens->length; ++i)
    {
        struct token_s *token = array_get(ctx->tokens, i);
        printf("%d\t%s\t%u\n", token->type, token->contents, token->number);
    }

    fclose(file);
    lexer_destroy(ctx);

    return 0;
}
