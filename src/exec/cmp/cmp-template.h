#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

bool concat(set_flags_,SUFFIX)(DATA_TYPE a,DATA_TYPE b) {
	DATA_TYPE temp = b - a;
	int count = 0;
	if (temp == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if (MSB(temp) != 0) cpu.SF = 1;
	else cpu.SF = 0;
    if ((MSB(a) != MSB(b)) && (MSB(a) == MSB(temp))) cpu.OF = 1;
	else cpu.OF = 0;
	if (b >= a) cpu.CF = 0;
	else if (b < a) cpu.CF = 1;

	temp = temp & 0xff;
	for (count = 0; temp ; count++) {
		temp &= temp - 1;
	}
	if((count%2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
	return true;
}


make_helper(concat(cmp_i2r_, SUFFIX)) {                   //83
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
        concat(set_flags_,SUFFIX)( imm, REG(m.R_M));
	    print_asm("cmp" str(SUFFIX) " 0x%x,%%%s", imm, REG_NAME(m.R_M));
	    return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + len + 1, 1);
        concat(set_flags_,SUFFIX)( imm, MEM_R(addr));
		print_asm("cmp" " 0x%x,%s", imm, ModR_M_asm);
		return 1 + len + 1;
	}
}

make_helper(concat(cmp_iv2rm_, SUFFIX)) {                      //81
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		DATA_TYPE imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		concat(set_flags_,SUFFIX)( imm, REG(m.R_M));
		print_asm("cmp" str(SUFFIX) " 0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 2 + DATA_BYTE;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		concat(set_flags_,SUFFIX)( imm, MEM_R(addr));
		print_asm("cmp" str(SUFFIX) " 0x%x,%s", imm, ModR_M_asm);
		return 1 + len + DATA_BYTE;
	}
}
    
make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(set_flags_, SUFFIX)( REG(m.reg), REG(m.R_M));
		print_asm("cmp"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE imm = MEM_R(addr);
		concat(set_flags_, SUFFIX)( REG(m.reg), imm);
		print_asm("cmp"str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
        return 1 + len;
	}
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(set_flags_, SUFFIX)( REG(m.R_M), REG(m.reg));
		print_asm("cmp"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE imm = MEM_R(addr);
		concat(set_flags_, SUFFIX)( imm, REG(m.reg));
		print_asm("cmp"str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
        return 1 + len;
	}
}

make_helper(concat(cmp_iv2ra_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    concat(set_flags_, SUFFIX)( imm, REG(0));
	print_asm("cmp"str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(0));
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
