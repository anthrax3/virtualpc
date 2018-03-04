/*
 * array.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdlib.h>

typedef struct array_s
{
    void (*delete_fn)(void *);

    size_t item_size;
    size_t length;
    size_t reserved;
    void *memory;
} * array_t;

array_t array_init(size_t item_size, void (*delete_fn)(void *));
void array_destroy(array_t array);

void array_resize(array_t array, size_t length);
void array_reserve(array_t array, size_t length);
void array_clear(array_t array);

void array_push(array_t array, const void *items, size_t count);
void *array_pop(array_t array);

void *array_get(array_t array, size_t index);
void array_set(array_t array, size_t index, const void *item);
