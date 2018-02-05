/*
 * main.c
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#include "pc.h"
#include "cpu.h"
#include "vio/vio.h"
#include "vio/dev/term.h"

#include <stdio.h>

int
main(int argc, const char **argv)
{
	struct pc_s pc;

	pc_init(&pc);
	vio_add_device(&pc.vio, viod_term);

	cpu_flag_set(&pc.cpu, CF_DEBUGF);

    cpu_start(&pc.cpu);
    printf("CPU done\n");
    return 0;
}


