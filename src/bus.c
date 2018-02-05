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

enum bus_error bus_address_check(struct bus_s *bus, uint8_t permissions, uintptr_t address, uintptr_t length)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	return bus_address_check_in_map(bus, mapping, permissions, address, length);
}

enum bus_error bus_address_check_in_map(struct bus_s *bus, struct bus_memory_mapping_s *mapping, uint8_t permissions, uintptr_t address, uintptr_t length)
{
	/* Mapping must exist */
	if (mapping == NULL)
		return BER_EXIST;

	/* Permissions must be set */
	if ((permissions & mapping->permissions) != permissions)
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

	if (address % 4 || length % 4)
	{
		return BER_ALIGN;
	}

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
	/* to be implemented */

	return BER_OTHER;
}

void bus_error_clear(struct bus_s *bus)
{
	bus->error = BER_SUCCESS;
}

enum bus_error bus_error_state(struct bus_s *bus)
{
	return bus->error;
}

uint8_t bus_read_byte(struct bus_s *bus, uintptr_t address)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_READABLE, address, 1);

	if (error != BER_SUCCESS)
	{
		bus->error = error;
		return 0;
	}

	return ((uint8_t *)mapping->memory)[address - mapping->address];
}

uint16_t bus_read_word(struct bus_s *bus, uintptr_t address)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_READABLE, address, 2);

	if (error != BER_SUCCESS)
	{
		bus->error = error;
		return 0;
	}

	return *(uint16_t *)((uint8_t *)mapping->memory + (address - mapping->address));
}

uint32_t bus_read_dword(struct bus_s *bus, uintptr_t address)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_READABLE, address, 4);

	if (error != BER_SUCCESS)
	{
		bus->error = error;
		return 0;
	}

	return *(uint32_t *)((uint8_t *)mapping->memory + (address - mapping->address));
}

enum bus_error bus_write_byte(struct bus_s *bus, uintptr_t address, uint8_t data)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_WRITABLE, address, 1);

	if (error != BER_SUCCESS)
		return error;

	((uint8_t *)mapping->memory)[address - mapping->address] = data;

	return BER_SUCCESS;
}

enum bus_error bus_write_word(struct bus_s *bus, uintptr_t address, uint16_t data)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_WRITABLE, address, 2);

	if (error != BER_SUCCESS)
		return error;

	*(uint16_t *)((uint8_t *)mapping->memory + (address - mapping->address)) = data;

	return BER_SUCCESS;
}

enum bus_error bus_write_dword(struct bus_s *bus, uintptr_t address, uint32_t data)
{
	struct bus_memory_mapping_s *mapping = bus_map_at(bus, address);

	enum bus_error error = bus_address_check_in_map(bus, mapping, BPERM_WRITABLE, address, 4);

	if (error != BER_SUCCESS)
		return error;

	*(uint32_t *)((uint8_t *)mapping->memory + (address - mapping->address)) = data;

	return BER_SUCCESS;
}


