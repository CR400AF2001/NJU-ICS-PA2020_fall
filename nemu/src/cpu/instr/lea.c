#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

make_instr_func(lea)
{
    OPERAND r, m;
    
    r.data_size = data_size;
    m.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &m);
    
    r.val = m.addr;
    
    operand_write(&r);
    
    return len;
}