#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/

make_instr_func(iret){
    OPERAND r;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    
    r.data_size = 32;
    r.addr = cpu.esp;
    operand_read(&r);
    cpu.eip = r.val;
    cpu.esp += 4;

    r.data_size = 32;
    r.addr = cpu.esp;
    operand_read(&r);
    r.val &= 0xFFFF;
    cpu.segReg[1].val = r.val;
    //load_sreg(1);
    cpu.esp += 4;

    r.data_size = 32;
    r.addr = cpu.esp;
    operand_read(&r);
    cpu.eflags.val = r.val;
    cpu.esp += 4;

    return 0;
}

