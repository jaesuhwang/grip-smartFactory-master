/*
 * dectect.c
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */


#include "detect.h"

#define KEY_IN1_PORT	GPIOC
#define KEY_IN1_PIN		GPIO_PIN_5

#define DC_IN_DET_PORT		GPIOB
#define DC_IN_DET_PIN		GPIO_PIN_8

#define CHAR_STA_PORT		GPIOB
#define CHAR_STA_PIN		GPIO_PIN_9

uint8_t get_key_in1(void)
{
	return HAL_GPIO_ReadPin(KEY_IN1_PORT, KEY_IN1_PIN)? 0:1;
}

uint8_t get_dc_in_det(void)
{
	return HAL_GPIO_ReadPin(DC_IN_DET_PORT, DC_IN_DET_PIN)? 0:1;
}

uint8_t get_char_sta(void)
{
	return HAL_GPIO_ReadPin(CHAR_STA_PORT, CHAR_STA_PIN)? 0:1;
}
