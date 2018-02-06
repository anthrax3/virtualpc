/*
 * term.c
 *
 *  Created on: Feb 4, 2018
 *      Author: nullifiedcat
 */

#include "vio/dev/term.h"
#include "vio/vio.h"

#include <stdlib.h>
#include <stdio.h>

void viod_term_init(struct vio_internal_device_s *device)
{
}

void viod_term_clock(struct vio_internal_device_s *device)
{
    switch (device->interface->registers[0])
    {
    case VDCS_TERM_PRINT:
    {
        /* Length of string is stored in the first register */
        uint32_t length = device->interface->registers[1];
        uint32_t i      = 0;

        for (; i < VIO_DATA_SIZE && i < length; ++i)
        {
            putc(device->interface->data[i], stdout);
        }
        /* Reset the command */
        device->interface->registers[0] = 0;
    }
    }
}

void viod_term_destroy(struct vio_internal_device_s *device)
{
}

struct vio_device_implementation_s viod_term = {
    .init    = viod_term_init,
    .clock   = viod_term_clock,
    .destroy = viod_term_destroy,
    .info =
        (struct vio_device_info_s){.category = VDC_OUTPUT,
                                   .vendor   = VDV_GENUINE_CAT,
                                   .device   = VD_GENUINE_CAT_OUTPUT_TERMINAL }
};
