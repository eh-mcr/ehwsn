/*
 * oa_func_set.h
 *
 *  Created on: 26 Jul 2016
 *      Author: Ralph S-J
 */

#ifndef OA_FUNC_SET_H_
#define OA_FUNC_SET_H_

void OA0_Config(void);	// configure OA0
void Start_OA0_Slow(void);	// start OA0 (in slow slew rate)
void Start_OA0_Fast(void);	// start OA0 (in fast slew rate)
void Stop_OA0(void);		// stop OA0 from any slew rate


void OA1_Config(void);	// configure OA1
void Start_OA1_Slow(void);	// start OA1 (in slow slew rate)
void Start_OA1_Fast(void);	// start OA1 (in fast slew rate)
void Stop_OA1(void);		// stop OA1 from any slew rate


#endif /* OA_FUNC_SET_H_ */
