#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

extern PDE *get_updir();
PTE table[NR_PTE] align_to_page;


void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */

	PDE *pdir = (PDE *)va_to_pa(get_updir());
	PTE *ptable = (PTE *)va_to_pa(table);
    uint32_t pdir_idx, ptable_idx, pframe_idx;
	/* fill PDE and PTEs */
    pdir_idx = 0x0;
	pdir[pdir_idx].val = make_pde(ptable);
	//pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);
    ptable += 0x0;
	pframe_idx = 0x0;
	for (ptable_idx = 0x0; ptable_idx < 1024; ptable_idx++){
	    ptable->val = make_pte(pframe_idx << 12);
		pframe_idx++;
		ptable++;
	}
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
