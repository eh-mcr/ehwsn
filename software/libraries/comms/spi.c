/*
 * spi.c
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */
#include "spi.h"

// variables
unsigned char SPI_TXData[10], SPI_RXData[10];	// TX and RX buffer limits
unsigned char* SPI_TXCntr;
unsigned char* SPI_RXCntr;
unsigned char SPI_TXByteCtr;
unsigned char SPI_RXByteCtr;
