#ifndef MEM_H
#define MEM_H

#include <stdint.h>

struct memory_s
{
    uint8_t *data;
    uint32_t size;
};

void memory_init(struct memory_s *out, uint32_t size);

void memory_zero(struct memory_s *out);

void memory_free(struct memory_s *in);

#endif /* MEM_H */
