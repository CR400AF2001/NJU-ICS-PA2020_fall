#include "common.h"
#include "x86.h"
#include "memory.h"
#include <string.h>

PDE kpdir[NR_PDE] align_to_page;				// kernel page directory
PTE kptable[PHY_MEM / PAGE_SIZE] align_to_page; // kernel page tables

PDE *get_kpdir() { return kpdir; }

/* set up page tables for kernel */
void init_page(void)
{
	CR0 cr0;
	CR3 cr3;
	PDE *pdir = (PDE *)va_to_pa(kpdir);
	PTE *ptable = (PTE *)va_to_pa(kptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;

	/* make all PDE invalid */
	memset(pdir, 0, NR_PDE * sizeof(PDE));

	/* fill PDEs and PTEs */
	pframe_idx = 0;
	for (pdir_idx = 0; pdir_idx < PHY_MEM / PT_SIZE; pdir_idx++)
	{
		pdir[pdir_idx].val = make_pde(ptable);
		pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);
		for (ptable_idx = 0; ptable_idx < NR_PTE; ptable_idx++)
		{
			ptable->val = make_pte(pframe_idx << 12);
			pframe_idx++;
			ptable++;
		}
	}

	/* make CR3 to be the entry of page directory */
	cr3.val = 0;
	cr3.page_directory_base = ((uint32_t)pdir) >> 12;
	write_cr3(cr3.val);

	/* set PG bit in CR0 to enable paging */
	cr0.val = read_cr0();
	cr0.paging = 1;
	write_cr0(cr0.val);
}

/* GDT in the kernel's memory, whose virtual memory is greater than 0xC0000000. */
SegDesc gdt[NR_SEGMENTS];

static void
set_segment(SegDesc *ptr, uint32_t pl, uint32_t type)
{
	ptr->limit_15_0 = 0xFFFF;
	ptr->base_15_0 = 0x0;
	ptr->base_23_16 = 0x0;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = pl;
	ptr->present = 1;
	ptr->limit_19_16 = 0xF;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 1;
	ptr->base_31_24 = 0x0;
}

/* This is similar with the one in start.S. However the previous
 * one cannot be accessed in user process, because its virtual address
 * below 0xC0000000, and is not in the user process' address space. */
void init_segment(void)
{
	memset(gdt, 0, sizeof(gdt));
	set_segment(&gdt[SEG_KERNEL_CODE], DPL_KERNEL, SEG_EXECUTABLE | SEG_READABLE);
	set_segment(&gdt[SEG_KERNEL_DATA], DPL_KERNEL, SEG_WRITABLE);

	write_gdtr(gdt, sizeof(gdt));
}
