/*
 * m4d_timer_0.h
 *
 * Created: 08.12.2020 1:32:38
 *  Author: m4d
 */ 


#ifndef M4D_TIMER_0_H_
#define M4D_TIMER_0_H_

#include "main.h"


void m4d_timer_0_off();
void m4d_timer_0_on_if_off();
void m4d_timer_0_init_1024();
void timer_0_overflow_interrupt();


#endif /* M4D_TIMER_0_H_ */