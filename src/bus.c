/*
 * bus.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "bus.h"

#include <stdlib.h>
#include <string.h>

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
		if (address >= current->address && address <= current->address + current->length)
		{
			return current;
		}

		current = current->next;
	}

	return NULL;
}

enum bus_error bus_address_check(struct bus_s *bus, struct bus_memory_mapping_s *mapping, uintptr_t address, uintptr_t length)
{
	/* Mapping must exist */
	if (mapping == NULL)
		return BER_EXIST;

	/* The requested region must be located completely inside the mapping */
	uintptr_t map_remaining_length = mapping->length + mapping->address - address;

	if (length > map_remaining_length)
		return BER_OVERLAP;

	return BER_SUCCESS;
}

enum bus_error bus_memory_map(struct bus_s *bus, uintptr_t address, uintptr_t length, void *user, bus_function read, bus_function write)
{
	struct bus_memory_mapping_s *current = bus->mappings;

	/* Address must be 256-byte aligned */
	if (address & 0xFF)
		return BER_ALIGN;

	if (length & 0xFF)
		length += 0x100;
	length &= 0xFF;

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
	mapping->user = user;
	mapping->read = read;
	mapping->write = write;

	return BER_SUCCESS;
}

enum bus_error bus_memory_unmap(struct bus_s *bus, uintptr_t address)
{
	/* to be implemented */

	return BER_OTHER;
}

enum bus_error bus_read(struct bus_s *bus, uintptr_t address, uintptr_t length, void *buffer)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check(bus, mapping, address, length);

	if (error != BER_SUCCESS)
		return error;

	return mapping->read(mapping->user, address - mapping->address, length, buffer);
}

enum bus_error bus_write(struct bus_s *bus, uintptr_t address, uintptr_t length, void *buffer)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check(bus, mapping, address, length);

	if (error != BER_SUCCESS)
		return error;

	return mapping->write(mapping->user, address - mapping->address, length, buffer);
}
