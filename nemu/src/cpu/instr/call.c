#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call_near)
{
    OPERAND rel,esp,ip;
    
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&esp);
    
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.val = cpu.eip + 1 + data_size / 8;
    
    esp.val -= 4;
    
    ip.addr = esp.val;
    
    operand_write(&esp);
    operand_write(&ip);
    
    cpu.eip += offset;
    return 1 + data_size / 8;
}

make_instr_func(call_near_indirect)
{
    OPERAND rm, esp, ip;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&esp);
    
    ip.data_size = 32;
    ip.type = OPR_MEM;
    ip.val = cpu.eip + len;
    
    esp.val -= 4;
    
    ip.addr = esp.val;
    
    operand_write(&esp);
    operand_write(&ip);
    
    if(data_size == 16){
        rm.val = sign_ext(rm.val, data_size);
    }
    cpu.eip = rm.val;
    
    return 0;
}