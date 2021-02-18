#ifndef __INSTR_XOR_H__
#define __INSTR_XOR_H__
/*
Put the declarations of `xor' instructions here.
*/

make_instr_func(xor_rm2r_v);
make_instr_func(xor_rm2r_b);
make_instr_func(xor_r2rm_b);
make_instr_func(xor_r2rm_v);
make_instr_func(xor_i2a_b);
make_instr_func(xor_i2a_v);
make_instr_func(xor_rm_i_v_short);
make_instr_func(xor_i2rm_b);
make_instr_func(xor_i2rm_v);

#endif
