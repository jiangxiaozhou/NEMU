#include "exec/helper.h"
#include "common.h"
#include "ui/breakpoint.h"
#include "ui/ui.h"
#include "memory.h"
#include "nemu.h"

BP* get_head();
extern bool break_;
hwaddr_t page_translate(lnaddr_t addr);
extern uint8_t dram[16][8][1024][1024];
uint32_t dram_read(uint32_t, size_t);

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	assert(0);
}

make_helper(int3) {
	BP *p = get_head();
	if(nemu_state == RUNNING){
	    printf("A breakpoint is hit!\n");
	    nemu_state = STOP;
	}
	while(p!=NULL)
	{
        if(p->addr == cpu.eip) {
			swaddr_write(cpu.eip,1,p->exec_num);
            break;
	 	}
		p=p->next;
	}
    cpu.eip--;
    break_ = true;	
	return 1;
}

make_helper(nemu_trap) {
	if (cpu.eax == 2) {
		Log("%x", cpu.ecx);
		uint32_t addr = page_translate(cpu.ecx);
		while((char)dram_read(addr,1) != '\0'){ 
			printf("%c",dram_read(addr ,1));
			addr++;
		}
	}
	else {
		printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
		nemu_state = END;
	}

	print_asm("nemu trap");
	return 1;
}
