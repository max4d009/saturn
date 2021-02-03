/*
 * m4d_servo.h
 *
 * Created: 08.12.2020 1:27:25
 *  Author: m4d
 */ 


#ifndef M4D_SERVO_H_
#define M4D_SERVO_H_

#include "main.h"

#define NUM_SERVO 4


#define TENSION_SERVO_LEFT_PIN PB1
#define REWIND_SERVO_PIN PB2
#define PLAY_SERVO_PIN PB3
#define TENSION_SERVO_RIGHT_PIN PB4

#define SERVO_PORT PORTB
#define SERVO_DDR DDRB

#define REWIND_ADC_PIN PC1
#define REWIND_INTERRUPT_PIN PD2

#define SERVO_0 0
#define SERVO_1 1
#define SERVO_2 2
#define SERVO_3 3
#define SERVO_4 4
#define SERVO_5 5
#define SERVO_6 6
#define SERVO_7 7


#define CYCLE 5000

#define SERVO_LEFT_TENSION SERVO_0
#define SERVO_REWIND SERVO_1
#define SERVO_PLAY SERVO_2
#define SERVO_RIGHT_TENSION SERVO_3

#define SERVO_LEFT_TENSION_ANGLE_DEFAULT 1500
#define SERVO_REWIND_ANGLE_DEFAULT 1500
#define SERVO_PLAY_ANGLE_DEFAULT 600
#define SERVO_RIGHT_TENSION_ANGLE_DEFAULT 1500
	
#define SERVO_LEFT_TENSION_SPEED 30
#define SERVO_REWIND_SPEED 5
#define SERVO_PLAY_SPEED 20
#define SERVO_RIGHT_TENSION_SPEED 40	

#define LEFT_TENSION_MAX_CORRECTION 200
#define LEFT_TENSION_MIN_CORRECTION -200
#define LEFT_TENSION_STEP_CORRECTION 10

#define TENSION_SENSOR_MIDDLE_EPROM_ADDR 1
#define TENSION_SENSOR_SECOND_EPROM_ADDR 3

#define SERVO_PLAY_POS_PLAY_EPROM_ADDR 5
#define SERVO_PLAY_POS_STOP_EPROM_ADDR 7
#define SERVO_PLAY_POS_PAUSE_EPROM_ADDR 9

#define SERVO_REWIND_POS_REWIND_EPROM_ADDR 11
#define SERVO_REWIND_POS_FORWARD_EPROM_ADDR 13
#define SERVO_REWIND_POS_STOP_EPROM_ADDR 15
#define SERVO_REWIND_POS_PLAY_EPROM_ADDR 17

#define SERVO_LEFT_TENSION_POS_MAX_BRAKE_EPROM_ADDR 19
#define SERVO_LEFT_TENSION_POS_MIN_BRAKE_EPROM_ADDR 21
#define SERVO_LEFT_TENSION_POS_NO_BRAKE_EPROM_ADDR 23

#define SERVO_RIGHT_TENSION_POS_MAX_BRAKE_EPROM_ADDR 25
#define SERVO_RIGHT_TENSION_POS_MIN_BRAKE_EPROM_ADDR 27
#define SERVO_RIGHT_TENSION_POS_PLAY_BRAKE_EPROM_ADDR 29
#define SERVO_RIGHT_TENSION_POS_NO_BRAKE_EPROM_ADDR 31

#define PR_MODE_NORMAL 0
#define PR_MODE_TENSION_MIDDLE 1
#define PR_MODE_TENSION_SECOND 2
#define PR_MODE_SERVO_PLAY_PLAY 3
#define PR_MODE_SERVO_PLAY_STOP 4
#define PR_MODE_SERVO_PLAY_PAUSE 5
#define PR_MODE_REW_REW 6
#define PR_MODE_REW_FORW 7
#define PR_MODE_REW_STOP 8
#define PR_MODE_REW_PLAY 9
#define PR_MODE_LFT_MAX_BR 10
#define PR_MODE_LFT_MIN_BR 11
#define PR_MODE_LFT_NO_BR 12
#define PR_MODE_RHT_MAX_BR 13
#define PR_MODE_RHT_MIN_BR 14
#define PR_MODE_RHT_NO_BR 15
#define PR_MODE_RHT_PLAY_BR 16

#define PR_COUNT_CONFIG 17

void setTensionServoPositionByAdc();
void resetInt0Button();

void m4d_servo_init();
void m4d_servo_start();
void m4d_servo_stop();
void tension_sensor_measure();
char program_mode_change(int program_mode);
void program_mode_value_change(int program_mode, char up);
void program_mode_value_save(int program_mode);
void program_mode_servo_move(char up, int servo_num);


// Общая структура для всех сервоприводов
struct Servo {
	int ddr;
	int port;
	char pin;
	int speed;
	int max_angle;
	int min_angle;
	int midde_angle;
	int current_angle;
	int need_angle;
	int auto_angle;
	int play_angle;
	int stop_angle;
	int forward_angle;
	int rewind_angle;
	int pause_angle;
};

// Общая структура для всех настроек eprom
struct EpromConfig {
	char *name;
	unsigned int eeprom_addr;
	char servo;
};

// Массив сервоприводов
struct Servo servo_list[NUM_SERVO];

// Массив настроек
struct EpromConfig config_list[PR_COUNT_CONFIG];

unsigned char get_servo_complete_signal();

#endif /* M4D_SERVO_H_ */