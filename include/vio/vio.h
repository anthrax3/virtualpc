#pragma once

#include "../pc.h"
#include "devicecat.h"

/*
 *  Virtual Input-Output
 */

#define VIO_MAX_DEVICES 32

enum vio_flags
{
	VIOF_READY = (1 << 0),
	VIOF_BUSY = (1 << 1),
	VIOF_ERROR = (1 << 2)
};

struct vio_basic_interface_s
{
	uint32_t flags;
	uint32_t command;
	uint32_t registers[4];
};

struct vio_device_info_s
{
	uint32_t category;
	uint32_t vendor;
	uint32_t device;
	uint32_t map_length;
};

struct vio_device_implementation_s
{
	struct vio_device_info_s info;

	void(*init)(struct vio_device_s *device);
	void(*clock)(struct vio_device_s *device);
	void(*destroy)(struct vio_device_s *device);
};

struct vio_device_s
{
	struct vio_s *vio;
	struct vio_device_implementation_s implementation;

	struct
	{
		uint32_t id;
		void *internal_memory;
		void *mapped_memory;
	} state;

	struct vio_device_interface_s *interface;
};

struct vio_memory_s
{
	struct
	{
		struct vio_basic_interface_s interface;
		struct vio_basic_interface_s device_interfaces[VIO_MAX_DEVICES];
	} readwrite;

	struct
	{

	} readonly;


};

struct vio_s
{
	struct pc_s *pc;
	uint32_t next_device_id;
	struct vio_device_s devices[VIO_MAX_DEVICES];
	struct vio_memory_s memory;
};

void vio_init(struct vio_s *vio, struct pc_s *pc, uintptr_t address);
uint32_t vio_add_device(struct vio_s *vio, struct vio_device_implementation_s implementation);
void vio_clock(struct vio_s *vio);
