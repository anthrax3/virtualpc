#ifndef VIO_H_
#define VIO_H_

#include "../cpu.h"

/*
 *  Virtual Input-Output
 */

struct vio_device_description_s
{
	uint32_t category;
	uint32_t vendor;
	uint32_t device;
	uint32_t map_length;
};

struct vio_memory_s
{
	uint32_t flags;
	uint32_t command;
	uint32_t registers[4];
	struct vio_device_description_s devices[32];
};

void
vio_init(void);

void
vio_call(struct cpu_s *cpu);

enum vio_calls
{
    VIO_NOP
};

#endif /* VIO_H_ */
