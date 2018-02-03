#pragma once

#include "mem.h"
#include "cpu.h"
#include "exception.h"

struct
pc_s
{
	struct memory_s memory;
	struct cpu_s cpu;
};

void pc_init(struct pc_s *pc);
void pc_raise_exception(struct pc_s *pc, enum pc_exception_category category, int data);
