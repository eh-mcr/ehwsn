/*
 * gen_func_set.h
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef GEN_FUNC_SET_H_
#define GEN_FUNC_SET_H_

#include "msp430x22x4.h"

extern unsigned int timer_state;
extern unsigned char change_mode;
extern unsigned char ftt_flag;
extern unsigned int battery_ready;
extern unsigned int in_delay;
extern char status;
extern unsigned int battery_full_flag;
extern unsigned long battery_full_timer;

extern unsigned int number_transmits;

extern unsigned char NodeStatus;
extern unsigned char DataInterruptSource;



void Set_TimerA (void);
void Set_TimerB(void);
void delay(unsigned int BlinkCount);
void delay_minutes(unsigned int minutes);
void StatusBlink_led1(int BlinkCount);
void StatusBlink_led2(int BlinkCount);
void status_indicator(char status, int status_led);
void delay(unsigned int BlinkCount);
void button_still_pressed(void);
unsigned int get_voltage(void);
void transmit_time_delay(void);
void display_mode(void);
void check_bat_full(void);
void createRandomAddress(void);
void sleep(unsigned int sleep_count);	// in intervals of 30s
#endif /* GEN_FUNC_SET_H_ */
