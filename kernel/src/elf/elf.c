#include "common.h"
#include "memory.h"
#include <string.h>

#include <elf.h>

#ifdef HAS_DEVICE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
void video_mapping_write_test();
void video_mapping_read_test();
void video_mapping_clear();

uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

#ifdef HAS_DEVICE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
#else
	/* The ELF file is located at memory address 0 */
	elf = (void *)0x0;
#endif

	/* Load each program segment */
	ph = (void *)elf->e_phoff;
	int i = 0;
	for(; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {
			int j;
			void* dst;
			const void* src;
			dst = (void*)mm_malloc(ph->p_vaddr, ph->p_memsz);
			src = (void*)elf + ph->p_offset;
			for(j = 0; j < ph->p_filesz; j++) {
				*(char*)dst = *(char*)src;
				dst = (char*)dst + 1;
				src = (char*)src + 1;
			}
			for(j = 0; j < ph->p_memsz - ph->p_filesz; j++) {
				*(char*)dst = 0;
				dst = (char*)dst + 1;
			}

			/* Record the prgram break for future use. */

			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
			ph++;
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
