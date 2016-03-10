#include "exec/helper.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#define DATA_BYTE 1
#include "exec/template-start.h"

make_helper(div_rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint8_t quo;
	uint8_t rem;
	if(m.mod == 3) {
		quo = (uint16_t)reg_w(R_AX) / (uint8_t)reg_b(m.R_M);
		rem = (uint16_t)reg_w(R_AX) % (uint8_t)reg_b(m.R_M);
		reg_b(R_AH) = rem;
		reg_b(R_AL) = quo;
		print_asm("div"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = (uint16_t)reg_w(R_AX) / (uint8_t)MEM_R(addr);
		rem = (uint16_t)reg_w(R_AX) % (uint8_t)MEM_R(addr);
        reg_b(R_AH) = rem;
		reg_b(R_AL) = quo;
		print_asm("div"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
    }
}

#include "exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "exec/template-start.h"
make_helper(div_rm_w) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint16_t quo;
	uint16_t rem;
	uint32_t src = (((uint32_t)REG(2))<< 16) | REG(0);
	if(m.mod == 3) {
		quo = src / (uint16_t)REG(m.R_M);
		rem = src % (uint16_t)REG(m.R_M);
		REG(2) = rem;
		REG(0) = quo;
		print_asm("div"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = src / (uint16_t)MEM_R(addr);
		rem = src % (uint16_t)MEM_R(addr);
        REG(2) = rem;
		REG(0) = quo;
		print_asm("div"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
     }
}

#include "exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "exec/template-start.h"

make_helper(div_rm_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint32_t quo;
	uint32_t rem;
	uint64_t src = (((uint64_t)REG(2))<< 32) | (uint32_t)REG(0);
	if(m.mod == 3) {
		quo = src / (uint32_t)REG(m.R_M);
		rem = src % (uint32_t)REG(m.R_M);
		REG(2) = rem;
		REG(0) = quo;
		print_asm("div"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = src / (uint32_t)MEM_R(addr);
		rem = src % (uint32_t)MEM_R(addr);
        REG(2) = rem;
		REG(0) = quo;
		print_asm("div"str(SUFFIX) " %s" ,ModR_M_asm);
		return 1 + len;
    }
}

#include "exec/template-end.h"
#undef DATA_BYTE

extern char suffix;

make_helper(div_rm_v) {
	return ( suffix =='l' ? div_rm_l(eip) : div_rm_w(eip));
}
