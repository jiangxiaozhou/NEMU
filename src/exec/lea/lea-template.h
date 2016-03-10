#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(lea_m2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	swaddr_t addr;
	int len = read_ModR_M(eip + 1, &addr);
	REG(m.reg) = (DATA_TYPE)addr;
	print_asm("lea"str(SUFFIX)" %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	return 1 + len;
}

#include "exec/template-end.h"
