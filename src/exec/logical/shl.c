#include "exec/helper.h"

#define DATA_BYTE 1
#include "shl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "shl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "shl-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(shl_ib2rm_v) {
	return ( suffix == 'l' ? shl_ib2rm_l(eip) : shl_ib2rm_w(eip));
}

make_helper(shl_rm_v) {
	return ( suffix == 'l' ? shl_rm_l(eip) : shl_rm_w(eip));
}

make_helper(shl_c2rm_v) {
	return ( suffix == 'l' ? shl_c2rm_l(eip) : shl_c2rm_w(eip));
}
