#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/

make_instr_func(out_b)
{
    OPERAND ra,rd;
    
    ra.data_size = 8;
    ra.type = OPR_REG;
    ra.addr = 0x0; 

    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = 0x2;    

    operand_read(&ra);
    operand_read(&rd);

    pio_write(rd.val, 1, ra.val);
    
    return 1;
}

make_instr_func(out_v)
{
    OPERAND ra,rd;
    
    ra.data_size = data_size;
    ra.type = OPR_REG;
    ra.addr = 0x0; 

    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = 0x2;    

    operand_read(&ra);
    operand_read(&rd);

    pio_write(rd.val, data_size / 8, ra.val);
    
    return 1;
}
