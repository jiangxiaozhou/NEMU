#include "cpu/modrm.h"
#include "exec/helper.h"
#include "arithmetic/arithmetic.h"
#include "data-mov/mov.h"
#include "cmp/cmp.h"
#include "setcc/setcc.h"
#include "logical/and.h"
#include "logical/not.h"
#include "logical/or.h"
#include "logical/shift.h"
#include "jcc/jcc.h"
#include "test/test.h"
#include "push/push.h"
#include "jmp/jmp.h"
#include "call/call.h"
#include "lgdt-lidt/lgdt-lidt.h"
#include "str_trans/stos.h"

make_helper(repeat_01) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 2:len = lgdt(eip);return len ;
		case 3:len = lgdt(eip);return len ;
	}
	return 0;
}

make_helper(repeat_83) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 1:len = or_ib2rm_v(eip);return len;
		case 5:len = sub_i2r_v(eip);return len;
		case 7:len = cmp_i2r_v(eip);return len;
		case 0:len = add_ib2rm_v(eip);return len;
		case 4:len = and_ib2rm_v(eip);return len;
		case 2:len = adc_ib2rm_v(eip);return len;
		case 3:len = sbb_ib2rm_v(eip);return len;
	}
	return 0;
}

make_helper(repeat_80) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 0:len = add_i2rm_b(eip);break;
		case 4:len = and_iv2rm_b(eip);break;
		case 1:len = or_ib2rm_b(eip);break;
		case 5:len = sub_iv2rm_b(eip);break;
		case 7:len = cmp_i2r_b(eip);break;
		case 2:len = adc_ib2rm_b(eip);break;
		case 3:len = sbb_ib2rm_b(eip);break;
	}
	return len;
}

make_helper(repeat_81) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 1:len = or_iv2rm_v(eip);break;
		case 0:len = add_i2rm_v(eip);break;
		case 7:len = cmp_iv2rm_v(eip);break;
		case 4:len = and_iv2rm_v(eip);break;
		case 5:len = sub_iv2rm_v(eip);break;
		case 2:len = adc_i2rm_v(eip);break;
		case 3:len = sbb_i2rm_v(eip);break;
	}
	return len;
}

make_helper(repeat_F6) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = idiv_rm_b(eip);break;
		case 5:len = imul_rm_b(eip);break;
		case 0:len = test_i2rm_b(eip);break;
		case 3:len = neg_rm_b(eip);break;
		case 4:len = mul_rm_b(eip);break;
		case 6:len = div_rm_b(eip);break;
	}
	return len;
}

make_helper(repeat_F7) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = idiv_rm_v(eip);break;
		case 5:len = imul_rm_v(eip);break;
		case 0:len = test_i2rm_v(eip);break;
		case 2:len = not_rm_v(eip);break;
		case 3:len = neg_rm_v(eip);break;
		case 4:len = mul_rm_v(eip);break;
		case 6:len = div_rm_v(eip);break;
	}
	return len;
}

make_helper(repeat_F3) {
	uint8_t imm;
	imm = instr_fetch(eip + 1, 1);
	int len = 0;
	if (cpu.ecx == 0) return 2;
	for(;cpu.ecx > 0; cpu.ecx--)
		switch(imm) {
			case 0xa4:len = movs_m2m_b(eip + 1);break;
			case 0xa5:len = movs_m2m_v(eip + 1);break;
			case 0xaa:len = stosb(eip + 1);break;
			case 0xab:len = stos_v(eip + 1);break;
		}
	return len + 1;
}

make_helper(repeat_FF) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 6:len = push_m_v(eip);break;
		case 4:len = jmp_rm32(eip);break;
		case 3:len = jmp_rm32(eip);break;
		case 2:len = call_rml(eip);break;
		case 0:len = inc_rm_v(eip);break;
		case 1:len = dec_rm_v(eip);break;
	}
	return len;
}

make_helper(repeat_FE) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 0:len = inc_rm_b(eip);break;
		case 1:len = inc_rm_b(eip);break;
	}
	return len;
}

make_helper(repeat_8F) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 0:len = pop_m_v(eip);break;
	}
	return len;
}

make_helper(repeat_D0) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = sar_rm_b(eip);break;
		case 4:len = shl_rm_b(eip);break;
		case 5:len = shr_rm_b(eip);break;
	}
	return len;
}


make_helper(repeat_D1) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = sar_rm_v(eip);break;
		case 4:len = shl_rm_v(eip);break;
		case 5:len = shr_rm_v(eip);break;
	}
	return len;
}

make_helper(repeat_D2) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = sar_c2rm_b(eip);break;
		case 5:len = shr_c2rm_b(eip);break;
		case 4:len = shl_c2rm_b(eip);break;
	}
	return len;
}

make_helper(repeat_D3) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = sar_c2rm_v(eip);break;
		case 4:len = shl_c2rm_v(eip);break;
		case 5:len = shr_c2rm_v(eip);break;
	}
	return len;
}

make_helper(repeat_c0) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 4:len = shl_ib2rm_b(eip);break;
		case 5:len = shr_ib2rm_b(eip);break;
		case 7:len = sar_ib2rm_b(eip);break;
	}
	return len;
}

make_helper(repeat_c1) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(m.reg) {
		case 7:len = sar_ib2rm_v(eip);break;
		case 4:len = shl_ib2rm_v(eip);break;
		case 5:len = shr_ib2rm_v(eip);break;
	}
	return len;
}

make_helper(pre_0f) {
	uint32_t imm;
	imm = instr_fetch(eip + 1, 1);
	int len = 0;
	switch(imm) {
		case 0x01:len = repeat_01(eip + 1);break;
		case 0x20:len = mov_cr2r(eip + 1);break;
		case 0x22:len = mov_r2cr(eip + 1);break;
		case 0x97:len = seta(eip + 1);break;
		case 0x93:len = setae(eip + 1);break;
		case 0x92:len = setb(eip + 1);break;
		case 0x96:len = setbe(eip + 1);break;
		case 0x94:len = sete(eip + 1);break;
		case 0x95:len = setne(eip + 1);break;
		case 0x9f:len = setg(eip + 1);break;
		case 0x9d:len = setge(eip + 1);break;
		case 0x9c:len = setl(eip + 1);break;
		case 0x9e:len = setle(eip + 1);break;
		case 0x91:len = setno(eip + 1);break;
		case 0x9b:len = setnp(eip + 1);break;
		case 0x99:len = setns(eip + 1);break;
		case 0x90:len = seto(eip + 1);break;
		case 0x9a:len = setp(eip + 1);break;
		case 0x98:len = sets(eip + 1);break;
		case 134:len = jbe_v(eip + 1);break;
		case 135:len = ja_v(eip + 1);break;
		case 131:len = jae_v(eip + 1);break;
		case 130:len = jb_v(eip + 1);break;
		case 132:len = jez_v(eip + 1);break;
		case 143:len = jg_v(eip + 1);break;
		case 141:len = jge_v(eip + 1);break;
		case 140:len = jl_v(eip + 1);break;
		case 142:len = jle_v(eip + 1);break;
		case 133:len = jne_v(eip + 1);break;
		case 129:len = jno_v(eip + 1);break;
		case 139:len = jnp_v(eip + 1);break;
        case 137:len = jns_v(eip + 1);break;
		case 128:len = jo_v(eip + 1);break;
		case 138:len = jpe_v(eip + 1);break;
		case 136:len = js_v(eip + 1);break;
		case 0xaf:len = imul_rm2r_v(eip + 1);break;
		case 0xbe:len = movsbv(eip + 1);break;
		case 0xbf:len = movswl(eip + 1);break;
		case 0xb6:len = movzbv(eip + 1);break;
		case 0xb7:len = movzwl(eip + 1);break;
		case 0x46:len = cmovbe_r2rm_v(eip + 1);break;
		case 0x44:len = cmove_r2rm_v(eip + 1);break;
		case 0x47:len = cmova_r2rm_v(eip + 1);break;
		case 0x48:len = cmovs_r2rm_v(eip + 1);break;
		case 0x4d:len = cmovge_r2rm_v(eip + 1);break;
	}
	return len + 1;
}
