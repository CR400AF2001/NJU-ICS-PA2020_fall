#ifndef __INSTR_ADC_H__
#define __INSTR_ADC_H__
/*
Put the declarations of `adc' instructions here.
*/

make_instr_func(adc_rm_r);
make_instr_func(adc_rm_r_short);
make_instr_func(adc_ra_i_short);
make_instr_func(adc_rm_i_v_short);
make_instr_func(adc_i2rm_b);
make_instr_func(adc_i2rm_v);
make_instr_func(adc_rm2r_b);
make_instr_func(adc_rm2r_v);
make_instr_func(adc_i2a_v);

#endif
