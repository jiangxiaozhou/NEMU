#include "exec/helper.h"
#include "cpu/reg.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;
uint32_t dram_read(hwaddr_t, size_t);
void init_tlb();

make_helper(mov_i2r_v) {
	return (suffix == 'l' ? mov_i2r_l(eip) : mov_i2r_w(eip));
}

make_helper(mov_i2rm_v) {
	return (suffix == 'l' ? mov_i2rm_l(eip) : mov_i2rm_w(eip));
}

make_helper(mov_r2rm_v) {
	return (suffix == 'l' ? mov_r2rm_l(eip) : mov_r2rm_w(eip));
}

make_helper(cmovbe_r2rm_v) {
	return (suffix == 'l' ? cmovbe_r2rm_l(eip) : cmovbe_r2rm_w(eip));
}

make_helper(cmovge_r2rm_v) {
	return (suffix == 'l' ? cmovge_r2rm_l(eip) : cmovge_r2rm_w(eip));
}

make_helper(cmove_r2rm_v) {
	return (suffix == 'l' ? cmove_r2rm_l(eip) : cmove_r2rm_w(eip));
}

make_helper(cmovs_r2rm_v) {
	return (suffix == 'l' ? cmovs_r2rm_l(eip) : cmovs_r2rm_w(eip));
}

make_helper(cmova_r2rm_v) {
	return (suffix == 'l' ? cmova_r2rm_l(eip) : cmova_r2rm_w(eip));
}

make_helper(mov_rm2r_v) {
	return (suffix == 'l' ? mov_rm2r_l(eip) : mov_rm2r_w(eip));
}

make_helper(mov_a2moffs_v) {
	return (suffix == 'l' ? mov_a2moffs_l(eip) : mov_a2moffs_w(eip));
}

make_helper(mov_moffs2a_v) {
	return (suffix == 'l' ? mov_moffs2a_l(eip) : mov_moffs2a_w(eip));
}

make_helper(movs_m2m_v) {
	return (suffix == 'l' ? movs_m2m_l(eip) : movs_m2m_w(eip));
}

make_helper(movsbv) {
	return (suffix == 'l' ? movsbl(eip) : movsbw(eip));
}

make_helper(movzbv) {
	return (suffix == 'l' ? movzbl(eip) : movzbw(eip));
}

make_helper(mov_cr2r) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.R_M == 0) 
		reg_l(m.reg) = cpu.cr0.val;
	else if(m.R_M == 3)
		reg_l(m.reg) = cpu.cr3.val;
	print_asm("mov" " %%%s,%%%s", crs[m.R_M], regsl[m.reg]);
	return 2;
}

make_helper(mov_r2cr) {
	Log("hehe");
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	Log("%x", eip + 1);
	if(m.reg == 0) 
		cpu.cr0.val = reg_l(m.R_M);
	else if(m.reg == 3) {
        init_tlb();
		cpu.cr3.val = reg_l(m.R_M);
	}
	print_asm("mov" " %%%s,%%%s", regsl[m.R_M], crs[m.reg]);
	return 2;
}

make_helper(mov_r2sreg) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		switch(m.reg) {
			case 0:cpu.es.val = reg_l(m.R_M);
				   cpu.es.hide_val = (((uint64_t)dram_read(cpu.gdtr.base + cpu.es.INDEX * 8 + 4, 4)) << 32) + dram_read(cpu.gdtr.base + cpu.es.INDEX * 8, 4);
				   print_asm("mov" " %%%s,%%es" , regsl[m.R_M]);
				   return 2;
			case 2:cpu.ss.val = reg_l(m.R_M);
				   cpu.ss.hide_val = (((uint64_t)dram_read(cpu.gdtr.base + cpu.ss.INDEX * 8 + 4, 4)) << 32) + dram_read(cpu.gdtr.base + cpu.ss.INDEX * 8, 4);
				   print_asm("mov" " %%%s,%%ss" , regsl[m.R_M]);
				   return 2;
			case 3:cpu.ds.val = reg_l(m.R_M);
				   cpu.ds.hide_val = (((uint64_t)dram_read(cpu.gdtr.base + cpu.ds.INDEX * 8 + 4, 4)) << 32) + dram_read(cpu.gdtr.base + cpu.ds.INDEX * 8, 4);
				   print_asm("mov" " %%%s,%%ds" , regsl[m.R_M]);
				   return 2;
		}
	}
	return 0;
}
