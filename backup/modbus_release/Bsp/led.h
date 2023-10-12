/*
 * led.h
 *
 *  Created on: 2022. 4. 10.
 *      Author: luckyman
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx_hal.h"

///////////////////////////////////////////////////////////////////

#define LED1_PORT	GPIOC
#define LED1_PIN	GPIO_PIN_7

#define LED2_PORT   GPIOC
#define LED2_PIN	GPIO_PIN_8

typedef enum _LED
{
	LED1,
	LED2
} LED_e;


extern void ledOn(LED_e led);
extern void ledOff(LED_e led);
extern void ledToggle(LED_e led);
extern void led1_Init(void);
extern void led2_Init(void);

///////////////////////////////////////////////////////////////////


extern void dbgLedOn(void);
extern void dbgLedOff(void);
extern void dbgLedToggle(void);
extern void dbgLedInit(void);


#endif /* LED_H_ */
