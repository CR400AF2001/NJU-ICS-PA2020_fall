#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/

make_instr_func(inc_rm){
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    
    rm.val = alu_add(1, rm.val, data_size);
    
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    
    operand_write(&rm);
    
    return len;
}

make_instr_func(inc_r){
    OPERAND r;
    
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = opcode & 0x7;
    
    operand_read(&r);
    
    int temp = 0;
    if(cpu.eflags.CF == 1){
        temp = 1;
    }
    
    r.val = alu_add(1, r.val, data_size);
    
    if(temp == 1){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
    
    operand_write(&r);
    
    return 1;
}