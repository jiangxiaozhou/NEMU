#include "exec/helper.h"
#include "cpu/modrm.h"
#define DATA_BYTE 4
#include "exec/template-start.h"

uint32_t dram_read(hwaddr_t, size_t);

make_helper(jmp_rel8) {
	int8_t imm = instr_fetch(eip + 1, 1);
	cpu.eip += imm;
	print_asm("jmp" " 0x%x",cpu.eip + 2);
	return 2;
}

make_helper(jmp_rel32) {
	int32_t imm = instr_fetch(eip + 1, 4);
	cpu.eip += imm;
	print_asm("jmp" " 0x%x",cpu.eip + 5);
	return 5;
}

make_helper(jmp_rm32) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		cpu.eip = REG(m.R_M);
		print_asm("jmp" " 0x%x", REG(m.R_M));
		return 0;
	}
	else {
		swaddr_t addr;
		read_ModR_M(eip + 1, &addr);
		int32_t imm = MEM_R(addr);
		cpu.eip = imm;
		print_asm("jmp" " 0x%x", imm);
		return 0;
	}
}

#include "exec/template-end.h"

make_helper(ljmp) {
	uint32_t imm = instr_fetch(eip + 1, 4);
    uint16_t seg = instr_fetch(eip + 5, 2);
	cpu.cs.val = seg;
	cpu.cs.hide_val = (((uint64_t)dram_read(cpu.gdtr.base + cpu.cs.INDEX * 8 + 4, 4)) << 32) + dram_read(cpu.gdtr.base + cpu.cs.INDEX * 8, 4);
	cpu.eip = imm - 7;
	print_asm("ljmp" " $0x%x,$0x%x",seg,imm);
	return 7;
}
