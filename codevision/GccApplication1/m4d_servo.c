/*
 * m4d_servo.c
 *
 * Created: 04.12.2020 1:10:36
 *  Author: m4d
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "m4d_servo.h"


// Коррекция значение АЦП для датчика натяжения 
int left_tension_servo_correction = 0;

uint16_t tension_sensor_center_position = 512;
uint16_t tension_sensor_second_position = 120;

char trololo[10] = "trololo";

/*
  Инициализация
*/
void m4d_servo_init()
{
	//char name;
	//strcpy(name, *"normal_mode     ");
	config_list[PR_MODE_NORMAL].name =			 (char *)"normal_mode" ;
	config_list[PR_MODE_TENSION_MIDDLE].name =   (char *)"center_sensor";
	config_list[PR_MODE_TENSION_SECOND].name =   (char *)"second_sensor";
	config_list[PR_MODE_SERVO_PLAY_PLAY].name =  (char *)"PLAY_pos_play";
	config_list[PR_MODE_SERVO_PLAY_STOP].name =  (char *)"PLAY_pos_stop";
	config_list[PR_MODE_SERVO_PLAY_PAUSE].name = (char *)"PLAY_pos_pause";
	config_list[PR_MODE_REW_REW].name =          (char *)"REW_pos_rewind";
	config_list[PR_MODE_REW_FORW].name =		 (char *)"REW_pos_forw";
	config_list[PR_MODE_REW_STOP].name =		 (char *)"REW_pos_stop";
	config_list[PR_MODE_REW_PLAY].name =		 (char *)"REW_pos_play";
	config_list[PR_MODE_LFT_MAX_BR].name =		 (char *)"LFT_pos_max";
	config_list[PR_MODE_LFT_MIN_BR].name =		 (char *)"LFT_pos_min";
	config_list[PR_MODE_LFT_NO_BR].name =		 (char *)"LFT_pos_no_brake";
	config_list[PR_MODE_RHT_MAX_BR].name =		 (char *)"RHT_pos_max";
	config_list[PR_MODE_RHT_MIN_BR].name =		 (char *)"RHT_pos_min";
	config_list[PR_MODE_RHT_NO_BR].name =		 (char *)"RHT_pos_no_brake";
	config_list[PR_MODE_RHT_PLAY_BR].name =		 (char *)"RHT_pos_play";
	
	config_list[PR_MODE_NORMAL].eeprom_addr =			0 ;
	config_list[PR_MODE_TENSION_MIDDLE].eeprom_addr =   TENSION_SENSOR_MIDDLE_EPROM_ADDR;
	config_list[PR_MODE_TENSION_SECOND].eeprom_addr =   TENSION_SENSOR_SECOND_EPROM_ADDR;
	config_list[PR_MODE_SERVO_PLAY_PLAY].eeprom_addr =  SERVO_PLAY_POS_PLAY_EPROM_ADDR;
	config_list[PR_MODE_SERVO_PLAY_STOP].eeprom_addr =  SERVO_PLAY_POS_STOP_EPROM_ADDR;
	config_list[PR_MODE_SERVO_PLAY_PAUSE].eeprom_addr = SERVO_PLAY_POS_PAUSE_EPROM_ADDR;
	config_list[PR_MODE_REW_REW].eeprom_addr =          SERVO_REWIND_POS_REWIND_EPROM_ADDR;
	config_list[PR_MODE_REW_FORW].eeprom_addr =		    SERVO_REWIND_POS_FORWARD_EPROM_ADDR;
	config_list[PR_MODE_REW_STOP].eeprom_addr =		    SERVO_REWIND_POS_STOP_EPROM_ADDR;
	config_list[PR_MODE_REW_PLAY].eeprom_addr =		    SERVO_REWIND_POS_PLAY_EPROM_ADDR;
	config_list[PR_MODE_LFT_MAX_BR].eeprom_addr =		SERVO_LEFT_TENSION_POS_MAX_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_LFT_MIN_BR].eeprom_addr =		SERVO_LEFT_TENSION_POS_MIN_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_LFT_NO_BR].eeprom_addr =		SERVO_LEFT_TENSION_POS_NO_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_RHT_MAX_BR].eeprom_addr =		SERVO_RIGHT_TENSION_POS_MAX_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_RHT_MIN_BR].eeprom_addr =		SERVO_RIGHT_TENSION_POS_MIN_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_RHT_NO_BR].eeprom_addr =		SERVO_RIGHT_TENSION_POS_NO_BRAKE_EPROM_ADDR;
	config_list[PR_MODE_RHT_PLAY_BR].eeprom_addr =	    SERVO_RIGHT_TENSION_POS_PLAY_BRAKE_EPROM_ADDR;
	
	config_list[PR_MODE_NORMAL].servo =			  0;
	config_list[PR_MODE_TENSION_MIDDLE].servo =   0;
	config_list[PR_MODE_TENSION_SECOND].servo =   0;
	config_list[PR_MODE_SERVO_PLAY_PLAY].servo =  SERVO_PLAY;
	config_list[PR_MODE_SERVO_PLAY_STOP].servo =  SERVO_PLAY;
	config_list[PR_MODE_SERVO_PLAY_PAUSE].servo = SERVO_PLAY;
	config_list[PR_MODE_REW_REW].servo =          SERVO_REWIND;
	config_list[PR_MODE_REW_FORW].servo =		  SERVO_REWIND;
	config_list[PR_MODE_REW_STOP].servo =		  SERVO_REWIND;
	config_list[PR_MODE_REW_PLAY].servo =		  SERVO_REWIND;
	config_list[PR_MODE_LFT_MAX_BR].servo =		  SERVO_LEFT_TENSION;
	config_list[PR_MODE_LFT_MIN_BR].servo =		  SERVO_LEFT_TENSION;
	config_list[PR_MODE_LFT_NO_BR].servo =		  SERVO_LEFT_TENSION;
	config_list[PR_MODE_RHT_MAX_BR].servo =		  SERVO_RIGHT_TENSION;
	config_list[PR_MODE_RHT_MIN_BR].servo =		  SERVO_RIGHT_TENSION;
	config_list[PR_MODE_RHT_NO_BR].servo =		  SERVO_RIGHT_TENSION;
	config_list[PR_MODE_RHT_PLAY_BR].servo =	  SERVO_RIGHT_TENSION;

	uint16_t stop_angle;
	
	stop_angle = EEPROM_read_word(SERVO_LEFT_TENSION_POS_MAX_BRAKE_EPROM_ADDR);
	
	servo_list[SERVO_LEFT_TENSION].ddr = SERVO_DDR;
	servo_list[SERVO_LEFT_TENSION].port = SERVO_PORT;
	servo_list[SERVO_LEFT_TENSION].pin = TENSION_SERVO_LEFT_PIN;
	servo_list[SERVO_LEFT_TENSION].speed = 500;
	servo_list[SERVO_LEFT_TENSION].current_angle = 1500;
	servo_list[SERVO_LEFT_TENSION].need_angle = 1500;
	servo_list[SERVO_LEFT_TENSION].max_angle = 2500;
	servo_list[SERVO_LEFT_TENSION].min_angle = 500;
	servo_list[SERVO_LEFT_TENSION].midde_angle = 1500;
	servo_list[SERVO_LEFT_TENSION].auto_angle = 0;
	servo_list[SERVO_LEFT_TENSION].play_angle = EEPROM_read_word(SERVO_LEFT_TENSION_POS_MIN_BRAKE_EPROM_ADDR);
	servo_list[SERVO_LEFT_TENSION].stop_angle = stop_angle;
	servo_list[SERVO_LEFT_TENSION].forward_angle = EEPROM_read_word(SERVO_LEFT_TENSION_POS_NO_BRAKE_EPROM_ADDR);
	servo_list[SERVO_LEFT_TENSION].rewind_angle = EEPROM_read_word(SERVO_LEFT_TENSION_POS_MIN_BRAKE_EPROM_ADDR);
	servo_list[SERVO_LEFT_TENSION].pause_angle = stop_angle;

	stop_angle = EEPROM_read_word(SERVO_REWIND_POS_STOP_EPROM_ADDR);
	servo_list[SERVO_REWIND].ddr = SERVO_DDR;
	servo_list[SERVO_REWIND].port = SERVO_PORT;
	servo_list[SERVO_REWIND].pin = REWIND_SERVO_PIN;
	servo_list[SERVO_REWIND].speed = 5;
	servo_list[SERVO_REWIND].current_angle = 1500;
	servo_list[SERVO_REWIND].need_angle = 1500;
	servo_list[SERVO_REWIND].max_angle = 2500;
	servo_list[SERVO_REWIND].min_angle = 500;
	servo_list[SERVO_REWIND].midde_angle = 1500;
	servo_list[SERVO_REWIND].auto_angle = 0;
	servo_list[SERVO_REWIND].play_angle = EEPROM_read_word(SERVO_REWIND_POS_PLAY_EPROM_ADDR);
	servo_list[SERVO_REWIND].stop_angle = stop_angle;
	servo_list[SERVO_REWIND].forward_angle = EEPROM_read_word(SERVO_REWIND_POS_FORWARD_EPROM_ADDR);
	servo_list[SERVO_REWIND].rewind_angle = EEPROM_read_word(SERVO_REWIND_POS_REWIND_EPROM_ADDR);
	servo_list[SERVO_REWIND].pause_angle = stop_angle;

	stop_angle = EEPROM_read_word(SERVO_PLAY_POS_STOP_EPROM_ADDR);
	servo_list[SERVO_PLAY].ddr = SERVO_DDR;
	servo_list[SERVO_PLAY].port = SERVO_PORT;
	servo_list[SERVO_PLAY].pin = PLAY_SERVO_PIN;
	servo_list[SERVO_PLAY].speed = 20;
	servo_list[SERVO_PLAY].current_angle = stop_angle;
	servo_list[SERVO_PLAY].need_angle = 1500;
	servo_list[SERVO_PLAY].max_angle = 2500;
	servo_list[SERVO_PLAY].min_angle = 500;
	servo_list[SERVO_PLAY].midde_angle = 1500;
	servo_list[SERVO_PLAY].auto_angle = 0;
	servo_list[SERVO_PLAY].play_angle = EEPROM_read_word(SERVO_PLAY_POS_PLAY_EPROM_ADDR);
	servo_list[SERVO_PLAY].stop_angle = EEPROM_read_word(SERVO_PLAY_POS_STOP_EPROM_ADDR);
	servo_list[SERVO_PLAY].forward_angle = stop_angle;
	servo_list[SERVO_PLAY].rewind_angle = stop_angle;
	servo_list[SERVO_PLAY].pause_angle = stop_angle;

	uint16_t left_min = EEPROM_read_word(SERVO_RIGHT_TENSION_POS_MIN_BRAKE_EPROM_ADDR);
	uint16_t left_without = EEPROM_read_word(SERVO_RIGHT_TENSION_POS_NO_BRAKE_EPROM_ADDR);
	servo_list[SERVO_RIGHT_TENSION].ddr = SERVO_DDR;
	servo_list[SERVO_RIGHT_TENSION].port = SERVO_PORT;
	servo_list[SERVO_RIGHT_TENSION].pin = TENSION_SERVO_RIGHT_PIN;
	servo_list[SERVO_RIGHT_TENSION].speed = 40;
	servo_list[SERVO_RIGHT_TENSION].current_angle = 1500;
	servo_list[SERVO_RIGHT_TENSION].need_angle = 1500;
	servo_list[SERVO_RIGHT_TENSION].max_angle = 2500;
	servo_list[SERVO_RIGHT_TENSION].min_angle = 500;
	servo_list[SERVO_RIGHT_TENSION].midde_angle = 1500;
	servo_list[SERVO_RIGHT_TENSION].auto_angle = 0;
	servo_list[SERVO_RIGHT_TENSION].play_angle = EEPROM_read_word(SERVO_RIGHT_TENSION_POS_PLAY_BRAKE_EPROM_ADDR);
	servo_list[SERVO_RIGHT_TENSION].stop_angle = left_min;
	servo_list[SERVO_RIGHT_TENSION].forward_angle = left_without;
	servo_list[SERVO_RIGHT_TENSION].rewind_angle = left_min;
	servo_list[SERVO_RIGHT_TENSION].pause_angle = left_without;
	
	// Настройка пинов для клавиатуры
	PORTD |= (1 << REWIND_INTERRUPT_PIN);
	PORTC |= (1 << REWIND_ADC_PIN);
	DDRD &= ~(1 << REWIND_INTERRUPT_PIN);

	// настройка прерывания для клавиатуры
	EIMSK |= (1 << INT0); // прерывание на ИНТ0
	MCUCR |= (1 << ISC01); // прерывание по спаду ИНТ0
	
	// Настройка шим
	OCR1A = 20000;
	TCCR1A = 0;
	TCCR1B |= (1 << WGM12);	
	
	tension_sensor_center_position = EEPROM_read_word(TENSION_SENSOR_MIDDLE_EPROM_ADDR);
	tension_sensor_second_position = EEPROM_read_word(TENSION_SENSOR_SECOND_EPROM_ADDR);
}

/*
  Запуск сервоприводов
*/
void m4d_servo_start()
{
	TCNT1 = 0;
	TCCR1B |= (1 << CS11);
	TIMSK1 |= (1 << OCIE1A);
	
	// Устанавливаем PB1,PB2,PB3,PB4 как выход
	SERVO_DDR |= (1 << TENSION_SERVO_LEFT_PIN);
	SERVO_DDR |= (1 << REWIND_SERVO_PIN);
	SERVO_DDR |= (1 << PLAY_SERVO_PIN);
	SERVO_DDR |= (1 << TENSION_SERVO_RIGHT_PIN);
	
	// Без подтягивающих резисторов
	servo_list[SERVO_LEFT_TENSION].port &= ~(1 <<  servo_list[SERVO_LEFT_TENSION].port);
	SERVO_PORT &= ~(1 << REWIND_SERVO_PIN);
	SERVO_PORT &= ~(1 << PLAY_SERVO_PIN);
	SERVO_PORT &= ~(1 << TENSION_SERVO_RIGHT_PIN);
}

 
/*
  Остановить работу сервоприводов
*/
void m4d_servo_stop()
{
	TCCR1B &= 0b11111000;
	TIMSK1 &= !(1 << OCIE1A);
}


/*
  "ВОСПРОИЗВЕДЕНИЕ"
*/
void m4d_play()
{
	servo_list[SERVO_RIGHT_TENSION].need_angle = 1800;
	servo_list[SERVO_REWIND].need_angle = servo_list[SERVO_REWIND].play_angle;
	servo_list[SERVO_PLAY].need_angle = servo_list[SERVO_PLAY].play_angle;
	servo_list[SERVO_LEFT_TENSION].auto_angle = 1;
	USART_send_var("Mode", "Play");
}


/*
  "ПАУЗА"
*/
void m4d_pause()
{
	servo_list[SERVO_RIGHT_TENSION].need_angle = servo_list[SERVO_RIGHT_TENSION].pause_angle;
	servo_list[SERVO_PLAY].need_angle = servo_list[SERVO_PLAY].pause_angle;
	servo_list[SERVO_REWIND].need_angle = servo_list[SERVO_REWIND].pause_angle;
	USART_send_var("Mode", "Pause");
}


/*
  ПЕРЕМОТКА "ВПЕРЕД"
*/
void m4d_forward()
{
	servo_list[SERVO_LEFT_TENSION].need_angle = servo_list[SERVO_LEFT_TENSION].forward_angle;
	servo_list[SERVO_RIGHT_TENSION].need_angle = servo_list[SERVO_RIGHT_TENSION].forward_angle;
	servo_list[SERVO_PLAY].need_angle = servo_list[SERVO_PLAY].forward_angle;
	servo_list[SERVO_REWIND].need_angle = servo_list[SERVO_REWIND].forward_angle;
	servo_list[SERVO_LEFT_TENSION].auto_angle = 0;
	USART_send_var("Mode", "Forward");
}

/*
  ПЕРЕМОТКА "НАЗАД"
*/
void m4d_rewind()
{
	servo_list[SERVO_LEFT_TENSION].auto_angle = 0;
	left_tension_servo_correction = 0;
	servo_list[SERVO_LEFT_TENSION].need_angle = servo_list[SERVO_LEFT_TENSION].rewind_angle;
	servo_list[SERVO_RIGHT_TENSION].need_angle = servo_list[SERVO_RIGHT_TENSION].rewind_angle;
	servo_list[SERVO_PLAY].need_angle = servo_list[SERVO_PLAY].rewind_angle;
	servo_list[SERVO_REWIND].need_angle = servo_list[SERVO_REWIND].rewind_angle;
	USART_send_var("Mode", "Rewind");
}


/*
  "СТОП"
*/
void m4d_stop()
{
	servo_list[SERVO_LEFT_TENSION].auto_angle = 0;
	left_tension_servo_correction = 0;
	servo_list[SERVO_LEFT_TENSION].need_angle = servo_list[SERVO_LEFT_TENSION].stop_angle;
	servo_list[SERVO_RIGHT_TENSION].need_angle = servo_list[SERVO_RIGHT_TENSION].stop_angle;
	servo_list[SERVO_PLAY].need_angle = servo_list[SERVO_PLAY].stop_angle;
	servo_list[SERVO_REWIND].need_angle = servo_list[SERVO_REWIND].stop_angle;
	USART_send_var("Mode", "Stop");
}


unsigned char takt = 0;
unsigned char viaOne = 0;
unsigned char servoNum = 0;

/*
  "Прерывание по таймеру 1"
*/
ISR(TIMER1_COMPA_vect) 
{
	
	unsigned int i;
	
	for (i = 0; i <= NUM_SERVO-1; i++)
	{
		int speed = servo_list[i].speed;
		int current_angle = servo_list[i].current_angle;
		int need_angle = servo_list[i].need_angle;
		
		if(current_angle > need_angle){
			int delta = current_angle - need_angle;
			if(delta > speed){
				 servo_list[i].current_angle = current_angle - speed;
			} else {
				 servo_list[i].current_angle = current_angle-delta;
			}
		}
		
		if( current_angle < need_angle) {
			int delta = need_angle - current_angle;
			if(delta > speed){
				 servo_list[i].current_angle = current_angle + speed;
			} else {
				 servo_list[i].current_angle = current_angle + delta;
			}
		}
	}

	
	if (viaOne == 0){
		SERVO_PORT |= (1 << servo_list[servoNum].pin);
		OCR1A = servo_list[servoNum].current_angle;
	} else {
		SERVO_PORT &= ~(1 << servo_list[servoNum].pin);
		OCR1A = CYCLE - servo_list[servoNum].current_angle;
	}	
	
	if (viaOne == 1) {
		viaOne = 0;
		servoNum = servoNum + 1;
	} else {
		viaOne = 1;
	}
	
	takt = takt + 1;
	if (takt == NUM_SERVO+NUM_SERVO) {
		takt = 0;
		servoNum = 0;
		tension_sensor_measure();
	}

}

int int0ButtonPress = 0;

void resetInt0Button(){
	int0ButtonPress = 0;
}
int program_mode = 0;
uint16_t pr_adc_val;


/*
  "Прерывание нажатию кнопки"
*/
ISR(INT0_vect){
	// Антидребезг по таймеру
	if(int0ButtonPress == 1) return;
	int0ButtonPress = 1;
	
	cli();
	
	_delay_ms(50);
	uint16_t adcKeyb;
	adcKeyb = m4d_adc_keyboard_read(120, 700);
	//USART_send_int_var("KeyboardAdc", adcKeyb);
	
	if(adcKeyb > 580 && adcKeyb < 610)
	{
		program_mode = program_mode_change(program_mode);
	} 
	else if(adcKeyb > 516 && adcKeyb < 556)
	{
		if(program_mode > 0) {sei();return;}
		m4d_pause();
	} 
	else if(adcKeyb > 459 && adcKeyb < 499)
	{
		if(program_mode > 0) {
			program_mode_value_save(program_mode);
			sei();
			return;
		}
		m4d_play();
	} 
	else if(adcKeyb > 387 && adcKeyb < 427)
	{
		if(program_mode > 0) {
			program_mode_value_change(program_mode, 0); 
			sei(); 
			return;
		}
		m4d_rewind();
	} 
	else if(adcKeyb > 293 && adcKeyb < 333)
	{
		if(program_mode > 0) {
			program_mode_value_change(program_mode, 1);
			sei(); 
			return;
		}
		m4d_forward();
	} 
	else if(adcKeyb > 150 &&adcKeyb < 205)
	{
		if(program_mode > 0) {sei();return;}
		m4d_stop();
	}
	sei();
}

/*
  Выбор типа настройки в программном режиме
*/
char program_mode_change(int program_mode)
{
	program_mode++;
	lcd_print(0,0, "                ");
	lcd_print(0,1, "                ");
	if(program_mode >= PR_COUNT_CONFIG){
		program_mode = 0;
	}
	lcd_print(0, 0,  config_list[program_mode].name);		
	return program_mode;
}

/*
  Переключение значений настроек в программном режиме
*/
void program_mode_value_change(int program_mode, char up)
{
	
	// Настройка центрального сенсора натяжения
	if(program_mode == PR_MODE_TENSION_MIDDLE){
		lcd_print(0, 1, "old:");
		lcd_num_to_str(4, 1, EEPROM_read_word(TENSION_SENSOR_MIDDLE_EPROM_ADDR), 4);
		lcd_print(8, 1, "new:");
		pr_adc_val = adc_sensor_tension_center_read();
		lcd_num_to_str(12, 1, pr_adc_val, 4);
		
	// Настройка вспомогательного сенсора натяжения	
	} else if(program_mode == PR_MODE_TENSION_SECOND){
		lcd_print(0, 1, "old:");
		lcd_num_to_str(4, 1, EEPROM_read_word(TENSION_SENSOR_SECOND_EPROM_ADDR), 4);
		lcd_print(8, 1, "new:");
		pr_adc_val = adc_sensor_tension_second_read();
		lcd_num_to_str(12, 1, pr_adc_val, 4);
		
	} else {
		unsigned int addr = config_list[program_mode].eeprom_addr;
		lcd_print(0, 1, "old:");
		lcd_num_to_str(0, 1, addr, 1);
		lcd_num_to_str(4, 1, EEPROM_read_word(addr), 4);
		program_mode_servo_move(up, config_list[program_mode].servo);
	}
}

/*
  Сохранение настроек в программном режиме
*/
void program_mode_value_save(int program_mode)
{
	unsigned int addr = config_list[program_mode].eeprom_addr; 
	unsigned int servo = config_list[program_mode].servo; 
	if(program_mode == PR_MODE_TENSION_MIDDLE || program_mode == PR_MODE_TENSION_SECOND){
		EEPROM_write_word(addr, pr_adc_val);
	} else {
		EEPROM_write_word(addr, servo_list[servo].need_angle);
	}
	lcd_print(0, 1, "Saved           ");
}

/*
  Перемещение сервопривода во время настройки
*/
void program_mode_servo_move(char up, int servo_num)
{
	if(up)
		servo_list[servo_num].need_angle = servo_list[servo_num].need_angle + 50;
	else
		servo_list[servo_num].need_angle = servo_list[servo_num].need_angle - 50;
	lcd_print(8, 1, "new:");
	lcd_num_to_str(12, 1, servo_list[servo_num].need_angle, 4);
}

/*
  Установка натяжения ленты по датчику натяжения
*/
void setTensionServoPositionByAdc() 
{
	if(servo_list[SERVO_LEFT_TENSION].auto_angle == 0){
		return;
	}
	
	uint16_t hight = adc_result.tension_hight / (adc_result.count_measure);
	
	///lcd_num_to_str(0,1,hight,3);
	//USART_send_2_int_var("Center", adc_result.tension_middle, "Second", adc_result.tension_hight);
	
	// Если недостаточное натяжение
	if(hight > 120){
		left_tension_servo_correction = left_tension_servo_correction - LEFT_TENSION_STEP_CORRECTION;
		if(left_tension_servo_correction < LEFT_TENSION_MIN_CORRECTION){
			left_tension_servo_correction = LEFT_TENSION_MIN_CORRECTION;
		}
	} else {
		left_tension_servo_correction = left_tension_servo_correction + LEFT_TENSION_STEP_CORRECTION;
		if(left_tension_servo_correction > LEFT_TENSION_MAX_CORRECTION){
			left_tension_servo_correction = LEFT_TENSION_MAX_CORRECTION;
		}
	}
	
	servo_list[SERVO_LEFT_TENSION].need_angle = servo_list[SERVO_LEFT_TENSION].midde_angle + left_tension_servo_correction;
}



unsigned char measure_divider = 0;
unsigned char middleTensionAdcMeasured = 0;
unsigned char measuredNum = 0;

/*
  Произвести измерения на датчике натяжения
*/
void tension_sensor_measure()
{

	if(measure_divider == 10){
		// Сброс int0 кнопки
		resetInt0Button();
		measure_divider = 0;
		
		if(servo_list[SERVO_LEFT_TENSION].auto_angle != 1){
			measuredNum = 0;
			middleTensionAdcMeasured = 0;
			m4d_adc_reset_result();
		}
	}
	measure_divider++;
	
	// Если не нужно автонатяжение выходим
	if(servo_list[SERVO_LEFT_TENSION].auto_angle != 1){
		measuredNum = 0;
		middleTensionAdcMeasured = 0;
		return;
	}
	
	// Получаем значения от АЦП
	struct adc adc_result;
	adc_result = get_adc_result();

	// При запросе к ацп через m4d_adc_center_tension_pin_read() будет произведено измерение. Результат записан в структуру adc_result.
	// Будет произведено столько измерений сколько задано в adc_result.count_measure
	// Прежде чем производить следующее измерение - ждем, что прошло предыдущее
	// Первое измерение пропускаем
	// Для датчика натяжения делаем сначала набор измерений для одного фоторезистора, затем для второго, высчитываем среднее
	if(middleTensionAdcMeasured == 0){
		if(adc_result.tension_middle_complete <= adc_result.count_measure ){
			if(adc_result.tension_middle_complete > measuredNum || measuredNum == 0){
				m4d_adc_center_tension_pin_read();
				measuredNum = adc_result.tension_middle_complete;
			}
		} else {
			middleTensionAdcMeasured = 1;
			measuredNum = 0;
		}
	}
	else if(middleTensionAdcMeasured == 1){
		if(adc_result.tension_middle/adc_result.count_measure > tension_sensor_center_position){
			measuredNum = 0;
			middleTensionAdcMeasured = 0;
			m4d_adc_reset_result();
			return;
		}
		if(adc_result.tension_hight_complete <= adc_result.count_measure ){
			if(adc_result.tension_hight_complete > measuredNum || measuredNum == 0){
				m4d_adc_second_tension_pin_read();
				measuredNum = adc_result.tension_hight_complete;
			}
			} else {
			middleTensionAdcMeasured = 3;
			measuredNum = 0;
		}
	}
	
	
	//lcd_num_to_str(0,1,adc_result.tension_hight_complete,4);
	//lcd_num_to_str(0,0,adc_result.tension_middle_complete,4);
	//_delay_ms(200);
	// После того как есть измерения для обоих фотодиодов вызовем setTensionServoPositionByAdc()
	if(adc_result.tension_hight_complete == (adc_result.count_measure+1) && adc_result.tension_middle_complete == (adc_result.count_measure+1)){
		setTensionServoPositionByAdc();
		middleTensionAdcMeasured = 0;
		m4d_adc_reset_result();
	}
}