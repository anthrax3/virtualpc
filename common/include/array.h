/*
 * array.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdlib.h>

struct array_s
{
    void (*delete_fn)(void *);

    size_t item_size;
    size_t length;
    size_t reserved;
    void *memory;
};

struct array_s *array_init(size_t item_size, void (*delete_fn)(void *));
void array_destroy(struct array_s *array);

void array_resize(struct array_s *array, size_t length);
void array_reserve(struct array_s *array, size_t length);

void array_push(struct array_s *array, const void *items, size_t count);
void *array_pop(struct array_s *array);

void *array_get(struct array_s *array, size_t index);
void array_set(struct array_s *array, size_t index, const void *item);
