#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	PDE* p1 = (PDE*) (hw_mem + (cpu.cr3.pdbr << 12) + 4 * ((laddr >> 22) & 0x000003FF));
	assert(p1->present == 1);
	PTE* p2 = (PTE*) (hw_mem + (p1->page_frame << 12) + 4 * ((laddr >> 12) & 0x000003FF));
	assert(p2->present == 1);
	uint32_t paddr = (p2->page_frame << 12) + (laddr & 0x00000FFF);
	return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
