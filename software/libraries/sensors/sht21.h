/*
 * sht21.h
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#include "msp430x22x4.h"
#include "usci.h"

#ifndef SHT21_H_
#define SHT21_H_

// ****************** SHT21 ****************** //

// constants
#define SHT21_SLAVE_ADDRESS	0x40	//slave address = 0b1000 000

#define WRITE_USER_REGISTER	0xE6	// 0b1110’0110
#define READ_USER_REGISTER	0xE7	// 0b1110’0111
#define	SOFT_RESET			0xFE	//0b1111 1110
#define	T_HOLD_MASTER		0xE3	//0b1110 0011
#define	RH_HOLD_MASTER		0xE5	//0b1110 0101

#define RES0	0x00	// resolution settings
#define RES1	0x01	// resolution settings
#define RES2	0x10	// resolution settings
#define RES3	0x11	// resolution settings

#define RES_UB	BIT7	// upper bit of resolution setting
#define RES_LB	BIT0	// lower bit of resolution setting

// variables
extern unsigned int Humidity;
extern signed int Temperature;

// ****************** END OF SHT21 ****************** //


void SHT21_Init(void);
void SHT21_Config(char Resolution);
unsigned char SHT21_Read_Register(char Register);
void SHT21_Write_Register(char Register, char Value);
unsigned int SHT21_Read_Data(char Register);
signed int SHT21_T_Read(void);
unsigned int SHT21_RH_Read(void);
signed int SHT21_T_Calib(unsigned int temp_data);
unsigned int SHT21_RH_Calib(unsigned int hum_data);

#endif /* SHT21_H_ */
