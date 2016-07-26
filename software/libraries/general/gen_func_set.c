/*
 * gen_func_set.c
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#include "gen_func_set.h"
#include "defs.h"
#include "gen_func_set.h"
#include "bsp_leds.h"
#include "vlo_rand.h"

unsigned int timer_state;
unsigned char change_mode;
unsigned char ftt_flag;
unsigned int battery_ready = 0;
unsigned int in_delay = 0;
char status = 0;
unsigned int battery_full_flag = 0;
unsigned long battery_full_timer = 0;

unsigned int number_transmits;

unsigned char NodeStatus = 0;
unsigned char DataInterruptSource = 0;



/*******************************************************************************
* BEGHDR
*
* NAME:createRandomAddress()
*
* DESCRIPTION: generate random address
*******************************************************************************/
void createRandomAddress()
{
  unsigned int rand, rand2;
  char *Flash_Addr;
  Flash_Addr = (char *)0x10F0;

  do
  {
    rand = TI_getRandomIntegerFromVLO();    // first byte can not be 0x00 of 0xFF
  }
  while( (rand & 0xFF00)==0xFF00 || (rand & 0xFF00)==0x0000 );
  rand2 = TI_getRandomIntegerFromVLO();

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  Flash_Addr[0]=(rand>>8) & 0xFF;
  Flash_Addr[1]=rand & 0xFF;
  Flash_Addr[2]=(rand2>>8) & 0xFF;
  Flash_Addr[3]=rand2 & 0xFF;

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}

/*******************************************************************************
* BEGHDR
* Function:    void StatusBlink_led1(int BlinkCount)
* DESCRIPTION: Blinks LED 1 - Red based on specified delay
* INPUTS:      BlinkCount
* PROCESSING:  Turns on and off the RED LED with specified blink time
* OUTPUTS:     VOID
*******************************************************************************/
void StatusBlink_led1(int BlinkCount)
{
  BSP_TURN_ON_LED1();
  delay(BlinkCount);
  BSP_TURN_OFF_LED1();
}

/*******************************************************************************
* BEGHDR
* Function:    void StatusBlink_led1(int BlinkCount)
* DESCRIPTION: Blinks LED 1 - Green based on specified delay
* INPUTS:      BlinkCount
* PROCESSING:  Turns on and off the Green LED with specified blink time
* OUTPUTS:     VOID
*******************************************************************************/
void StatusBlink_led2(int BlinkCount)
{
  BSP_TURN_ON_LED2();
  delay(BlinkCount);
  BSP_TURN_OFF_LED2();
}

/*******************************************************************************
* BEGHDR
* Function:    void delay(unsigned int BlinkCount)
* DESCRIPTION: Creates a low-power delay by entering LPM3 using Timer B.
*              Timer B frequency = VLO/8 = 1500 Hz.
* INPUTS:      BlinkCount
* PROCESSING:  Delay length of time of BlinkCount
* OUTPUTS:     VOID
********************************************************************************/
void delay(unsigned int BlinkCount)
{
  int TimerTemp;
  TimerTemp = TBCCR0;                       // Save current content of TBCCR0
  TBCCR0 = BlinkCount;                      // Set new TBCCR0 delay
  TBCTL |= TBCLR;                           // Clear TBR counter
  TBCCTL0 &= ~CCIFG;                        // Clear CCIFG Flag
  TBCTL |= MC_1;                            // Start Timer B
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3
  TBCTL &= ~(MC_1);                         // Stop Timer B
  TBCCR0 = TimerTemp;
}

/*******************************************************************************
* BEGHDR
* Function:    void status_indicator(char status , int status_led)
* DESCRIPTION: This can be usefull to blink the LED to indicate where the
*              program is executing for debugging purposes. It blinks the red or
*              green LED the number of times in status. For example, status_five
*              blinks the LED 5 times.
* INPUTS:      status, status_led
* PROCESSING:  Blinks the red or green led the number of times in status and the
*              correct led in status_led
* OUTPUTS:     VOID
*******************************************************************************/
void status_indicator(char status , int status_led)
{
  volatile unsigned int i = 0;
  switch (status)
  {
    case status_one:
      if (status_led == 1)
        StatusBlink_led1(15);
      if (status_led == 2)
        StatusBlink_led2(15);
      break;

    case status_two:
      if (status_led == 1)
      {
        StatusBlink_led1(15);
        delay(delay_time);
        StatusBlink_led1(15);
      }
      if (status_led==2)
      {
        StatusBlink_led2(15);
        delay(delay_time);
        StatusBlink_led2(15);
      }
      break;

    case status_three:
      if(status_led == 1)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led1(15);
          delay(delay_time);
        }
        StatusBlink_led1(15);
      }
      if(status_led == 2)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led2(15);
          delay(delay_time);
        }
        StatusBlink_led2(15);
      }
      break;

    case status_four:
      if(status_led == 1)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led1(15);
          delay(delay_time);
        }
        StatusBlink_led1(15);
      }
      if(status_led == 2)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led2(15);
          delay(delay_time);
        }
        StatusBlink_led2(15);
      }
      break;

    case status_five:
      if(status_led == 1)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led1(15);
          delay(delay_time);
        }
        StatusBlink_led1(15);
      }
      if(status_led == 2)
      {
        for(i=0 ; i < (status-1) ; i++)
        {
          StatusBlink_led2(15);
          delay(delay_time);
        }
        StatusBlink_led2(15);
      }
      break;
    default:
      break;
  }
}

/*******************************************************************************
* BEGHDR
* Function:    unsigned int get_voltage(void)
* DESCRIPTION: Get battery voltage with A/D
* INPUTS:      void
* PROCESSING:  Read battery voltage from ADC10 and returns the value
* OUTPUTS:     Battery voltage from A/D
*******************************************************************************/
unsigned int get_voltage(void)
{
  unsigned int rt_volts;

  ADC10CTL1 = INCH_11;                    // AVcc/2
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;	// + REF2_5V;
  __delay_cycles(250);                    // delay to allow reference to settle
  ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
  __bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled
  rt_volts = ADC10MEM;
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 &= ~(REFON + ADC10ON);        // turn off A/D to save power
  rt_volts = ((long)rt_volts*375)/128;	//625)/128;
  return (unsigned int)(rt_volts);
}

/*******************************************************************************
* BEGHDR
* NAME:        void transmit_time_delay(void)
* DESCRIPTION: Sets timer to transmit time based on timer_state
* INPUTS:      void
* PROCESSING:  Sets timer to transmit time, for 2 min and 4 min transmit times
*              loop number of 30 sec times to make 2 min and 4 min.
* OUTPUTS:     void
********************************************************************************/
void transmit_time_delay(void)
{
  volatile unsigned int i = 0;
  in_delay = 1;
  switch (timer_state)
  {
    case timer_state_1:                     // Timer State == 1; 5 Secs
      delay(sec5);
      in_delay = 0;
      battery_full_timer += sec5;
      break;
    case timer_state_2:                     // Timer State == 2; 10 Secs
      delay(sec10);
      in_delay = 0;
      battery_full_timer += sec10;
      break;

    case timer_state_3:                     // Timer State == 3; 20 Secs
      delay(sec20);
      in_delay = 0;
      battery_full_timer += sec20;
      break;

    case timer_state_4:                     // Timer State == 4; 40 Secs
      delay(sec40);
      in_delay = 0;
      battery_full_timer += sec40;
      break;

    case timer_state_5:                     // Timer State == 5; 2 mins
      while((i++ < 4) && (timer_state == timer_state_5))
      {
        delay(sec30_2);
        in_delay = 0;
        battery_full_timer += sec30_2;
      }
      break;

    case timer_state_6:                     // Timer State == 6; 4 mins
      while((i++ < 8) && (timer_state == timer_state_6))
      {
        delay(sec30_4);
        in_delay = 0;
        battery_full_timer += sec30_4;
      }
      break;
	  case timer_state_7:                     // Timer State == 40; 40 Secs
      delay(sec40);
      in_delay = 0;
      battery_full_timer += sec40;
      break;
    default:
      break;
  }
}// void transmit_time_delay(void)

/*******************************************************************************
* BEGHDR
* NAME:        void display_mode(void)
* DESCRIPTION: Sets mode time to be displayed on the GUI in the voltage stage
*              for the first display
* INPUTS:      void
* PROCESSING:  Sets change_mode number based on timer_state
* OUTPUTS:     void
*******************************************************************************/
void display_mode(void)
{
  switch(timer_state)
  {
    case timer_state_1:
      change_mode=5;                        //~=5 sec
      break;
    case timer_state_2:
      change_mode=10;                       //~=10 sec
      break;
    case timer_state_3:
      change_mode=20;                       //~=20 sec
      break;
    case timer_state_4:
      change_mode=40;                       //~=40 sec
      break;
    case timer_state_5:
      change_mode=2;                        //~=2 min
      break;
    case timer_state_6:
      change_mode=4;                        //~=4 min
      break;
    default:
      break;
  }
}

/*******************************************************************************
* BEGHDR
* NAME:        void check_batt_full(void)
* DESCRIPTION: Check if the battery has been charging for 1 hour using
*              battery_full_timer as the counter. battery_full_timer is updated
*              after each delay inside transmit_time_delay() function.
* INPUTS:      void
* PROCESSING:  If battery_full_timer has arrived, switch off batteries.
*              battery_full_flag set if charging time is matched.
* OUTPUTS:     void
*******************************************************************************/
void check_bat_full(void)
{
  // If /Charge is high, No Solar, turn on battery
  if(P3IN & 0x20)                           // /Charge=1; battery, Blink Red
  {
    P3OUT &= ~0x10;                         // turn on battery
    battery_full_flag = 0;
    battery_full_timer = 0;
  }
  else                                      // /Charge=0; Solar charging,
  {                                         // blink green
    if(battery_full_timer >= one_hour)      // If battery has been charging for
    {                                       // an hour, turn off battery
      battery_full_flag = 1;
      battery_full_timer = 0;
      P3OUT |= 0x10;                        // turn off battery
    }
  }
}

// Timer
void Set_TimerB(void)
{
	BCSCTL3 |= LFXT1S_2;                      // LFXT1 = ACLK = VLO
	TBCCTL0 = 0;						// TBCCR0 interrupt disabled
	TBCTL = TBSSEL_1 + MC_1 + ID_3;           // ACLK, upmode, Divider = 8
}


void sleep(unsigned int sleep_count)	// in intervals of 30s
{
	int i;
	NodeStatus &= ~Awake;	// node is asleep
	for (i=0 ; i < sleep_count; i++)
	{
		   delay(delay_period);                  // enter sleep mode between measurements
	}
	NodeStatus |= Awake;	// node is awake
}
/***********************************************************************
*BEGHDR
*NAME:        __interrupt void ADC10_ISR(void)
*DESCRIPTION: ADC10 interrupt service routine
*INPUTS:      void
*PROCESSING:  Exit from LPM after interrupt
*OUTPUTS:     void
***********************************************************************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(LPM0_bits);     // Clear CPUOFF bit from 0(SR)
}

/*******************************************************************************
* BEGHDR
* NAME:        __interrupt void Timer_B (void)
* DESCRIPTION: Timer B0 interrupt service routine
* INPUTS:      Void
* PROCESSING:  Exit from LPM after interrupt
* OUTPUTS:     Void
*******************************************************************************/
#pragma vector=TIMERB0_VECTOR
__interrupt void TimerB_ISR (void)
{
  __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bit from 0(SR)
}

/*******************************************************************************
* BEGHDR
* NAME:        __interrupt void Port_1(void)
* DESCRIPTION: Port 1 interrupt service routine function key
* INPUTS:      void
* PROCESSING:  process the push button to switch to the next time mode
* OUTPUTS:     void
********************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  if((P3IN & 0x20))                         // /Charge=1; battery, Blink Red
  {
    BSP_TURN_ON_LED1();
    __delay_cycles(10000);
    BSP_TURN_OFF_LED1();
  }
  else                                      // /Charge=0; Solar, blink green
  {
    BSP_TURN_ON_LED2();
    __delay_cycles(10000);
    BSP_TURN_OFF_LED2();
  }

  // If successful link, change timer state.
  if(status == status_six || status == status_five)
  {
    if(timer_state >= timer_state_6)        // If transmit time is == 6,
    {                                       // Set timer_state = 1
      timer_state = timer_state_1;
      display_mode();                       // Change GUI display time
    }
    else
    {
      timer_state++;                        // Change transmit time state
      display_mode();                       // Change GUI display time
    }
    if(in_delay)                            // If in transmit delay, exit and
    {                                       // send a new packet with new time
      __bic_SR_register_on_exit(LPM4_bits); // Clear LPM3 bit from 0(SR)
    }
  }
  __delay_cycles(150000);                   // Debounce software delay
  while(!(P1IN & 0x04));                    // Loop if button is still pressed
  P1IFG &= ~0x04;                           // P1.2 IFG cleared key interuped
}
