#define DATA_BYTE 4
#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(call_rel32) {
	int32_t imm = instr_fetch(eip + 1, DATA_BYTE);
	REG(4) -= 4;
	MEM_W(REG(4), eip + 4);
	//Log("imm = %x", imm);
	cpu.eip += imm;
	print_asm("call" " %x" , cpu.eip + 5);
	return 5;
}

make_helper(call_m32) {
	uint32_t imm = instr_fetch(eip + 2, DATA_BYTE);
	REG(4) -= 4;
	MEM_W(REG(4), eip + 6);
	cpu.eip = MEM_R(imm);
	print_asm("call" " *0x%x" ,imm);
	return 0;
}

make_helper(call_rml) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		MEM_W(REG(4)-4, eip);
		REG(4) -= 4;
        cpu.eip = REG(m.R_M)-2;
		print_asm("call" " *(%%%s)",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		MEM_W(REG(4)-4, eip + len);
		REG(4) -= 4;
		cpu.eip = MEM_R(addr) - len - 1;
//		Log("%x",MEM_R(addr));
		print_asm("call" " *%s", ModR_M_asm);
		return len + 1;
	}
}
#include "exec/template-end.h"
