#include "common.h"
#include "cpu/reg.h"
#include <time.h>
#include <stdlib.h>

extern CPU_state cpu;
uint32_t dram_read(hwaddr_t addr, size_t len);

/* the 32bit Page Directory(first level page table) data structure */
typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1; 
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

typedef union lnaddress_division {
	struct {
		uint32_t offset       : 12;
		uint32_t page         : 10;
		uint32_t dir          : 10;
	};
	uint32_t val;
} lnaddr_div;

typedef struct tlb {
	bool valid;
	uint32_t tag;
	PTE tlb_pte;
} TLB;

TLB tlb[64];

void init_tlb () {
	int i = 0;
	for(; i < 64; i++) {
		tlb[i].valid = false;
		tlb[i].tag = 0;
	}
}

PTE tlb_rw(lnaddr_t addr) {
	bool hit = false;
	bool full = true;
	uint32_t addr_tag = addr & 0xfffff000;
	int i = 0;
	for (; i < 64; i++) {
		if(tlb[i].tag == addr_tag && tlb[i].valid == true) {
			hit = true;
			break;
		}
	}

	if (!hit) {
		lnaddr_div temp;
	    PDE pde;
	    PTE pte;
		temp.val = addr;
		i = 0;
		for(; i < 64; i++) {
			if(tlb[i].valid == false) {
				full = false;
				break;
			}
		}

		if(full) {
			srand(time(0));
			i = rand() % 64;
		}
			
        pde.val = dram_read(((cpu.cr3.val & 0xfffff000) + temp.dir * 4), 4);
//	    assert( pde.present != 0);
	    pte.val = dram_read(((pde.val & 0xfffff000)+ temp.page * 4), 4); 
//	    assert( pte.present != 0);
		tlb[i].tlb_pte.val = pte.val;
		tlb[i].valid = true;
		tlb[i].tag = addr_tag;

	}
	return tlb[i].tlb_pte;
}



hwaddr_t page_translate(lnaddr_t addr) {
	lnaddr_div temp;
	temp.val = addr;
	/*PDE pde;
	PTE pte;
    pde.val = dram_read(((cpu.cr3.val & 0xfffff000) + temp.dir * 4), 4);
	assert( pde.present != 0);
	pte.val = dram_read(((pde.val & 0xfffff000)+ temp.page * 4), 4); 
	assert( pte.present != 0);
    Log(" %x %x %x %x",(cpu.cr3.val & 0xfffff000) + temp.dir * 4, pde.val, (pde.val & 0xfffff000)+ temp.page * 4, pte.val);
	return (pte.val & 0xfffff000) + temp.offset);*/
    return (tlb_rw(addr).val & 0xfffff000) + temp.offset;
}

