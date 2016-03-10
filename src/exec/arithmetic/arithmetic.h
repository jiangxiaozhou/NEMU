#ifndef __ARIT_H__
#define __ARIT_H__

make_helper(sub_i2r_b);
make_helper(sub_i2r_v);
make_helper(sub_iv2rm_b);
make_helper(sub_iv2rm_v);
make_helper(sub_r2rm_b);
make_helper(sub_r2rm_v);
make_helper(sub_rm2r_b);
make_helper(sub_rm2r_v);

make_helper(add_ib2rm_v);
make_helper(add_r2rm_b);
make_helper(add_r2rm_v);
make_helper(add_rm2r_b);
make_helper(add_rm2r_v);
make_helper(add_i2rm_b);
make_helper(add_i2rm_v);
make_helper(add_ib);
make_helper(add_iv);

make_helper(adc_r2rm_v);
make_helper(adc_r2rm_b);
make_helper(adc_rm2r_v);
make_helper(adc_rm2r_b);
make_helper(adc_i2rm_v);
make_helper(adc_ib2rm_v);
make_helper(adc_ib2rm_b);
make_helper(adc_a_v);
make_helper(adc_a_b);

make_helper(sbb_r2rm_v);
make_helper(sbb_r2rm_b);
make_helper(sbb_rm2r_v);
make_helper(sbb_rm2r_b);
make_helper(sbb_i2rm_v);
make_helper(sbb_ib2rm_v);
make_helper(sbb_ib2rm_b);
make_helper(sbb_a_v);
make_helper(sbb_a_b);

make_helper(imul_rm2r_v);
make_helper(imul_rm_b);
make_helper(imul_rm_v);
make_helper(imul_irm2r_b);
make_helper(imul_irm2r_v);
make_helper(imul_ibrm2r_b);
make_helper(imul_ibrm2r_v);

make_helper(mul_rm_v);
make_helper(mul_rm_b);

make_helper(idiv_rm_b);
make_helper(idiv_rm_v);

make_helper(div_rm_b);
make_helper(div_rm_v);

make_helper(inc_rm_b);
make_helper(inc_rm_v);
make_helper(inc_r_v);
make_helper(inc_r_b);

make_helper(dec_rm_b);
make_helper(dec_rm_v);
make_helper(dec_r_v);
make_helper(dec_r_b);

make_helper(neg_rm_v);
make_helper(neg_rm_b);

#endif
