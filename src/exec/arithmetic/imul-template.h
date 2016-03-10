#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

void concat(imul_flag, SUFFIX)(int64_t src) {
	DATA_TYPE_S temp = src;
	if( src != temp) {
		cpu.OF = 1;
		cpu.CF = 1;
	}
	else {
		cpu.OF = 0;
		cpu.CF = 0;
	}
}

make_helper(concat(imul_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
    if(m.mod == 3) {
		REG(m.reg) = (DATA_TYPE_S)REG(m.reg)*(DATA_TYPE_S)REG(m.R_M);
        concat(imul_flag, SUFFIX)( (DATA_TYPE_S)REG(m.reg)*(DATA_TYPE_S)REG(m.R_M));
		print_asm("imul"str(SUFFIX)" %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		REG(m.reg) *= (DATA_TYPE_S)MEM_R(addr);
        concat(imul_flag, SUFFIX)( (DATA_TYPE_S)REG(m.reg)*(DATA_TYPE_S)MEM_R(addr));
		print_asm("imul"str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
}

make_helper(concat(imul_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
    if(m.mod == 3) {
		if (DATA_BYTE == 1) {
			int16_t temp = (int16_t)REG(0) * (int16_t)REG(m.R_M);
            REG(4) = (int8_t)(temp >> 8);
            REG(0) = (int8_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 2) {
			int32_t temp = (int32_t)REG(0) * (int32_t)REG(m.R_M);
            REG(2) = (int16_t)(temp >> 16);
            REG(0) = (int16_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 4) {
			int64_t temp = (int64_t)REG(0) * (int64_t)REG(m.R_M);
            REG(2) = (temp >> 32);
			//Log("%x %x", REG(0),REG(m.R_M));
			//Log("%x", REG(2));
            REG(0) = (uint32_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
		print_asm("imul"str(SUFFIX)" %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
        if (DATA_BYTE == 1) {
			int16_t temp = (int16_t)REG(0) * (int16_t)MEM_R(addr);
            REG(4) = (int8_t)(temp >> 8);
            REG(0) = (int8_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 2) {
			int32_t temp = (int32_t)REG(0) * (int32_t)MEM_R(addr);
            REG(2) = (int16_t)(temp >> 16);
            REG(0) = (int16_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
        if (DATA_BYTE == 4) {
			int64_t temp = (int64_t)REG(0) * (int64_t)MEM_R(addr);
            REG(2) = (int32_t)(temp >> 32);
            REG(0) = (int32_t)temp ;
            concat(imul_flag, SUFFIX)(temp);
		}
		print_asm("imul"str(SUFFIX) " %s", ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(imul_irm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE_S imm;
    if(m.mod == 3) {
		imm = instr_fetch(eip + 2, DATA_BYTE);
		REG(m.reg) = imm * (DATA_TYPE_S)REG(m.R_M);
        concat(imul_flag, SUFFIX)( imm * (DATA_TYPE_S)REG(m.R_M));
		print_asm("imul"str(SUFFIX)" $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		REG(m.reg) = imm * (DATA_TYPE_S)MEM_R(addr);
        concat(imul_flag, SUFFIX)( imm * (DATA_TYPE_S)MEM_R(addr));
		print_asm("imul"str(SUFFIX) " $0x%x,%s,%%%s",imm, ModR_M_asm, REG_NAME(m.reg));
		return 1 + len + DATA_BYTE;
	}
}

make_helper(concat(imul_ibrm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int8_t imm;
    if(m.mod == 3) {
		imm = instr_fetch(eip + 2, 1);
		REG(m.reg) = imm * (DATA_TYPE_S)REG(m.R_M);
        concat(imul_flag, SUFFIX)( imm * (DATA_TYPE_S)REG(m.R_M));
		print_asm("imul"str(SUFFIX)" $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		REG(m.reg) = imm * (DATA_TYPE_S)MEM_R(addr);
        concat(imul_flag, SUFFIX)( imm * (DATA_TYPE_S)MEM_R(addr));
		print_asm("imul"str(SUFFIX) " $0x%x,%s,%%%s",imm, ModR_M_asm, REG_NAME(m.reg));
		return 2 + len ;
	}
}


#include "exec/template-end.h"
