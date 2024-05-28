#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE_BIT_bm (1<<0)
#define COUNTER_RESET_BIT_bm (1<<1)
#define RESET_ON_MR0_BIT_bm (1<<1)
#define INTERRUPT_ON_MR0_BIT_bm (1<<0)
#define MR0_INTERRUPT_FLAG_bm (1<<0)

void InitTimer0(void) {
	
	T0TCR = COUNTER_ENABLE_BIT_bm;
}

void WaitOnTimer0(unsigned int uiTime) {
	
	T0TCR = T0TCR | COUNTER_RESET_BIT_bm;
	T0TCR = T0TCR & (~COUNTER_RESET_BIT_bm);
	uiTime = uiTime * 15;
	while(T0TC <= uiTime) {}
}

void InitTimer0Match0(unsigned int uiDelayTime) {
	
	T0MR0 = uiDelayTime * 15;
	T0MCR = T0MCR | (RESET_ON_MR0_BIT_bm | INTERRUPT_ON_MR0_BIT_bm);
	T0TCR = COUNTER_ENABLE_BIT_bm | COUNTER_RESET_BIT_bm;
	T0TCR = T0TCR & (~COUNTER_RESET_BIT_bm);
}
	
void WaitOnTimer0Match0(void) {
	while(MR0_INTERRUPT_FLAG_bm != (MR0_INTERRUPT_FLAG_bm & T0IR)) {}
	T0IR = MR0_INTERRUPT_FLAG_bm;
}
