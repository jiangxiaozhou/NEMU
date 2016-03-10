#include "common.h"
#include <stdlib.h>
#include <time.h>

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
uint32_t L2_cache_read(hwaddr_t addr, size_t len);
void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data);

typedef struct {
	bool valid;
	uint8_t data[64];
	uint32_t tag;
} L1_cache_block;

typedef struct {
    bool valid;
	bool dirty;
	uint8_t data[64];
	uint32_t tag;
} L2_cache_block;

typedef struct {
	union {
		struct {
			uint32_t in_block_index : 6;
			uint32_t group_index : 7;
			uint32_t tag : 14;
		};
		uint32_t addr;
	}; 
} cache_addr;

typedef struct {
	union {
		struct {
			uint32_t in_block_index : 6;
			uint32_t group_index : 12;
			uint32_t tag : 9;
		};
		uint32_t addr;
	};
} L2_cache_addr;

static L1_cache_block L1_cache[1 << 7][8];
static L2_cache_block L2_cache[1 << 12][16];

void init_cache(){
	int i, j;
	for(i = 0; i < 128; i++)
		for(j = 0; j < 8; j++) {
			L1_cache[i][j].valid = false;
			L1_cache[i][j].tag = 0;
		}
	for(i = 0; i < (1 << 12); i++)
		for(j = 0; j < 16; j++) {
			L2_cache[i][j].valid = false;
			L2_cache[i][j].dirty = false;
			L2_cache[i][j].tag = 0;
		}
}

void print_cache(hwaddr_t addr) {
	cache_addr temp;
	temp.addr = addr;
	int i;
	Log("group_index = %x", temp.group_index);
	for(i = 0;i < 8; i++) {
		printf("tag = %x, valid = %d\n",L1_cache[temp.group_index][i].tag, L1_cache[temp.group_index][i].valid);
		int j;
		for(j = 0;j < 64; j++)
			printf(" %02x", L1_cache[temp.group_index][i].data[j]);
		printf("\n");
	}
}


uint32_t cache_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	cache_addr temp;
	uint32_t offset = addr & 0x3f;
	temp.addr = addr;
	uint32_t group_index = temp.group_index; 
	bool hit = false;
	bool full = true;
    int i;
	for (i = 0;i < 8;i++) {
       if(L1_cache[group_index][i].valid == true && L1_cache[group_index][i].tag == temp.tag) {
		   hit = true;
		   break;
	   }
	}
	
	if (hit == false) {
		i = 0;
		while (i < 8) {
			if(L1_cache[group_index][i].valid == false) {
				full = false;
				break;
			}
			i++;
		}
		int j;
		if(!full) {
			uint32_t addr_start = addr / 64 * 64;
            for(j = 0; j < 64; j++) {
				L1_cache[group_index][i].data[j] = L2_cache_read(addr_start + j, 1);
			}
			L1_cache[group_index][i].tag = temp.tag;
			L1_cache[group_index][i].valid = true;
		}
		else {
			uint32_t addr_start = addr / 64 * 64;
			srand(time(0));
			i = rand() % 8;
            for(j = 0; j < 64; j++) {
				L1_cache[group_index][i].data[j] = L2_cache_read(addr_start + j, 1);
			}
			L1_cache[group_index][i].tag = temp.tag;
		}
	}
     
	if (((addr / 64 * 64) != ((addr + len) / 64 * 64)) && (len > 1)) return dram_read( addr, len);
	return *(uint32_t *)(L1_cache[group_index][i].data + offset) & (~0u >> ((4 - len) << 3));
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	cache_addr temp;
	temp.addr = addr;
	uint32_t group_index = temp.group_index; 
	bool hit = false;
    int i;

	for (i = 0;i < 8;i++) {
       if(L1_cache[group_index][i].valid == true && L1_cache[group_index][i].tag == temp.tag) {
		   hit = true;
		   break;
	   }
	}
	
	/* write through */
	if(hit == true) {
	   dram_write(addr, len, data);
	   L2_cache_write(addr, len, data);
	   int j;
	   for (j = 0;j < len; j++) {
		   L1_cache[group_index][i].data[temp.in_block_index + j] = dram_read(addr + j,1);
	   }
	}
    
	/* not write allocate */
	if (hit == false) { 
		dram_write(addr , len ,data);
		L2_cache_write(addr, len, data);
	}
		
}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	L2_cache_addr temp;
	uint32_t offset = addr & 0x3f;
	temp.addr = addr;
	uint32_t group_index = temp.group_index; 
	bool hit = false;
	bool full = true;
    int i;
	for (i = 0;i < 16;i++) {
       if(L2_cache[group_index][i].valid == true && L2_cache[group_index][i].tag == temp.tag) {
		   hit = true;
		   break;
	   }
	}
	
	if (hit == false) {
		i = 0;
		while (i < 16) {
			if(L2_cache[group_index][i].valid == false) {
				full = false;
				break;
			}
			i++;
		}
		int j;
		if(!full) {
			uint32_t addr_start = addr / 64 * 64;
            for(j = 0; j < 64; j++) {
				L2_cache[group_index][i].data[j] = dram_read(addr_start + j, 1);
			}
			L2_cache[group_index][i].tag = temp.tag;
			L2_cache[group_index][i].valid = true;
		}
		else {
			uint32_t addr_start = addr / 64 * 64;
			srand(time(0));
			i = rand() % 16;
			if(L2_cache[group_index][i].dirty == true) {                  //write back to dram
                L2_cache_addr write_back_addr;
				write_back_addr.tag = L2_cache[group_index][i].tag;
				write_back_addr.group_index = group_index;
				write_back_addr.in_block_index = 0;
				for(j = 0;j < 64; j++)
					dram_write( write_back_addr.addr + j, 1, L2_cache[group_index][i].data[j]);
			}

            for(j = 0; j < 64; j++) {
				L2_cache[group_index][i].data[j] = dram_read(addr_start + j, 1);
			}
			L2_cache[group_index][i].tag = temp.tag;
			L2_cache[group_index][i].dirty = false;
		}
	}
     
	if (((addr / 64 * 64) != ((addr + len) / 64 * 64)) && (len > 1)) return dram_read( addr, len);
	return *(uint32_t *)(L2_cache[group_index][i].data + offset) & (~0u >> ((4 - len) << 3));
}

void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	L2_cache_addr temp;
	temp.addr = addr;
	uint32_t group_index = temp.group_index; 
	bool hit = false;
	bool full = true;
    int i;

	for (i = 0;i < 8;i++) {
       if(L2_cache[group_index][i].valid == true && L2_cache[group_index][i].tag == temp.tag) {
		   hit = true;
		   break;
	   }
	}
	
	/* write back */
	if(hit == true) {
	   //int j;
	   //dram_write(addr, len ,data);    false write back
	   L2_cache[group_index][i].dirty = true;
	   /*for (j = 0;j < len; j++) {
		   L2_cache[group_index][i].data[temp.in_block_index + j] = dram_read(addr + j,1);
	   }*/
       if (len == 1) 
		   L2_cache[group_index][i].data[temp.in_block_index] = (uint8_t)data;
	   else if (len == 2)
		   *(uint16_t *)(L2_cache[group_index][i].data + temp.in_block_index) = (uint16_t)(data & (~0u >> ((4 - len) << 3)));
       else if (len == 4)
		   *(uint32_t *)(L2_cache[group_index][i].data + temp.in_block_index) = (uint32_t)(data & (~0u >> ((4 - len) << 3)));

	}
    
	/* write allocate && write back */
	if (hit == false) {
     	i = 0;
		while (i < 16) {
			if(L2_cache[group_index][i].valid == false) {
				full = false;
				break;
			}
			i++;
		}
		int j;
		if(!full) {
			uint32_t addr_start = addr / 64 * 64;
            dram_write(addr, len, data);           
			for(j = 0; j < 64; j++) {
				L2_cache[group_index][i].data[j] = dram_read(addr_start + j, 1);
			}
         	L2_cache[group_index][i].tag = temp.tag;
			L2_cache[group_index][i].valid = true;
		} else {
	        uint32_t addr_start = addr / 64 * 64;
			srand(time(0));
			i = rand() % 16;
			if(L2_cache[group_index][i].dirty == true) {                  //write back to dram
                L2_cache_addr write_back_addr;
				write_back_addr.tag = L2_cache[group_index][i].tag;
				write_back_addr.group_index = group_index;
				for(j = 0;j < 64; j++)
					dram_write( write_back_addr.addr + j, 1, L2_cache[group_index][i].data[j]);
			}
			dram_write(addr , len, data);
            for (j = 0;j < 64; j++) {
		        L2_cache[group_index][i].data[addr_start + j] = dram_read(addr_start + j,1);
	        }
			L2_cache[group_index][i].tag = temp.tag;
			L2_cache[group_index][i].dirty = false;
		}
	}

	/* not write allocate */
	//dram_write(addr, len, data);
}

