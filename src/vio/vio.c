/*
 * vio.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#include "vio/vio.h"
#include "bus.h"
#include "pc.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

enum bus_error vio_bus_read(struct vio_s *vio, uint32_t address,
                            uint32_t length, void *buffer);
enum bus_error vio_bus_write(struct vio_s *vio, uint32_t address,
                             uint32_t length, void *buffer);

void vio_init(struct vio_s *vio, struct pc_s *pc, uint32_t address)
{
    memset(vio, 0, sizeof(struct vio_s));

    vio->pc             = pc;
    vio->next_device_id = 1;

    enum bus_error error = bus_memory_map(&pc->bus, address, sizeof(struct vio_memory_s), vio, (bus_function)vio_bus_read, (bus_function)vio_bus_write);
    printf("Virtual I/O: bus status %d\n", error);
}

uint32_t vio_add_device(struct vio_s *vio,
                        struct vio_device_implementation_s implementation)
{
    uint32_t i = 0;

    for (; i < VIO_MAX_DEVICES; ++i)
    {
        if (vio->devices[i].state.id == 0)
        {
            uint32_t id = vio->next_device_id;

            memset(&vio->devices[i], 0, sizeof(struct vio_internal_device_s));

            vio->devices[i].vio            = vio;
            vio->devices[i].implementation = implementation;
            vio->devices[i].state.id       = id;
            vio->devices[i].interface =
                &vio->memory.readwrite.device_interface[i];

            memset(&vio->memory.readwrite.device_interface[i], 0,
                   sizeof(struct vio_device_interface_s));
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
    uint32_t i = 0;

    for (; i < VIO_MAX_DEVICES; ++i)
    {
        vio->devices[i].implementation.clock(&vio->devices[i]);
    }
}

enum bus_error vio_bus_read(struct vio_s *vio, uint32_t address,
                            uint32_t length, void *buffer)
{
    /* I'll just leave it like that for now.
     * Later I'll have to implement internal boundary checks */

    if (address + length >= sizeof(struct vio_memory_s))
        return BER_RANGE;

    memcpy(buffer, ((uint8_t *) &vio->memory) + address, length);

    return BER_SUCCESS;
}

enum bus_error vio_bus_write(struct vio_s *vio, uint32_t address,
                             uint32_t length, void *buffer)
{
    /* Same as with _read */

    if (address + length >= sizeof(struct vio_memory_s))
        return BER_RANGE;

    if (address >= offsetof(struct vio_memory_s, readonly))
        return BER_ACCESS;

    if (address + length >= offsetof(struct vio_memory_s, readonly))
        return BER_ALIGN;

    memcpy(((uint8_t *) &vio->memory) + address, buffer, length);

    return BER_SUCCESS;
}
