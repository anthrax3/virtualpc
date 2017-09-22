#ifndef VIO_H_
#define VIO_H_

#include "../cpu.h"

/*
 *  Virtual Input-Output
 */

void
vio_init(void);

void
vio_call(struct cpu_s *cpu);

enum vio_calls
{
    VIO_NOP
};

#endif /* VIO_H_ */
