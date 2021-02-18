#ifndef __CPU_H__
#define __CPU_H__

// interface for cpu functions

#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/alu.h"
#include "cpu/reg_fpu.h"

extern CPU_STATE cpu;

// initialize the cpu states
void init_cpu();

// execute n instructions starting from the current eip
// change eip according to the length of the instruction in each step
void exec(uint32_t n);

// execute an instruction pointed by the current eip
// return the length of the instruction
int exec_inst();

// do interrupt
void do_intr();

#endif
