#include "pc.h"

#include <stdio.h>

void pc_init(struct pc_s *pc)
{
    memory_init(&pc->memory, 16 * 1024);
    cpu_init(&pc->cpu, &pc->memory);
}

void pc_raise_exception(struct pc_s *pc, enum pc_exception_category category, int data)
{

}
