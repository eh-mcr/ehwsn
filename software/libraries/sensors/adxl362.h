/*
 * adxl362.h
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef ADXL362_H_
#define ADXL362_H_

// ******************* ADXL362 *********************** //

// constants
/* --- command --- */
#define XL362_REG_WRITE			0x0A
#define XL362_REG_READ			0x0B
#define XL362_FIFO_READ			0x0D

/* ------- Register names ------- */
// ID constants
#define XL362_DEVID_AD           0x00
#define XL362_DEVID_MST          0x01
#define XL362_PARTID             0x02
#define XL362_REVID              0x03
#define XL362_XID                0x04

/* -- RO data -- */
#define XL362_XDATA8             0x08
#define XL362_YDATA8             0x09
#define XL362_ZDATA8             0x0A
#define XL362_STATUS             0x0B
#define XL362_FIFO_ENTRIES_L     0x0C
#define XL362_FIFO_ENTRIES_H     0x0D
#define XL362_XDATAL             0x0e
#define XL362_XDATAH             0x0f
#define XL362_YDATAL             0x10
#define XL362_YDATAH             0x11
#define XL362_ZDATAL             0x12
#define XL362_ZDATAH             0x13
#define XL362_TEMPL              0x14
#define XL362_TEMPH              0x15
#define XL362_X_ADCL             0x16
#define XL362_X_ADCH             0x17

/* -- Control and Config -- */
#define XL362_SOFT_RESET         0x1f
#define XL362_THRESH_ACTL        0x20
#define XL362_THRESH_ACTH        0x21
#define XL362_TIME_ACT           0x22
#define XL362_THRESH_INACTL      0x23
#define XL362_THRESH_INACTH      0x24
#define XL362_TIME_INACTL        0x25
#define XL362_TIME_INACTH        0x26
#define XL362_ACT_INACT_CTL      0x27
#define XL362_FIFO_CONTROL       0x28
#define XL362_FIFO_SAMPLES       0x29
#define XL362_INTMAP1            0x2a
#define XL362_INTMAP2            0x2b
#define XL362_FILTER_CTL         0x2c
#define XL362_POWER_CTL          0x2d
#define XL362_SELF_TEST			0x2e // can be used to check function of device.
#define XL362_ID				0xe2	// device should return this number when ID is read.

// soft reset                                              */
#define XL362_SOFT_RESET_KEY      0x52

// ACT_INACT_CTL
#define XL362_ACT_ENABLE      0x01
#define XL362_ACT_DISABLE     0x00
#define XL362_ACT_AC          0x02
#define XL362_ACT_DC          0x00
#define XL362_INACT_ENABLE    0x04
#define XL362_INACT_DISABLE   0x00
#define XL362_INACT_AC        0x08
#define XL362_INACT_DC        0x00
#define XL362_ACT_INACT_LINK  0x10
#define XL362_ACT_INACT_LOOP  0x20

//FIFO_CTL
#define XL362_FIFO_MODE_OFF     0x00
#define XL362_FIFO_MODE_FIFO    0x01
#define XL362_FIFO_MODE_STREAM  0x02
#define XL362_FIFO_MODE_TRIGGER 0x03
#define XL362_FIFO_TEMP         0x04
#define XL362_FIFO_SAMPLES_AH   0x08

// INTMAP1 and INTMAP2
#define XL362_INT_DATA_READY     0x01
#define XL362_INT_FIFO_READY     0x02
#define XL362_INT_FIFO_WATERMARK 0x04
#define XL362_INT_FIFO_OVERRUN   0x08
#define XL362_INT_ACT            0x10
#define XL362_INT_INACT          0x20
#define XL362_INT_AWAKE          0x40
#define XL362_INT_LOW            0x80

// FILTER_CTL
#define XL362_RATE_400        0x05
#define XL362_RATE_200        0x04
#define XL362_RATE_100        0x03  /* default */
#define XL362_RATE_50         0x02
#define XL362_RATE_25         0x01
#define XL362_RATE_12_5       0x00

#define XL362_EXT_TRIGGER     0x08
#define XL362_HALF_BW		  0x10

#define XL362_AXIS_X          0x00
#define XL362_AXIS_Y          0x10
#define XL362_AXIS_Z          0x20

#define XL362_RANGE_2G        0x00
#define XL362_RANGE_4G        0x40
#define XL362_RANGE_8G        0x80

// POWER_CTL
#define XL362_STANDBY           0x00
#define XL362_MEASURE        	0x02
#define XL362_AUTO_SLEEP        0x04
#define XL362_SLEEP             0x08

#define XL362_LOW_POWER         0x00
#define XL362_LOW_NOISE1        0x10
#define XL362_LOW_NOISE2        0x20
#define XL362_LOW_NOISE3        0x30

#define XL362_EXT_CLOCK         0x40
#define XL362_EXT_ADC           0x80

// SELF_TEST
#define XL362_SELFTEST_ON       0x01
#define XL362_SELFTEST_OFF      0x00

#define	XL362_DEV_ID			0xAD


// pin definitions
#define INT1	BIT1	// INT1 pin = P1.INT1
#define INT2	BIT2	// INT2 pin = P2.INT2

// variables
extern signed int Acceleration_Hi_Res[3];
extern signed char Acceleration_Lo_Res[3];

extern unsigned char ID_adxl;

// functions
void ADXL362_Init(void);
unsigned char ADXL362_ReadID(void);
unsigned char ADXL362_Read_Register(char Register);
void ADXL362_Write_Register(char Register, char Value);
void ADXL362_Single_Read(void);
void ADXL362_Single_Read_Hi_Res(void);
void ADXL362_Config(void);
void ADXL362_Read_XYZ_Reg(void);
void ADXL362_Read_XYZ_Reg_Hi_Res(void);
void ADXL362_Sleep(void);
int ADXL362_Present(void);

// ******************* END OF ADXL362 *********************** //

#endif /* ADXL362_H_ */
