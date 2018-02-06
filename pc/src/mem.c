#include "mem.h"
#include "bus.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum bus_error mem_bus_read(struct memory_s *memory, uint32_t address,
                            uint32_t length, void *buffer);
enum bus_error mem_bus_write(struct memory_s *memory, uint32_t address,
                             uint32_t length, void *buffer);

void memory_init(struct memory_s *out, uint32_t size)
{
    out->data = calloc(size, sizeof(uint8_t));
    out->size = size;
}

void memory_zero(struct memory_s *out)
{
    memset(out->data, 0, out->size);
}

void memory_free(struct memory_s *in)
{
    free(in->data);
}

void memory_map(struct memory_s *memory, struct bus_s *bus, uint32_t address)
{
    enum bus_error error = bus_memory_map(bus, address, memory->size, memory,
                                          (bus_function) mem_bus_read,
                                          (bus_function) mem_bus_write);

    printf("Memory (%08x): bus status: %d\n", memory->size, error);
}

enum bus_error mem_bus_read(struct memory_s *memory, uint32_t address,
                            uint32_t length, void *buffer)
{
    if (address + length > memory->size)
        return BER_RANGE;

    memcpy(buffer, memory->data + address, length);

    return BER_SUCCESS;
}

enum bus_error mem_bus_write(struct memory_s *memory, uint32_t address,
                             uint32_t length, void *buffer)
{
    if (address + length > memory->size)
        return BER_RANGE;

    memcpy(memory->data + address, buffer, length);

    return BER_SUCCESS;
}
