#include "exec/helper.h"

#define DATA_BYTE 1
#include "arithmetic-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "arithmetic-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "arithmetic-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(sub_i2r_v) {
	return ( suffix == 'l' ? sub_i2r_l(eip) : sub_i2r_w(eip));
}

make_helper(sub_iv2rm_v) {
	return ( suffix == 'l' ? sub_iv2rm_l(eip) : sub_iv2rm_w(eip));
}

make_helper(sub_r2rm_v) {
	return ( suffix == 'l' ? sub_r2rm_l(eip) : sub_r2rm_w(eip));
}

make_helper(sub_rm2r_v) {
	return ( suffix == 'l' ? sub_rm2r_l(eip) : sub_rm2r_w(eip));
}

make_helper(add_ib2rm_v) {
	return ( suffix == 'l' ? add_ib2rm_l(eip) : add_ib2rm_w(eip));
}

make_helper(add_r2rm_v) {
	return ( suffix == 'l' ? add_r2rm_l(eip) : add_r2rm_w(eip));
}

make_helper(add_rm2r_v) {
	return ( suffix == 'l' ? add_rm2r_l(eip) : add_r2rm_w(eip));
}

make_helper(add_iv) {
	return ( suffix == 'l' ? add_il(eip) : add_iw(eip));
}

make_helper(add_i2rm_v) {
	return ( suffix == 'l' ? add_i2rm_l(eip) : add_i2rm_w(eip));
}
			
