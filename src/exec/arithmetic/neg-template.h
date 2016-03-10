#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

void concat(neg_flags_, SUFFIX)(DATA_TYPE src) {
	cpu.CF = (src == 0) ? 1 : 0;
	cpu.OF = (src == 0x80 || src == 0x8000 || src == 0x80000000 ) ? 1 : 0;
    DATA_TYPE temp = -src;
	cpu.ZF = (temp == 0) ? 1 : 0;
	cpu.SF = MSB(temp);
	int count = 0;
	temp &= 0xff;
	for(; temp ; count++)
		temp &= temp - 1;
	cpu.PF = count % 2;
}

make_helper(concat(neg_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		concat(neg_flags_,SUFFIX) (REG(m.R_M));
		REG(m.R_M) = -REG(m.R_M);
		print_asm("neg"str(SUFFIX) " %%%s",REG_NAME(m.R_M));
		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(neg_flags_,SUFFIX) (MEM_R(addr));
		MEM_W(addr, -MEM_R(addr));
		print_asm("neg"str(SUFFIX) " %s", ModR_M_asm);
		return 1 +len;
	}
}

#include "exec/template-end.h"
