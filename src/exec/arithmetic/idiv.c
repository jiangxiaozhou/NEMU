#include "exec/helper.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#define DATA_BYTE 1
#include "exec/template-start.h"

make_helper(idiv_rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t quo;
	int8_t rem;
	if(m.mod == 3) {
		quo = (int16_t)reg_w(R_AX) / (int8_t)reg_b(m.R_M);
		rem = (int16_t)reg_w(R_AX) % (int8_t)reg_b(m.R_M);
		reg_b(R_AH) = rem;
		reg_b(R_AL) = quo;
		print_asm("idiv"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = (int16_t)reg_w(R_AX) / (int8_t)MEM_R(addr);
		rem = (int16_t)reg_w(R_AX) % (int8_t)MEM_R(addr);
        reg_b(R_AH) = rem;
		reg_b(R_AL) = quo;
		print_asm("idiv"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
    }
}

#include "exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "exec/template-start.h"
make_helper(idiv_rm_w) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int16_t quo;
	int16_t rem;
	int32_t src = (((int32_t)REG(2))<< 16) | REG(0);
	if(m.mod == 3) {
		quo = src / (int16_t)REG(m.R_M);
		rem = src % (int16_t)REG(m.R_M);
		REG(2) = rem;
		REG(0) = quo;
		print_asm("idiv"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = src / (int16_t)MEM_R(addr);
		rem = src % (int16_t)MEM_R(addr);
        REG(2) = rem;
		REG(0) = quo;
		print_asm("idiv"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
     }
}

#include "exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "exec/template-start.h"

make_helper(idiv_rm_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int32_t quo;
	int32_t rem;
	int64_t src = (((int64_t)REG(2))<< 32) | (uint32_t)REG(0);
	if(m.mod == 3) {
		quo = src / (int32_t)REG(m.R_M);
		rem = src % (int32_t)REG(m.R_M);
		REG(2) = rem;
		REG(0) = quo;
		print_asm("idiv"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = src / (int32_t)MEM_R(addr);
		rem = src % (int32_t)MEM_R(addr);
        REG(2) = rem;
		REG(0) = quo;
		print_asm("idiv"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
    }
}

#include "exec/template-end.h"
#undef DATA_BYTE

extern char suffix;

make_helper(idiv_rm_v) {
	return ( suffix =='l' ? idiv_rm_l(eip) : idiv_rm_w(eip));
}
