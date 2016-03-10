#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "nemu.h"
#include "memory.h"
#include <setjmp.h>

#define LOADER_START 0x100000

int exec(swaddr_t);
void load_prog();
void init_dram();
void isfirst_time();
void reset_bp();
BP *get_head();
void watchpoint();
void init_cache();
void init_tlb();
swaddr_t eip_last = 0;
char assembly[40];
jmp_buf jbuf;	/* Make it easy to perform exception handling */

extern uint8_t loader [];
extern uint32_t loader_len;

extern int quiet;

void restart() {
	/* Perform some initialization to restart a program */
	load_prog();
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);

	cpu.eip = LOADER_START;
	cpu.esp = 0x8000000;
	cpu.ebp = 0x0;
	cpu.eflags = 0x0;
	cpu.cr0.protect_enable = 0;
	cpu.cr0.paging = 0;
	
	cpu.cs.hide.limit_15_0 = 0xffff;
	cpu.cs.hide.limit_19_16 = 0xf;
	cpu.cs.hide.base_15_0 = 0x0;
	cpu.cs.hide.base_23_16 = 0x0;

	init_tlb();
	init_cache();
	init_dram();
}

static void print_bin_instr(swaddr_t eip, int len) {
	int i;
	printf("%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		printf("%02x ", swaddr_read(eip + i, 1));
	}
	printf("%*.s", 50 - (12 + 3 * len), "");
}

void cpu_exec(volatile uint32_t n) {
	isfirst_time();
	volatile uint32_t n_temp = n;
	setjmp(jbuf);
	for(; n > 0; n --) {
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);
		cpu.eip += instr_len;

		reset_bp();
        watchpoint();
		if(n_temp != -1 || (enable_debug && !quiet)) {
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);
		}

		if(nemu_state == INT) {
			printf("\n\nUser interrupt\n");
			return;
		} 
		if(nemu_state == STOP) return;
		else if(nemu_state == END) { return; }
	}
}
