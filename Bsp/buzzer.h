/*
 * buzzer.h
 *
 *  Created on: 2022. 4. 10.
 *      Author: luckyman
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "stm32f4xx_hal.h"

extern void buzzOn(void);
extern void buzzOff(void);
extern void buzzToggle(void);
extern void buzz_Init(void);

#endif /* BUZZER_H_ */
