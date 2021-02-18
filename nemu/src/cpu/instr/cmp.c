#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/

make_instr_func(cmp_rm_i_v_short)
{
    OPERAND rm,imm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    imm.type = OPR_IMM;
    imm.addr = eip + len;
    imm.data_size = 8;
    
    operand_read(&rm);
    operand_read(&imm);
    
    if((imm.val & 0x80) != 0){
        imm.val = 0xFFFFFF00 | imm.val;
    }
    
    alu_sub(imm.val, rm.val, data_size);
    return len + 1;
}

make_instr_func(cmp_rm)
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
    
    alu_sub(imm.val, rm.val, data_size);
    return len + data_size / 8;
}

make_instr_func(cmp_rm_r)
{
    OPERAND rm,r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    
    alu_sub(r.val, rm.val, data_size);
    return len;
}

make_instr_func(cmp_r_rm)
{
    OPERAND rm,r;
    
    rm.data_size = data_size;
    r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    
    alu_sub(rm.val, r.val, data_size);
    return len;
}

make_instr_func(cmp_r_rm_short)
{
    OPERAND rm,r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&r);
    operand_read(&rm);
    
    alu_sub(rm.val, r.val, 8);
    return len;
}

make_instr_func(cmp_r_i_short)
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
    
    alu_sub(sign_ext(imm.val & (0xFFFFFFFF >> (32 - data_size)), data_size), sign_ext(r.val & (0xFFFFFFFF >> (32 - data_size)), data_size), data_size);
    return 2;
}

make_instr_func(cmp_rm_i_short)
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
    
    alu_sub(sign_ext(imm.val & (0xFFFFFFFF >> (32 - data_size)), data_size), sign_ext(rm.val & (0xFFFFFFFF >> (32 - data_size)), data_size), data_size);
    return len + 1;
}
   
make_instr_func(cmp_i2a_v){
    OPERAND r,imm;
    
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = 0x0;
    
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    
    operand_read(&r);
    operand_read(&imm);
    alu_sub(imm.val, r.val, data_size);
    
    return 1 + data_size / 8;
} 


make_instr_func(cmp_r2rm_b)
{
    OPERAND rm, r;
    
    rm.data_size = 8;
    r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    
    operand_read(&rm);
    operand_read(&r);
    
    alu_sub(sign_ext(r.val & (0xFFFFFFFF >> (32 - data_size)), data_size), sign_ext(rm.val & (0xFFFFFFFF >> (32 - data_size)), data_size), data_size);
    
    operand_write(&rm);
    
    return len;
}
