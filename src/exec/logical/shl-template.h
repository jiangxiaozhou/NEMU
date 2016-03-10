#include "exec/helper.h"
#include "exec/flag-change.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

bool concat(shl_flags_, SUFFIX)(DATA_TYPE_S src, int8_t b) {
	if (b == 1) cpu.OF = ( MSB(src) != cpu.CF);
	src <<= b-1;
    cpu.CF = MSB(src);
	src <<= 1;
	flags(src);
	return true;
}

make_helper(concat(shl_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		concat(shl_flags_, SUFFIX)(REG(m.R_M), 1);
		DATA_TYPE_S temp = (DATA_TYPE_S)REG(m.R_M) << 1;
		REG(m.R_M) = temp;
		print_asm("shl" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		DATA_TYPE_S temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shl_flags_, SUFFIX)( MEM_R(addr), 1);
		temp = (DATA_TYPE_S)MEM_R(addr) << 1;
		MEM_W(addr , temp);
		print_asm("shl" " %s", ModR_M_asm);
		return 1 + len ;
	}
}

make_helper(concat(shl_c2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		concat(shl_flags_, SUFFIX)(REG(m.R_M), reg_b(R_CL));
		DATA_TYPE_S temp = (DATA_TYPE_S)REG(m.R_M) << reg_b(R_CL);
		REG(m.R_M) = temp;
		print_asm("shl" " %%%s,%%%s", regsb[R_CL], REG_NAME(m.R_M));
		return 2;
	}
	else {
		DATA_TYPE_S temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shl_flags_, SUFFIX)( MEM_R(addr), reg_b(R_CL));
		temp = (DATA_TYPE_S)MEM_R(addr) << reg_b(R_CL);
		MEM_W(addr , temp);
		print_asm("shl" " %s", ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(shl_ib2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm = instr_fetch(eip + 2, 1);
	if (m.mod == 3) {
		concat(shl_flags_, SUFFIX)(REG(m.R_M), imm);
		DATA_TYPE_S temp = (DATA_TYPE_S)REG(m.R_M) << imm;
		REG(m.R_M) = temp;
		print_asm("shl" " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		DATA_TYPE_S temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shl_flags_, SUFFIX)( MEM_R(addr), imm);
		temp = (DATA_TYPE_S)MEM_R(addr) << imm;
		MEM_W(addr , temp);
		print_asm("shl" " $0x%x,%s", imm, ModR_M_asm);
		return 1 + len + 1;
	}
}
  
#include "exec/template-end.h"
