/*
 * i2c.c
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#include "i2c.h"


unsigned char Slave_Present = 2;
unsigned char RegisterData = 0x00;

unsigned char I2C_TXData[2], I2C_RXData[3];	// TX and RX buffer limits
unsigned char* I2C_TXCntr;
unsigned char* I2C_RXCntr;
unsigned char I2C_TXByteCtr;
unsigned char I2C_RXByteCtr;
unsigned char READ;// = NO;
unsigned char Slave_Present;
unsigned char RegisterData;


// ********** **********  I2C ********** **********//

// ****************************************** //
// I2C_Init
// INPUT: NONE
// OUTPUT: NONE
// OPERATION: Sets up UCBO for I2C communication
// ****************************************** //
void I2C_Init(void)
{
	P3OUT |= 0x01;
	P3DIR |= 0x01;
	// Select I2C pins, write USCI registers
	P3SEL |= SDA_PIN + SCL_PIN;                 // Assign I2C pins to USCI
	UCB0CTL1 = UCSWRST;                         // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;       // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;              // Use SMCLK, keep SW reset
	UCB0BR0 = 10;                         		// set prescaler (fSCL = 100kHz)
	UCB0BR1 = 0;
	// UCB0I2CSA = SHT21_ADDRESS;                  // set slave address
	UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
//	UCB0I2CIE = UCNACKIE;
//	IE2 |= UCB0RXIE | UCB0TXIE;                 // Enable interrupts
}

// ****************************************** //
// SlavePresent
// INPUT: NONE
// OUTPUT: NONE
// OPERATION: Pulse red light repeatedly when no slave is connected.  Otherwise pulse green 5 times.
// ****************************************** //
char I2C_Slave_Test(char Slave_Address){

	char Slave_Present = 2;

    IE2 &= ~UCB0TXIE;                       // disable Transmit ready interrupt
    IE2 &= ~UCB0RXIE;                        // disable Receive ready interrupt

	UCB0I2CSA = Slave_Address;              // set slave to test for
	while (Slave_Present != YES)
	{
		UCB0CTL1 |= UCTR + UCTXSTT + UCTXSTP;       // I2C TX, start condition (Transmit mode), STOP
		 while (UCB0CTL1 & UCTXSTP);                 // wait for STOP condition
		 if ((UCB0STAT & UCNACKIFG)== 0)		// SLAVE PRESENT?
		 {
			  Slave_Present = YES;
	          status_indicator(status_five, 0);
		 }
		else
		{
			Slave_Present = NO;
			P1DIR |= BIT0+BIT1;			// Light LED
			P1OUT &= ~BIT1;				// GREEN LED OFF
			P1OUT |= BIT0;				// RED LED ON
			delay(150);	// pause 0.1 second
			P1OUT &= ~(BIT0);				// RED LEDs
			delay(150);	// pause 0.1 second
		}
	}
	 return Slave_Present;
}

// ****************************************** //
// I2C_Read_Register
// INPUT: Adress of register
// OUTPUT: Register value (char)
// ****************************************** //
unsigned char I2C_Read_Register(unsigned char Register_Address)
{
	unsigned char Register_Content;

	I2C_TXByteCtr = 1;                          // Load TX byte counter
	I2C_RXByteCtr = 1;
	I2C_TXCntr = I2C_TXData;
	I2C_RXCntr = I2C_RXData;

	I2C_TXData[0] = Register_Address;
	I2C_Write_Init();
	READ = YES;
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as START is sent???
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
												// Remain in LPM0 until all data
												// is TX'd
	I2C_Read_Init();                      // Init I2C module for a read operation
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;                // start condition generation
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

	Register_Content = I2C_RXData[0];
	return Register_Content;

}
// Write I2C Data
void I2C_Write_Register(char Register, char Value)
{
	I2C_TXByteCtr = 2;                          // Load TX byte counter
	I2C_RXByteCtr = 0;
	I2C_TXCntr = I2C_TXData;
	I2C_RXCntr = I2C_RXData;

	I2C_TXData[0] = Register;
	I2C_TXData[1] = Value;
	I2C_Write_Init();
	READ = NO;
//	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as START is sent???
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}

/*----------------------------------------------------------------------------*/
// Initialization of the I2C Module for Write operation.
void I2C_Write_Init(void)
{
    UCB0CTL1 |= UCTR;                       // UCTR=1 => Transmit Mode (R/W bit = 0)
    IFG2 &= ~UCB0TXIFG;                     // Clear TXIFG flag
    IE2 &= ~UCB0RXIE;                       // disable Receive ready interrupt
    IE2 |= UCB0TXIE;                       // enable Transmit ready interrupt
}

/*----------------------------------------------------------------------------*/
//   Initialization of the I2C Module for Read operation.
void I2C_Read_Init(void)
{
    UCB0CTL1 &= ~UCTR;                      // UCTR=0 => Receive Mode (R/W bit = 1)
    IFG2 &= ~UCB0RXIFG;                     // Clear RXIFG flag
    IE2 &= ~UCB0TXIE;                       // disable Transmit ready interrupt
    IE2 |= UCB0RXIE;                        // enable Receive ready interrupt
}

// ********** ********** END OF I2C ********** **********//
