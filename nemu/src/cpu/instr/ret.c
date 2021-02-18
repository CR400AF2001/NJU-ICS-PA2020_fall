#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/

make_instr_func(ret_near){
    
    OPERAND esp,ip;
    
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&esp);
    
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.sreg = SREG_SS;
    ip.addr = esp.val;
    
    operand_read(&ip);
    
    esp.val += 4;
    
    cpu.eip = ip.val;
    
    operand_write(&esp);
    
    return 0;
}

make_instr_func(ret_near_i)
{
    OPERAND esp,ip,imm;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&esp);
    
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.sreg = SREG_SS;
    ip.addr = esp.val;
    
    operand_read(&ip);
    
    imm.data_size = 16;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    
    operand_read(&imm);
    
    esp.val += 4;
    
    cpu.eip = ip.val;
    
    esp.val += imm.val;
    
    operand_write(&esp);
    
    return 0;
}