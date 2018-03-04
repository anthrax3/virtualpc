/*
  Created by nullifiedcat on 03.03.18.
*/

#include "cutter.h"
#include "model.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct cutter_s *cutter_init(struct array_s *output)
{
    struct cutter_s *result = calloc(1, sizeof(struct cutter_s));

    result->characters = array_init(sizeof(char), NULL);
    /* free()'ing the words is done when deleting the sentence */
    result->words  = array_init(sizeof(word_t), NULL);
    result->output = output;

    return result;
}

void cutter_push(struct cutter_s *cutter, char c)
{
    if (c == '\n')
    {
        cutter->flags.reading_comment = 0;
        cutter->flags.reading_string  = 0;

        cutter_cut_word(cutter);
        cutter_cut_sentence(cutter);

        return;
    }

    if (cutter->flags.reading_comment)
        return;

    bool lazy_cut = false;

    switch (c)
    {
    case ' ':
    case '\t':
        if (!cutter->flags.reading_string)
        {
            cutter_cut_word(cutter);
            return;
        }
        break;
    case '"':
        if (cutter->flags.reading_string)
            lazy_cut                 = true;
        cutter->flags.reading_string = !cutter->flags.reading_string;
        break;
    case ';':
        if (!cutter->flags.reading_string)
        {
            cutter_cut_word(cutter);
            cutter->flags.reading_comment = 1;
            return;
        }
        break;
    case '[':
    case ']':
    case '+':
    case '-':
    case ':':
    case ',':
        if (!cutter->flags.reading_string)
        {
            cutter_cut_word(cutter);
            lazy_cut = true;
        }
        break;
    default:
        break;
    }

    array_push(cutter->characters, &c, 1);

    if (lazy_cut)
        cutter_cut_word(cutter);
}

void cutter_cut_word(struct cutter_s *cutter)
{
    if (cutter->characters->length)
    {
        word_t word =
            word_create(cutter->characters->memory, cutter->characters->length);
        array_clear(cutter->characters);
        array_push(cutter->words, &word, 1);
    }
}

void cutter_cut_sentence(struct cutter_s *cutter)
{
    if (cutter->words->length)
    {
        sentence_t sentence = sentence_create(cutter->words);
        array_clear(cutter->words);
        array_push(cutter->output, &sentence, 1);
    }
}

void cutter_end(struct cutter_s *cutter)
{
    cutter_cut_word(cutter);
    cutter_cut_sentence(cutter);
}

void cutter_destroy(struct cutter_s *cutter)
{
    if (cutter->characters->length || cutter->words->length)
    {
        printf("WARNING: Leftover characters/words in cutter buffers! Memory "
               "will not be free!\n");
    }

    array_destroy(cutter->characters);
    array_destroy(cutter->words);
    free(cutter);
}