/*
 * parser.h
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include "array.h"

struct ncas_context_s
{
    uint32_t counter;

    struct array_s *result;
};
