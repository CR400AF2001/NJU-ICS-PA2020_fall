#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "nemu.h"
#include "device/port_io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_IRQ 1

void keyboard_down(uint32_t sym);
void keyboard_up(uint32_t sym);
void keyboard_start();
void keyboard_stop();

make_pio_handler(handler_keyboard_data);

/* keyboard scancode set 2
 * http://www.mouseos.com/os/doc/scan_code.html */
enum
{
	UNDEF,
	K_ESC,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,
	K_0,
	K_MINUS,
	K_EQUALS,
	K_BACK,
	K_TAB,
	K_q,
	K_w,
	K_e,
	K_r,
	K_t,
	K_y,
	K_u,
	K_i,
	K_o,
	K_p,
	K_LEFTBRACKET,
	K_RIGHTBRACKET,
	K_ENTER,
	K_LCTRL,
	K_a,
	K_s,
	K_d,
	K_f,
	K_g,
	K_h,
	K_j,
	K_k,
	K_l,
	K_SEMICOLON,
	K_QUOTE,
	K_BACKQUOTE,
	K_LSHIFT,
	K_BACKSLASH,
	K_z,
	K_x,
	K_c,
	K_v,
	K_b,
	K_n,
	K_m,
	K_COMMA,
	K_PERIOD,
	K_SLASH,
	K_RSHIFT, /*UNDEF, */
	K_LALT = 0x38,
	K_SPACE,
	K_CAPSLOCK,
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_NUMLOCK,
	K_SCROLLOCK,
	K_HOME,
	K_UP,
	K_PAGEUP,
	/*UNDEF,*/ K_LEFT = 0x4b,
	/*UNDEF,*/ K_RIGHT = 0x4d,
	/*UNDEF,*/ K_END = 0x4f,
	K_DOWN,
	K_PAGEDOWN,
	K_INSERT,
	K_DELETE,
	/*UNDEF, UNDEF, UNDEF,*/ K_F11 = 0x57,
	K_F12
};

const static uint8_t sym2scancode[2][128] = {{UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
											  K_BACK, K_TAB, UNDEF, UNDEF, UNDEF /*SDLK_CLEAR*/, K_ENTER, UNDEF, UNDEF,
											  UNDEF, UNDEF, UNDEF, UNDEF /*SDLK_PAUSE*/, UNDEF, UNDEF, UNDEF, UNDEF,
											  UNDEF, UNDEF, UNDEF, K_ESC, UNDEF, UNDEF, UNDEF, UNDEF,
											  K_SPACE, UNDEF /*SDLK_EXCLAIM*/, UNDEF /*SDLK_QUOTEDBL*/, UNDEF /*SDLK_HASH*/, UNDEF /*SDLK_DOLLAR*/, UNDEF, UNDEF /*SDLK_AMPERSAND*/, K_QUOTE,
											  UNDEF /*SDLK_LEFTPAREN*/, UNDEF /*SDLK_RIGHTPAREN*/, UNDEF /*SDLK_ASTERISK*/, UNDEF /*SDLK_PLUS*/, K_COMMA, K_MINUS, K_PERIOD, K_SLASH,
											  K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7,
											  K_8, K_9, UNDEF /*SDLK_COLON*/, K_SEMICOLON, UNDEF /*SDLK_LESS*/, K_EQUALS, UNDEF /*K_GREATER*/, UNDEF /*K_QUESTION*/,
											  UNDEF /*SDLK_AT*/, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
											  UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
											  UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
											  UNDEF, UNDEF, UNDEF, K_LEFTBRACKET, K_BACKSLASH, K_RIGHTBRACKET, UNDEF /*SDLK_CARET*/, UNDEF /*SDLK_UNDERSCORE*/,
											  K_BACKQUOTE, K_a, K_b, K_c, K_d, K_e, K_f, K_g,
											  K_h, K_i, K_j, K_k, K_l, K_m, K_n, K_o,
											  K_p, K_q, K_r, K_s, K_t, K_u, K_v, K_w,
											  K_x, K_y, K_z, UNDEF, UNDEF, UNDEF, UNDEF, K_DELETE},
											 {

												 UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
												 UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
												 /*
	SDLK_KP0, SDLK_KP1, SDLK_KP2, SDLK_KP3, SDLK_KP4, SDLK_KP5, SDLK_KP6, SDLK_KP7, 
	SDLK_KP8, SDLK_KP9, SDLK_KP_PERIOD, SDLK_KP_DIVIDE, SDLK_KP_MULTIPLY, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_KP_ENTER, 
	*/

												 UNDEF /*SDLK_KP_EQUALS*/, K_UP, K_DOWN, K_RIGHT, K_LEFT, K_INSERT, K_HOME, K_END,
												 K_PAGEUP, K_PAGEDOWN, K_F1, K_F2, K_F3, K_F4, K_F5, K_F6,
												 K_F7, K_F8, K_F9, K_F10, K_F11, K_F12, UNDEF /*SDLK_F13*/, UNDEF /*K_F14*/,
												 UNDEF /*SDLK_F15*/, UNDEF, UNDEF, UNDEF, K_NUMLOCK, K_CAPSLOCK, K_SCROLLOCK, K_RSHIFT,
												 K_LSHIFT, K_LCTRL, K_LCTRL, K_LALT, K_LALT, UNDEF /*SDLK_RMETA*/, UNDEF /*SDLK_LMETA*/, UNDEF /*SDLK_LSUPER*/,
												 UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
												 /*	SDLK_RSUPER, SDLK_MODE, UNDEF, SDLK_HELP, SDLK_PRINT, SDLK_SYSREQ, SDLK_BREAK, SDLK_MENU, */
												 /*SDLK_POWER, SDLK_EURO,  */
											 }

};

#endif
