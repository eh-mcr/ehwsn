/*
 * spi.c
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */
#include "spi.h"
#include "defs.h"
#include "msp430x22x4.h"

// variables
unsigned char SPI_TXData[10], SPI_RXData[10];	// TX and RX buffer limits
unsigned char* SPI_TXCntr;
unsigned char* SPI_RXCntr;
unsigned char SPI_TXByteCtr;
unsigned char SPI_RXByteCtr;

// ********** ********** SPI ********** **********//

// Initialise SPI (for ADXL362)
void SPI_Init(void)
{
	// Set /CS line for CC2500 HIGH (hardwired into code) and create /CS1 line in software.

	P3OUT |= BIT0;					// Set CC2500 CS line high
	P3DIR |= BIT0;

	P2OUT |= nSS;					// preload nSS for slave (s) to HIGH
	P2DIR |= nSS;					// set nSS pin for slave(s) to OUTPUT
//		P3DIR &= ~BIT2;		// set SOMI pin as input
//		P3OUT |= BIT2;		// set SOMI pin as HIGH
	P3SEL = BIT1 | BIT2 | BIT3;
	// SPI mode 3 needs CPOL=CKPL=1, CPHA=1 -> CKPH=0; msb first, master,
	//   8 bit (default), 3-wire (default, mode 0), synchronous
	UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;
	UCB0CTL1 = UCSSEL1 | UCSWRST;	// Clock from SMCLK; hold in reset
	UCB0BR1 = 0;					// Upper byte of divider word
	UCB0BR0 = 1;					// Clock = SMCLK / 1 = 1MHz
	UCB0CTL1 &= ~UCSWRST;			// Release from reset - increases power consumption
	IE2 &= ~UCB0RXIE;				// Disable interrupts on receive
	IE2 &= ~UCB0TXIE;				// Disable interrupts on transmit otherwise flag will be lifted instanltl
}

// ********** ********** END OF SPI ********** **********//
