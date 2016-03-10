#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

bool concat(set_flags_, SUFFIX)(DATA_TYPE_S, DATA_TYPE_S); 

bool concat(ex_add_flags_, SUFFIX)(DATA_TYPE_S a, DATA_TYPE_S b) {
    DATA_TYPE_S sum = a + b;
	int count = 0;
	if(sum == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if(MSB(sum) == 1) cpu.SF = 1;
	else cpu.SF = 0;
	if((MSB(a) == MSB(b)) && (MSB(sum) != MSB(a))) cpu.OF = 1;
	else cpu.OF = 0;
	if (((DATA_TYPE)sum < (DATA_TYPE)a) && ((DATA_TYPE)sum < (DATA_TYPE)b)) cpu.CF = 1;
	else cpu.CF = 0;
	//Log("sum:%d",sum);

	sum = sum & 0xff;
	for (count = 0; sum!=0 ; count++) {
		//Log("haha");
		sum &= sum - 1;
	}
	if((count % 2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
	return true;
}

make_helper(concat(sub_i2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
		concat(set_flags_, SUFFIX)( imm, REG(m.R_M));
		REG(m.R_M) -= (DATA_TYPE_S)imm;
		print_asm("sub"  " $0x%x,%%%s", imm, REG_NAME(m.R_M));
	    return 3;
	}
	else {
        swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		concat(set_flags_, SUFFIX)(imm, MEM_R(addr));
		MEM_W(addr, MEM_R(addr) - (DATA_TYPE_S)imm);
		print_asm("sub" " $0x%x,%s", imm ,ModR_M_asm);
		return 2 + len;
	}
}

make_helper(concat(sub_iv2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		concat(set_flags_, SUFFIX)( imm, REG(m.R_M));
		REG(m.R_M) -= imm;
		print_asm("sub"  " $0x%x,%%%s", imm, REG_NAME(m.R_M));
	    return 2 + DATA_BYTE;
	}
	else {
        swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		concat(set_flags_, SUFFIX)( imm, MEM_R(addr));
		MEM_W(addr, MEM_R(addr) - imm);
		print_asm("sub" " $0x%x,%s", imm ,ModR_M_asm);
		return 1 + len + DATA_BYTE;
	}
}

make_helper(concat(sub_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(set_flags_, SUFFIX)( REG(m.R_M), REG(m.reg));
		REG(m.reg) -= REG(m.R_M);
		print_asm("sub"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	    return 2;
	}
	else {
        swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(set_flags_, SUFFIX)(  MEM_R(addr),REG(m.reg));
	    REG(m.reg) -= MEM_R(addr);
		print_asm("sub" " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
}

make_helper(concat(sub_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(set_flags_, SUFFIX)( REG(m.reg), REG(m.R_M));
		REG(m.R_M) -= REG(m.reg);
		print_asm("sub"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	    return 2;
	}
	else {
        swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(set_flags_, SUFFIX)( REG(m.reg), MEM_R(addr));
		int temp = MEM_R(addr) - REG(m.reg);
		MEM_W( addr, temp);
		print_asm("sub" " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(add_ib2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 2, 1);
        concat(ex_add_flags_, SUFFIX)(imm, REG(m.R_M));
		REG(m.R_M) += imm;
		print_asm("add"str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
        concat(ex_add_flags_, SUFFIX)(imm, MEM_R(addr));
		MEM_W(addr, MEM_R(addr) + imm);
		print_asm("add"str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 2;
	}
}

make_helper(concat(add_i2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S imm;
	if(m.mod == 3) {
		imm = instr_fetch(eip + 2, DATA_BYTE);
        concat(ex_add_flags_, SUFFIX)(imm, REG(m.R_M));
		REG(m.R_M) += imm;
		print_asm("add"str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return DATA_BYTE + 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
        concat(ex_add_flags_, SUFFIX)(imm, MEM_R(addr));
		MEM_W(addr, MEM_R(addr) + imm);
		print_asm("add"str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(add_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(ex_add_flags_, SUFFIX)(REG(m.R_M), REG(m.reg));
        REG(m.R_M) += REG(m.reg);
		print_asm("add"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(ex_add_flags_, SUFFIX)(REG(m.reg), MEM_R(addr));
		MEM_W(addr, MEM_R(addr) + REG(m.reg));
		print_asm("add"str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
        return 1 + len;
	}
}

make_helper(concat(add_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(ex_add_flags_, SUFFIX)(REG(m.R_M), REG(m.reg));
        REG(m.reg) += REG(m.R_M);
		print_asm("add"str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(ex_add_flags_, SUFFIX)(REG(m.reg), MEM_R(addr));
		MEM_W(addr, MEM_R(addr) + REG(m.reg));
		print_asm("add"str(SUFFIX) " %s,%%%s",  ModR_M_asm, REG_NAME(m.reg));
        return 1 + len;
	}
}

make_helper(concat(add_i, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	concat(ex_add_flags_, SUFFIX)(REG(0), imm);
	REG(0) += imm;
	print_asm("add"str(SUFFIX) " %%%s", REG_NAME(0));
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
