 #include "cpu/cpu.h"

void set_CF_add(uint32_t res, uint32_t src, size_t data_size){
    res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(res < src){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
}

void set_CF_adc(uint32_t res, uint32_t src, size_t data_size){
    res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(res == src){
        if(cpu.eflags.CF == 1){
            cpu.eflags.CF = 1;
        }
        else{
            cpu.eflags.CF = 0;
        }
    }
    else if(res < src){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
}

void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(dest < src){
        cpu.eflags.CF = 1;
    }
    else{
        cpu.eflags.CF = 0;
    }
}

void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(dest < src){
        cpu.eflags.CF = 1;
    }
    else{
        if(cpu.eflags.CF == 0){
            cpu.eflags.CF = 0;
        }
        else if(dest - src - 1 > dest - src){
            cpu.eflags.CF = 1;
        }
        else{
            cpu.eflags.CF = 0;
        }
    }
}


void set_PF(uint32_t res){
    int num = 0;
    for(int i = 0; i < 8; ++i){
        if(res % 2 == 1){
            num++;
        }
        res >>= 1;
    }
    if(num % 2 == 0){
        cpu.eflags.PF = 1;
    }
    else{
        cpu.eflags.PF = 0;
    }
}

/*void set_AF(){
    
}
*/

void set_ZF(uint32_t res, size_t data_size){
    res = res & (0xFFFFFFFF >> (32 - data_size));
    if(res == 0){
        cpu.eflags.ZF = 1;
    }
    else{
        cpu.eflags.ZF = 0;
    }
}

void set_SF(uint32_t res, size_t data_size){
    res = sign_ext(res & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.SF = sign(res);
    
}

void set_OF_add(uint32_t res, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            res = sign_ext(res & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            res = sign_ext(res & 0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default:
            break;
    }
    if(sign(src) != sign(dest)){
        cpu.eflags.OF = 0;
    }
    else{
        if(sign(src) != sign(res)){
            cpu.eflags.OF = 1;
        }
        else{
            cpu.eflags.OF = 0;
        }
    }
}

void set_OF_sub(uint32_t res, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            res = sign_ext(res & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            res = sign_ext(res & 0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default:
            break;
    }
    if(sign(src) == sign(dest)){
        cpu.eflags.OF = 0;
    }
    else{
        if(sign(dest) != sign(res)){
            cpu.eflags.OF = 1;
        }
        else{
            cpu.eflags.OF = 0;
        }
    }
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest + src;
    set_CF_add(res, src, data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res, data_size);
    set_SF(res, data_size);
    set_OF_add(res, src, dest, data_size);
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest + src + cpu.eflags.CF;
    set_CF_adc(res, src, data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res, data_size);
    set_SF(res, data_size);
    set_OF_add(res, src, dest, data_size);
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest - src;
    set_CF_sub(dest, src, data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res, data_size);
    set_SF(res, data_size);
    set_OF_sub(res, src, dest, data_size);
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest - src - cpu.eflags.CF;
    set_CF_sbb(dest, src, data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res, data_size);
    set_SF(res, data_size);
    set_OF_sub(res, src, dest, data_size);
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
    uint64_t src1 = src;
    uint64_t dest1 = dest;
    uint64_t res = src1 * dest1;
    if(res >> data_size != 0){
        cpu.eflags.CF = 1;
        cpu.eflags.OF = 1;
    }
    else{
        cpu.eflags.CF = 0;
        cpu.eflags.OF = 0;
    }
        return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
    int64_t src1 = src;
    int64_t dest1 = dest;
    int64_t res = src1 * dest1;
    if(res >> data_size != 0){
        cpu.eflags.CF = 1;
        cpu.eflags.OF = 1;
    }
    else{
        cpu.eflags.CF = 0;
        cpu.eflags.OF = 0;
    }
    return res;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
    assert(src != 0);
    uint32_t res = dest / src;
    return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
    assert(src != 0);
    int32_t res = dest / src;
    return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
    assert(src != 0);
    uint32_t res = dest % src;
    return res;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
    assert(src != 0);
    int32_t res = dest % src;
    return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest & src;
    cpu.eflags.CF = 0;
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    cpu.eflags.OF = 0;
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest ^ src;
    cpu.eflags.CF = 0;
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    cpu.eflags.OF = 0;
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest | src;
    cpu.eflags.CF = 0;
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    cpu.eflags.OF = 0;
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest;
    uint32_t temp = src;
    while(temp != 0){
        cpu.eflags.CF = res >> (data_size - 1);
        res *= 2;
        temp--;
    }
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src == 1){
        if((res >> (data_size - 1)) != cpu.eflags.CF){
            cpu.eflags.OF = 1;
        }
        else{
            cpu.eflags.OF = 0;
        }
    }
    return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest;
    uint32_t temp = src;
    while(temp != 0){
        cpu.eflags.CF = res & 0x00000001;
        res = res / 2;
        temp--;
    }
    res = res & (0xFFFFFFFF >> (32 - data_size + src));
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src == 1){
        cpu.eflags.OF = res >> (data_size - 1);
    }
    return res;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest;
    uint32_t temp = src;
    while(temp != 0){
        cpu.eflags.CF = res & 0x00000001;
        res = res / 2;
        temp--;
    }
    if(((dest & (0xFFFFFFFF << (data_size - 1))) & (0xFFFFFFFF >> (32 - data_size))) == 0){
        res = res & (0xFFFFFFFF >> (32 - data_size + src));
    }
    else{
        res = res | (0xFFFFFFFF << (data_size - src));
        res = res & (0xFFFFFFFF >> (32 - data_size));
    }
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src == 1){
        cpu.eflags.OF = 0;
    }
    return res;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = dest;
    uint32_t temp = src;
    while(temp != 0){
        cpu.eflags.CF = res >> (data_size - 1);
        res *= 2;
        temp--;
    }
    set_PF(res);
    set_ZF(res, data_size);
    set_SF(res, data_size);
    if(src == 1){
        if((res >> (data_size - 1)) != cpu.eflags.CF){
            cpu.eflags.OF = 1;
        }
        else{
            cpu.eflags.OF = 0;
        }
    }
    return res & (0xFFFFFFFF >> (32 - data_size));
}
