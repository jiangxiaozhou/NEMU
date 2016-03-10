#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(xchg_a2r_, SUFFIX)){
	int reg_code = eip & 0x7;
	DATA_TYPE temp;
	temp = REG(reg_code);
	REG(reg_code) = REG(0);
	REG(0) = temp;
	print_asm("xchg" " %%%s,%%%s", REG_NAME(0),REG_NAME(reg_code));
	return 1;
}

make_helper(concat(xchg_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE temp;
	if(m.mod == 3) {
		temp = REG(m.reg);
		REG(m.reg) = REG(0);
		REG(m.R_M) = temp;
		print_asm("xchg" " %%%s,%%%s", REG_NAME(m.reg),REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		temp = REG(m.reg);
		REG(m.reg) = MEM_R(addr);	
		MEM_W(addr, temp);
		print_asm("xchg" " %%%s,%s", REG_NAME(m.reg),ModR_M_asm);
		return len + 1;
	}
}
    

#include "exec/template-end.h"
