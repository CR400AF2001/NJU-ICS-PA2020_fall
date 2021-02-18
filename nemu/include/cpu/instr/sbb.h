#ifndef __INSTR_SBB_H__
#define __INSTR_SBB_H__
/*
Put the declarations of `sbb' instructions here.
*/

make_instr_func(sbb_rm2r_v);
make_instr_func(sbb_rm2r_b);
make_instr_func(sbb_r2rm_b);
make_instr_func(sbb_r2rm_v);
make_instr_func(sbb_i2a_b);
make_instr_func(sbb_i2a_v);
make_instr_func(sbb_i2rm_b);
make_instr_func(sbb_i2rm_v);
make_instr_func(sbb_rm_i_v_short);

#endif
