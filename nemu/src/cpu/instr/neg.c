#include "cpu/instr.h"
/*
Put the implementations of `neg' instructions here.
*/

make_instr_func(neg_rm_v)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    if(rm.val == 0){
        cpu.eflags.CF = 0;
    }    
    else{
        cpu.eflags.CF = 1;
    }
    rm.val = -rm.val;
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(neg_rm_b)
{
    OPERAND rm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    if(rm.val == 0){
        cpu.eflags.CF = 0;
    }    
    else{
        cpu.eflags.CF = 1;
    }
    rm.val = -rm.val;
    
    operand_write(&rm);
    
    return len;
}
