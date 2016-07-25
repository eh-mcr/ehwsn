/*
 * usci.c
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#include "usci.h"


// variables
char UCB0_MODE = 0;


// ************************************************************************** //
// INTERRUPT VECTORS
// ************************************************************************** //

// I2C *******************************************
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	switch (UCB0_MODE)
	{
		case I2C:
			if(UCB0TXIFG & IFG2)
			{
				if (I2C_TXByteCtr)                            // Check TX byte counter
				{
					UCB0TXBUF = *I2C_TXCntr++;                     // Load TX buffer
					I2C_TXByteCtr--;                            // Decrement TX byte counter
				}
				else
				{
					if(READ != YES)
					{
						UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
					}

					IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
					__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
				}
			}
			else if(UCB0RXIFG & IFG2)                    // Make sure it is an RX interrupt
			{
				*I2C_RXCntr++ = UCB0RXBUF;               // store received data in buffer
				I2C_RXByteCtr--;
				if (I2C_RXByteCtr == 1)		// second to last TX
				{
					UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
				}
				else if (I2C_RXByteCtr == 0)
				{
					IFG2 &= ~UCB0RXIFG;             // Clear RXIFG flag
					IFG2 &= ~UCB0TXIFG;             // Clear TXIFG flag
					IE2 &= ~UCB0TXIE;               // Disable Transmit ready interrupt
					IE2 &= ~UCB0RXIE;               // Disable Receive ready interrupt
					__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
				}
			}
		break;

		case SPI:
			UCB0TXBUF = *SPI_TXCntr++;                     // Load TX buffer
			SPI_TXByteCtr--;                            // Decrement TX byte counter
			if (SPI_TXByteCtr==0)				//last byte
			{
				IE2 &= ~UCB0TXIE;		// Disable further interrupts
				  __bic_SR_register_on_exit(LPM0_bits);     // Clear CPUOFF bit from 0(SR)
			}
		break;

		default:
		break;

	}	// end of switch
}

// SPI*******************************************

//// USCIAB0TX_ISR - pre-load TXByteCtr with the no. of bytes to TX before this interrupt is called
//
//#pragma vector = USCIAB0TX_VECTOR
//__interrupt void USCIAB0TX_ISR(void)
//{
//	case SPI:
//		UCB0TXBUF = *SPI_TXCntr++;                     // Load TX buffer
//		SPI_TXByteCtr--;                            // Decrement TX byte counter
//		if (SPI_TXByteCtr==0)				//last byte
//		{
//			IE2 &= ~UCB0TXIE;		// Disable further interrupts
//			  __bic_SR_register_on_exit(LPM0_bits);     // Clear CPUOFF bit from 0(SR)
//		}
//	break;
//}

//----------------------------------------------------------------------
 // ISR for USCI_A,B0 RX: store data, return when all bytes received
 //----------------------------------------------------------------------
 #pragma vector = USCIAB0RX_VECTOR
 __interrupt void USCIAB0RX_ISR (void)	// Acknowledge by read of RXBUF
 {
 	*SPI_RXCntr++ = UCB0RXBUF;	// Store recd data, update counter
 	SPI_RXByteCtr--;
 	if (SPI_RXByteCtr)
 	{
 		UCB0TXBUF = 0x00;
 	}
 	else
 	{		// Received complete message?
 		IE2 &= ~UCB0RXIE;		// Disable further interrupts
 		  __bic_SR_register_on_exit(LPM0_bits);     // Clear CPUOFF bit from 0(SR)
 	}
 }


