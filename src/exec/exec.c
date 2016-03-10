#include "exec/helper.h"

#include "all-instr.h"

typedef int (*helper_fun)(swaddr_t);


/* TODO: Add more instructions!!! */

helper_fun opcode_table [256] = {
/* 0x00 */	add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v, 
/* 0x04 */	add_ib, add_iv, inv, inv, 
/* 0x08 */	or_r2rm_b, or_r2rm_v, or_rm2r_b, or_rm2r_v,
/* 0x0c */	or_i2ra_b, or_i2ra_v, inv, pre_0f, 
/* 0x10 */	adc_r2rm_b, adc_r2rm_v, adc_rm2r_b, adc_rm2r_v, 
/* 0x14 */	adc_a_b, adc_a_v, inv, inv, 
/* 0x18 */	sbb_r2rm_b, sbb_r2rm_v, sbb_rm2r_b, sbb_rm2r_v, 
/* 0x1c */	sbb_a_b, sbb_a_v, inv, inv, 
/* 0x20 */	and_r2rm_b, and_r2rm_v, and_rm2r_b, and_rm2r_v, 
/* 0x24 */	and_i2ra_b, and_i2ra_v, inv, inv,
/* 0x28 */	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	xor_r2rm_b, xor_r2rm_v, xor_rm2r_b, xor_rm2r_v, 
/* 0x34 */	xor_i2ra_b, xor_i2ra_v, inv, inv,
/* 0x38 */	cmp_r2rm_b, cmp_r2rm_v, cmp_rm2r_b, cmp_rm2r_v, 
/* 0x3c */	cmp_iv2ra_b, cmp_iv2ra_v, inv, inv, 
/* 0x40 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v, 
/* 0x44 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x48 */	dec_r_v, dec_r_v, dec_r_v, dec_r_v, 
/* 0x4c */	dec_r_v, dec_r_v, dec_r_v, dec_r_v, 
/* 0x50 */	push_r, push_r, push_r, push_r,  //1 2 3 4 tested 
/* 0x54 */	push_r, push_r, push_r, push_r,  //1 2 3 4 tested
/* 0x58 */	pop_r, pop_r, pop_r, pop_r, 
/* 0x5c */	pop_r, pop_r, pop_r, pop_r, 
/* 0x60 */	pusha, popa, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,  //3 tested
/* 0x68 */	push_i_v, imul_irm2r_v, push_i_b, imul_ibrm2r_v, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	jo_b, jno_b, jb_b, jae_b,
/* 0x74 */	jez_b, jne_b, jbe_b, ja_b,   //1 3 tested
/* 0x78 */	js_b, jns_b, jpe_b, jnp_b, 
/* 0x7c */	jl_b, jge_b, jle_b, jg_b, 
/* 0x80 */	repeat_80, repeat_81, nemu_trap, repeat_83, //3  tested
/* 0x84 */	test_r2rm_b, test_r2rm_v, xchg_rm2r_b, xchg_rm2r_v, //1 2 tested
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea_m2r_v, mov_r2sreg, repeat_8F, 
/* 0x90 */	nop, xchg_a2r_v, xchg_a2r_v, xchg_a2r_v, 
/* 0x94 */	xchg_a2r_v, xchg_a2r_v, xchg_a2r_v, xchg_a2r_v,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	movs_m2m_b, movs_m2m_v, inv, inv,
/* 0xa8 */	test_a2r_b, test_a2r_v, stosb, stos_v,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	repeat_c0, repeat_c1, ret_b, ret,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, leave, inv, inv,
/* 0xcc */	int3, int_r, inv, iret,
/* 0xd0 */	repeat_D0, repeat_D1, repeat_D2, repeat_D3,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_rel32, jmp_rel32, ljmp, jmp_rel8,   //1 4 tested
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, repeat_F3,
/* 0xf4 */	inv, inv, repeat_F6, repeat_F7,
/* 0xf8 */	inv, inv, cli, inv,
/* 0xfc */	cld, std, repeat_FE, repeat_FF,
};

make_helper(exec) {
	return opcode_table[ instr_fetch(eip, 1) ](eip);
}
