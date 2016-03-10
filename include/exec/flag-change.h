#ifndef __FL_H__
#define __FL_H__

#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/reg.h"

void flags(DATA_TYPE src) {
	if (src == 0) cpu.ZF = 1;
	else cpu.ZF = 0;
	if(MSB(src) == 0) cpu.SF = 0;
	else cpu.SF = 1;

	int count = 0;
	src = src & 0xff;
    for (count = 0; src ; count++) {
		src &= src - 1;
	}
	if((count%2) == 1) cpu.PF = 1;
	else cpu.PF = 0;
}

#include "exec/template-end.h"

#endif
