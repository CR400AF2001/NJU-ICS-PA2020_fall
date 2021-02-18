#include "memory/mmu/cache.h"
#include <stdlib.h>

#ifdef CACHE_ENABLED 

struct CacheLine cache[1024];
extern uint8_t hw_mem[];
extern void hw_mem_write(paddr_t paddr, size_t len, uint32_t data);

// init the cache
void init_cache()
{
	for(int i = 0; i < 1024; ++i){
	    cache[i].valid = false;
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
    uint32_t set = (paddr >> 6) & 0x0000007F;
	uint32_t targetTag = paddr >> 13;
	uint32_t wordAddr = paddr & 0x0000003F;
	if(wordAddr + len <= 64){
	    for(int i = 8 * set; i < 8 * (set + 1); ++i){
	        if(cache[i].valid && targetTag == cache[i].tag){
	            memcpy(&(cache[i].data[wordAddr]), &data, len);
	            hw_mem_write(paddr, len, data);
	            return;
	        }
	    }
	    hw_mem_write(paddr, len, data);
	    return;
	}
	else{
	    for(int k = 0; k < 2; ++k){
	        if(set + k == 128){
	            set = -1;
	            targetTag++;
	        }
	        bool flag = false;
	        for(int i = 8 * (set + k); i < 8 * (set + k + 1); ++i){
	            if(cache[i].valid && targetTag == cache[i].tag){
	                if(k == 0){
	                    memcpy(&(cache[i].data[wordAddr]), &data, 64 - wordAddr);
	                }
	                else{
	                    memcpy(&(cache[i].data[0]), ((uint8_t*) (&data)) + 64 - wordAddr, len - (64 - wordAddr));
	                }
	                hw_mem_write(paddr, len, data);
	                flag = true;
	                break;
	            }
	        }
	        if(flag){
	            continue;
	        }
	    }
	    hw_mem_write(paddr, len, data);
	    return;
	}
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	uint32_t set = (paddr >> 6) & 0x0000007F;
	uint32_t targetTag = paddr >> 13;
	uint32_t wordAddr = paddr & 0x0000003F;
	if(wordAddr + len <= 64){
	    for(int i = 8 * set; i < 8 * (set + 1); ++i){
	        if(cache[i].valid && targetTag == cache[i].tag){
	            uint32_t ret = 0;
	            memcpy(&ret, &(cache[i].data[wordAddr]), len);
	            return ret;
	        }
	    }
	    for(int i = 8 * set; i < 8 * (set + 1); ++i){
	        if(!cache[i].valid){
	            uint32_t startAddr = paddr & 0xFFFFFFC0;
	            memcpy(cache[i].data, hw_mem + startAddr, 64);
	            cache[i].valid = true;
	            cache[i].tag = targetTag;
	            uint32_t ret = 0;
	            memcpy(&ret, &(cache[i].data[wordAddr]), len);
	            return ret;
	        }
	    }
	    int random = 8 * set + rand() % 8;
	    uint32_t startAddr = paddr & 0xFFFFFFC0;
	    memcpy(cache[random].data, hw_mem + startAddr, 64);
	    cache[random].valid = true;
	    cache[random].tag = targetTag;
	    uint32_t ret = 0;
	    memcpy(&ret, &(cache[random].data[wordAddr]), len);
	    return ret;
	}
	else{
	    uint32_t ret = 0;
	    for(int k = 0; k < 2; ++k){
	        if(set + k == 128){
	            set = -1;
	            targetTag++;
	        }
	        bool flag = false;
	        for(int i = 8 * (set + k); i < 8 * (set + k + 1); ++i){
	            if(cache[i].valid && targetTag == cache[i].tag){
	                uint32_t temp = 0;
	                if(k == 0){
	                    memcpy(&temp, &(cache[i].data[wordAddr]), 64 - wordAddr);
	                    ret = temp;
	                }
	                else{
	                    memcpy(&temp, &(cache[i].data[0]), len - (64 - wordAddr));
	                    ret = ret + (temp << (8 * (64 - wordAddr)));
	                }
	                flag = true;
	                break;
	            }
	        }
	        if(flag){
	            continue;
	        }
	        for(int i = 8 * (set + k); i < 8 * (set + k + 1); ++i){
	            if(!cache[i].valid){
	                memcpy(cache[i].data, hw_mem + (((targetTag << 7) + (set + k)) << 6), 64);
	                cache[i].valid = true;
	                cache[i].tag = targetTag;
	                uint32_t temp = 0;
	                if(k == 0){
	                    memcpy(&temp, &(cache[i].data[wordAddr]), 64 - wordAddr);
	                    ret = temp;
	                }
	                else{
	                    memcpy(&temp, &(cache[i].data[0]), len - (64 - wordAddr));
	                    ret = ret + (temp << (8 * (64 - wordAddr)));
	                }
	                flag = true;
	                break;
	            }
	        }
	        if(flag){
	            continue;
	        }
	        int random = 8 * (set + k) + rand() % 8;
	        memcpy(cache[random].data, hw_mem + (((targetTag << 7) + (set + k)) << 6), 64);
	        cache[random].valid = true;
	        cache[random].tag = targetTag;
	        uint32_t temp = 0;
	        if(k == 0){
	            memcpy(&temp, &(cache[random].data[wordAddr]), 64 - wordAddr);
	            ret = temp;
	        }
	        else{
	            memcpy(&temp, &(cache[random].data[0]), len - (64 - wordAddr));
	            ret = ret + (temp << (8 * (64 - wordAddr)));
	        }
	    }
	    return ret;
	}
}

#endif

