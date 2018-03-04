/*
 * main.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"
#include <stdio.h>
#include <model.h>
#include <analyzer.h>
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

    model_t model = model_create();

    struct cutter_s *cutter = cutter_init(model->sentences);

    int c;
    while ((c = fgetc(file)) != EOF)
        cutter_push(cutter, (char) c);
    cutter_end(cutter);
    cutter_destroy(cutter);

    size_t i = 0;
    for (; i < model->sentences->length; ++i)
    {
        sentence_t sentence = *(sentence_t *) array_get(model->sentences, i);

        size_t j = 0;
        for (; j < sentence->words->length; ++j)
        {
            word_t word = *(word_t *) array_get(sentence->words, j);
            printf("%s ", word->content);
        }

        printf("\n");
    }

    printf("\n");

    analyze_sentences(model->sentences);

    model_destroy(model);

    /*struct compiler_state_s *state = compiler_init();

    int result = compiler_process_file(state, argv[1]);
    printf("compiler exited with code %d\n", result);

    compiler_destroy(state);*/

    return 0;
}
