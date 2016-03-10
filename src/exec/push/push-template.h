#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(push_r, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	//DATA_TYPE temp = REG(reg_code);
#if DATA_BYTE == 1 
	swaddr_write(cpu.esp-4,4,(int32_t)REG(reg_code));
	cpu.esp -= 4;
#else 
    swaddr_write(cpu.esp - DATA_BYTE,DATA_BYTE, REG(reg_code));
	cpu.esp -= DATA_BYTE;
#endif
	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(push_i_, SUFFIX)) {
	//Log("hehe");
	Log("%x", cpu.esp);
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
#if DATA_BYTE == 1
	cpu.esp -= 4; 
	Log("%x", cpu.esp);
	MEM_W(cpu.esp, (int32_t)imm);
#else
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, imm);
#endif
//	Log("haha");
	print_asm("push" str(SUFFIX) " $0x%x", imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(push_m_, SUFFIX)) {
	swaddr_t addr;
	int len = read_ModR_M(eip + 1, &addr);
	REG(4) -= DATA_BYTE; 
	MEM_W(REG(4), MEM_R(addr));
	print_asm("push" str(SUFFIX) " %s", ModR_M_asm);
	return 1 + len;
}

make_helper(concat(pop_r, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	REG(reg_code) = MEM_R(REG(4));
	REG(4) += DATA_BYTE; 
	print_asm("pop" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(pop_m_, SUFFIX)) {
	swaddr_t addr;
	int len = read_ModR_M(eip + 1, &addr);
	MEM_W(addr, MEM_R(REG(4)));
	REG(4) += DATA_BYTE; 
	print_asm("pop" str(SUFFIX) " %s", ModR_M_asm);
	return 1 + len;
}

#include "exec/template-end.h"
