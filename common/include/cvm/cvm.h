/*
 * cvm.h
 *
 *  Created on: Feb 6, 2018
 *      Author: nullifiedcat
 */

#pragma once

#include <stdint.h>

/*
 * A cvm file describes the virtual machine and contains data that will be
 * mapped to machine's address space
 * This is very basic format right now, and there is only one type of section
 * The format is a bit inspired by ELF
 */

enum cvm_section_type
{
    CVM_S_NONE,
    CVM_S_CUSTOM,
    CVM_S_BINARY,
    CVM_S_COUNT
};

struct cvm_sechion_header_s
{
    uint32_t type;
    uint32_t header_size;
    uint32_t offset;
    uint32_t length;
    uint32_t address;
};

struct cvm_header_s
{
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t section_count;
    uint32_t section_table_offset;
};
