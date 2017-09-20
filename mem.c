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

uint8_t
memory_read(struct memory_s *in, uint32_t addr)
{
    if (addr < in->size)
    {
        return in->data[addr];
    }
    return 0xFF;
}

void
memory_write(struct memory_s *out, uint32_t addr, uint8_t data)
{
    if (addr < out->size)
    {
        out->data[addr] = data;
    }
}

void
memory_free(struct memory_s *in)
{
    free(in->data);
}