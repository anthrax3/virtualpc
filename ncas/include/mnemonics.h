/*
  Created by nullifiedcat on 25.02.18.
*/

#pragma once

#include <stdint.h>

int instruction_from_mnemonic(const char *mnemonic, uint32_t *out,
                              uint32_t *size);