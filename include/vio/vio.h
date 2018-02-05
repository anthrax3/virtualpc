#pragma once

#include "devicecat.h"

/*
 *  Virtual Input-Output
 */

#define VIO_MAX_DEVICES 32
#define VIO_GP_REGISTERS 8
#define VIO_DATA_SIZE 64

struct vio_internal_device_s;

struct vio_device_interface_s
{
	uint32_t registers[VIO_GP_REGISTERS];
	uint8_t  data[VIO_DATA_SIZE];
};

struct vio_device_info_s
{
	uint32_t category;
	uint32_t vendor;
	uint32_t device;
};

struct vio_device_implementation_s
{
	struct vio_device_info_s info;

	void(*init)(struct vio_internal_device_s *device);
	void(*clock)(struct vio_internal_device_s *device);
	void(*destroy)(struct vio_internal_device_s *device);
};

struct vio_internal_device_s
{
	struct vio_s *vio;
	struct vio_device_implementation_s implementation;

	struct
	{
		uint32_t id;
		void *internal_memory;
	} state;

	struct vio_device_interface_s *interface;
};

struct vio_memory_s
{
	struct
	{
		struct vio_device_interface_s interface;
		struct vio_device_interface_s device_interface[VIO_MAX_DEVICES];
	} readwrite;

	struct
	{
		struct vio_device_info_s device_information[VIO_MAX_DEVICES];
	} readonly;
};

struct vio_s
{
	struct pc_s *pc;
	uint32_t next_device_id;
	struct vio_internal_device_s devices[VIO_MAX_DEVICES];
	struct vio_memory_s memory;
};

void vio_init(struct vio_s *vio, struct pc_s *pc, uintptr_t address);
uint32_t vio_add_device(struct vio_s *vio, struct vio_device_implementation_s implementation);
void vio_clock(struct vio_s *vio);
