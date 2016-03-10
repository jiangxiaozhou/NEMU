#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(not_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if(m.mod == 3) {
		REG(m.R_M) = ~REG(m.R_M);
		print_asm("not"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
		imm = ~imm;
		MEM_W(addr, imm);
		print_asm("not"str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	}
}

#include "exec/template-end.h"
