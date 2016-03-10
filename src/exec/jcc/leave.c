#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(leave) {
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("leave");
	return 1;
}

make_helper(ret) {
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}

make_helper(ret_b) {
	int16_t imm = instr_fetch(eip + 1, 2);
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += imm;
	print_asm("ret" " $0x%x", imm);
	return 0;
}

