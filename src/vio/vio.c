/*
 * vio.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "vio/vio.h"
#include "bus.h"

enum bus_error vio_bus_read(struct vio_s *vio, uintptr_t address, uintptr_t length, void *buffer);
enum bus_error vio_bus_write(struct vio_s *vio, uintptr_t address, uintptr_t length, void *buffer);

void vio_init(struct vio_s *vio, struct pc_s *pc, uintptr_t address)
{
	memset(vio, 0, sizeof(struct vio_s));

	vio->pc = pc;
	vio->next_device_id = 1;
}

uint32_t vio_add_device(struct vio_s *vio, struct vio_device_implementation_s implementation)
{
	uint32_t i = 0;

	for (; i < VIO_MAX_DEVICES; ++i)
	{
		if (vio->devices[i].state.id == 0)
		{
			uint32_t id = vio->next_device_id;

			memset(&vio->devices[i], 0, sizeof(struct vio_internal_device_s));

			vio->devices[i].vio = vio;
			vio->devices[i].implementation = implementation;
			vio->devices[i].state.id = id;
			vio->devices[i].interface = &vio->memory.readwrite.device_interface[i];

			memset(&vio->memory.readwrite.device_interface[i], 0, sizeof(struct vio_device_interface_s));
			vio->memory.readonly.device_information[i] = implementation.info;

			implementation.init(&vio->devices[i]);

			vio->next_device_id++;
			return id;
		}
	}

	return 0;
}

void vio_clock(struct vio_s *vio)
{

}

enum bus_error vio_bus_read(struct vio_s *vio, uintptr_t address, uintptr_t length, void *buffer)
{


	return BER_SUCCESS;
}

enum bus_error vio_bus_write(struct vio_s *vio, uintptr_t address, uintptr_t length, void *buffer)
{
	return BER_SUCCESS;
}


