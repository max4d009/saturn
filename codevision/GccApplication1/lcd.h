#ifndef LCD_H_
#define LCD_H_

// ��������� ����� ��� ����������� � 74��595
#define REG_PORT PORTB // PORT
#define REG_DDR DDRB // DDR
#define ST_CP PB6 // ��� ��� st_sp - 12 ���� ��������
#define SH_CP PB5 // ��� ��� sh_cp - 11 ���� ��������
#define DS PB7 // ��� ��� ds - 14 ���� ��������


// �������
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