#include <LPC21xx.H>
#include "keyboard.h"

#define SW0_bm (1<<4)
#define SW1_bm (1<<6)
#define SW2_bm (1<<5)
#define SW3_bm (1<<7)

void KeyboardInit(void) {
	
	IO0DIR = IO0DIR & (~(SW0_bm | SW1_bm | SW2_bm | SW3_bm));
}

enum KeyboardState eKeyboardRead(void) {
	
	KeyboardInit();
	if((IO0PIN & SW0_bm) != SW0_bm) {
		return BUTTON_0;
	}
	else if((IO0PIN & SW1_bm) != SW1_bm) {
		return BUTTON_1;
	}
	else if((IO0PIN & SW2_bm) != SW2_bm) {
		return BUTTON_2;
	}
	else if((IO0PIN & SW3_bm) != SW3_bm) {
		return BUTTON_3;
	}
	else {
		return RELASED;
	}
}
