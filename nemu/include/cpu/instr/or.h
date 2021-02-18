#ifndef __INSTR_OR_H__
#define __INSTR_OR_H__
/*
Put the declarations of `or' instructions here.
*/

make_instr_func(or_rm_r);
make_instr_func(or_r_rm_short);
make_instr_func(or_rm_i_v_short);
make_instr_func(or_i2rm_b);
make_instr_func(or_i2rm_v);
make_instr_func(or_r2rm_b);
make_instr_func(or_rm2r_v);
make_instr_func(or_i2a_b);
make_instr_func(or_i2a_v);

#endif
