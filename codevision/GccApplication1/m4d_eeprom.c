/*
 * m4d_eeprom.c
 *
 * Created: 17.12.2020 1:29:13
 *  Author: m4d
 */ 

#include "m4d_eeprom.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE)) //ждем освобождения флага окончания последней операцией с памятью
	{}
	EEAR = uiAddress; //Устанавливаем адрес
	EEDR = ucData; //Пишем данные в регистр
	EECR |= (1<<EEMPE); //Разрешаем запись
	EECR |= (1<<EEPE); //Пишем байт в память
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	while(EECR & (1<<EEPE))
	{} //ждем освобождения флага окончания последней операцией с памятью
	EEAR = uiAddress; //Устанавливаем адрес
	EECR |= (1<<EERE); //Запускаем операцию считывания из памяти в регистр данных
	return EEDR; //Возвращаем результат
}

void EEPROM_write_word(unsigned int uiAddress, uint16_t ucData)
{
	EEPROM_write(uiAddress, (unsigned char) ucData);
	unsigned char dt = ucData>>8;
	EEPROM_write(uiAddress+1, dt);
}

uint16_t EEPROM_read_word(unsigned int uiAddress)
{
	uint16_t dt = EEPROM_read(uiAddress+1)*256;
	asm("nop");
	dt += EEPROM_read(uiAddress);
	return dt;
}

void EEPROM_write_dword(unsigned int uiAddress, uint32_t ucData)
{
	EEPROM_write_word(uiAddress, (uint16_t) ucData);
	uint16_t dt = ucData>>16;
	EEPROM_write_word(uiAddress+2, dt);
}

uint32_t EEPROM_read_dword(unsigned int uiAddress)
{
	uint32_t dt = EEPROM_read_word(uiAddress+2)*65536;
	asm("nop");
	dt += EEPROM_read_word(uiAddress);
	return dt;
}

void EEPROM_write_string(unsigned int uiAddress, char str1[])
{
	wchar_t n;
	for(n=0;str1[n]!='\0';n++)
	EEPROM_write(uiAddress+n,str1[n]);
}

const char* EEPROM_read_string(unsigned int uiAddress, unsigned int sz)
{
	unsigned int i;
	char* str1;
	str1 = (char *) realloc(NULL,sz);
	for (i=0;i<sz;i++)
	str1[i] = EEPROM_read(uiAddress+i);
	return str1;
}