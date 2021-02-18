#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

make_instr_func(pop_r_v){
    
    OPERAND r,esp;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&esp);
    
    r.data_size = 32;
    r.type = OPR_MEM;
    r.sreg = SREG_SS;
    r.addr = esp.val;
    
    operand_read(&r);
    
    esp.val += 4;
    
    r.addr = opcode & 0x7;
    r.type = OPR_REG;
    r.data_size = data_size;
    
    operand_write(&esp);
    operand_write(&r);
    
    return 1;
}


make_instr_func(popa){
    OPERAND r;
    r.data_size = 32;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    
    r.addr = cpu.esp;
    operand_read(&r);
    cpu.edi = r.val;
    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.esi = r.val;
    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.ebp = r.val;
    cpu.esp += 4;

    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.ebx = r.val;
    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.edx = r.val;
    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.ecx = r.val;
    cpu.esp += 4;

    r.addr = cpu.esp;
    operand_read(&r);
    cpu.eax = r.val;
    cpu.esp += 4;

    return 1;
}
