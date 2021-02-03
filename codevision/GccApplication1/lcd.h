#ifndef LCD_H_
#define LCD_H_

// настройка пинов для подключения к 74НС595
#define REG_PORT PORTB // PORT
#define REG_DDR DDRB // DDR
#define ST_CP PB6 // пин для st_sp - 12 нога регистра
#define SH_CP PB5 // пин для sh_cp - 11 нога регистра
#define DS PB7 // пин для ds - 14 нога регистра


// функции
void servo_init();
void registr(unsigned char data, unsigned char WriteOrErase);
void write_to_lcd(char p, unsigned char rs);
void lcd_init(void);
void lcd_puts(char *str);
void lcd_num_to_str(int x, int y, unsigned int value, unsigned char nDigit);
void lcd_gotoxy(int x, int y);
void lcd_print(int x, int y, char *str);
int set_tape_tension_positions_by_microswitch();

#endif /* LCD_H_ */