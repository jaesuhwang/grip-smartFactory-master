/*
 * buzzer.c
 *
 *  Created on: 2022. 4. 10.
 *      Author: luckyman
 */

#include "buzzer.h"

#define BUZZER_PORT	GPIOA
#define BUZZER_PIN	GPIO_PIN_8


void buzzOn(void)
{
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void buzzOff(void)
{
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
}

void buzzToggle(void)
{
	HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);
}

void buzz_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC7 */
	GPIO_InitStruct.Pin = BUZZER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);
}
