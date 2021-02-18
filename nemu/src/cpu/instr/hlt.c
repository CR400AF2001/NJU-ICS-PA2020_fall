#include "cpu/instr.h"
#include <unistd.h>

make_instr_func(hlt)
{
	print_asm_0("hlt", "", 1);
#ifdef HAS_DEVICE_TIMER
	is_nemu_hlt = true;
#endif
	return 1;
}
