#ifndef __MOV_H__
#define __MOV_H__

make_helper(mov_i2r_b);
make_helper(mov_i2rm_b);
make_helper(mov_r2rm_b);
make_helper(mov_rm2r_b);
make_helper(mov_a2moffs_b);
make_helper(mov_moffs2a_b);

make_helper(mov_i2r_v);
make_helper(mov_i2rm_v);
make_helper(mov_r2rm_v);
make_helper(mov_rm2r_v);
make_helper(mov_a2moffs_v);
make_helper(mov_moffs2a_v);

make_helper(movs_m2m_b);
make_helper(movs_m2m_v);

make_helper(movzbv);
make_helper(movzwl);
make_helper(movsbv);
make_helper(movswl);

make_helper(mov_cr2r);
make_helper(mov_r2cr);

make_helper(mov_r2sreg);

make_helper(cmovbe_r2rm_v);
make_helper(cmovge_r2rm_v);
make_helper(cmove_r2rm_v);
make_helper(cmovs_r2rm_v);
make_helper(cmova_r2rm_v);

#endif
