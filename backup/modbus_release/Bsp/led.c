/*
 * led.c
 *
 *  Created on: 2022. 4. 10.
 *      Author: luckyman
 */
#include "led.h"

#define DBG_LED_PORT	GPIOC
#define DBG_LED_PIN		GPIO_PIN_13


void ledOn(LED_e led)
{
	if ( led == LED1 )
	{
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
	}
}

void ledOff(LED_e led)
{
	if ( led == LED1 )
	{
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
	}
}

void ledToggle(LED_e led)
{
	if ( led == LED1 )
	{
		HAL_GPIO_TogglePin(LED1_PORT, LED1_PIN);
	}
	else
	{
		HAL_GPIO_TogglePin(LED2_PORT, LED2_PIN);
	}
}

void led1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = LED1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);
}

void led2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = LED2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(LED2_PORT, &GPIO_InitStruct);
}
///////////////////////////////////////////////////////////////////


void dbgLedOn(void)
{
	HAL_GPIO_WritePin(DBG_LED_PORT, DBG_LED_PIN, GPIO_PIN_RESET);
}

void dbgLedOff(void)
{
	HAL_GPIO_WritePin(DBG_LED_PORT, DBG_LED_PIN, GPIO_PIN_SET);
}

void dbgLedToggle(void)
{
	HAL_GPIO_TogglePin(DBG_LED_PORT, DBG_LED_PIN);
}

void dbgLedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(DBG_LED_PORT, DBG_LED_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = DBG_LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(DBG_LED_PORT, &GPIO_InitStruct);
}





