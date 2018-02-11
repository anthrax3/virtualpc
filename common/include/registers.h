
/*
 * registers.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>

struct register_description_s
{
    /* Offset from base register pointer in CPU */
    uint32_t offset;
    /* Size in bytes */
    uint32_t size;
    /* Can be accessed? */
    uint8_t exposed;
    /* Name as short string */
    char name[8];
};

enum register_name
{
    REGISTER_A,
    REGISTER_B,
    REGISTER_C,
    REGISTER_D,
    REGISTER_E,
    REGISTER_F,
    REGISTER_G,
    REGISTER_H,

    REGISTER_PC,
    REGISTER_SP,
    REGISTER_BP,
    REGISTER_FLAGS,

    REGISTER_COUNT
};

const struct register_description_s *register_get(uint32_t name);
