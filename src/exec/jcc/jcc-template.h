#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(jez_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if(cpu.ZF == 1) {
		cpu.eip += imm;
	    print_asm("je" " %x",cpu.eip + 2);
	}
	else print_asm("je" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jb_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.CF == 1) {
		cpu.eip += imm;
	    print_asm("jb" " %x",cpu.eip + 2);
	}
	else print_asm("jb" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jbe_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.CF == 1 || cpu.ZF == 1) {
		cpu.eip += imm;
	    print_asm("jbe" " %x",cpu.eip + 2);
	}
	else print_asm("jbe" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jl_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.SF != cpu.OF) {
		cpu.eip += imm;
	    print_asm("jl" " %x",cpu.eip + 2);
	}
	else print_asm("jl" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jle_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.ZF == 1 || cpu.SF != cpu.OF ) {
		cpu.eip += imm;
	    print_asm("jle" " %x",cpu.eip + 2);
	}
	else print_asm("jle" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}


make_helper(concat(jg_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.ZF == 0 && cpu.SF == cpu.OF) {
		cpu.eip += imm;
	    print_asm("jg" " %x",cpu.eip + 2);
	}
	else print_asm("jg" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jge_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.SF == cpu.OF) {
		cpu.eip += imm;
	    print_asm("jge" " %x",cpu.eip + 2);
	}
	else print_asm("jge" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(ja_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.CF == 0 && cpu.ZF == 0) {
		cpu.eip += imm;
	    print_asm("ja" " %x",cpu.eip + 2);
	}
	else print_asm("ja" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jae_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.CF == 0) {
		cpu.eip += imm;
	    print_asm("jae" " %x",cpu.eip + 2);
	}
	else print_asm("jae" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jne_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.ZF == 0) {
		cpu.eip += imm;
	    print_asm("jne" " %x",cpu.eip + 2);
	}
	else print_asm("jne" " %x",cpu.eip + 2 + imm);
	//Log("hehe");
	return 1 + DATA_BYTE;
}

make_helper(concat(jno_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.OF == 0) {
		cpu.eip += imm;
	    print_asm("jno" " %x",cpu.eip + 2);
	}
	else print_asm("jno" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jnp_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.PF == 0) {
		cpu.eip += imm;
	    print_asm("jnp" " %x",cpu.eip + 2);
	}
	else print_asm("jnp" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jns_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.SF == 0) {
		cpu.eip += imm;
	    print_asm("jns" " %x",cpu.eip + 2);
	}
	else print_asm("jns" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jo_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.OF == 1) {
		cpu.eip += imm;
	    print_asm("jo" " %x",cpu.eip + 2);
	}
	else print_asm("jo" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jpe_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.PF == 1) {
		cpu.eip += imm;
	    print_asm("jpe" " %x",cpu.eip + 2);
	}
	else print_asm("jpe" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(js_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	if (cpu.SF == 1) {
		cpu.eip += imm;
	    print_asm("js" " %x",cpu.eip + 2);
	}
	else print_asm("js" " %x",cpu.eip + 2 + imm);
	return 1 + DATA_BYTE;
}

#include "exec/template-end.h"
