/*
 * lps25h.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "lps25h.h"
unsigned long Pressure = 1234;


// ********** ********** LPS25H ********** **********//

// ****************************************** //
// ReadPressure
// INPUT: NONE
// OUTPUT: Pressure (mbar * 100) as long
// ****************************************** //
unsigned long ReadPressure()
{
	unsigned long PRESSURE_LSB, Pressure;
    I2C_TXByteCtr = 1;                          // Load TX byte counter
    I2C_RXByteCtr = 3;
    I2C_TXCntr = I2C_TXData;
    I2C_RXCntr = I2C_RXData;

    I2C_TXData[0] = PRESS_OUT_XL | BIT7;
    I2C_Write_Init();
    READ = YES;
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
	                                            // Remain in LPM0 until all data
	                                            // is TX'd
	I2C_Read_Init();                      // Init I2C module for a read operation
    UCB0CTL1 |= UCTXSTT;// + UCTXSTP;                // start condition generation
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

	PRESSURE_LSB = I2C_RXData[2]<<8;
	PRESSURE_LSB = PRESSURE_LSB<<8;
	PRESSURE_LSB |= ((I2C_RXData[1]<<8) & 0x0000FF00);
	PRESSURE_LSB |= I2C_RXData[0];

	PRESSURE_LSB &= ~(0xFF000000);

	Pressure = (PRESSURE_LSB*100)>>12;

	return Pressure;
}

// ********** ********** END OF LPS25H ********** **********//
