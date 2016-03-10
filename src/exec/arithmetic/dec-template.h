#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

bool concat(dec_flags_, SUFFIX)(DATA_TYPE_S a,DATA_TYPE b) {
	DATA_TYPE_S sum = b - a;
	int count = 0;
	if(sum == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if(MSB(sum) == 1) cpu.SF = 1;
	else cpu.SF = 0;
	if((MSB(a) != MSB(b)) && (MSB(a) == MSB(sum))) cpu.OF = 1;
	else cpu.OF = 0;	
	sum = sum & 0xff;
	for (count = 0; sum!=0 ; count++) 		
		sum &= sum - 1;
	if((count % 2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
	return true;
}


make_helper(concat(dec_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	concat(dec_flags_, SUFFIX)(1,REG(reg_code));
	REG(reg_code)--;
	print_asm("dec"str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(dec_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
	    concat(dec_flags_, SUFFIX)( 1, REG(m.reg));
		REG(m.reg)--;
	    print_asm("dec"str(SUFFIX) " %%%s", REG_NAME(m.reg));
	    return 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
	    concat(dec_flags_, SUFFIX)( 1, MEM_R(addr));
		MEM_W(addr, MEM_R(addr)-1);
		print_asm("dec"str(SUFFIX) " %s", ModR_M_asm);
		return 1 + len;
	}
}

#include "exec/template-end.h"
