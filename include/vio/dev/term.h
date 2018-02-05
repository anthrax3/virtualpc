/*
 * term.h
 *
 *  Created on: Feb 4, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include "vio/vio.h"

enum viod_term_commands
{
	VDCS_TERM_PRINT = 1
};

extern struct vio_device_implementation_s viod_term;
