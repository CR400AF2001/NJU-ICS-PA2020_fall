#include "cpu/instr.h"
/*
Put the implementations of `not' instructions here.
*/

make_instr_func(not_rm_v)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    rm.val = ~rm.val;
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(not_rm_b)
{
    OPERAND rm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    rm.val = ~rm.val;
    
    operand_write(&rm);
    
    return len;
}
