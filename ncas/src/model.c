/*
  Created by nullifiedcat on 04.03.18.
*/

#include "model.h"

#include <memory.h>
#include <stdio.h>

void sentence_free(void *sentence)
{
    sentence_t s = *(sentence_t *) sentence;
    array_destroy(s->words);
    free(s);
}

void word_free(void *word)
{
    word_t w = *(word_t *) word;
    free(w->content);
    free(w);
}

word_t word_create(const char *content, size_t length)
{
    word_t result = calloc(1, sizeof(struct word_s));

    result->content = calloc(length + 1, sizeof(char));
    strncpy(result->content, content, length);

    return result;
}

sentence_t sentence_create(array_t words)
{
    sentence_t sentence = calloc(1, sizeof(struct sentence_s));
    sentence->words     = array_init(sizeof(word_t), word_free);
    array_push(sentence->words, words->memory, words->length);

    return sentence;
}

model_t model_create()
{
    model_t result = calloc(1, sizeof(struct model_s));

    result->sentences = array_init(sizeof(sentence_t), sentence_free);

    return result;
}

void model_destroy(model_t model)
{
    array_destroy(model->sentences);
    free(model);
}