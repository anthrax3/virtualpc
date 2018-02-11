/*
 * array.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "array.h"

#include <stdlib.h>
#include <string.h>

struct array_s *array_init(size_t item_size, void(*delete_fn)(void *))
{
    struct array_s *result = calloc(1, sizeof(struct array_s));

    result->item_size = item_size;
    result->delete_fn = delete_fn;

    return result;
}

void array_destroy(struct array_s *array)
{
    if (array->delete_fn)
    {
        size_t i = 0;
        for (; i < array->length; ++i)
        {
            array->delete_fn(array_get(array, i));
        }
    }
    free(array->memory);
    free(array);
}

void array_resize(struct array_s *array, size_t length)
{
    array->memory = realloc(array->memory, array->item_size * length);
    /* Shrinking array */
    if (array->length > length)
    {
        if (array->delete_fn)
        {
            size_t i = array->length - 1;
            for (; i >= length; --i)
            {
                array->delete_fn(array_get(array, i));
            }
        }
        array->length = length;
    }
    array->reserved = length;
}

void array_reserve(struct array_s *array, size_t length)
{
    if (array->reserved < length)
        array_resize(array, length);
}

void array_push(struct array_s *array, const void *items, size_t count)
{
    size_t new_size = array->reserved;

    while (new_size < array->length + count)
    {
        new_size += 2;
        new_size *= 2;
    }

    if (new_size != array->reserved)
        array_reserve(array, new_size);

    memcpy(array->memory + array->length * array->item_size, items, count * array->item_size);

    array->length += count;
}

void *array_get(struct array_s *array, size_t index)
{
    if (index > array->length)
        return NULL;

    return array->memory + array->item_size * index;
}

void array_set(struct array_s *array, size_t index, const void *item)
{
    if (index >= array->length)
        return;

    if (array->delete_fn)
    {
        array->delete_fn(array_get(array, index));
    }

    memcpy(array->memory + index * array->item_size, item, array->item_size);
}


