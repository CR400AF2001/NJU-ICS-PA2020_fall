#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/

make_instr_func(lgdt){
    OPERAND m;
    
    m.data_size = 16;
    int len = 1;
    len += modrm_rm(eip + 1, &m);
    
    operand_read(&m);
    
    cpu.gdtr.limit = m.val;
    
    m.data_size = 32;
    m.addr += 2;
    
    operand_read(&m);
    
    cpu.gdtr.base = m.val;
    
    return len;
}
