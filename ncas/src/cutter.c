/*
  Created by nullifiedcat on 03.03.18.
*/

#include "cutter.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

void word_delete_fn(void *word)
{
    free(*(char **)word);
}

void sentence_delete_fn(void *sentence)
{
    char **list = *(char ***)sentence;

    while (*list != NULL)
    {
        free(*list);
        ++list;
    }

    free(*(char ***)sentence);
}

struct cutter_s *cutter_init()
{
    struct cutter_s *result = calloc(1, sizeof(struct cutter_s));

    result->characters = array_init(sizeof(char), NULL);
    result->words = array_init(sizeof(char *), word_delete_fn);
    result->sentences = array_init(sizeof(char **), sentence_delete_fn);

    return result;
}

void cutter_push(struct cutter_s *cutter, char c)
{
    if (c == '\n')
    {
        cutter->flags.reading_comment = 0;
        cutter->flags.reading_string = 0;

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
            lazy_cut = true;
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
    }

    array_push(cutter->characters, &c, 1);

    if (lazy_cut)
        cutter_cut_word(cutter);
}

void cutter_cut_word(struct cutter_s *cutter)
{
    if (cutter->characters->length)
    {
        char *word = malloc(cutter->characters->length + 1);
        word[cutter->characters->length] = 0;
        strncpy(word, cutter->characters->memory, cutter->characters->length);
        array_clear(cutter->characters);
        array_push(cutter->words, &word, 1);
    }
}

void cutter_cut_sentence(struct cutter_s *cutter)
{
    if (cutter->words->length)
    {
        char **sentence = malloc((cutter->words->length + 1) * sizeof(char *));
        sentence[cutter->words->length] = NULL;

        size_t i = 0;
        for (; i < cutter->words->length; ++i)
        {
            const char *word = *(char **)array_get(cutter->words, i);

            sentence[i] = calloc(sizeof(char), strlen(word) + 1);
            strcpy(sentence[i], word);
        }

        array_clear(cutter->words);
        array_push(cutter->sentences, &sentence, 1);
    }
}

void cutter_end(struct cutter_s *cutter)
{
    cutter_cut_word(cutter);
    cutter_cut_sentence(cutter);
}

void cutter_destroy(struct cutter_s *cutter)
{
    array_destroy(cutter->characters);
    array_destroy(cutter->words);
    array_destroy(cutter->sentences);
}