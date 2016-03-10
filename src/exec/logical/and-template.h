#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"


bool concat(and_flags_, SUFFIX)(DATA_TYPE_S a,DATA_TYPE_S b) {
	DATA_TYPE_S temp = a & b;
	int count = 0;
	cpu.CF = 0;
	cpu.OF = 0;
	if( temp == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if( MSB(temp) == 1) cpu.SF = 1;
	else cpu.SF = 0;

	temp = temp & 0xff;
	for (count = 0; temp ; count++) {
		temp &= temp - 1;
	}
	if((count % 2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
	return true;
}



make_helper(concat(and_ib2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
		concat(and_flags_, SUFFIX)( REG(m.R_M), imm);
		REG(m.R_M) &= imm;
		print_asm("and" " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		concat(and_flags_, SUFFIX)( MEM_R(addr), imm);
		MEM_W(addr, imm & MEM_R(addr));
		print_asm("and" " $0x%x,%s", imm, ModR_M_asm);
		return 2 + len;
	}
}

make_helper(concat(and_i2ra_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	concat(and_flags_, SUFFIX)( REG(0), imm);
	REG(0) &= imm;
	print_asm("and" " $0x%x,%%%s", imm, REG_NAME(0));
	return 1 + DATA_BYTE;
}

make_helper(concat(and_iv2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		concat(and_flags_, SUFFIX)( REG(m.R_M), imm);
		REG(m.R_M) &= imm;
		print_asm("and" " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 2 + DATA_BYTE;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		concat(and_flags_, SUFFIX)( MEM_R(addr), imm);
		MEM_W(addr, imm & MEM_R(addr));
		print_asm("and" " $0x%x,%s", imm, ModR_M_asm);
		return 1 + len + DATA_BYTE;
	}
}

make_helper(concat(and_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if(m.mod == 3) {
		concat(and_flags_, SUFFIX)( REG(m.R_M), REG(m.reg));
		REG(m.R_M) &= REG(m.reg);
		print_asm("and" " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
		concat(and_flags_, SUFFIX)( REG(m.reg), imm);
		MEM_W(addr, imm & REG(m.reg));
		print_asm("and" " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(and_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if(m.mod == 3) {
		concat(and_flags_, SUFFIX)( REG(m.R_M), REG(m.reg));
		REG(m.reg) &= REG(m.R_M);
		print_asm("and" " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
		concat(and_flags_, SUFFIX)( REG(m.reg), imm);
		REG(m.reg) &= imm;
		print_asm("and" " %s,%%%s",  ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
}

#include "exec/template-end.h"
