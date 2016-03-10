#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

void concat(mul_flag, SUFFIX)(uint64_t src) {
	DATA_TYPE temp = src;
	if( temp != src) {
		cpu.OF = 1;
		cpu.CF = 1;
	}
	else {
		cpu.OF = 0;
		cpu.CF = 0;
	}
}

make_helper(concat(mul_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
    if(m.mod == 3) {
		if (DATA_BYTE == 1) {
			uint16_t temp = (uint16_t)REG(0) * (uint16_t)REG(m.R_M);
            REG(4) = (uint8_t)(temp >> 8);
            REG(0) = (uint8_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 2) {
			uint32_t temp = (uint32_t)REG(0) * (uint32_t)REG(m.R_M);
            REG(2) = (uint16_t)(temp >> 16);
            REG(0) = (uint16_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 4) {
			uint64_t temp = (uint64_t)REG(0) * (uint64_t)REG(m.R_M);
            REG(2) = (uint32_t)(temp >> 32);
            REG(0) = (uint32_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
		print_asm("mul"str(SUFFIX)" %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
        if (DATA_BYTE == 1) {
			uint16_t temp = (uint16_t)REG(0) * (uint16_t)MEM_R(addr);
            REG(4) = (uint8_t)(temp >> 8);
            REG(0) = (uint8_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 2) {
			uint32_t temp = (uint32_t)REG(0) * (uint32_t)MEM_R(addr);
            REG(2) = (uint16_t)(temp >> 16);
            REG(0) = (uint16_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 4) {
			uint64_t temp = (uint64_t)REG(0) * (uint64_t)MEM_R(addr);
            REG(2) = (uint32_t)(temp >> 32);
            REG(0) = (uint32_t)temp ;
            concat(mul_flag, SUFFIX)(temp);
		}
		print_asm("mul"str(SUFFIX) " %s", ModR_M_asm);
		return 1 + len;
	}
}


#include "exec/template-end.h"
