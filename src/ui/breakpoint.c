#include "ui/breakpoint.h"
#include "exec/helper.h"
#include "nemu.h"
#include "memory.h"
#include "ui/ui.h"
#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;
extern bool first_time;
extern bool break_;
extern swaddr_t eip_last;
uint32_t expr(char*,bool*);

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].type = breakp;
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO = i;
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
}

/* TODO: Implement the function of breakpoint */

BP *new_bp(){
	BP *new = free_;
	BP *p = head;
	if (free_ == NULL){
		printf("We don't have enough breakpoints!");
	    assert(0);
	}
	else {
        if(head == NULL)
		    head = new;
        else {
			while(p->next != NULL)
			{
				p=p->next;
			}
			p->next=new;
		}
		free_=free_->next;
		new->next=NULL;
		}
	return new;
}
		
void cmd_b (uint32_t address){
	BP *n = new_bp();
	if(n == NULL){ assert(0); return;}
	n->addr = address;
	n->type = breakp;
	n->exec_num = swaddr_read(address,1);
	swaddr_write(address,1,0xcc);
}

void free_bp(uint32_t bp_NO) {
	BP *bp;
	BP *p = head;
	if (head == NULL){
		assert(0);
		return;
	}
	if (head->NO == bp_NO){
		bp = head;
		if((bp->type) == breakp)
		swaddr_write(bp->addr,1,bp->exec_num);
		head = head->next;
		bp->next = free_;
		free_=bp;
		return;
	}
	while(p->next->NO != bp_NO)
		p = p->next;
	if (p == NULL) { assert(0); return; }
    else bp = p->next;	
	if((bp->type) == breakp)
	swaddr_write(bp->addr,1,bp->exec_num);
	p->next =p->next->next;
	bp->next=free_;
	free_=bp;
}

void print_bp () {
	printf("NUM\tTYPE\tWHAT\n");
	BP *p=head;
	if(p == NULL) return;
	while(p != NULL){
		if(p->type == breakp)
		printf("%d\tbreakpoint\t0x%08x\n",p->NO,p->addr);
		else if (p->type == watchp) {
			printf("%d\twatchpoint\t",p->NO);
			printf("%s\n",p->expr);
		}
		p = p->next;
	}
}

void watchpoint () {
	BP *p = head;
	if(p == NULL) return;
	while(p!=NULL){
		if (p->type == breakp) { p = p->next; continue;}
		else {
			bool succ = true;
			uint32_t result = expr(p->expr, &succ);
			if( p-> val != result)
			{
				printf("NO.%d watchpoint %s old value:%u\n",p->NO,p->expr,p->val);
				printf("NO.%d watchpoint %s now:%u\n",p->NO,p->expr,result);
				p->val = result;
				nemu_state = STOP;
			}
		}
		p = p->next;
	}
}
 
void recovery_all() {
	BP *p = head;
	while(p != NULL)
	{
		if(p->type == breakp)
		swaddr_write(p->addr,1,p->exec_num);
		p = p->next;
	}
}

void isfirst_time(){
	if(first_time){
		BP *p=head;
		while(p!=NULL)
		{
			if(p->type == breakp)
			swaddr_write(p->addr,1,0xcc);
			p=p->next;
		}
		first_time = false;
	}
}
  	
void reset_bp()
{
	BP *p = head;
	if(break_){
		eip_last = cpu.eip;
		break_=false;
	}
	while(p!=NULL){
		if(p->addr == eip_last && p->type == breakp)
			break;
		p=p->next;
	}
	if(p == NULL) return;
	if(cpu.eip>eip_last)
		swaddr_write(eip_last,1,0xcc);
}

BP *get_head(){
	return head;
}
	
