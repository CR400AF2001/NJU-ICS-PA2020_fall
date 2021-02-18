#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/

make_instr_func(test_rm_r_v)
{
    OPERAND rm, r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    rm.val = alu_and(r.val, rm.val, data_size);
    
    if(rm.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return len;
}

make_instr_func(test_rm_r_short)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    rm.val = alu_and(r.val, rm.val, 8);
    
    if(rm.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return len;
}

make_instr_func(test_rm_i_v)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = data_size;
    
    operand_read(&rm);
    operand_read(&imm);
    
    rm.val = alu_and(imm.val, rm.val, data_size);
    
    if(rm.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return len + data_size / 8;
}

make_instr_func(test_rm_i_short)
{
    OPERAND rm,imm;
    
    rm.data_size = 8;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    
    rm.val = alu_and(imm.val, rm.val, data_size);
    
    if(rm.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return len + 1;
}

make_instr_func(test_ra_i_v)
{
    OPERAND r,imm;
    
    r.data_size = data_size;
    r.addr = 0x0;
    r.type = OPR_REG;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    operand_read(&r);
    operand_read(&imm);
    
    r.val = alu_and(imm.val, r.val, data_size);
    
    if(r.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return 1 + data_size / 8;
}

make_instr_func(test_ra_i_short)
{
    OPERAND r,imm;
    
    r.data_size = 8;
    r.addr = 0x0;
    r.type = OPR_REG;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    
    operand_read(&r);
    operand_read(&imm);
    
    r.val = alu_and(imm.val, r.val, data_size);
    
    if(r.val == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
    
    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    
    return 2;
}
