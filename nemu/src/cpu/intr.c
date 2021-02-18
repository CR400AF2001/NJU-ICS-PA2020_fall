#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	OPERAND r;
    r.sreg = SREG_SS;
    r.type = OPR_MEM;
    
    cpu.esp -= 4;
    r.data_size = 32;
    r.addr = cpu.esp;
    r.val = cpu.eflags.val;
    operand_write(&r);

    cpu.esp -= 4;
    r.data_size = 32;
    r.addr = cpu.esp;
    r.val = cpu.segReg[1].val;
    operand_write(&r);
    
    cpu.esp -= 4;
    r.data_size = 32;
    r.addr = cpu.esp;
    r.val = cpu.eip;
    operand_write(&r);

    GateDesc* gateDesc = (GateDesc*) (hw_mem + page_translate(segment_translate(cpu.idtr.base + 8 * intr_no, SREG_CS)));
    if(gateDesc->type == 0xe){
        cpu.eflags.IF = 0;
    }
    uint32_t addr = (gateDesc->offset_31_16 << 16) + gateDesc->offset_15_0;
    cpu.segReg[1].val = gateDesc->selector;
    //load_sreg(1);
    cpu.eip = addr;

#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
