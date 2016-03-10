#include "ui/ui.h"
#include "memory.h"
#include "ui/breakpoint.h"
#include <ctype.h>
#include "nemu.h"
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int nemu_state = END;
CPU_state cpu;
void recovery_all();
BP *get_head();
void cpu_exec(uint32_t);
void restart();
BP* new_bp();
void free_bp(uint32_t);
void print_bp();
uint32_t expr(char* , bool*);
void cmd_b(uint32_t);
void init_bp_pool();
void print_cache(hwaddr_t addr);

extern bool first_time;

void print_now_func(swaddr_t,int);
uint32_t dram_read(hwaddr_t, size_t);


/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) { 
		nemu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

static void cmd_c() {
	if(nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
	}
	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP;}
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
 			}
 		}
 	}


restart_:
	restart();
	nemu_state = STOP;
	first_time = true;
	cmd_c();
}

static uint32_t cmd_p(char *p){
   bool succ = true;
   uint32_t result = 0;
   expr(p, &succ);
   if(succ == false) assert(0);
   else {
	   result = expr(p, &succ);
	   printf("Result:%u\n",result);
   }
   return result;
}

static void cmd_bt() {
	swaddr_t ebp = cpu.ebp;
	swaddr_t addr = cpu.eip;
	if(cpu.eip <= 0x800000) {
		printf("No stack");
		return;
	}
	int index = 0;
	print_now_func(addr,index);
	index++;
	addr = swaddr_read(ebp + 4, 4);
	ebp = swaddr_read(ebp , 4);
	while(ebp != 0 && addr!=0 ){
	    print_now_func(addr,index);
        index++;
	    addr = swaddr_read(ebp + 4, 4);
	    ebp = swaddr_read(ebp , 4);
	}
}


static void cmd_w(char *p) {
	bool succ = true;
	uint32_t result = expr(p,&succ);
	if(succ == false) assert(0);
	else {
		BP *w = new_bp();
		if(w == NULL) { assert(0); return;}
		w->type = watchp;
		strcpy(w->expr,p);
		printf("%s\n",p);
		printf("%s\n",w->expr);
	    w->val = result;
	}
}


static void cmd_si (int n) {
	if(nemu_state == END) {
		restart();
	}

	nemu_state = RUNNING;
	cpu_exec(n);
	if(nemu_state != END) {
		nemu_state = STOP;
	}
}

static void print_reg() {
	printf("eax         0x%x       %d\n",cpu.eax,cpu.eax);
	printf("ecx         0x%x       %d\n",cpu.ecx,cpu.ecx);
	printf("edx         0x%x       %d\n",cpu.edx,cpu.edx);
	printf("ebx         0x%x       %d\n",cpu.ebx,cpu.ebx);
	printf("esp         0x%x       0x%x\n",cpu.esp,cpu.esp);
	printf("ebp         0x%x       0x%x\n",cpu.ebp,cpu.ebp);
	printf("esi         0x%x       %d\n",cpu.esi,cpu.esi);
	printf("edi         0x%x       %d\n",cpu.edi,cpu.edi);
	printf("eip         0x%x       0x%x\n",cpu.eip,cpu.eip);
	printf("CF:%d PF:%d ZF:%d SF:%d IF:%d DF:%d OF:%d\n",cpu.CF,cpu.PF,cpu.ZF,cpu.SF,cpu.IF,cpu.DF,cpu.OF);
	printf("(lnaddr)gdtr.base = %x,limit = %x\n",cpu.gdtr.base,cpu.gdtr.limit);
	printf("(lnaddr)idtr.base = %x,limit = %x\n",cpu.idtr.base,cpu.idtr.limit);
	printf("cr0         0x%x\n",cpu.cr0.val);
	printf("cr3         0x%x\n",cpu.cr3.val);
	printf("cs          0x%x  hide:0x%016llx\n",cpu.cs.val,cpu.cs.hide_val);
	printf("ds          0x%x hide:0x%016llx\n",cpu.ds.val,cpu.ds.hide_val);
	printf("ss          0x%x hide:0x%016llx\n",cpu.ss.val,cpu.ss.hide_val);
	printf("es          0x%x hide:0x%016llx\n",cpu.es.val,cpu.es.hide_val);
}


static void print_memory(uint32_t addr,int num) {
	int i;
	for(i=0;i < num; i++)
	{
	   int j=0;
	   printf("0x%x:", addr);
	   for(j=0;j<4;j++){
		   unsigned int data = swaddr_read(addr,1);
		   printf(" %02x",data);
		   addr = addr + 1;
	   }
	   printf("\n");
	}
}

bool isdigit1(char* p)
{
	for(;*p!='\0';p++)
	{
		if((*p >= '0' && *p <= '9') || (*p>='a'&&*p<='f')||(*p>='A'&&*p<='F'))
			continue;
		else return false;
	}
	return true;
}

int32_t char2x(char* p){
	int32_t sum = 0;
	while(*p != '\0')
	{
		uint32_t n;
		if (*p >='0' && *p <= '9')
			 n = *p - '0';
		else if (*p >= 'A' && *p <= 'F')
			 n = *p - 55;
		else
			 n = *p - 87;
		sum = sum *16 + n;
		p++;
	}
	return sum;
}


void main_loop() {
	char *cmd;
	while(1) {
		cmd = rl_gets();
		char *p = strtok(cmd, " ");

		if(p == NULL) { continue; }

		if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "q") == 0) { return; }
		else if(strcmp(p, "bt") == 0) { cmd_bt(); }
		else if(strcmp(p, "p") == 0) {
			p = strtok(NULL, "\0");
			if(p == NULL) return;
			else
			  cmd_p(p);
		}
		else if(strcmp(p, "b") == 0) {
			p = strtok(NULL," ");
			if(p == NULL) continue;
			else if(*p != '*') assert(0); 
			else{
				p++;
				cmd_b(cmd_p(p));
			}
		}
		else if(strcmp(p, "w") == 0) {
			p = strtok(NULL,"\0");
			if(p == NULL) return;
			else 
				cmd_w(p);
			
		}
		else if (strcmp(p, "d") == 0){
		   p = strtok(NULL," ");
	       if (p == NULL) { recovery_all(); init_bp_pool();}
           else if(isdigit(*p)) free_bp(atoi(p));
		}
        else if(strcmp(p, "si") == 0)
		{
			p = strtok(NULL," ");
			if (p == NULL)
            cmd_si(1);
			else 	
				cmd_si(atoi(p));
		}
        else if(strcmp(p, "info") == 0)
		{
			p = strtok(NULL," ");
			if (p == NULL)
				continue;
			else if(strcmp(p, "r") == 0)
				print_reg();
			else if(strcmp(p, "b") == 0)
				print_bp();
			else if(strcmp(p, "cache") == 0) {
				p = strtok(NULL," ");
				print_cache(cmd_p(p));
			}

		}
		else if(strcmp(p, "x") == 0)
		{
			p = strtok(NULL," ");
			if (p == NULL)
				continue;
			else {
				int num = atoi(p);
				p = strtok(NULL," ");
				if (p == NULL)
				{
					printf("Please input true address format!");
					continue;
				}
				else 
					print_memory(cmd_p(p),num);
			    }
		}
	

		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
	}
}
