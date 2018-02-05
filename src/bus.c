/*
 * bus.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "bus.h"

#include <stdlib.h>

void bus_init(struct bus_s *bus, struct pc_s *pc)
{
	memset(bus, 0, sizeof(struct bus_s));

	bus->pc = pc;
}

struct bus_memory_mapping_s *bus_map_at(struct bus_s *bus, uintptr_t address)
{
	struct bus_memory_mapping_s *current;

	current = bus->mappings;

	while (current)
	{
		if (address >= current->address && address <= current->address + current->length);
		{
			return current;
		}

		current = current->next;
	}

	return NULL;
}

enum bus_error bus_address_check(struct bus_s *bus, uint8_t permissions, uintptr_t address, uintptr_t length)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	/* Mapping must exist */
	if (mapping == NULL)
		return BER_EXIST;

	/* Permissions must be set */
	if (permissions & mapping->permissions != permissions)
		return BER_ACCESS;

	/* The requested region must be located completely inside the mapping */
	uintptr_t map_remaining_length = mapping->length + mapping->address - address;

	if (length > map_remaining_length)
		return BER_OVERLAP;

	return BER_SUCCESS;
}

enum bus_error bus_memory_map(struct bus_s *bus, uintptr_t address, uintptr_t length, void *memory, uint8_t permissions)
{
	struct bus_memory_mapping_s *current = bus->mappings;

	while (current)
	{
		/* current region ends after new one starts */
		if (current->address + current->length >= address)
		{
			/* current region begins before new one ends */
			if (current->address < address + length)
			{
				/* regions collide */
				return BER_OVERLAP;
			}
		}

		if (current->next == NULL)
			break;

		current = current->next;
	}

	struct bus_memory_mapping_s *mapping = calloc(1, sizeof(struct bus_memory_mapping_s));

	current->next = mapping;
	mapping->prev = current;
	mapping->address = address;
	mapping->length = length;
	mapping->memory = memory;
	mapping->permissions = permissions;

	return BER_SUCCESS;
}

enum bus_error bus_memory_unmap(struct bus_s *bus, uintptr_t address)
{
	return false;
}

uint8_t bus_read_byte(uintptr_t address);
uint16_t bus_read_word(uintptr_t address);
uint32_t bus_read_dword(uintptr_t address);

void bus_write_byte(uintptr_t address, uint8_t data);
void bus_write_word(uintptr_t address, uint16_t data);
void bus_write_dword(uintptr_t address, uint32_t data);


