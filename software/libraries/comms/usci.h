/*
 * usci.h
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef USCI_H_
#define USCI_H_

#include "msp430x22x4.h"

#include "spi.h"
#include "i2c.h"

// constants
#define	SPI	0x01
#define	I2C	0x02

extern char UCB0_MODE; // = 0;

#endif /* USCI_H_ */
