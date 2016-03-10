#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"



make_helper(concat(mov_i2r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	REG(reg_code) = imm;

	print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}

make_helper(concat(mov_i2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		REG(m.R_M) = imm;
		print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		MEM_W(addr, imm);
		print_asm("mov" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(mov_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.R_M) = REG(m.reg);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		MEM_W(addr, REG(m.reg));

		print_asm("mov" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(cmovbe_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if((cpu.CF == 1 )||(cpu.ZF == 1))
			REG(m.reg) = REG(m.R_M);
		//Log("%x %x %s",m.val,m.reg, REG_NAME(m.reg));
		print_asm("cmovbe" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if ((cpu.CF == 1 ) || (cpu.ZF == 1))
			MEM_W(addr, REG(m.R_M));

		print_asm("cmovbe" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(cmovge_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if(cpu.SF == cpu.OF)
			REG(m.reg) = REG(m.R_M);
		//Log("%x %x %s",m.val,m.reg, REG_NAME(m.reg));
		print_asm("cmovge" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (cpu.SF == cpu.OF)
			MEM_W(addr, REG(m.R_M));

		print_asm("cmovge" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(cmove_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if(cpu.ZF == 1)
			REG(m.reg) = REG(m.R_M);
		//Log("%x %x %s",m.val,m.reg, REG_NAME(m.reg));
		print_asm("cmove" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (cpu.ZF == 1)
			MEM_W(addr, REG(m.R_M));

		print_asm("cmove" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(cmovs_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if(cpu.SF == 1)
			REG(m.reg) = REG(m.R_M);
		//Log("%x %x %s",m.val,m.reg, REG_NAME(m.reg));
		print_asm("cmovs" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (cpu.SF == 1)
			MEM_W(addr, REG(m.R_M));

		print_asm("cmovs" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(cmova_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if((cpu.ZF == 0) && (cpu.CF == 0))
			REG(m.reg) = REG(m.R_M);
		//Log("%x %x %s",m.val,m.reg, REG_NAME(m.reg));
		print_asm("cmova" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if ((cpu.ZF == 1) && (cpu.CF == 0))
			MEM_W(addr, REG(m.R_M));

		print_asm("cmova" str(SUFFIX) " %%%s,%s", REG_NAME(m.R_M), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(mov_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.reg) = REG(m.R_M);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		REG(m.reg) = MEM_R(addr);

		print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return len + 1;
	}
}

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(movs_m2m_, SUFFIX)) {
	swaddr_write(cpu.edi , DATA_BYTE, MEM_R(cpu.esi));
	if(cpu.DF == 0) {
		cpu.edi += DATA_BYTE;
		cpu.esi += DATA_BYTE;
	}
	else {
		cpu.edi -= DATA_BYTE;
		cpu.esi -= DATA_BYTE;
	}
	print_asm("movs" str(SUFFIX) " %%ds:(%%esi),%%es:(%%edi)");
	return 1;
}


#include "exec/template-end.h"

#ifndef __MOVZSX_H__
#define __MOVZSX_H__

make_helper(movzbw) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_w(m.reg) = (uint16_t)reg_b(m.R_M);
	    print_asm("movzbw" " %%%s,%%%s",regsb[m.R_M], regsw[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_w(m.reg) = (uint16_t)swaddr_read(addr,1);
	    print_asm("movzbw" " %s,%%%s", ModR_M_asm, regsw[m.reg]);
		return 1 + len;
	}
}

make_helper(movzbl) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (uint32_t)reg_b(m.R_M);
	    print_asm("movzbl" " %%%s,%%%s",regsb[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_l(m.reg) = (uint32_t)swaddr_read(addr,1);
	    print_asm("movzbl" " %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}

make_helper(movzwl) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (uint32_t)reg_w(m.R_M);
	    print_asm("movzwl" " %%%s,%%%s",regsw[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
//		Log("233");
		int len = read_ModR_M(eip + 1, &addr);
//		Log("%x", instr_fetch(eip,4));
		reg_l(m.reg) = (uint32_t)swaddr_read(addr,2);
	    print_asm("movzwl" " %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}

make_helper(movsbw) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_w(m.reg) = (int16_t)reg_b(m.R_M);
	    print_asm("movsbw" " %%%s,%%%s",regsb[m.R_M], regsw[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_w(m.reg) = (int16_t)swaddr_read(addr,1);
	    print_asm("movsbw" " %s,%%%s", ModR_M_asm, regsw[m.reg]);
		return 1 + len;
	}
}

make_helper(movsbl) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (int32_t)reg_b(m.R_M);
	    print_asm("movsbl" " %%%s,%%%s",regsb[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_l(m.reg) = (int32_t)swaddr_read(addr,1);
	    print_asm("movsbl" " %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}

make_helper(movswl) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_l(m.reg) = (int32_t)reg_w(m.R_M);
	    print_asm("movswl" " %%%s,%%%s",regsw[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		reg_l(m.reg) = (int32_t)swaddr_read(addr,2);
	    print_asm("movswl" " %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}


#endif
