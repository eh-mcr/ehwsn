/*
 * adxl362.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "adxl362.h"
#include "usci.h"
#include "defs.h"
#include "gen_func_set.h"

signed int Acceleration_Hi_Res[3];

// ********** ********** ADXL362 ********** **********//

// Initialise ADXL362
void ADXL362_Init(void)
{
	// Initialise SPI
//	USCI_MODE = SPI_Init();
	//Configure ADXL362
	ADXL362_Config();
	// Set ADCL362 to sleep mode
	ADXL362_Sleep();
	ADXL362_Interrupts_Init();    // set up interrupt pin
}
void ADXL362_Interrupts_Init(void)
{
    P2DIR &= ~ADXL362INT;   // INPUT
    P2DIR &= ~ADXL362INT;    // LO
    // P2IES |=    P2DIR &= ~ADXL362INT;   // ENABLE INTERRUPTS
    // P2IE..   // HI-Lo transition??
}
unsigned char ADXL362_ReadID(void)
{
	unsigned char id_adxl;

	SPI_TXData[0] = XL362_REG_READ;		// Make outgoing message		WRITE
	SPI_TXData[1] = XL362_DEVID_AD;		// Make outgoing message		ADDRESS(POWER_CTL)

	SPI_TXByteCtr = 2;      // Load TX byte counter with no. of bytes to write.  This is only 1 in the case that a read is made.
	SPI_TXCntr = SPI_TXData;		// set TX pointer to start of TXData array
	SPI_RXData[0] = 0xFF;
	SPI_RXByteCtr = 1;		// recieve only 1 byte through SPI.
	SPI_RXCntr = SPI_RXData;		// set RX pointer to start of RXData array.

	P2OUT &= ~nSS;					// Lower nSS (make active)
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	IFG2 &= ~UCB0RXIFG;		// clear RX flag so doesn't trigger straight away
	UCB0TXBUF = 55;		// =01010101                     // Load TX buffer to trigger read from accelerometer
	IE2 |= UCB0RXIE;		// Enable RX interrupts
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	P2OUT |= nSS;					// Raise nSS (make inactive)

	id_adxl = SPI_RXData[0];

	return id_adxl;
}

unsigned char ADXL362_Read_Register(char Register)
{
	unsigned char Register_Content = 0;

	// Set resolution/operating mode of ADXL362void ConfigureADXL()
	SPI_TXData[0] = XL362_REG_READ;		// Make outgoing message		WRITE
	SPI_TXData[1] = Register;		// Make outgoing message		ADDRESS(POWER_CTL)

	SPI_TXByteCtr = 2;      // Load TX byte counter with no. of bytes to write.  This is only 1 in the case that a read is made.
	SPI_TXCntr = SPI_TXData;		// set TX pointer to start of TXData array
	SPI_RXData[0] = 0;
	SPI_RXByteCtr = 1;		// recieve only 1 byte through SPI.
	SPI_RXCntr = SPI_RXData;		// set RX pointer to start of RXData array.

	P2OUT &= ~nSS;					// Lower nSS (make active)
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled

	IFG2 &= ~UCB0RXIFG;		// clear RX flag so doesn't trigger straight away
	UCB0TXBUF = 0;                     // Load TX buffer to trigger read from accelerometer

	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0RXIE;		// Enable RX interrupts
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	P2OUT |= nSS;					// Raise nSS (make inactive)

	Register_Content = SPI_RXData[0];

	return Register_Content;
}

// Read ADXL362 (X,Y,Z data)
void ADXL362_Read_XYZ_Reg(void)
{
	SPI_TXData[0] = XL362_REG_READ;		// Make outgoing message		WRITE
	SPI_TXData[1] = XL362_XDATA8;		// Make outgoing message		ADDRESS(POWER_CTL)

	SPI_TXByteCtr = 2;      // Load TX byte counter with no. of bytes to write.  This is only 1 in the case that a read is made.
	SPI_TXCntr = SPI_TXData;		// set TX pointer to start of TXData array
	SPI_RXData[0] = 0;
	SPI_RXByteCtr = 3;		// recieve 3 bytes through SPI.
	SPI_RXCntr = SPI_RXData;		// set RX pointer to start of RXData array.

	P2OUT &= ~nSS;					// Lower nSS (make active)
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	IFG2 &= ~UCB0RXIFG;		// clear RX flag so doesn't trigger straight away
	UCB0TXBUF = 0;                     // Load TX buffer to trigger read from accelerometer
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0RXIE;		// Enable RX interrupts
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	P2OUT |= nSS;					// Raise nSS (make inactive)

	int i = 0;
	for (i=0; i<3; i++)
		{
			Acceleration_Lo_Res[i] = SPI_RXData[i];
		}

 }

// Read ADXL362 (X,Y,Z data)
void ADXL362_Read_XYZ_Reg_Hi_Res(void)
{

	SPI_TXData[0] = XL362_REG_READ;		// Make outgoing message		READ
	SPI_TXData[1] = XL362_XDATAL;		// Make outgoing message		ADDRESS(POWER_CTL)

	SPI_TXByteCtr = 2;      // Load TX byte counter with no. of bytes to write.  This is only 1 in the case that a read is made.
	SPI_TXCntr = SPI_TXData;		// set TX pointer to start of TXData array
	SPI_RXData[0] = 0;
	SPI_RXByteCtr = 6;		// recieve byte no. through SPI.
	SPI_RXCntr = SPI_RXData;		// set RX pointer to start of RXData array.

	P2OUT &= ~nSS;					// Lower nSS (make active)
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	IFG2 &= ~UCB0RXIFG;		// clear RX flag so doesn't trigger straight away
	UCB0TXBUF = 0;                     // Load TX buffer to trigger read from accelerometer
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0RXIE;		// Enable RX interrupts
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
	P2OUT |= nSS;					// Raise nSS (make inactive)


	int i = 0;
	for (i=0; i<3; i++)
	{
		Acceleration_Hi_Res[i] = (SPI_RXData[2*i]<<4 |(( SPI_RXData[2*i+1]&0x0F)<<12))/16;
	}

}
void ADXL362_Single_Read(void)
{
	unsigned char Register;
	Register = ADXL362_Read_Register(XL362_POWER_CTL);

	// power on
	ADXL362_Write_Register(XL362_POWER_CTL,(XL362_MEASURE |Register));
	delay(150);		// wait for samples

	// read data
	ADXL362_Read_XYZ_Reg();

	// power off
	Register = ADXL362_Read_Register(XL362_POWER_CTL);
	ADXL362_Write_Register(XL362_POWER_CTL,Register&~XL362_MEASURE);
}
void ADXL362_Single_Read_Hi_Res(void)
{
	unsigned char Register;
	Register = ADXL362_Read_Register(XL362_POWER_CTL);

	// power on
	ADXL362_Write_Register(XL362_POWER_CTL,(XL362_MEASURE |Register));
	delay(150);		// wait for samplee

	// read data
	ADXL362_Read_XYZ_Reg_Hi_Res();

	// power off
	Register = ADXL362_Read_Register(XL362_POWER_CTL);
	ADXL362_Write_Register(XL362_POWER_CTL,Register&~XL362_MEASURE);
}
void ADXL362_Write_Register(char Register, char Value)
{
	// Set resolution/operating mode of ADXL362void ConfigureADXL()
	SPI_TXData[0] = XL362_REG_WRITE;		// Make outgoing message		WRITE
	SPI_TXData[1] = Register;		// Make outgoing message		ADDRESS(POWER_CTL)
	SPI_TXData[2] = Value;

	SPI_TXByteCtr = 3;      // Load TX byte counter with no. of bytes to write.  This is only 1 in the case that a read is made.
	SPI_TXCntr = SPI_TXData;		// set TX pointer to start of TXData array

	P2OUT &= ~nSS;					// Lower nSS (make active)
	__bic_SR_register(GIE);			// disable GIE so interrupt is not triggered as soon as TXIE is set
	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled - wait for interrupts
	P2OUT |= nSS;					// Raise nSS (make inactive)
}
// Configure ADXL362
void ADXL362_Config(void)
{
	// Set resolution/operating mode of ADXL362void ConfigureADXL()
	SPI_TXData[0] = XL362_REG_WRITE;		// Make outgoing message		WRITE
	SPI_TXData[1] = XL362_POWER_CTL;		// Make outgoing message		ADDRESS(POWER_CTL)
	SPI_TXData[2] = XL362_MEASURE;		// Make outgoing message		WRITE
	//SPI_TXData[3] = 0x02;		// Make outgoing message		RANDOM
	//SPI_TXData[4] = 0x02;		// Make outgoing message		RANDOM

	SPI_RXCntr = SPI_TXCntr = 0;		// Initialize message byte counters
	P2OUT &= ~nSS;					// Lower nSS (make active)

	IE2 |= UCB0TXIE;			// Enable TX interrupts, starts SPI
	__low_power_mode_0();		// Wait for SPI (needs SMCLK)
	// SPI transaction completed		(but USCI would keep SMCLK active)
	P2OUT |= nSS;					// Raise nSS (make inactive)
}

// Set ADXL362 into sleep mode
void ADXL362_Sleep(void)
{
	// Write value to CTRL register
}
//#pragma vector=PORT2_VECTOR
//__interrupt void Port_2(void)
//{
//	if(P2IFG & INT1)	// is it on P2.INT1
//	{
//		P2OUT &= ~INT2;		// INT2 LOW
//		  __bic_SR_register_on_exit(LPM0_bits);     // Clear CPUOFF bit from 0(SR)
//		  P2IFG &= ~INT1;                           // P2.INT1 IFG cleared key interuped
//	}
//}
int ADXL362_Present(void)
{
	unsigned char id_adxl;
	id_adxl = ADXL362_ReadID();
	if (id_adxl == XL362_DEV_ID)
	{
		return YES;
	}
	else
	{
		return NO;
	}
}
// ********** **********  END OF ADXL362 ********** **********//
