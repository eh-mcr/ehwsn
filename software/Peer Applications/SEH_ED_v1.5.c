// ED_all_sensors_transmit_data.c
// READ ALL SENSORS AND TX DATA
// 18/09/2015
// use with corresponding AP
#include "bsp.h"
#include "mrfi.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_frame.h"
#include "nwk.h"

#include "vlo_rand.h"

#include "msp430x22x4.h"
#include "defs.h"
#include "gen_func_set.h"
#include "sensor_func_set.h"


void linkTo(void);

// definitions for this node
#define MESSAGE_LENGTH		19	// must be less than or equal to MAX_APP_PAYLOAD	//5
#define NODE_ID	0x03				// NODE ID - MUST BE SEPERATE FOR EVERY NODE USED

// directly-used variables
unsigned int Vcc;
unsigned int i;
unsigned char ID_adxl;
unsigned int SlavePresent;
unsigned int MEASURE = 0;
unsigned char msg_status =0;
unsigned char ID_adxl;
unsigned int Vpd;

void main (void)
{
  addr_t lAddr;
  char *Flash_Addr;
  unsigned int current_voltage;


  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |= 0x03;                            // Set P1.0,1 Output
  if( CALBC1_1MHZ == 0xFF && CALDCO_1MHZ == 0xFF &&
      CALBC1_8MHZ == 0xFF && CALDCO_8MHZ == 0xFF )// Do not run if cal values
  {                                         // are erased and set LEDs ON
    P1OUT |= 0x03;                          // Set P1.0,1 High
    __bis_SR_register(LPM4_bits);           // Enter LPM4 if Cal missing
  }

  // Blink LED for startup feedback
  P1OUT |= 0x03;                            // Set P1.0,1 High
  __delay_cycles(10000);
  P1OUT &= ~0x03;                           // Set P1.0,1 Low

  Flash_Addr = (char *)0x10F0;              // RF Address = 0x10F0
  if( Flash_Addr[0] == 0xFF &&              // Check if device Address is missing
      Flash_Addr[1] == 0xFF &&
      Flash_Addr[2] == 0xFF &&
      Flash_Addr[3] == 0xFF )
  {
    createRandomAddress();                  // Create Random device address at
  }                                         // initial startup if missing
  lAddr.addr[0] = Flash_Addr[0];
  lAddr.addr[1] = Flash_Addr[1];
  lAddr.addr[2] = Flash_Addr[2];
  lAddr.addr[3] = Flash_Addr[3];
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);

  BSP_Init();                               // Initialize eZ430 hardware

  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = ACLK = VLO
  TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
  TBCCR0 =  WakeupPeriod;                   // ~10 sec (=15000/(12000/8))
  TBCTL = TBSSEL_1 + MC_1 + ID_3;           // ACLK, upmode, Divider = 8

  status = status_four;                     // Set status to 4

  // Initialize SimpliciTI
  while(SMPL_NO_JOIN == SMPL_Init((uint8_t (*)(linkID_t))0))
  {
    __bis_SR_register(LPM3_bits + GIE);     // LPM3 with interrupts enabled
  }

  // Put radio to sleep
  SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, "" );

  ftt_flag = 1;                             // first time thru the program flag

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO = 1MHz
  DCOCTL = CALDCO_1MHZ;

  // SimpliciTI will change port pin settings as well
  P1DIR = 0xFB;                             // P1.2 (button) = input
  P1OUT = 0x04;                             // P1.2 pullup
  P1REN |= 0x04;                            // P1.2 pullup
  P1IE |= 0x04;                             // P1.3 interrupt enabled
  P1IES |= 0x04;                            // P1.3 Hi/lo edge
  P1IFG &= ~0x04;                           // P1.3 IFG cleared
  P2DIR = 0x2E;
  P2REN |= 0x01;
  P2OUT = 0x01;
  P3DIR |= 0xD0;                            // port 3 set after initilization
  P3OUT &= ~0x30;                           // set up port 3
  P3REN |= 0x20;                            // Enable Pull-Down Res for /Charge
  P4DIR = 0xFF;                             // setup port 4
  P4OUT = 0x00;

  P3OUT &= ~0x10;                         // turn on battery

  timer_state = timer_state_7;              // set timer state to 2 ~ 10 sec
  change_mode = 10;                         // Default GUI display mode set to
                                            // 10 sec

  TBCTL |= TBCLR;                           // Clear TBR counter
  TBCCR0 = sec10;	// a_d_wakeup_time;                 // set timer to wakeup time ~ 3 sec

	if (TEST == 0 )
	{
	  current_voltage = get_voltage();          // get current battery voltage
	  if(current_voltage < VCC_RUN_VOLTAGE)
	  {
		current_voltage = 0;
		while (current_voltage < BAT_MIN_VOTAGE)
		{
		  __bis_SR_register(LPM3_bits + GIE);   // Enter LPM3 w/ interrupts
		  current_voltage = get_voltage();
		}
	  }
	}

  SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, "" );
  // unconditional link to AP which is listening due to successful join.
  linkTo();
}

/*******************************************************************************
 * @fn          linkTo
 ******************************************************************************/
void linkTo(void)
{
    linkID_t linkID1;
    uint8_t  msg[MESSAGE_LENGTH];
    unsigned int delay_count = 1;

    // keep trying to link... Uses Timer B to wake up periodically
    while (SMPL_SUCCESS != SMPL_Link(&linkID1))
    {
        __bis_SR_register(LPM3_bits + GIE);     // LPM3 with interrupts enabled
    }

    // put radio to sleep once a successfull connection has been established
    SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, "" );

    //   I2C
    I2C_Init();
    UCB0_MODE = I2C;
    Slave_Present = I2C_Slave_Test(LPS25H_SLAVE_ADDRESS);
    RegisterData = I2C_Read_Register(WHO_AM_I);

    Slave_Present = I2C_Slave_Test(SHT21_SLAVE_ADDRESS);

    // ADXL362
    P2SEL &= ~nSS;
    UCB0_MODE = SPI;
    SPI_Init();
    SlavePresent = ADXL362_Present();
    ADXL362_Write_Register(XL362_SOFT_RESET,XL362_SOFT_RESET_KEY);		// soft reset
    ID_adxl = ADXL362_ReadID();
    UCB0CTL1 |= UCSWRST;			// set to reset (power down)

    msg_status |= START_FLAG;

    if (TEST ==0 )
    {
        delay(sec10);                  // enter sleep mode to ensure battery is replenished
    }


    //create message
    while(1)
    {
        // main sensor loop
        NodeStatus |= Pending;	// measurement is pending
        while (NodeStatus & Pending)// at the start of the sensor measurement loop
        {
            NodeStatus &= ~Pending;		// clear pending flag, no more measurements are scheduled
            NodeStatus |= InMeasurement;	// node is now measuring
            // save old values
            Vcc_last = Vcc;
            Temperature_last = Temperature;
            Humidity_last = Humidity;
            Pressure_last = Pressure;
            int i;
            for (i=0; i<3; i++)
            {
                Acceleration_last[i] = Acceleration_Hi_Res[i];
            }

            // Vcc
            Vcc = get_voltage();		// using Vref = 1.5

            Vcc_diff = Vcc-Vcc_last;
            if (Vcc_diff <0 )
            {
                Vcc_diff *= -1;
            }
            if (Vcc_diff > VCC_DIFF_MAX)
            {
                msg_status |= VCC_FLAG;
            }

            if ( Vcc < VCC_SLEEP_VOLTAGE )
            {
                msg_status |= STOP_FLAG;
                MEASURE = 0;
            }
            //	  	RegisterData = 0;
            P3SEL &= ~BIT0;					// set to IO function
            P3OUT |= BIT0;					// Set CC2500 CS line high (DISABLE RADIO COMMS)
            P3DIR |= BIT0;

            I2C_Init();
            UCB0_MODE = I2C;

            // LPS25H
            // TESTI2C BUS FOR LPS25H
            UCB0CTL1 |= UCTR + UCTXSTT + UCTXSTP;       // I2C TX, start condition (Transmit mode), STOP
            while (UCB0CTL1 & UCTXSTP);                 // wait for STOP condition
            UCB0I2CSA = LPS25H_SLAVE_ADDRESS;
            //	    RegisterData = I2C_Read_Register(WHO_AM_I);
            I2C_Write_Register(CTRL_REG1, PD);		// turn on pressure sensor (one-shot mode)
            I2C_Write_Register(CTRL_REG2, ONE_SHOT);		// trigger pressure measurement

            delay(90);		// pause 60 ms
            Pressure = ReadPressure();
            I2C_Write_Register(CTRL_REG1, 0);		// turn off pressure sensor

            Pressure_diff = Pressure - Pressure_last;
            if (Pressure_diff <0 )
            {
                Pressure_diff *= -1;
            }
            if (Pressure_diff > P_DIFF_MAX)
            {
                msg_status |= P_FLAG;
            }

            //
            ////	  	delay(300);		// pause
            //
            // SHT21
            UCB0I2CSA = SHT21_SLAVE_ADDRESS;
            Humidity = SHT21_RH_Read();
            Humidity_diff = Humidity - Humidity_last;
            if ( Humidity_diff < 0 )
            {
                Humidity_diff *= -1;
            }
            if ( ( Humidity_diff > RH_DIFF_MAX ) | ( Humidity < RH_LOWER ) | ( Humidity > RH_UPPER ) )
            {
                msg_status |= RH_FLAG;
            }

            //	  	delay(150);		// pause
            Temperature = SHT21_T_Read();
            Temperature_diff = Temperature-Temperature_last;
            if (Temperature_diff <0 )
            {
                Temperature_diff *= -1;
            }
            if ( ( Temperature_diff > T_DIFF_MAX ) | ( Temperature < T_LOWER ) | ( Temperature > T_UPPER ) )
            {
                msg_status |= T_FLAG;
            }
            //
            ////	  	delay(300);		// pause

            // ADXL362
            SPI_Init();
            UCB0_MODE = SPI;
            //		ADXL362_Write_Register(XL362_SOFT_RESET,XL362_SOFT_RESET_KEY);		// soft reset
            //ID_adxl = ADXL362_ReadID();
            // power on
            ADXL362_Write_Register(XL362_POWER_CTL,XL362_MEASURE);		// set into measurement emode
            delay(15);		// wait for samples
            // read data
            //		ADXL362_Read_XYZ_Reg();
            ADXL362_Read_XYZ_Reg_Hi_Res();
            // power off
            ADXL362_Write_Register(XL362_POWER_CTL,0);		// set into sleep emode
            UCB0CTL1 |= UCSWRST;			// set to reset (power down)
            for (i=0; i < 3; i++)
            {
                Acceleration_diff[i] = Acceleration_Hi_Res[i] - Acceleration_last[i];
                if (Acceleration_diff[i] <0 )
                {
                    Acceleration_diff[i] *= -1;
                }
                if (Acceleration_diff[i] > ACC_DIFF_MAX )
                {
                    msg_status |= ACC_FLAG;
                }
            }

            //Vpd = 111;
            //		Vpd = Read_PD();	// read photodiode voltage (connected to P2.0(-) to P2.2 (+))
            Vpd = 0;
            P2DIR |= (BIT0 + BIT1 + BIT2);
            P2OUT &= ~(BIT0 + BIT1 + BIT2);

            //		delay_count =9/((Vpd>>7)+1);
            //		delay_count = 2;
            //			if ( msg_status != 0 )
            {

                /* 		int i = 0;
                for (i=0 ; i < (MESSAGE_LENGTH) ; i++)
                {
                msg[i] = i;
                } */

                i=0;
                msg[i++] = NODE_ID;		// ID of node
                msg[i++] = (Vcc>>8)&0xFF;
                msg[i++] = Vcc&0xFF;
                msg[i++] = (Temperature>>8)&0xFF;
                msg[i++] = Temperature&0xFF;
                msg[i++] = (Humidity>>8)&0xFF;
                msg[i++] = Humidity&0xFF;
                msg[i++] = (Acceleration_Hi_Res[0]>>8)&0xFF;
                msg[i++] = Acceleration_Hi_Res[0]&0xFF;
                msg[i++] = (Acceleration_Hi_Res[1]>>8)&0xFF;
                msg[i++] = Acceleration_Hi_Res[1]&0xFF;
                msg[i++] = (Acceleration_Hi_Res[2]>>8)&0xFF;
                msg[i++] = Acceleration_Hi_Res[2]&0xFF;
                msg[i++] = ((Pressure>>8)>>8)&0xFF;
                msg[i++] = (Pressure>>8)&0xFF;
                msg[i++] = Pressure&0xFF;
                msg[i++] = (Vpd>>8)&0xFF;
                msg[i++] = Vpd&0xFF;
                msg[i++] = msg_status;


                MRFI_Init();
                //		mrfiSpiInit();
                // Wake radio-up
                SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, "" );

                // Send message
                if (SMPL_SUCCESS == SMPL_Send(linkID1, msg, sizeof(msg)))
                {
                    //         status_indicator(status_one, 2);	// Blink Green LED if success.
                }
                else                                  // Blink RED LED if transmission
                {                                     // failed
                    //        status_indicator(status_one, 1);
                }
                SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, "" );


                //   status_indicator(status_one, 2);	// Blink Green LED if success.
                msg_status = 0;
            }	// end of status if loop

            // end of sensor loop
            NodeStatus &= ~InMeasurement;	// not measuring any more!
        }   // end of         while (NodeStatus & Pending)// at the start of the sensor measurement loop
        DataInterruptSource = 0;	// clear data interrupt flags before next loop

        if (TEST)
        {
            delay(test_delay);                  // enter sleep mode between measurements
            MEASURE = 1;
        }
        else
        {
            sleep(delay_count);
        }
    }   // end of while loop
}
