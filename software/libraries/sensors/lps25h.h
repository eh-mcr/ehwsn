/*
 * lps25h.h
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef LPS25H_H_
#define LPS25H_H_

#include "msp430x22x4.h"
#include "usci.h"

// ******************* LPS25H *********************** //

#define	LPS25H_SLAVE_ADDRESS	0x5C		// = 0b101 1100
// registers
#define	REF_P_XL		0x08
#define	REF_P_L			0x09
#define	REF_P_H			0x0A
#define	WHO_AM_I		0x0F
#define	RES_CONF		0x10
	#define	AVGT1			BIT3
	#define	AVGT0			BIT2
	#define	AVGP1			BIT1
	#define	AVGP0			BIT0
#define	CTRL_REG1		0x20
	#define	PD				BIT7
	#define	ODR2			BIT6
	#define	ODR1			BIT5
	#define	ODR0			BIT4
	#define	DIFF_EN			BIT3
	#define	BDU				BIT2
	#define	RESET_AZ		BIT1
	#define	SIM				BIT0
#define CTRL_REG2		0x21	// CTRL_REG2
	#define	BOOT			BIT7
	#define	FIFO_EN			BIT6
	#define	WTM_EN			BIT5
	#define	FIFO_MEAN_DEC	BIT4
	#define	SPI_ENABLE		BIT3
	#define	SWRESET			BIT2
	#define	AUTO_ZERO		BIT1
	#define	ONE_SHOT		BIT0
#define	CTRL_REG3		0x22
	#define	INT_H_L			BIT7
	#define	PP_OD			BIT6
	#define	INT1_S2			BIT1
	#define	INT1_S1			BIT0
#define CTRL_REG4		0x23	// CTRL_REG4
	#define	P1_EMPTY		BIT3
	#define	P1_WTM			BIT2
	#define	P1_OVERRUN		BIT1
	#define	P1_DRDY			BIT0
#define	INT_CFG			0x24
	#define	LIR				BIT2
	#define	PL_E			BIT1
	#define	PH_E			BIT0
#define	INT_SOURCE		0x25
	#define	IA				BIT2
	#define	PL				BIT1
	#define	PH				BIT0
#define	STATUS_REG		0x27
	#define	P_OR			BIT5
	#define	T_OR			BIT4
	#define	P_DA			BIT1
	#define	T_DA			BIT0
#define	PRESS_OUT_XL	0x28
#define	PRESS_OUT_L		0x29
#define	PRESS_OUT_H		0x2A
#define	TEMP_OUT_L		0x2B
#define	TEMP_OUT_H		0x2C
#define	FIFO_CTRL		0x2E
	#define	F_MODE2			BIT7
	#define	F_MODE1			BIT6
	#define	F_MODE0			BIT5
	#define	WTM_POINT4		BIT4
	#define	WTM_POINT3		BIT3
	#define	WTM_POINT2		BIT2
	#define	WTM_POINT1		BIT1
	#define	WTM_POINT0		BIT0
#define	FIFO_STATUS		0x2F
	#define	WTM_FIFO		BIT7
	#define	FULL_FIFO		BIT6
	#define	EMPTY_FIFO		BIT5
	#define	DIFF_POINT4		BIT4
	#define	DIFF_POINT3		BIT3
	#define	DIFF_POINT2		BIT2
	#define	DIFF_POINT1		BIT1
	#define	DIFF_POINT0		BIT0
#define	THS_P_L			0x30
#define	THS_P_H			0x31
#define	RPDS_L			0x39
#define	RPDS_H			0x3A

#define AUTO_INCR	BIT7		// add to register address to auto-increment read/write

// variables
extern unsigned long Pressure;

// functions
unsigned long ReadPressure();

// ******************* END OF LPS25H *********************** //

#endif /* LPS25H_H_ */
