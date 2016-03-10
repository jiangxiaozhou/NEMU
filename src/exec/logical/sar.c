#include "exec/helper.h"

#define DATA_BYTE 1
#include "sar-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sar-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sar-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(sar_ib2rm_v) {
	return ( suffix == 'l' ? sar_ib2rm_l(eip) : sar_ib2rm_w(eip));
}

make_helper(sar_rm_v) {
	return ( suffix == 'l' ? sar_rm_l(eip) : sar_rm_w(eip));
}

make_helper(sar_c2rm_v) {
	return ( suffix == 'l' ? sar_c2rm_l(eip) : sar_c2rm_w(eip));
}
