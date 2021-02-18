#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/


make_instr_func(in_b)
{
    OPERAND ra,rd;
    
    ra.data_size = 8;
    ra.type = OPR_REG;
    ra.addr = 0x0; 

    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = 0x2;    

    operand_read(&rd);

    ra.val = pio_read(rd.val, 1);
    operand_write(&ra);
    
    return 1;
}

make_instr_func(in_v)
{
    OPERAND ra,rd;
    
    ra.data_size = data_size;
    ra.type = OPR_REG;
    ra.addr = 0x0; 

    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = 0x2;    

    operand_read(&rd);

    ra.val = pio_read(rd.val, data_size / 8);
    operand_write(&ra);
    
    return 1;
}
