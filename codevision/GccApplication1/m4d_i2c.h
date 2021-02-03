/*
 * m4d_i2c.h
 *
 * Created: 17.12.2020 1:03:28
 *  Author: m4d
 */ 


#ifndef M4D_I2C_H_
#define M4D_I2C_H_

#include "main.h"

void m4d_i2c_init_as_master(void);
void m4d_i2c_init_as_slave(void);
void m4d_i2c_start_condition(void);
void m4d_i2c_stop_condition(void);
void m4d_i2c_do_send_byte(unsigned char c);
void m4d_i2c_send_byte(unsigned char value);


#define RTC_ADDR 0x4E // ����� ��������. ��������� ��� �� �������� ����� ������. 7 ��� ����� � ��������� ���� ������/������
#define SLA_R RTC_ADDR|0b00000001 // ������ ������ + ��� ������
#define SLA_W RTC_ADDR&0b11111110 // ������ ������ + ��� ������
#define ME_ADDR 0x4E // ��������� ���������� ������� �� �����

#endif /* M4D_I2C_H_ */