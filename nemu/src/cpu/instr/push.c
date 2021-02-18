#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/


make_instr_func(push_r_v)
{
    OPERAND r,esp;
    
    r.data_size = 32;
    r.type = OPR_REG;
    r.addr = opcode & 0x7;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&r);
    operand_read(&esp);
    
    esp.val -= 4;
    
    r.addr = esp.val;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    
    operand_write(&esp);
    operand_write(&r);
    
    return 1;
}

make_instr_func(push_m)
{
    OPERAND m,esp;
    
    m.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &m);
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&m);
    operand_read(&esp);
    
    esp.val -= 4;
    
    m.addr = esp.val;
    m.type = OPR_MEM;
    m.sreg = SREG_SS;
    
    operand_write(&esp);
    operand_write(&m);
    
    return len;
}

make_instr_func(push_short)
{
    OPERAND imm,esp;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&imm);
    operand_read(&esp);
    
    esp.val -= 4;
    
    imm.addr = esp.val;
    imm.type = OPR_MEM;
    imm.sreg = SREG_SS;
    imm.val = sign_ext(imm.val & (0xFFFFFFFF >> (32 - data_size)), data_size);
    imm.data_size = 32;
    
    operand_write(&esp);
    operand_write(&imm);
    
    return 2;
}

make_instr_func(push_i_v)
{
    OPERAND imm,esp;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    esp.data_size = 32;
    esp.type = OPR_REG;
    esp.addr = 0x4;
    
    operand_read(&imm);
    operand_read(&esp);
    
    esp.val -= 4;
    
    imm.addr = esp.val;
    imm.type = OPR_MEM;
    imm.sreg = SREG_SS;
    imm.val = sign_ext(imm.val & (0xFFFFFFFF >> (32 - data_size)), data_size);
    imm.data_size = 32;
    
    operand_write(&esp);
    operand_write(&imm);
    
    return 1 + data_size / 8;
}

make_instr_func(pusha){
    uint32_t temp = cpu.esp;
    OPERAND r;
    r.data_size = 32;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    
    cpu.esp -= 4;
    r.val = cpu.eax;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.ecx;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.edx;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.ebx;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = temp;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.ebp;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.esi;
    r.addr = cpu.esp;
    operand_write(&r);

    cpu.esp -= 4;
    r.val = cpu.edi;
    r.addr = cpu.esp;
    operand_write(&r);

    return 1;
}
    
    
