#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

bool concat(ex_sbb_flags_, SUFFIX)(DATA_TYPE_S a, DATA_TYPE_S b, DATA_TYPE_S result) {
	int count = 0;
	if(result == 0) cpu.ZF = 1;				
	else cpu.ZF = 0;
	if(MSB(result) == 1) cpu.SF = 1;
	else cpu.SF = 0;
	if((MSB(a) != MSB(b)) && (MSB(a) == MSB(result))) cpu.OF = 1;
	else cpu.OF = 0;
	if (b >= (a + cpu.CF)) cpu.CF = 0;
	else cpu.CF = 1;
	result = result & 0xff;
	for (count = 0; result ; count++) {
		result &= result - 1;
	}
	if((count % 2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
	return true;
}

make_helper(concat(sbb_a_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE_S temp;
    temp = REG(0) - (cpu.CF + imm);
    concat(ex_sbb_flags_, SUFFIX)(imm, REG(0), temp);
	REG(0) = temp;
	print_asm("sbb"str(SUFFIX)" $0x%x", imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(sbb_i2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S temp;
	if(m.mod == 3) {
		DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
		temp = REG(m.R_M) - ( imm + cpu.CF);
		concat(ex_sbb_flags_, SUFFIX)(imm, REG(m.R_M), temp);
		REG(m.R_M) = temp;
		print_asm("sbb"str(SUFFIX) " $0x%x,%%%s", imm,REG_NAME(m.R_M));
		return 1 + DATA_BYTE;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE_S imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		temp = MEM_R(addr) - (imm + cpu.CF);
		concat(ex_sbb_flags_, SUFFIX)(imm, MEM_R(addr), temp);
		MEM_W(addr, temp);
		print_asm("sbb"str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return 1 + len + DATA_BYTE;
	}
}

make_helper(concat(sbb_ib2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S temp;
	if(m.mod == 3) {
		DATA_TYPE_S imm = instr_fetch(eip + 1, 1);
		temp = REG(m.R_M) - ( imm + cpu.CF);
		concat(ex_sbb_flags_, SUFFIX)(imm, REG(m.R_M), temp);
		REG(m.R_M) = temp;
		print_asm("sbb"str(SUFFIX) " $0x%x,%%%s", imm,REG_NAME(m.R_M));
		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE_S imm = instr_fetch(eip + 1 + len, 1);
		temp = MEM_R(addr) - (imm + cpu.CF);
		concat(ex_sbb_flags_, SUFFIX)(imm, MEM_R(addr), temp);
		MEM_W(addr, temp);
		print_asm("sbb"str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return 1 + len + 1;
	}
}


make_helper(concat(sbb_rm2r_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		DATA_TYPE_S temp;
		if(m.mod == 3) {
			temp = REG(m.reg) -( REG(m.R_M) + cpu.CF);
			concat(ex_sbb_flags_, SUFFIX)(REG(m.R_M), REG(m.reg), temp);
			REG(m.reg) = temp;
			print_asm("sbb"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
			return 2;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
            temp = REG(m.reg)- (MEM_R(addr) + cpu.CF);
			concat(ex_sbb_flags_, SUFFIX)(REG(m.reg),MEM_R(addr), temp);
			REG(m.reg) = temp;
			print_asm("sbb"str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
			return 1 + len;
		}
}

make_helper(concat(sbb_r2rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		DATA_TYPE_S temp;
		if(m.mod == 3) {
			temp = REG(m.R_M) -( REG(m.reg) + cpu.CF);
			concat(ex_sbb_flags_, SUFFIX)(REG(m.reg), REG(m.R_M), temp);
			REG(m.R_M) = temp;
			print_asm("sbb"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
			return 2;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
            temp = MEM_R(addr)- (REG(m.reg) + cpu.CF);
			concat(ex_sbb_flags_, SUFFIX)(MEM_R(addr),REG(m.reg), temp);
			MEM_W(addr, temp);
			print_asm("sbb"str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
			return 1 + len;
		}
}

#include "exec/template-end.h"
