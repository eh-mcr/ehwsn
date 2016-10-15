/*
 * sensor_func_set.h
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef SENSOR_FUNC_SET_H_
#define SENSOR_FUNC_SET_H_

#include "sht21.h"
#include "lps25h.h"
#include "adxl362.h"
#include "adc_func_set.h"
#include "oa_func_set.h"
#include "pd_func_set.h"

unsigned int Vcc_last;
signed int Temperature_last;
unsigned int Humidity_last;
unsigned long Pressure_last;
signed int Acceleration_last[3];

signed int Vcc_diff;
signed int Temperature_diff;
signed int Humidity_diff;
signed long Pressure_diff;
signed int Acceleration_diff[3];

#endif /* SENSOR_FUNC_SET_H_ */
