/*
 * cc1120_gpio.h
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#ifndef CC1120_GPIO_H_
#define CC1120_GPIO_H_

#include "main.h"

//#define GPIO0_PIN	GPIO_PIN_12
#define GPIO0_PIN	GPIO_PIN_3
//#define GPIO2_PIN	GPIO_PIN_8

#define CC1120_RST_PORT   GPIOC
#define CC1120_RST_PIN    GPIO_PIN_4

#define CC1120_NSS_PORT		GPIOA
#define CC1120_NSS_PIN		GPIO_PIN_4

#endif /* CC1120_GPIO_H_ */
