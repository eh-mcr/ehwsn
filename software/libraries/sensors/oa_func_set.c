/*
 * oa_func_set.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "oa_func_set.h"
#include "msp430x22x4.h"
#include "defs.h"
#include "gen_func_set.h"

 /*
  * OA_Func_Set.c
  *
  *  Created on: 16 Nov 2014
  *      Author: Ralph S-J
  * Functions for reading op-amps
  */
 void OA0_Config(void)	// configure OA0 and start op-amp
 {
     OA0CTL0 = OAN_1 | OAP_0;			//-ive input = OA0I1, +ive input = OA0I0, output channels O:A1,A3,A5
     ADC10AE0 = BIT0 + BIT1 + BIT2;		// route output to A1 (p2.1) and enable inputs.  Without this line the pins are not enabled.
 }

 void Start_OA0_Slow(void)	// start op amp (in slow slew rate)
 {
 	OA0CTL0 |= OAPM_1;		// start op-amp in slow slew rate mode (lowest power consumption
 }

 void Start_OA0_Fast(void)	// start op amp (in slow slew rate)
 {
 	OA0CTL0 |= OAPM_3;		// start op-amp in fast slew rate mode (highest power consumption
 }

 void Stop_OA0(void)
 {
 	OA0CTL0 &= ~OAPM_3;		// stops op-amp by clearing bits form slew rate selector (will turn op-amp off form fast. medium or slow mode)
 }

 void OA1_Config(void)	// configure OA0 and start op-amp
 {
 	OA1CTL0 = OAN_1 | OAP_0 | OAADC0;		// select input/output channels I-:OA1I1, I+:OA1I0, O:A12,13,14
     ADC10AE1 = BIT5;		// enable AO1 output to A13 (p4.4)
     ADC10AE0 =  BIT3 + BIT4;	// enable OA1I1, enable OA1I0
 }

 void Start_OA1_Slow(void)	// start op amp (in slow slew rate)
 {
 	OA1CTL0 |= OAPM_1;		// start op-amp in slow slew rate mode (lowest power consumption
 }
 void Start_OA1_Fast(void)	// start op amp (in slow slew rate)
 {
 	OA1CTL0 |= OAPM_3;		// start op-amp in fast slew rate mode (highest power consumption
 }
 void Stop_OA1(void)
 {
 	OA1CTL0 &= ~OAPM_3;		// stops op-amp by clearing bits form slew rate selector (will turn op-amp off form fast. medium or slow mode)
 }
