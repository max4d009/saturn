/*
 * m4d_i2c.c
 *
 * Created: 17.12.2020 0:42:18
 *  Author: m4d
 */ 

#include "m4d_i2c.h"
	
// Инициализаця в роли ведущего устройства	
void m4d_i2c_init_as_master(void)
{
	// Скорость передачи (при 8 мГц получается 100 кГц)
	TWBR = 0x80; 
}

// Инициализаця в роли ведомого устройства
void m4d_i2c_init_as_slave(void)
{
	TWAR = ME_ADDR & 0xFE;
	TWCR =
	(1 << TWIE)| // разрешаем подтверждение
	(1 << TWEA)|
	(1 << TWEN) // разрешаем прерывание
	;
}

// Переход sda из высокого в низкий при условии, что на scl высокое
void m4d_i2c_start_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR&(1 << TWINT))); // Подождем пока установится TWIN
	// вызывается прерывание и висит пока не будет обработано
}

void m4d_i2c_stop_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

// Отправить байт внутренняя функция
void m4d_i2c_do_send_byte(unsigned char c)
{
	TWDR = c; // Запишем байт в регистр данных
	TWCR = (1 << TWINT) | (1 << TWEN); // Включим передачу данных
	while (!(TWCR&(1 << TWINT))); 
	// Подождем пока установится TWIN. На данный момент дождемся первого возможного прерывания, но нужно обрабатывать ошибки
	// в статусном регистре
}

// Отправить байт
void m4d_i2c_send_byte(unsigned char value)
{
	cli();
	m4d_i2c_start_condition();
	m4d_i2c_do_send_byte(SLA_W);
	m4d_i2c_do_send_byte(value);
	m4d_i2c_stop_condition();
	sei();
}


// ISR ISR ISR ISR ISR //
/////////////////////////
///// ISR TWI_vect //////
/////////////////////////
// Срабатывает когда устройство ведомое
ISR(TWI_vect)
{
	cli();
	unsigned char status; // Переменная хранения
	status = TWSR & 0xF8; // (11111000) Маска. Если настроили скорость.. но последние биты не являются нулями

	if(status == 0x80) // Пришли данные от мастера. (До это мы уже приняли свой адрес)
	{
		//lcd_print(0, 0, (char*)"4444");
		//lcd_num_to_str(0, 1, TWDR, 4); // вывод текста.
	}
	// Сбрасываем флаг. Иначе прерывание будет зациклено
	TWCR |= (1 << TWEN);
	sei();
}


	//unsigned char twi_status;
	//static unsigned char i2c_state;
	
// 	twi_status = TWSR & 0xF8;
// 		switch(twi_status) {
// 			case TW_SR_SLA_ACK:	// 0x60: SLA+W received, ACK returned
// 				i2c_state=0;	// Start I2C State for Register Address required
// 			break;
//
// 			case TW_SR_DATA_ACK:	// 0x80: data received, ACK returned
// 			if (i2c_state == 0) {
// 					regaddr = TWDR;	// Save data to the register address
// 					i2c_state = 1;
// 			} else {
// 					regdata = TWDR;	// Save to the register data
// 				i2c_state = 2;
// 			}
// 			break;
//
// 			case TW_SR_STOP:	// 0xA0: stop or repeated start condition received while selected
// 			if (i2c_state == 2) {
// 				//i2c_slave_action(1);	// Call Write I2C Action (rw_status = 1)
// 				lcd_print(6, 0, (char*)"ST1");
// 				i2c_state = 0;		// Reset I2C State
// 			}
// 			break;
//
// 			case TW_ST_SLA_ACK:	// 0xA8: SLA+R received, ACK returned
// 			case TW_ST_DATA_ACK:	// 0xB8: data transmitted, ACK received
// 			if (i2c_state == 1) {
// 				//i2c_slave_action(0);	// Call Read I2C Action (rw_status = 0)
// 				lcd_print(6, 0, (char*)"ST0");
// 				TWDR = regdata;		// Store data in TWDR register
// 				i2c_state = 0;		// Reset I2C State
// 			}
// 			break;
//
// 			case TW_ST_DATA_NACK:	// 0xC0: data transmitted, NACK received
// 			case TW_ST_LAST_DATA:	// 0xC8: last data byte transmitted, ACK received
// 			case TW_BUS_ERROR:	// 0x00: illegal start or stop condition
// 			default:
// 			i2c_state = 0;	// Back to the Begining State
// 		}
//
// 	lcd_print(0, 0, (char*)"PRE");
// 	lcd_num_to_str(5, 0, twi_status, 3); // вывод текста.
// 	lcd_print(0, 0, (char*)"                      ");
// 	TWCR |= (1 << TWEN);
// 	_delay_ms(50);