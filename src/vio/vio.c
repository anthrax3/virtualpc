/*
 * vio.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "vio/vio.h"

void vio_init(struct vio_s *vio, struct pc_s *pc, uintptr_t address)
{
	memset(vio, 0, sizeof(struct vio_s));

	vio->pc = pc;
}

uint32_t vio_add_device(struct vio_s *vio, struct vio_device_implementation_s handlers)
{
	uint32_t id = 0;

	vio->devices.state.id;

	return 0;
}

void vio_clock(struct vio_s *vio)
{

}


