/*
 * bus.h
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

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
	BER_ALIGN
};

struct bus_memory_mapping_s
{
	uint8_t permissions;
	uintptr_t address;
	uintptr_t length;
	void *memory;

	struct bus_memory_mapping_s *prev;
	struct bus_memory_mapping_s *next;
};

struct bus_s
{
	enum bus_error error;
	struct pc_s *pc;
	struct bus_memory_mapping_s *mappings;
};

void bus_init(struct bus_s *bus, struct pc_s *pc);
struct bus_memory_mapping_s *bus_map_at(struct bus_s *bus, uintptr_t address);

enum bus_error bus_address_check(struct bus_s *bus, uint8_t permissions, uintptr_t address, uintptr_t length);
enum bus_error bus_address_check_in_map(struct bus_s *bus, struct bus_memory_mapping_s *mapping, uint8_t permissions, uintptr_t address, uintptr_t length);
enum bus_error bus_memory_map(struct bus_s *bus, uintptr_t address, uintptr_t length, void *memory, uint8_t permissions);
enum bus_error bus_memory_unmap(struct bus_s *bus, uintptr_t address);

void bus_error_clear(struct bus_s *bus);
enum bus_error bus_error_state(struct bus_s *bus);

/* Read operations can modify bus->error */
uint8_t bus_read_byte(struct bus_s *bus, uintptr_t address);
uint16_t bus_read_word(struct bus_s *bus, uintptr_t address);
uint32_t bus_read_dword(struct bus_s *bus, uintptr_t address);

enum bus_error bus_write_byte(struct bus_s *bus, uintptr_t address, uint8_t data);
enum bus_error bus_write_word(struct bus_s *bus, uintptr_t address, uint16_t data);
enum bus_error bus_write_dword(struct bus_s *bus, uintptr_t address, uint32_t data);

