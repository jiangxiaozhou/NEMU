#include "exec/helper.h"
#include "cpu/modrm.h"
#define DATA_BYTE 1
#include "exec/template-start.h"

make_helper(seta) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.CF == 0 && cpu.ZF == 0 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("seta" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.CF == 0 && cpu.ZF == 0 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("seta" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setae) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.CF == 0 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setae" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.CF == 0 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setae" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setb) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.CF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setb" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.CF == 1 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setb" " %s", ModR_M_asm);
		return len + 2;
	}
}


make_helper(setbe) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.CF == 1 || cpu.ZF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setbe" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.CF == 1 || cpu.ZF == 1)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setbe" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(sete) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.ZF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("sete" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.ZF == 1 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("sete" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setg) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.ZF == 0 && cpu.SF == cpu.OF )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setg" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.ZF == 0 && cpu.SF == cpu.OF )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setg" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setge) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.SF == cpu.OF )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setge" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.SF == cpu.OF )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setge" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setl) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.SF != cpu.OF )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setl" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.SF != cpu.OF )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("sete" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setle) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.ZF == 1 || cpu.SF == cpu.OF)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setle" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.ZF == 1 || cpu.SF == cpu.OF)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setle" " %s", ModR_M_asm);
		return len + 2;
	}
}


make_helper(setnb) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.CF == 0)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setnb" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.CF == 0)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setnb" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setne) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.ZF == 0)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setne" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.ZF == 0)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setne" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setno) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.OF == 0)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setno" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.OF == 0)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setno" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setnp) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.PF == 0)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setnp" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.PF == 0)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setnp" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setns) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.SF == 0)
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setns" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.SF == 0)
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setns" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(seto) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.OF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("seto" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.OF == 1 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("seto" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(setp) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.PF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("setp" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.PF == 1 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("setp" " %s", ModR_M_asm);
		return len + 2;
	}
}

make_helper(sets) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if (cpu.SF == 1 )
			REG(m.R_M) = 1;
		else REG(m.R_M) = 0;
		print_asm("sets" " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		DATA_TYPE imm;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
     	if (cpu.SF == 1 )
			imm = 1;
		else imm = 0;
		MEM_W(addr, imm);
		print_asm("sets" " %s", ModR_M_asm);
		return len + 2;
	}
}

#include "exec/template-end.h"
