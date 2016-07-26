/*
 * pd_func_set.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "pd_func_set.h"
#include "msp430x22x4.h"
#include "defs.h"
#include "gen_func_set.h"
#include "oa_func_set.h"
#include "adc_func_set.h"

 unsigned int Read_PD(void)	// read photodiode voltage (connected to P2.0 pin3(+) to P2.2 (-) pin5)
 {
 	unsigned int voltage;
 	unsigned long voltage_long;
 	unsigned int ADC;

 	OA0_Config();	// configure OA0 (pins and input/ouput channels)
 	Start_OA0_Slow();	// start op amp (in slow slew rate)
 	delay(150);		// delay to allow op-amp to settle
     ADC = ReadADC_1_5(INCH_1);	//Vref = 1.5V read ADC from INCH channel A1, P2.1 pin4
     Stop_OA0();

     voltage_long = (unsigned long)ADC;
     voltage_long = voltage_long*1000;
     voltage = voltage_long>>10;

 	return voltage;
 }

 unsigned int Read_PD1(void)	// read photodiode voltage (connected to OA1I0 P2.3 pin6(+) - OA1I1 P2.4 (-) pin7)
 {
 	unsigned int voltage;
 	unsigned long voltage_long;
 	unsigned int ADC;

 	OA1_Config();	// configure OA0 (pins and input/ouput channels)
 	Start_OA1_Slow();	// start op amp (in slow slew rate)
 	delay(150);		// delay to allow op-amp to settle
     ADC = ReadADC_1_5(INCH_13);	//Vref = 1.5V read ADC from INCH channel A13, p4.4 pin 9
     Stop_OA1();

     voltage_long = (unsigned long)ADC;
     voltage_long = voltage_long*1500;
     voltage = voltage_long>>10;

 	return voltage;
 }

