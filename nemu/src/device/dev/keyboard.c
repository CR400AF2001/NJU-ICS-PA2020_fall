#include "nemu.h"
#include "device/keyboard.h"
#include "device/i8259_pic.h"
#include <SDL/SDL.h>

static uint8_t scan_code_buf;
static bool keyboard_active = false;
static uint64_t last_time;

// called by do_keyboard() on detecting a key down event
void keyboard_down(uint32_t sym)
{
	// put the scan code into the buffer
	scan_code_buf = sym2scancode[sym >> 8][sym & 0xff];
	// issue an iterrupt
	i8259_raise_intr(KEYBOARD_IRQ);
	// maybe the kernel will be interested and come to read on the data port
}

// called by do_keyboard() on detecting a key up event
void keyboard_up(uint32_t sym)
{
	// put the scan code into the buffer
	scan_code_buf = sym2scancode[sym >> 8][sym & 0xff] | 0x80;
	// issue an iterrupt
	i8259_raise_intr(KEYBOARD_IRQ);
	// maybe the kernel will be interested and come to read on the data port
}

// called when the kernel issues an 'in' instruction on the keyboard's data port
make_pio_handler(handler_keyboard_data)
{
	if (!is_write)
	{
		// only read allowed, and we do not consider race condition here
		write_io_port(port, len, scan_code_buf);
	}
}

void keyboard_start()
{
	keyboard_active = true;
	last_time = get_current_time_ms();
}

void keyboard_stop()
{
	keyboard_active = false;
}

void do_keyboard()
{
	SDL_Event e;
	uint64_t current_time = get_current_time_ms();
	if(keyboard_active && (current_time - last_time) >= 1)
	{
		//Read all the events that occured
		while (SDL_PollEvent(&e))
		{
			//If user closes the window
			if (e.type == SDL_QUIT)
			{
				nemu_state = NEMU_STOP;
				keyboard_active = false;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				keyboard_down(e.key.keysym.sym);
			}
			else if (e.type == SDL_KEYUP)
			{
				keyboard_up(e.key.keysym.sym);
			}
		}
		last_time = current_time;
	}
}
