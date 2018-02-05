/*
 * term.c
 *
 *  Created on: Feb 4, 2018
 *      Author: nullifiedcat
 */

#include "term.h"

#include <stdlib.h>

struct term_mapped_memory_s
{
	char buffer[32];
};

// Exposed functions

void viod_term_init(struct vio_device_s *device)
{
}

void viod_term_clock(struct vio_device_s *device)
{
	switch (device->interface->basic_interface.command)
	{
	case VDCS_TERM_PRINT_STRING:
	{
		// Length of string is stored in the first register
		uint32_t length = device->interface->basic_interface.registers[0];
		uint32_t i = 0;
		struct term_mapped_memory_s *memory = (struct term_mapped_memory_s *) device->state.mapped_memory;
		for (; i < sizeof(memory->buffer) && i < length; ++i)
		{
			putc(memory->buffer[i], stdout);
		}
		// Reset the command
		device->interface->basic_interface.command = 0;
	}
	}
}

void viod_term_destroy(struct vio_device_s *device)
{
}

struct vio_device_implementation_s viod_term = {
	.init = viod_term_init,
	.clock = viod_term_clock,
	.destroy = viod_term_destroy,
	.info = (struct vio_device_info_s) {
		.category = VDC_OUTPUT,
		.vendor = VDV_GENUINE_CAT,
		.device = VD_GENUINE_CAT_OUTPUT_TERMINAL,
		.map_length = sizeof(struct term_mapped_memory_s)
	}
};
