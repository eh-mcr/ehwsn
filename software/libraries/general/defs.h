/*
 * defs.h
 *
 *  Created on: 25 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef DEFS_H_
#define DEFS_H_

#define run_voltage       3000                // Minimum voltage to execute startup
#define ad_check_voltage  29
#define key_down_count    12                // # times to check if button is
                                            // still button pressed

#define battery_time_test 174               // 3 min count at 10 sec for testing
#define running_on_battery 100              // Tells GUI that it is running on
                                            // battery
#define xmt_count         400               // # max transmit on battery - 400
                                            // magic number

// NodeStatus fields
#define Awake	BIT0	// used to indicate the node is awake within NodeStatus
#define Pending 	BIT1	// used to indicate measurement pending status of node within NodeStatus
#define InMeasurement	BIT2	// set to indicate that the node is currently performing a measurement

// DataInterruptSource fields
#define Shock   BIT0


// general
#define	ON                1
#define OFF               0

#define TEST	ON	// set to non-zero for testing
#define WakeupPeriod      15000             // ~10 sec (=15000/(12000/8))
#define a_d_wakeup_time   15000              // ~30 sec
#define TXPeriod          7500              // ~5 sec  (=7500/(12000/8))
#define delay_time        500               // led delay time
#define debounce_time     750               // key debounce

//Timer count for time between transmit
#define sec1              1500              // ~1 sec
#define sec2              2610
#define sec3              4500
#define sec4              6000
#define sec5              7500              // ~5 sec  (=7500/(12000/8))
#define sec10             15000             // ~10 sec
#define sec20             30000             // ~20 swec
#define	sec30			  45000				// ~ 30 seconds
#define sec40             60000             // ~40 sec
#define sec30_2           43000             // ~30sec 2 min?
#define sec30_4           50434             // ~30sec 4 min?
#define one_hour          5400000

#define	test_delay		sec2				// delay between measurements in TEST mode

#define port_delay        10                // 6ms - 1.5 msec

#define	YES	1
#define	NO	0

#define status_one        1
#define status_two        2
#define status_three      3
#define status_four       4
#define status_five       5
#define status_six        6
#define status_seven      7

#define timer_state_1     1
#define timer_state_2     2
#define timer_state_3     3
#define timer_state_4     4
#define timer_state_5     5
#define timer_state_6     6
#define timer_state_7     7


#define START_FLAG		BIT0
#define RESTART_FLAG	BIT1
#define T_FLAG			BIT2
#define RH_FLAG			BIT3
#define P_FLAG			BIT4
#define ACC_FLAG		BIT5
#define VCC_FLAG		BIT6
#define STOP_FLAG		BIT7

#define	delay_period		sec30

// sensor trigger limits
// Vcc - units of mV
#define	VCC_DIFF_MAX		10		// trigger tx if difference in voltage eceeds this
#define VCC_SLEEP_VOLTAGE	1700	//	3000	// send to sleep if voltage goes below this
#define VCC_REAWAKE_VOLTAGE	1700	//	3200	// to re-awaken after sleep mode
#define VCC_RUN_VOLTAGE		1700	//	3300	// for first time setup
#define BAT_MIN_VOTAGE		1700	//	2500	// minimum voltage for AA battery pack

// T - units of 0.01 C
#define T_DIFF_MAX		15
#define T_UPPER			3000
#define T_LOWER			2000

// RH - units of 0.01%
#define RH_DIFF_MAX		500
#define RH_UPPER 		7000
#define RH_LOWER 		3000

// P - unit of 0.01 mbar
#define P_DIFF_MAX		25
// Acc - units of mg
#define ACC_DIFF_MAX	50

#endif /* DEFS_H_ */
