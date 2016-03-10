#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

bool concat(shr_flags_, SUFFIX)(DATA_TYPE src, int8_t b) {
	if(b == 1) cpu.OF = MSB(src);
	for (; b!=0 ; b--) {
		cpu.CF = src & 0x1;
		src >>= 1;
	}
	cpu.SF = MSB(src);
	cpu.ZF = (src == 0);
	int count = 0;
	for(; src ; count++)
		src &= src - 1;
	cpu.PF = count%2;
	return true;
}

make_helper(concat(shr_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		concat(shr_flags_, SUFFIX)(REG(m.R_M),1);
		DATA_TYPE temp = (DATA_TYPE)REG(m.R_M) >> 1;
		REG(m.R_M) = temp;
		print_asm("shr" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		DATA_TYPE temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shr_flags_, SUFFIX)( MEM_R(addr), 1);
		temp = (DATA_TYPE)MEM_R(addr) >> 1;
		MEM_W(addr , temp);
		print_asm("shr" " %s",  ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(shr_c2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		concat(shr_flags_, SUFFIX)(REG(m.R_M), reg_b(R_CL));
		DATA_TYPE temp = (DATA_TYPE)REG(m.R_M) >> reg_b(R_CL);
		REG(m.R_M) = temp;
		print_asm("shr" " %%%s,%%%s", regsb[R_CL], REG_NAME(m.R_M));
		return 2;
	}
	else {
		DATA_TYPE temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shr_flags_, SUFFIX)( MEM_R(addr), reg_b(R_CL));
		temp = (DATA_TYPE)MEM_R(addr) >> reg_b(R_CL);
		MEM_W(addr , temp);
		print_asm("shr" " %%%s,%s", regsb[R_CL], ModR_M_asm);
		return 1 + len;
	}
}
  
make_helper(concat(shr_ib2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm = instr_fetch(eip + 2, 1);
	if (m.mod == 3) {
		concat(shr_flags_, SUFFIX)(REG(m.R_M), imm);
		DATA_TYPE temp = (DATA_TYPE)REG(m.R_M) >> imm;
		REG(m.R_M) = temp;
		print_asm("shr" " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		DATA_TYPE temp;
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(shr_flags_, SUFFIX)( MEM_R(addr), imm);
		temp = (DATA_TYPE)MEM_R(addr) >> imm;
		MEM_W(addr , temp);
		print_asm("shr" " $0x%x,%s", imm, ModR_M_asm);
		return 1 + len + 1;
	}
}
  
#include "exec/template-end.h"
