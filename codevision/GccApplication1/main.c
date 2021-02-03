/*
 * Real-to-real tape recorder main.c
 *
 * Created: 08.04.2019 2:01:38
 * Author : m4d
 */ 

#include "main.h"



int main(void)
{
	sei();
	//m4d_timer_0_on_if_off(0);
	USART_ini(8, 0);
	
	lcd_init();
	
	m4d_adc_init_8();
	m4d_servo_init();
	m4d_servo_start();
	
	m4d_timer_0_init_1024();
	
 	//m4d_i2c_init_as_master();
 	//int i=0;

	//m4d_i2c_init_as_slave();

    while (1) 
    {	
		//m4d_i2c_send_byte(i);
		//_delay_ms(50);
		//i+=1;	
		//lcd_num_to_str(5, 0, i, 4); // вывод текста.
		//lcd_num_to_str(0,0,i,5);
		
	
    }
}