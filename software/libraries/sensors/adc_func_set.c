/*
 * adc_func_set.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "adc_func_set.h"
#include "msp430x22x4.h"
#include "defs.h"
#include "gen_func_set.h"

 unsigned int ReadADC_2_5(unsigned int INCH)	//Vref = 2.5V read ADC from INCH channel
 {
 	unsigned int ADC_Read;
     // Measure PD Voltage
     ADC10CTL1 = INCH;                    // read input channel
     ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE + REF2_5V;
     __delay_cycles(350);                    // delay to allow reference to settle
     ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
     __bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
     ADC_Read = ADC10MEM;
     ADC10CTL0 &= ~ENC;
     ADC10CTL0 &= ~(REFON + ADC10ON);        // turn off A/D to save power

 	return ADC_Read;
 }

 unsigned int ReadADC_1_5(unsigned int INCH)	//Vref = 2.5V read ADC from INCH channel
 {
 	unsigned int ADC_Read;
     // Measure PD Voltage
     ADC10CTL1 = INCH;                    // read input channel
     ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;	//
     __delay_cycles(350);                    // delay to allow reference to settle
     ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
     __bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
     ADC_Read = ADC10MEM;
     ADC10CTL0 &= ~ENC;
     ADC10CTL0 &= ~(REFON + ADC10ON);        // turn off A/D to save power

 	return ADC_Read;
 }

