/*
 * cat.h
 *
 *  Created on: Feb 5, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include "../../cpu.h"

cpu_instruction cat_cpu_instruction_lookup(uint8_t size, uint32_t instruction);
