/*
 * basic.c
 *
 *  Created on: Sep 21, 2017
 *      Author: nullifiedcat
 */

#include "basic.h"
#include "../../mem.h"

#include <stdio.h>

void
i_halt(struct cpu_s *in)
{
    printf("HALT\n");
    in->state.halt = 1;
}

uint32_t*
iset_basic_addr_reg(struct cpu_s *in, enum iset_basic_regname name)
{
    if (name >= 0 && name < 4)
        return &in->state.regs.rx[name];
    return &in->state.regs.rx[0];
}

void
i_jmp8(struct cpu_s *in)
{
    uint8_t imm;
    imm = cpu_mem_read_immediate_byte(in);
    in->state.regs.pc = imm;
}

void
i_pop8(struct cpu_s *in)
{
/*    enum iset_basic_regname reg;
    reg = cpu_mem_read_immediate_byte(in);*/

}

void
i_loadm32(struct cpu_s *in)
{
    enum iset_basic_regname reg;
    uint32_t addr;
    reg = cpu_mem_read_immediate_byte(in);
    addr = cpu_mem_read_immediate_dword(in);
    *iset_basic_addr_reg(in, reg) = cpu_mem_read_remote_dword(in, addr);
}

void
i_loadimm8(struct cpu_s *in)
{
    enum iset_basic_regname reg;
    uint8_t imm;
    reg = cpu_mem_read_immediate_byte(in);
    imm = cpu_mem_read_immediate_byte(in);
    *iset_basic_addr_reg(in, reg) = imm;
}

void
i_add(struct cpu_s *in)
{
    enum iset_basic_regname reg1, reg2;
    reg1 = cpu_mem_read_immediate_byte(in);
    reg2 = cpu_mem_read_immediate_byte(in);
    *iset_basic_addr_reg(in, reg1) += *iset_basic_addr_reg(in, reg2);
}

void
i_print(struct cpu_s *in)
{
    enum iset_basic_regname reg;
    reg = cpu_mem_read_immediate_byte(in);
    printf("register %d: 0x%08x\n", reg, *iset_basic_addr_reg(in, reg));
}

void
i_hello_world(struct cpu_s *in)
{
    printf("hello world\n");
}

void
iset_basic_init(instruction_fn *iset_out)
{
    iset_out[ISET_BASIC_HALT] = &i_halt;
    iset_out[ISET_BASIC_HELLOWORLD] = &i_hello_world;
    iset_out[ISET_BASIC_LOADIMM8] = &i_loadimm8;
    iset_out[ISET_BASIC_ADD] = &i_add;
    iset_out[ISET_BASIC_PRINT] = &i_print;
    iset_out[ISET_BASIC_LOADM32] = &i_loadm32;
}
