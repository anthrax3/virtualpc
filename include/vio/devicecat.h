/*
 * devicecat.h
 *
 *  Created on: Feb 4, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>

enum viod_category
{
	VDC_INPUT,
	VDC_OUTPUT,
	VDC_UNKNOWN
};

enum viod_vendor
{
	VDV_GENUINE_CAT,
	VDV_UNKNOWN
};

enum vdv_genuine_cat_devices
{
	VD_GENUINE_CAT_OUTPUT_TERMINAL
};
