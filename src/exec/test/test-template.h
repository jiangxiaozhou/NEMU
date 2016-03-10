#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

void concat(test_flags_,SUFFIX) (DATA_TYPE a,DATA_TYPE b) {
	    DATA_TYPE temp = a & b;
        int count = 0;
	    cpu.CF = 0;
	    cpu.OF = 0;
	    if (temp == 0) cpu.ZF = 1;
	    else cpu.ZF = 0;
	    if (MSB(temp) != 0) cpu.SF = 1;
	    else cpu.SF = 0;
	    temp = temp & 0xff;
	    for (count = 0; temp ; count++)
		    temp &= temp - 1;
	    if( (count%2) == 1) cpu.PF = 1;
	    else cpu.PF = 0;
}

make_helper(concat(test_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) { 
        concat(test_flags_,SUFFIX)(REG(m.R_M),REG(m.reg));
	    print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else 
	{
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		concat(test_flags_,SUFFIX)( REG(m.reg) , MEM_R(addr));
        print_asm("test" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(m.reg));
		return len + 1;
	}
}

make_helper(concat(test_a2r_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    concat(test_flags_,SUFFIX)(REG(0),imm);
	print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(0));
	return 1 + DATA_BYTE;
}

make_helper(concat(test_i2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) { 
		DATA_TYPE imm = instr_fetch(eip + 2, DATA_BYTE);
        concat(test_flags_,SUFFIX)( REG(m.R_M),imm);
	    print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 2 + DATA_BYTE;
	}
	else 
	{
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		concat(test_flags_,SUFFIX)( imm, MEM_R(addr));
        print_asm("test" str(SUFFIX) " 0x%x,%%%s", imm, ModR_M_asm);
		return len + 1 + DATA_BYTE;
	}
}


#include "exec/template-end.h"
