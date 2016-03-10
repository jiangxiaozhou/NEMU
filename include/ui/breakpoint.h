#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

enum {breakp,watchp};

typedef struct breakpoint {
	int NO;
	int type;
	uint32_t addr;
	struct breakpoint *next;
	uint8_t exec_num;
	char expr[100];
	uint32_t val;

	/* TODO: Add more members if necessary */


} BP;
bool first_time;
bool break_;

#endif
