/*
 * m4d_adc.h
 *
 * Created: 08.12.2020 1:18:27
 *  Author: m4d
 */ 
#ifndef M4D_ADC_H_
#define M4D_ADC_H_

#include "main.h"

struct adc {
	uint16_t keyboard;
	uint16_t tension_middle;
	uint16_t tension_hight;
	char tension_middle_complete;
	char tension_hight_complete;
	char count_measure;
};

struct adc adc_result;


void m4d_adc_init_8(void);
uint16_t m4d_adc_tension_read();
uint16_t m4d_adc_keyboard_read(int min, int max);
void m4d_adc_center_tension_pin_read();
void m4d_adc_second_tension_pin_read();
struct adc get_adc_result();
void m4d_adc_reset_result();
uint16_t adc_sensor_tension_center_read();
uint16_t adc_sensor_tension_second_read();
#endif /* M4D_ADC_H_ */