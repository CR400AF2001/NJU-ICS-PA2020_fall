#ifndef __NEMU2_H__
#define __NEMU2_H__

#include "macro.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <config.h>
#include <time.h>

enum
{
	NEMU_STOP,
	NEMU_RUN,
	NEMU_READY,
	NEMU_BREAK
};

extern int nemu_state;

typedef uint8_t bool;
typedef uint32_t paddr_t;
typedef uint32_t laddr_t;
typedef uint32_t vaddr_t;

typedef uint16_t ioaddr_t;

extern bool verbose;
extern bool is_nemu_hlt;

typedef union {
	struct
	{
		uint32_t low;
		uint32_t high;
	};
	uint64_t val;
} UINT64_T;

#define true 1
#define false 0

inline static void memcpy_with_mask(void *dest, const void *src, size_t len, uint8_t *mask)
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (mask[i])
		{
			((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
		}
	}
}

inline static uint64_t get_current_time_ms()
{
	uint64_t ret = 0;
	struct timespec temp_time;
	clock_gettime(CLOCK_REALTIME, &temp_time);
	ret = temp_time.tv_sec * 1000 + temp_time.tv_nsec / 1000000;
	return ret;
}

#endif
