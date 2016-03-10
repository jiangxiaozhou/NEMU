#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();
extern inline PDE* get_kpdir();
static PTE ptable_s[1024] align_to_page;

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	uint32_t pdir_addr;
	asm volatile("movl %%cr3,%0" : "=r"(pdir_addr));
	pdir_addr &= 0xfffff000;
	pdir_addr -= 3000;
	uint32_t addr = VMEM_ADDR;
	*(uint32_t *)pdir_addr = (uint32_t)va_to_pa(ptable_s);
	int i = ((addr & 0x3ff000) >> 12);
	while(addr <= 0xaf000) {
		ptable_s[i].val = make_pte(addr);
		i++;
		addr += 0x1000;
	}
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

