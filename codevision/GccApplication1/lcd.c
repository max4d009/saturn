
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

// Команды для управления портами LCD
/* RS */
#define RS1 registr(0x01, 1)
#define RS0 registr(0x01, 0)

/* E */
#define E1 registr(0x02, 1)
#define E0 registr(0x02, 0)

/* D4 */
#define D41 registr(0x04, 1)
#define D40 registr(0x04, 0)

/* D5 */
#define D51 registr(0x08, 1)
#define D50 registr(0x08, 0)

/* D6 */
#define D61 registr(0x10, 1)
#define D60 registr(0x10, 0)

/* D7 */
#define D71 registr(0x20, 1)
#define D70 registr(0x20, 0)

volatile static unsigned char tempdata = 0; // буферная переменная.


// Функция передачи данных в регистр
void registr(unsigned char data, unsigned char WriteOrErase)
{
	if(WriteOrErase == 1){
		tempdata = (tempdata|data);
	} else {
		tempdata &= ~(data);
	}

	int addr[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };
	int i;
	for (i = 0; i<=7; i++)
	{
		if(addr[i] == 0x80){
			REG_PORT &= ~(1 << ST_CP);
		} else {
			REG_PORT |= (1 << SH_CP);
		}
		
		REG_PORT &= ~(1 << SH_CP);

		if(tempdata & addr[i]){
			REG_PORT |= (1 << DS);
		} else {
			REG_PORT &= ~(1 << DS);
		}
	}
	
	if(tempdata & 0x01){
		REG_PORT |= (1 << DS);
	} else {
		REG_PORT &= ~(1 << DS);
	}
	
	REG_PORT |= (1 << SH_CP);
	REG_PORT |= (1 << ST_CP);
	
}

// Функция передачи данных или команды в LCD
void write_to_lcd(char p, unsigned char rs)
{
	if(rs == 1) RS1;
	else RS0;
	
	E1;

	if(p&0x10) D41; else D40;
	if(p&0x20) D51; else D50;
	if(p&0x40) D61; else D60;
	if(p&0x80) D71; else D70;
	E0;
	_delay_ms(2);
	
	E1;
	
	if(p&0x01) D41; else D40;
	if(p&0x02) D51; else D50;
	if(p&0x04) D61; else D60;
	if(p&0x08) D71; else D70;
	E0;

	_delay_ms(2);
}

// Функция инициализации LCD
void lcd_init(void)
{
	REG_DDR|=(1<<DS) | (1<<ST_CP) | (1<<SH_CP);
	write_to_lcd(0x02, 0); // Курсор в верхней левой позиции
	write_to_lcd(0x28, 0); // Шина 4 бит, LCD - 2 строки
	write_to_lcd(0x0C, 0); // Разрешаем вывод изображения, курсор не виден
	write_to_lcd(0x01, 0); // Очищаем дисплей
}

// Функция вывода строки
void lcd_puts(char *str)
{
	unsigned char i = 0;
	while(str[i])
	write_to_lcd(str[i++], 1);
}

// Функция вывода переменной
void lcd_num_to_str(int x, int y, unsigned int value, unsigned char nDigit)
{
	lcd_gotoxy(x,  y);
	switch(nDigit)
	{
		case 5: write_to_lcd((value/10000)+'0', 1);
		case 4: write_to_lcd((value/1000)+'0', 1);
		case 3: write_to_lcd(((value/100)%10)+'0', 1);
		case 2: write_to_lcd(((value/10)%10)+'0', 1);
		case 1: write_to_lcd((value%10)+'0', 1);
	}
}

// Функция установки курсора в указанную точку
void lcd_gotoxy(int x, int y)
{
	 write_to_lcd(0x80|((x)+((y)*0x40)), 0);
}

void lcd_print(int x, int y, char *str)
{
	lcd_gotoxy(x,  y);
	lcd_puts(str);
}