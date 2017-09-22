#ifndef BASIC_H_
#define BASIC_H_

#include "../../cpu.h"

enum iset_basic_regname
{
    ISET_BASIC_REG_R0,
    ISET_BASIC_REG_R1,
    ISET_BASIC_REG_R2,
    ISET_BASIC_REG_R3,

    ISET_BASIC_REG_PC
};

uint32_t*
iset_basic_addr_reg(struct cpu_s *in, enum iset_basic_regname name);

enum iset_basic
{
    ISET_BASIC_HALT = 0,
    ISET_BASIC_HELLOWORLD,
    ISET_BASIC_LOADIMM8,
    ISET_BASIC_ADD,
    ISET_BASIC_PRINT,
    ISET_BASIC_JMP8,
    ISET_BASIC_PUSH8,
    ISET_BASIC_POP8,
    ISET_BASIC_LOADR,
    ISET_BASIC_SUB,
    ISET_BASIC_LOADM32,

    ISET_BASIC_LOADIMM16,
    ISET_BASIC_LOADIMM32,

    ISET_BASIC_VIO_CALL
};

void
iset_basic_init(instruction_fn *iset_out);

#endif /* BASIC_H_ */
