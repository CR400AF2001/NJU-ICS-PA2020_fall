#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/


make_instr_func(leave)
{
    OPERAND r, esp, ebp;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    ebp.data_size = 32;
    ebp.type = OPR_REG;
    ebp.addr = 0x5;
    
    operand_read(&ebp);
    
    esp.val = ebp.val;
    
    r.data_size = 32;
    r.type = OPR_MEM;
    r.sreg = SREG_SS;
    r.addr = esp.val;
    
    operand_read(&r);
    
    esp.val += 4;
    
    ebp.data_size = 32;
    ebp.val = r.val;
    
    operand_write(&esp);
    operand_write(&ebp);
    
    return 1;
}