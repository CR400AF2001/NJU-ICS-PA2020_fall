#include "nemu.h"
#include "device/timer.h"
#include "device/i8259_pic.h"

#include <SDL/SDL.h>
#include <time.h>

static bool timer_active = false;
static uint64_t last_time, time_interval;

void timer_intr()
{
	if (nemu_state == NEMU_RUN)
	{
		i8259_raise_intr(TIMER_IRQ); // multi-thread safe
	}
}

void do_timer()
{
	uint64_t current_time = get_current_time_ms();
	if(timer_active && (current_time - last_time) >= time_interval)
	{
		timer_intr();
		last_time = current_time;
	}
}

// start a timer with hz Hz
void timer_start(int hz)
{
	timer_active = true;
	time_interval = 1000 / hz; // ms
	last_time = get_current_time_ms();
}

void timer_stop()
{
	timer_active = false;
}

make_pio_handler(handler_timer) {}
