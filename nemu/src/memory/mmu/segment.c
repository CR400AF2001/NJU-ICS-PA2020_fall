#include "cpu/cpu.h"
#include "memory/memory.h"

extern uint8_t hw_mem[];

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t segDesAddr = (uint32_t) hw_mem + cpu.gdtr.base + 8 * cpu.segReg[sreg].index;
	SegDesc* segDesc = (SegDesc*) segDesAddr;
	uint32_t base = (segDesc->base_31_24 << 24) + (segDesc->base_23_16 << 16) + segDesc->base_15_0;
	uint32_t limit = (segDesc->limit_19_16 << 16) + segDesc->limit_15_0;
	uint32_t privilege_level = segDesc->privilege_level;
	assert(base == 0);
	assert(limit == 0xFFFFF);
	assert(segDesc->granularity == 1);
	assert(segDesc->present == 1);
	cpu.segReg[sreg].base = base;
	cpu.segReg[sreg].limit = limit;
	cpu.segReg[sreg].privilege_level = privilege_level;
}
