/*
 * usart.c
 *
 * Created: 14.01.2021 15:41:00
 *  Author: m4d
 */ 

#include "usart.h"

int debug = 0;
void USART_ini(unsigned int speed, int debug_)
{
	UBRR0H = (unsigned char) (speed >> 8);
	UBRR0L = (unsigned char) speed;
	UCSR0B = (1 << RXEN0)|(1 << TXEN0); // Включаем прием и передачу по usart
	UCSR0B |= (1 << RXCIE0); // Разрешаем прерывания при приеме
	UCSR0A |= (1 << U2X0); // Удвоение частоты
	UCSR0C = (1 << USBS0)|(1 << UCSZ01)|(1 << UCSZ00); // Обращаемся именно к регистру UCSR0C
	// Асинхронный режим (UNSEL=0), без контроля длительность (UPM1=0,UPM0=0)
	debug = debug_;
}

void USART_Transmit(unsigned char data)
{
	if(debug == 0){
		return;
	}
	// Начнем передавать данные, но только убедившись, что буфер пуст
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void USART_send_var(char *name, char *value)
{
	unsigned char i = 0;
	while(name[i]){
		USART_Transmit(name[i]);
		i = i + 1;
	}
	USART_Transmit (':');
	USART_Transmit (' ');
	i = 0;
	while(value[i]){
		USART_Transmit(value[i]);
		i = i + 1;
	}
	USART_Transmit (0x0d);
	USART_Transmit (0x0a);
}

void USART_send_string(char *string)
{
	unsigned char i = 0;
	while(string[i]){
		USART_Transmit(string[i]);
		i = i + 1;
	}
	USART_Transmit (0x0d);
	USART_Transmit (0x0a);
}

void USART_send_int_var(char *name, int value)
{
	unsigned char i = 0;
	while(name[i]){
		USART_Transmit(name[i]);
		i = i + 1;
	}
	USART_Transmit (':');
	USART_Transmit (' ');
	char str[15];
	itoa(value, str, 10);
	
	i = 0;
	while(str[i]){
		USART_Transmit(str[i]);
		i = i + 1;
	}
		
	USART_Transmit (0x0d);
	USART_Transmit (0x0a);
}

void USART_send_2_int_var(char *name, int value, char *name2, int value2)
{
	unsigned char i = 0;
	while(name[i]){
		USART_Transmit(name[i]);
		i = i + 1;
	}
	USART_Transmit ('/');
	i = 0;
	while(name2[i]){
		USART_Transmit(name2[i]);
		i = i + 1;
	}	
	
	USART_Transmit (':');
	USART_Transmit (' ');
	
	
	char str[15];
	itoa(value, str, 10);
	char str2[15];
	itoa(value2, str2, 10);
		
	i = 0;
	while(str[i]){
		USART_Transmit(str[i]);
		i = i + 1;
	}
	
	USART_Transmit ('/');
	
	i = 0;
	while(str2[i]){
		USART_Transmit(str2[i]);
		i = i + 1;
	}
	
	USART_Transmit (0x0d);
	USART_Transmit (0x0a);
}

// void itoa(uint32_t n, char *str) {
// 	uint8_t i, length;
// 	char c;
// 
// 	i = 0;
// 	do { /* генерируем цифры в обратном порядке */
// 		str[i++] = n % 10 + '0'; /* берем следующую цифру */
// 		} while ((n /= 10) > 0); /* удаляем */
// 
// 		str[i] = '\0'; // конец строки
// 
// 		/*переворачиваем строку*/
// 		for (i = 0, length = strlen(str) - 1; i < length; i++, length--) {
// 			c = str[i];
// 			str[i] = str[length];
// 			str[length] = c;
// 		}
// 	}
