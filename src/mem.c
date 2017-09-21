#include "mem.h"

#include <stdlib.h>
#include <string.h>

void
memory_init(struct memory_s *out, uint32_t size)
{
    out->data = calloc(size, sizeof(uint8_t));
    out->size = size;
}

void
memory_zero(struct memory_s *out)
{
    memset(out->data, 0, out->size);
}

void
memory_free(struct memory_s *in)
{
    free(in->data);
}
