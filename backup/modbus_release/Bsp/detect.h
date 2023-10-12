/*
 * detect.h
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#ifndef DETECT_H_
#define DETECT_H_

#include "stm32f4xx_hal.h"

extern uint8_t get_key_in1(void);
extern uint8_t get_dc_in_det(void);
extern uint8_t get_char_sta(void);

#endif /* DETECT_H_ */
