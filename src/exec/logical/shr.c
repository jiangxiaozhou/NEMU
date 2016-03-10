#include "exec/helper.h"

#define DATA_BYTE 1
#include "shr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "shr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "shr-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(shr_ib2rm_v) {
	return ( suffix == 'l' ? shr_ib2rm_l(eip) : shr_ib2rm_w(eip));
}

make_helper(shr_rm_v) {
	return ( suffix == 'l' ? shr_rm_l(eip) : shr_rm_w(eip));
}

make_helper(shr_c2rm_v) {
	return ( suffix == 'l' ? shr_c2rm_l(eip) : shr_c2rm_w(eip));
}
