#include "exec/helper.h"
#include "cpu/reg.h"
#include "setjmp.h"

extern jmp_buf jbuf;
extern CPU_state cpu;
lnaddr_t lnaddr_read(uint32_t, size_t);

void raise_intr(uint16_t NO) {
	lnaddr_t addr = cpu.idtr.base + (NO << 3);
	uint32_t offset = lnaddr_read(addr, 2) + (lnaddr_read(addr + 6, 2) << 16);
	//uint16_t index = inaddr_read(addr + 2 , 2);
	cpu.eip = offset;
	Log("%x", offset);
	longjmp(jbuf, 1);
}

make_helper(int_r) {
	uint16_t imm = instr_fetch (eip + 1, 1);
    cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eflags);
	cpu.esp -= 2;
	swaddr_write(cpu.esp, 2, cpu.cs.val);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	raise_intr(imm);
	print_asm("int" " $0x%x", imm);
	return 2;
}

