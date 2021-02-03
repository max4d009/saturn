/*
 * m4d_timer_0.c
 *
 * Created: 04.12.2020 2:29:46
 *  Author: m4d
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "m4d_timer_0.h"
// Инкрементится по переполнению таймера
unsigned char timer_0_current = 0;


// Инициализировать и запустить таймер 0 с предделителем 1024
void m4d_timer_0_init_1024() {
	timer_0_current = 0;
	TCNT0 = 0; // сам регистр таймера куда пишутся тики. Обнулим его
	TIMSK0 |= (1 << TOIE0); // По переполнению
}

// Остановить таймер 0
void m4d_timer_0_off() 
{
	TCCR0B &= ~(1 << CS00);
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS02);
}

// Запустить таймер 0
void m4d_timer_0_on_if_off() 
{
	TCCR0B |= (1 << CS00);
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);
}




ISR(TIMER0_OVF_vect)
{

}