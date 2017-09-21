#ifndef BASIC_H_
#define BASIC_H_

#include "../../cpu.h"

enum iset_basic_regname
{
    ISET_BASIC_REG_R0,
    ISET_BASIC_REG_R1,
    ISET_BASIC_REG_R2,
    ISET_BASIC_REG_R3
};

uint32_t*
iset_basic_addr_reg(struct cpu_s *in, enum iset_basic_regname name);

enum iset_basic
{
    ISET_BASIC_HALT,
    ISET_BASIC_HELLOWORLD,
    ISET_BASIC_LOADIMM8,
    ISET_BASIC_ADD,
    ISET_BASIC_PRINT
};

void
iset_basic_init(instruction_fn *iset_out);

#endif /* BASIC_H_ */
