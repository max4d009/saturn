/*
 * m4d_adc.с
 *
 * Created: 02.12.2020 1:24:46
 *  Author: m4d
 */ 

#include "m4d_adc.h"

// Возвращает структуру с результами ацп
struct adc get_adc_result()
{
    return adc_result;
}

void m4d_adc_reset_result()
{
	adc_result.keyboard = 0;
	adc_result.tension_middle = 0;
	adc_result.tension_hight = 0;
	adc_result.tension_middle_complete = 0;
	adc_result.tension_hight_complete = 0;
	adc_result.count_measure = 2;
}

// Инициализация АЦП:
void m4d_adc_init_8(void) 
{
    m4d_adc_reset_result();
	
	// Включить АЦП
	ADCSRA |= (1 << ADEN);
	// Частота дискретизации 8
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA &= ~(1 << ADPS2);
	
	// Опорное напряжение по VCC 5v
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	
	// Разрешить прерывания
	ADCSRA |= (1 << ADIE);
}

// Считываем с ADC0
void m4d_admux_keyboard()
{
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
}

// Считываем с ADC1
void m4d_admux_tension_low()
{
	ADMUX |=  (1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
}

// Считываем с ADC2
void m4d_admux_tension_center()
{
	//if (ADMUX != 0x42){
		ADMUX &= ~(1 << MUX0);
		ADMUX |=  (1 << MUX1);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX3);
	//}
}

// Считываем с ADC3
void m4d_admux_tension_hight()
{
	//if (ADMUX != 0x43){
		ADMUX |=  (1 << MUX0);
		ADMUX |=  (1 << MUX1);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX3);
	//}
}

// Считать показания с ADC
uint16_t m4d_adc_read()
{
	// Сбрасываем ADIF в 0
	//ADCSRA &= ~(1 << ADIF);
	
	// Включаем АЦП
	ADCSRA |= (1 << ADSC);
	
	// Производим измерение
	while(ADCSRA & (1 << ADSC));
	
	return ADC;	
}

void m4d_adc_center_tension_pin_read()
{
 	m4d_admux_tension_center();
	ADCSRA |= (1 << ADSC);
}

void m4d_adc_second_tension_pin_read()
{
	m4d_admux_tension_hight();
	ADCSRA |= (1 << ADSC);
}

// Чтение канала АЦП для клавиатуры
uint16_t m4d_adc_keyboard_read(int min, int max) 
{
    m4d_admux_keyboard();
	uint16_t result = 0;
	uint16_t tmp;
	uint16_t tmp2 = 0;
	int i;
	for (i = 0; i<=20;i++){
		tmp = m4d_adc_read();
		if(i > 0 && tmp < max && tmp > min){
			result = result + tmp;
			tmp2 = tmp2 + 1;
		}
	}
	if(tmp2 == 0){
		return 0;
	}
	result = result / tmp2;
	
	return result;
}


// Чтение канала АЦП для центрального сенсора натяжения
uint16_t adc_sensor_tension_center_read()
{
	m4d_admux_tension_center();
	uint16_t result = 0;
	uint16_t tmp;
	uint16_t tmp2 = 0;
	int i;
	for (i = 0; i<=20;i++){
		tmp = m4d_adc_read();
		if(i > 0){
			result = result + tmp;
			tmp2 = tmp2 + 1;
		}
	}
	if(tmp2 == 0){
		return 0;
	}
	result = result / tmp2;
	
	return result;
}

// Чтение канала АЦП для второго сенсора натяжения
uint16_t adc_sensor_tension_second_read()
{
	m4d_admux_tension_hight();
	uint16_t result = 0;
	uint16_t tmp;
	uint16_t tmp2 = 0;
	int i;
	for (i = 0; i<=20;i++){
		tmp = m4d_adc_read();
		if(i > 0){
			result = result + tmp;
			tmp2 = tmp2 + 1;
		}
	}
	if(tmp2 == 0){
		return 0;
	}
	result = result / tmp2;
	
	return result;
}


// 
ISR(ADC_vect)
{
	cli();

	if (!(ADMUX & (1 << MUX0)) && (ADMUX & (1 << MUX1)) && !(ADMUX & (1 << MUX2)) && !(ADMUX & (1 << MUX3))){
		// Производим измерение, только если оно не первое.
		// Только если кол-во измерений меньше чем указанно в структуре.
		
		if(adc_result.tension_middle_complete > 0 && adc_result.tension_middle_complete <= adc_result.count_measure){
			adc_result.tension_middle = adc_result.tension_middle + ADC;
		}
		adc_result.tension_middle_complete = adc_result.tension_middle_complete + 1;
		// Сбрасываем ADIF в 0
		ADCSRA &= ~(1 << ADIF);
		return;
	}
		
	if ((ADMUX & (1 << MUX0)) && (ADMUX & (1 << MUX1)) && !(ADMUX & (1 << MUX2)) && !(ADMUX & (1 << MUX3))){

		if(adc_result.tension_hight_complete > 0 && adc_result.tension_hight_complete <= adc_result.count_measure){
			adc_result.tension_hight = adc_result.tension_hight + ADC;	
		}
		adc_result.tension_hight_complete = adc_result.tension_hight_complete + 1;
		// Сбрасываем ADIF в 0
		ADCSRA &= ~(1 << ADIF);
		return;
	}
		
	// Сбрасываем ADIF в 0
	ADCSRA &= ~(1 << ADIF);
	
	sei();
}