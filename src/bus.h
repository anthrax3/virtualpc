/*
 * bus.h
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "pc.h"

enum bus_address_check_flags
{
	BUS_CHECK_READABLE = (1 << 0),
	BUS_CHECK_WRITABLE = (1 << 1)
};

struct bus_memory_mapping_s
{
	uintptr_t address;
	uintptr_t length;
	void *memory;

	struct bus_memory_mapping_s *prev;
	struct bus_memory_mapping_s *next;
};

struct bus_s
{
	struct pc_s *pc;

	struct bus_memory_mapping_s *mappings;
};

void bus_init(struct bus_s *bus, struct pc_s *pc);
bool bus_address_check(struct bus_s *bus, enum bus_address_check_flags flags, uintptr_t address, uintptr_t length);
void bus_memory_map(struct bus_s *bus, uintptr_t address, uintptr_t length, void *memory);
void bus_memory_unmap(struct bus_s *bus, uintptr_t address);

uint8_t bus_read_byte(uintptr_t address);
uint16_t bus_read_word(uintptr_t address);
uint32_t bus_read_dword(uintptr_t address);

void bus_write_byte(uintptr_t address, uint8_t data);
void bus_write_word(uintptr_t address, uint16_t data);
void bus_write_dword(uintptr_t address, uint32_t data);

