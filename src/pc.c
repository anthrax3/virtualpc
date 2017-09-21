#include "pc.h"
#include "cpu.h"
#include "mem.h"
#include "is/basic/basic.h"

#include <stdio.h>

int
main(int argc, const char **argv)
{
    struct memory_s memory;
    int pc = 0;
    memory_init(&memory, 16 * 1024);
    struct cpu_s cpu;
    cpu_init(&cpu, &memory);
    iset_basic_init(cpu.iset);
    memory.data[pc++] = ISET_BASIC_HELLOWORLD;
    memory.data[pc++] = ISET_BASIC_LOADIMM8;
    memory.data[pc++] = ISET_BASIC_REG_R0;
    memory.data[pc++] = 0x34;
    memory.data[pc++] = ISET_BASIC_LOADIMM8;
    memory.data[pc++] = ISET_BASIC_REG_R1;
    memory.data[pc++] = 0x69;
    memory.data[pc++] = ISET_BASIC_ADD;
    memory.data[pc++] = ISET_BASIC_REG_R0;
    memory.data[pc++] = ISET_BASIC_REG_R1;
    memory.data[pc++] = ISET_BASIC_PRINT;
    memory.data[pc++] = ISET_BASIC_REG_R0;
    memory.data[pc++] = ISET_BASIC_HALT;
    cpu_run(&cpu);
    printf("CPU done\n");
    return 0;
}
