/*
 * main.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"
#include <stdio.h>
#include "compiler.h"
#include "cutter.h"

int main(int argc, const char **argv)
{
    if (argc < 2)
    {
        printf("ncas <file>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
        return EXIT_FAILURE;

    struct cutter_s *cutter = cutter_init();

    char c;
    while ((c = fgetc(file)) != EOF)
        cutter_push(cutter, c);

    size_t i = 0;
    for (; i < cutter->sentences->length; ++i)
    {
        char **sentence = *(char ***)array_get(cutter->sentences, i);

        while (sentence && *sentence != NULL)
        {
            printf("%s, ", *sentence);
            ++sentence;
        }

        printf("\n");
    }

    printf("\n");

    cutter_destroy(cutter);

    /*struct compiler_state_s *state = compiler_init();

    int result = compiler_process_file(state, argv[1]);
    printf("compiler exited with code %d\n", result);

    compiler_destroy(state);*/

    return 0;
}
