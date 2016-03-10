#include "exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(push_r) {
	return (suffix == 'l' ? push_rl(eip) : push_rw(eip));
}

make_helper(push_i_v) {
	return (suffix == 'l' ? push_i_l(eip) : push_i_w(eip));
}

make_helper(push_m_v) {
	return (suffix == 'l' ? push_m_l(eip) : push_m_w(eip));
}

make_helper(pop_r) {
	return (suffix == 'l' ? pop_rl(eip) : pop_rw(eip));
}

make_helper(pop_m_v) {
	return (suffix == 'l' ? pop_m_l(eip) : pop_m_w(eip));
}

make_helper(pusha) {
	uint32_t temp = cpu.esp;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eax);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ecx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, temp);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebp);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.esi);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edi);
	print_asm("pusha");
	return 1;
}

make_helper(popa) {
	cpu.edi = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.esi = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 8;
	cpu.ebx = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.edx = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.ecx = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.eax = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	print_asm("popa");
	return 1;
}
