/*
 * main.c
 *
 *  Created on: Feb 3, 2018
 *      Author: nullifiedcat
 */

#include "pc.h"
#include "vio/dev/term.h"
#include "cpu/implcat.h"

#include <stdio.h>
#include <string.h>

#define INSTR_MAKE_HEAD(size, op1, op2)                                        \
    CPU_INSTRUCTION_LENGTH_##size | (CPU_OPERAND_MODE_##op1 << 3) |            \
        CPU_OPERAND_MODE_##op2

void test_bus(struct pc_s *pc)
{
    struct
    {
        uint32_t data[4];
    } test_struct;

    test_struct.data[0] = 123;
    test_struct.data[1] = 456;
    test_struct.data[2] = 789;
    test_struct.data[3] = 321;

    printf("test: bus write struct: %d\n",
           bus_write(&pc->bus, 0x100000, sizeof(test_struct), &test_struct));

    memset(&test_struct, 0, sizeof(test_struct));

    printf("test: bus read struct: %d\n",
           bus_read(&pc->bus, 0x100000, sizeof(test_struct), &test_struct));

    printf("test: %u %u %u %u\n", test_struct.data[0], test_struct.data[1],
           test_struct.data[2], test_struct.data[3]);
}

int main(int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    struct pc_s pc;

    pc_init(&pc);
    vio_add_device(&pc.vio, viod_term);

    pc.cpu.implementation         = cat_cpu_instruction_lookup;
    pc.cpu.state.regs.flags.debug = 1;

    /*test_bus(&pc);*/

    cpu_start(&pc.cpu);
    printf("CPU done\n");
    return 0;
}
