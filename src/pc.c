#include "pc.h"
#include "cpu.h"
#include "mem.h"
#include "is/basic/basic.h"

#include <stdio.h>

int
main(int argc, const char **argv)
{
    struct memory_s memory;
    int it = 0;
    memory_init(&memory, 16 * 1024);
    struct cpu_s cpu;
    cpu_init(&cpu, &memory);
    iset_basic_init(cpu.iset);
    memory.data[it++] = ISET_BASIC_HELLOWORLD;
    memory.data[it++] = ISET_BASIC_LOADIMM8;
    memory.data[it++] = ISET_BASIC_REG_R0;
    memory.data[it++] = 0x34;
    memory.data[it++] = ISET_BASIC_LOADIMM8;
    memory.data[it++] = ISET_BASIC_REG_R1;
    memory.data[it++] = 0x69;
    memory.data[it++] = ISET_BASIC_ADD;
    memory.data[it++] = ISET_BASIC_REG_R0;
    memory.data[it++] = ISET_BASIC_REG_R1;
    memory.data[it++] = ISET_BASIC_PRINT;
    memory.data[it++] = ISET_BASIC_REG_R0;

    memory.data[it++] = ISET_BASIC_LOADM32;
    memory.data[it++] = ISET_BASIC_REG_R2;
    cpu_mem_write_remote_dword(&cpu, 0x0100, 0x12345678);
    cpu_mem_write_remote_dword(&cpu, it, 0x0100);
    it += 4;
    memory.data[it++] = ISET_BASIC_PRINT;
    memory.data[it++] = ISET_BASIC_REG_R2;

    memory.data[it++] = ISET_BASIC_LOADM32;
    memory.data[it++] = ISET_BASIC_REG_R3;
    cpu_mem_write_remote_dword(&cpu, it, 0x12345678);
    it += 4;
    memory.data[it++] = ISET_BASIC_PRINT;
    memory.data[it++] = ISET_BASIC_REG_R3;

    memory.data[it++] = ISET_BASIC_HELLOWORLD;
    memory.data[it++] = ISET_BASIC_HALT;
    cpu_run(&cpu);
    printf("CPU done\n");
    return 0;
}
