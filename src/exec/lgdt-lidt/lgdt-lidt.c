#include "exec/helper.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

uint32_t swaddr_read(uint32_t , size_t);

make_helper(lgdt) {
	if (instr_fetch(eip + 1, 1) == 0x15) {
		uint32_t imm = instr_fetch(eip + 2, 4);
		cpu.gdtr.limit = swaddr_read( imm, 2);
		cpu.gdtr.base = swaddr_read( imm + 2, 4);
		print_asm("lgdt" " $0x%x", imm);
		return 6;
	} else if (instr_fetch(eip + 1, 1) == 0x10){
		cpu.gdtr.limit = swaddr_read( cpu.eax, 2);
		cpu.gdtr.base = swaddr_read( cpu.eax + 2, 4);
		print_asm("lgdt" " (%%eax)");
		return 2;
	} else if (instr_fetch(eip + 1, 1) == 0x18) {
		cpu.idtr.limit = swaddr_read( cpu.eax, 2);
		cpu.idtr.base = swaddr_read( cpu.eax + 2, 4);
		print_asm("lidt" " (%%eax)");
		return 2;
	}
	return 0;
}

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.cs.val = swaddr_read(cpu.esp, 2);
	cpu.esp += 2;
	cpu.eflags = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	print_asm("iret");
	return 2;
}

	
