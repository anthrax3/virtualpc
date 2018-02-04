/*
 * main.c
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#include "pc.h"
#include "is/basic/basic.h"
#include "dev/term.h"

int
main(int argc, const char **argv)
{
	struct pc_s pc;

	pc_init(&pc);

	vio_add_device(&pc->vio, viod_term);

    int it = 0;
    iset_basic_init(pc.cpu.iset);
    pc.memory.data[it++] = ISET_BASIC_HELLOWORLD;
    pc.memory.data[it++] = ISET_BASIC_LOADIMM8;
    pc.memory.data[it++] = ISET_BASIC_REG_R0;
    pc.memory.data[it++] = 0x34;
    pc.memory.data[it++] = ISET_BASIC_LOADIMM8;
    pc.memory.data[it++] = ISET_BASIC_REG_R1;
    pc.memory.data[it++] = 0x69;
    pc.memory.data[it++] = ISET_BASIC_ADD;
    pc.memory.data[it++] = ISET_BASIC_REG_R0;
    pc.memory.data[it++] = ISET_BASIC_REG_R1;
    pc.memory.data[it++] = ISET_BASIC_PRINT;
    pc.memory.data[it++] = ISET_BASIC_REG_R0;

    pc.memory.data[it++] = ISET_BASIC_LOADM32;
    pc.memory.data[it++] = ISET_BASIC_REG_R2;
    cpu_mem_write_remote_dword(&pc.cpu, 0x0100, 0x12345678);
    cpu_mem_write_remote_dword(&pc.cpu, it, 0x0100);
    it += 4;
    pc.memory.data[it++] = ISET_BASIC_PRINT;
    pc.memory.data[it++] = ISET_BASIC_REG_R2;

    pc.memory.data[it++] = ISET_BASIC_LOADM32;
    pc.memory.data[it++] = ISET_BASIC_REG_R3;
    cpu_mem_write_remote_dword(&pc.cpu, it, 0x12345678);
    it += 4;
    pc.memory.data[it++] = ISET_BASIC_PRINT;
    pc.memory.data[it++] = ISET_BASIC_REG_R3;

    pc.memory.data[it++] = ISET_BASIC_HELLOWORLD;
    pc.memory.data[it++] = ISET_BASIC_HALT;
    cpu_run(&pc.cpu);
    printf("CPU done\n");
    return 0;
}


