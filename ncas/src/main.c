/*
 * main.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"
#include "tokenizer.h"

#include <stdio.h>

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

    struct tokenizer_context_s *ctx = tokenizer_init();

    char c;

    while (1)
    {
        c = fgetc(file);

        if (c == EOF)
            break;

        tokenizer_push_char(ctx, c);
    }
    tokenizer_split(ctx);

    size_t i = 0;
    for (; i < ctx->tokens->length; ++i)
    {
        printf("%s\n", *(char **)array_get(ctx->tokens, i));
    }

    fclose(file);
    tokenizer_destroy(ctx);

    return 0;
}
