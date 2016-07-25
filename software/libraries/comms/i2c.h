/*
 * i2c.h
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#include "msp430x22x4.h"
#include "defs.h"


#ifndef I2C_H_
#define I2C_H_

// ********** ********** I2C ********** **********//
// pins
#define SDA_PIN 		BIT1		// msp430x261x 24x UCB0SDA pin
#define SCL_PIN 		BIT2		// msp430x261x 24x UCB0SCL pin

// variables
extern unsigned char I2C_TXData[2], I2C_RXData[3];	// TX and RX buffer limits
extern unsigned char* I2C_TXCntr;
extern unsigned char* I2C_RXCntr;
extern unsigned char I2C_TXByteCtr;
extern unsigned char I2C_RXByteCtr;
extern unsigned char READ;// = NO;
extern unsigned char Slave_Present;
extern unsigned char RegisterData;

// functions
void I2C_Init(void);
void I2C_Write_Init(void);
void I2C_Read_Init(void);
char I2C_Slave_Test(char Slave_Address);
unsigned char I2C_Read_Register(unsigned char Register_Address);
void I2C_Write_Register(char Register, char Value);

// ********** ********** END OF I2C ********** **********//

#endif /* I2C_H_ */
