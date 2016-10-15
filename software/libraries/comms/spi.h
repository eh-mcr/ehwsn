/*
 * spi.h
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef SPI_H_
#define SPI_H_

// constants
#define nSS		BIT7		// P2.nSS used for /CS for ADXL362

// variables
extern unsigned char SPI_TXData[10], SPI_RXData[10];	// TX and RX buffer limits
extern unsigned char* SPI_TXCntr;
extern unsigned char* SPI_RXCntr;
extern unsigned char SPI_TXByteCtr;
extern unsigned char SPI_RXByteCtr;

void SPI_Init(void);        // Initialise SPI (for ADXL362)



#endif /* SPI_H_ */
