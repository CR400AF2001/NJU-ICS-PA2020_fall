#include "cpu/instr.h"

make_instr_func(jmp_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);

    cpu.eip += offset;

    return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);
    
    if((rel.val & 0x80) != 0){
        rel.val = 0xFFFFFF00 | rel.val;
    }

    int offset = sign_ext(rel.val, data_size);

    cpu.eip += offset;

    return 2;
}

make_instr_func(jmp_near_indirect)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    if(data_size == 16){
        rm.val = rm.val & 0x0000FFFF;
    }

    cpu.eip = rm.val;

    return 0;
}

make_instr_func(jmp_far_imm){
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);
    
    cpu.eip = rel.val;
    
    rel.data_size = 16;
    rel.addr += data_size / 8;
    
    operand_read(&rel);
    
    cpu.segReg[1].val = rel.val;
    load_sreg(1);
    
    return 0;
}
