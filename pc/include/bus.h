/*
 * bus.h
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

struct pc_s;

enum bus_memory_permission
{
    BPERM_READABLE = (1 << 0),
    BPERM_WRITABLE = (1 << 1)
};

enum bus_error
{
    BER_SUCCESS = 0,
    BER_OTHER,
    BER_EXIST,
    BER_ACCESS,
    BER_OVERLAP,
    BER_RANGE,
    BER_ALIGN
};

typedef enum bus_error (*bus_function)(void *, uint32_t, uint32_t, void *);

struct bus_memory_mapping_s
{
    uint32_t address;
    uint32_t length;

    void *user;
    enum bus_error (*read)(void *user, uint32_t address, uint32_t length,
                           void *buffer);
    enum bus_error (*write)(void *user, uint32_t address, uint32_t length,
                            void *buffer);

    struct bus_memory_mapping_s *prev;
    struct bus_memory_mapping_s *next;
};

struct bus_s
{
    struct pc_s *pc;
    struct bus_memory_mapping_s *mappings;
};

void bus_init(struct bus_s *bus, struct pc_s *pc);
struct bus_memory_mapping_s *bus_map_at(struct bus_s *bus, uint32_t address);

enum bus_error bus_address_check(struct bus_s *bus,
                                 struct bus_memory_mapping_s *mapping,
                                 uint32_t address, uint32_t length);
enum bus_error bus_memory_map(struct bus_s *bus, uint32_t address,
                              uint32_t length, void *user, bus_function read,
                              bus_function write);
enum bus_error bus_memory_unmap(struct bus_s *bus, uint32_t address);

enum bus_error bus_read(struct bus_s *bus, uint32_t address, uint32_t length,
                        void *buffer);
enum bus_error bus_write(struct bus_s *bus, uint32_t address, uint32_t length,
                         void *buffer);
