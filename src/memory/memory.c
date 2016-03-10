#include "common.h"
#include "cpu/reg.h"

extern int current_sreg;
extern CPU_state cpu;

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);
uint32_t lnaddr_read(lnaddr_t addr, size_t len);
void lnaddr_write(lnaddr_t addr, size_t len,uint32_t data);
hwaddr_t page_translate(lnaddr_t addr);

/* Memory accessing interfaces */

lnaddr_t segment_translate(swaddr_t addr, int sreg_index) {
	lnaddr_t offset = 0;
	switch(sreg_index) {
		case 1:offset = (((uint32_t)cpu.cs.hide.base_31_24 )<< 24) + (((uint32_t)cpu.cs.hide.base_23_16)<< 16) + cpu.cs.hide.base_15_0;break; 
		case 0:offset = (((uint32_t)cpu.es.hide.base_31_24 )<< 24) + (((uint32_t)cpu.es.hide.base_23_16)<< 16) + cpu.es.hide.base_15_0;break;
		case 2:offset = (((uint32_t)cpu.ss.hide.base_31_24 )<< 24) + (((uint32_t)cpu.ss.hide.base_23_16)<< 16) + cpu.ss.hide.base_15_0;break;
		case 3:offset = (((uint32_t)cpu.ds.hide.base_31_24 )<< 24) + (((uint32_t)cpu.ds.hide.base_23_16)<< 16) + cpu.ds.hide.base_15_0;break;
	}
	//Log(" offset = %x", offset);
	return addr + offset;
}

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return dram_read(addr, len);
	//return cache_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	//cache_write(addr, len, data);
	dram_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	if(cpu.cr0.protect_enable == 0) return hwaddr_read(addr, len);
	lnaddr_t lnaddr = segment_translate(addr, current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
//	Log("%x  %x",data,addr);
//	assert(addr!=0xbfffffa4);
	if(cpu.cr0.protect_enable == 0) {
		hwaddr_write(addr, len, data);
		return;
	}
	lnaddr_t lnaddr = segment_translate(addr, current_sreg);
	lnaddr_write(lnaddr, len, data);
}

uint32_t lnaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_t hwaddr = addr;
	if(cpu.cr0.protect_enable == 1 && cpu.cr0.paging == 1) {
		if (1 == 0) {

			assert(0);
		} else {
			hwaddr = page_translate(addr);
		}
	}
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_t hwaddr = addr;
	if(cpu.cr0.protect_enable == 1 && cpu.cr0.paging == 1) {
		if (1 == 0) {

			assert(0);
		} else {
			hwaddr = page_translate(addr);
		}
	}
	hwaddr_write(hwaddr, len, data);
}

uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return cache_read(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, 1);
	if(cpu.cr0.protect_enable == 0) 
		return hwaddr_read_instr(addr, len);
	return lnaddr_read(lnaddr, len);
}
