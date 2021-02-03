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
// ������������� �� ������������ �������
unsigned char timer_0_current = 0;


// ���������������� � ��������� ������ 0 � ������������� 1024
void m4d_timer_0_init_1024() {
	timer_0_current = 0;
	TCNT0 = 0; // ��� ������� ������� ���� ������� ����. ������� ���
	TIMSK0 |= (1 << TOIE0); // �� ������������
}

// ���������� ������ 0
void m4d_timer_0_off() 
{
	TCCR0B &= ~(1 << CS00);
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS02);
}

// ��������� ������ 0
void m4d_timer_0_on_if_off() 
{
	TCCR0B |= (1 << CS00);
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);
}




ISR(TIMER0_OVF_vect)
{

}