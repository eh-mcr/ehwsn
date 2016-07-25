/*
 * sht21.c
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#include "sht21.h"

// variables
unsigned int Humidity;
signed int Temperature;

// ********** **********  SHT21 ********** **********//

// Initialise SHT21
void SHT21_Init(void)
{
	// Initialise I2C
//	USCI_MODE = I2C_Init();
//	SHT21_Config();
//	SHT21 remains in LPM between measurements
}

// Configure SHT21 sensor
void SHT21_Config(char Resolution)
{
	unsigned char UserRegister;
	UserRegister = 	I2C_Read_Register(READ_USER_REGISTER);
	switch (Resolution)
	{
		case RES0:
			UserRegister &= ~(RES_UB|RES_LB);		// set resolution bits to LO
		break;

		case RES1:
			UserRegister |= RES_LB;			// LB to HI
			UserRegister &= ~RES_UB;		// set UB to LO
		break;

		case RES2:
			UserRegister &= ~RES_LB;		// LB to LO
			UserRegister |= RES_UB;			// set UB to HI
		break;

		case RES3:
			UserRegister |= (RES_UB|RES_LB);		// set resolution bits to HI
		break;

		default:
		break;
	}

//	UserRegister |= Resolution;	// set '1' bits HIGH
//
//	//UserRegister &= ~Resolution;		// set '0' bit LOW
//	UserRegister &= (Resolution|(~(RES_UB|RES_LB)));		// set '0' bits LOW

	SHT21_Write_Register(WRITE_USER_REGISTER, UserRegister);
}
// Read SHT21 Data
unsigned char SHT21_Read_Register(char Register)
{
	unsigned char RegisterContent;

	UCB0I2CSA = SHT21_SLAVE_ADDRESS;		// set I2C SA to SHT21

	I2C_TXByteCtr = 1;                          // Load TX byte counter
	I2C_RXByteCtr = 1;
	I2C_TXCntr = I2C_TXData;
	I2C_RXCntr = I2C_RXData;

	I2C_TXData[0] = Register;
	I2C_Write_Init();
	READ = YES;
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
												// Remain in LPM0 until all data
												// is TX'd
	I2C_Read_Init();                      // Init I2C module for a read operation
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;                // start condition generation
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

	RegisterContent = I2C_RXData[0];
	return RegisterContent;
}
// Write SHT21 Data
void 	SHT21_Write_Register(char Register, char Value)
{
	UCB0I2CSA = SHT21_SLAVE_ADDRESS;		// set I2C SA to SHT21

	I2C_TXByteCtr = 2;                          // Load TX byte counter
	I2C_RXByteCtr = 0;
	I2C_TXCntr = I2C_TXData;
	I2C_RXCntr = I2C_RXData;

	I2C_TXData[0] = Register;
	I2C_TXData[1] = Value;
	I2C_Write_Init();
	READ = NO;
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}
// Read SHT21 Register
unsigned int SHT21_Read_Data(char Register)
{
	unsigned int Data;

	UCB0I2CSA = SHT21_SLAVE_ADDRESS;		// set I2C SA to SHT21

	I2C_TXByteCtr = 1;                          // Load TX byte counter
	I2C_RXByteCtr = 2;
	I2C_TXCntr = I2C_TXData;
	I2C_RXCntr = I2C_RXData;

	I2C_TXData[0] = Register;
	I2C_Write_Init();
	READ = YES;
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;       // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
												// Remain in LPM0 until all data
												// is TX'd
	I2C_Read_Init();                      // Init I2C module for a read operation
	UCB0CTL1 |= UCTXSTT;// + UCTXSTP;                // start condition generation
//	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
//	UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

	Data = ((I2C_RXData[0]<<8) + I2C_RXData[1]);
	return Data;
}

// Read SHT21(T & RH)-> sleep SHT21
signed int SHT21_T_Read(void)
{
	signed int Temperature;
	unsigned int T_uncal;
	T_uncal = SHT21_Read_Data(T_HOLD_MASTER);
	T_uncal = ((I2C_RXData[0]<<8) + I2C_RXData[1]);
	Temperature = SHT21_T_Calib(T_uncal);
	return Temperature;
}
unsigned int SHT21_RH_Read(void)
{
	signed int Humidity;
	unsigned int Rh_uncal;
	Rh_uncal = SHT21_Read_Data(RH_HOLD_MASTER);
	Rh_uncal = ((I2C_RXData[0]<<8) + I2C_RXData[1]);
	Humidity = SHT21_RH_Calib(Rh_uncal);
	return Humidity;
}


signed int SHT21_T_Calib(unsigned int temp_data)

{
  float temp;              // variable for result

  temp_data &= ~0x0003;           // clear bits [1..0] (status bits)
  //-- calculate temperature [у] --
  temp = -46.85 + 175.72/65536 *(float)temp_data; //T= -46.85 + 175.72 * ST/2^16

  return (signed int)(temp*100);
}

unsigned int SHT21_RH_Calib(unsigned int hum_data)

{
  float hum;              // variable for result

  hum_data &= ~0x0003;          // clear bits [1..0] (status bits)
  //-- calculate relative humidity [%RH] --
  hum = -6.0 + 125.0/65536 * (float)hum_data; // RH= -6 + 125 * SRH/2^16

  return (unsigned int)(hum*100);
}

// ********** **********  END OF SHT21 ********** **********//
