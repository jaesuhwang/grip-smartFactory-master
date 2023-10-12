/*
 * packet.c
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#include "packet.h"
#include "tdma_c.h"
#include "flash.h"

#include "cc1120.h"
#include "usart.h"

#include "fs.h"

#define RX_BUFFERSIZE 128

uint8_t uart_flag;

uint32_t used;

ringbuffer_t rxrb;
uint8_t rxdata[RX_BUFFERSIZE];
uint8_t cmdBuffer[RX_BUFFERSIZE];
extern Node_sleep_time_t node_id[10];
extern USER_FLASH_USER_t userFlashSetting;

static uint16_t pktLen = 0;

uint8_t write_flash_temp[16];
uint8_t read_flash_temp[16];

typedef struct
{
	uint8_t device_addr;
	uint8_t baud_rate;
	uint8_t magic_number;
}flash_tag_t;


flash_tag_t user_flash;

void uartRingBuff_init(void) {
	ringbuf_init(&rxrb, rxdata, RX_BUFFERSIZE);
}

int recvUartPkt() {
	uint8_t c;
	uint16_t i = 0;
	//uint32_t used;

	for (i = 0; i < RX_BUFFERSIZE; i++)
		cmdBuffer[i] = 0;

	used = ringbuf_used(&rxrb);

	for (i = 0; i < used; i++) {
		c = ringbuf_readone(&rxrb);
		if (c == 0x03)
		{
			pktLen = i;
			return pktLen;
		} else
			cmdBuffer[i] = c;
	}
	pktLen = 0;
	return -1;
}
uint8_t readTest_1[120];
uint8_t uartPcBuffer[32];

typedef struct
{
	GPIO_TypeDef  *Port ;
	uint16_t Pin;
}ExGPIO_t;

ExGPIO_t ExGPIO[6] =
{
		{GPIOB, GPIO_PIN_1},
		{GPIOC, GPIO_PIN_6},
		{GPIOB, GPIO_PIN_15},
		{GPIOB, GPIO_PIN_14},
		{GPIOB, GPIO_PIN_13},
		{GPIOB, GPIO_PIN_12}
};

void exGpioPortDirInit(uint8_t idxPort , uint8_t portSts)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if ( portSts == '0') // no pull input
	{
		GPIO_InitStruct.Pin = ExGPIO[idxPort].Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ExGPIO[idxPort].Port, &GPIO_InitStruct);
	}
	else if ( portSts == '1')// pull down input
	{
		GPIO_InitStruct.Pin = ExGPIO[idxPort].Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(ExGPIO[idxPort].Port, &GPIO_InitStruct);
	}
	else if ( portSts == '2')// pull up input
	{
		GPIO_InitStruct.Pin = ExGPIO[idxPort].Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(ExGPIO[idxPort].Port, &GPIO_InitStruct);
	}
	else if ( portSts == '3')// output
	{
		GPIO_InitStruct.Pin = ExGPIO[idxPort].Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(ExGPIO[idxPort].Port, &GPIO_InitStruct);
	}
}



int handleUart2Pkt() {
	#if 0 
	uint8_t sleep_minute_h;
	uint8_t sleep_minute_l;
	uint8_t sleep_second_h;
	uint8_t sleep_second_l;
	#endif 
	uint8_t id_buff;
	uint8_t temp_sec;
	uint8_t temp_min;


	switch (cmdBuffer[0]) 
	{
		case 0x02:
		{
//			if ( cmdBuffer[1] == 'T')
//			{
//				id_buff = cmdBuffer[1] - '0';
//				temp_min = (cmdBuffer[2] - '0')*10 + (cmdBuffer[3] - '0');
//				temp_sec = (cmdBuffer[4] - '0')*10 + (cmdBuffer[5] - '0');
//				node_id[id_buff-1].sec = temp_sec;
//				node_id[id_buff-1].min = temp_min;
//				#if 0
//					write_flash_temp[id_buff*2] = temp_min;
//					write_flash_temp[id_buff*2+1] = temp_sec;
//					stm32_flash_write(0x80000 , (uint8_t*)&write_flash_temp, 16);
//					stm32_flash_read(0x80000 , (uint8_t*)&readTest_1, 16);
//				#endif
//			}
			if ( cmdBuffer[1] == 'F' )
			{
				if( cmdBuffer[5] == '.')
				{
					fs_t fs;
					uint16_t dec[2];
					float tmpFreq;
					dec[0] = (cmdBuffer[2]-0x30) *100 + (cmdBuffer[3]-'0') * 10 + (cmdBuffer[4]-'0') * 1 ;
					dec[1] = (cmdBuffer[6]-0x30) *100 + (cmdBuffer[7]-'0') * 10 + (cmdBuffer[8]-'0') * 1 ;
					sprintf(uartPcBuffer, "%d.%d\r\n", dec[0], dec[1]);
					HAL_UART_Transmit(&huart4, uartPcBuffer, strlen(uartPcBuffer), 100);

					tmpFreq =dec[0] + (float)dec[1]*0.001f;
					//sprintf(uartPcBuffer, "%3f\r\n", tmpFreq);
					//HAL_UART_Transmit(&huart1, uartPcBuffer, strlen(uartPcBuffer), 100);
					CC1120_setFrequency(tmpFreq);

					if (fsFileOpen(&fs, "mtr_pos") == true)
					{
						userFlashSetting.wirelessFreq = tmpFreq;
						fsFileWrite(&fs, &userFlashSetting, sizeof(USER_FLASH_USER_t));
						fsFileClose(&fs);
					}
				}
			}
			else if ( cmdBuffer[1] == 'S') // set gpio in out sts port dir
			{
				if ( cmdBuffer[2] == 'I') // set port dir
				{
					for ( int i = 0 ; i < 6; i++ )
					{
						exGpioPortSts[i] =cmdBuffer[3+i];
						exGpioPortDirInit(i, exGpioPortSts[i]);
					}
				}
				else if ( cmdBuffer[2] == 'P') // set high low
				{
					for ( int i = 0 ; i < 6; i++ )
						HAL_GPIO_WritePin(ExGPIO[i].Port, ExGPIO[i].Pin, cmdBuffer[3+i]-0x30);
				}

			}
			else if ( cmdBuffer[1] == 'G') //get
			{
				uint8_t tmpGpioRead[6];
				if ( cmdBuffer[2] == 'I') // get port dir
				{
					sprintf(uartPcBuffer , "%c%d%d%d%d%d%d%c", 0x02, exGpioPortSts[0], exGpioPortSts[1], exGpioPortSts[2], exGpioPortSts[3], exGpioPortSts[4], exGpioPortSts[5], 0x03);
					HAL_UART_Transmit(&huart4, uartPcBuffer, strlen(uartPcBuffer), 100);
				}
				else if ( cmdBuffer[2] == 'P') // high low
				{
					for ( int i = 0 ; i < 6; i++ )
					{
						tmpGpioRead[i] = HAL_GPIO_ReadPin(ExGPIO[i].Port, ExGPIO[i].Pin);
					}
					sprintf(uartPcBuffer , "%c%d%d%d%d%d%d%c", 0x02, tmpGpioRead[0], tmpGpioRead[1], tmpGpioRead[2], tmpGpioRead[3], tmpGpioRead[4], tmpGpioRead[5], 0x03);
					HAL_UART_Transmit(&huart4, uartPcBuffer, strlen(uartPcBuffer), 100);
				}
			}
			else if ( cmdBuffer[1] == 'L' ) // LED
			{
				if ( cmdBuffer[2] == '0') // LED1
				{
					if ( cmdBuffer[3] == '0') // oFF
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
					else if ( cmdBuffer[3] == '1')// on
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
				}
				else if (cmdBuffer[2] == '1')
				{
					if ( cmdBuffer[3] == '0') // oFF
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
					else if ( cmdBuffer[3] == '1')// on
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
				}
			}
			else if ( cmdBuffer[1] == 'B' ) // BUZZ
			{
				if ( cmdBuffer[2] == '0') //OFF
				{
					// PWM OFF
				}
				else if (cmdBuffer[2] == '1')//ON
				{
					// PWM ON
				}
			}
			else if ( cmdBuffer[1] == 'D' ) // DBG
			{
				if ( cmdBuffer[2] == '0') // OFF
				{
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
				}
				else if (cmdBuffer[2] == '1') // ON
				{
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
				}
			}
			else if ( cmdBuffer[1] == 'R' )
			{
				if ( cmdBuffer[2] == 'S' && cmdBuffer[3] == 'T' )
				{
					//  software reset

				}
			}
			else if ( cmdBuffer[1] == 'I')
			{
				uint8_t gpioDet;
				gpioDet = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);

				if ( cmdBuffer[2] == 'E' && cmdBuffer[3] == 'T' )
				{
					sprintf(uartPcBuffer , "%c%d%c", 0x02,  gpioDet, 0x03);
					HAL_UART_Transmit(&huart4, uartPcBuffer, strlen(uartPcBuffer), 100);
					HAL_UART_Transmit(&huart1, uartPcBuffer, strlen(uartPcBuffer), 100);
				}
			}
			else
			{
				id_buff = cmdBuffer[1] - '0';
				temp_min = (cmdBuffer[2] - '0')*10 + (cmdBuffer[3] - '0');
				temp_sec = (cmdBuffer[4] - '0')*10 + (cmdBuffer[5] - '0');
				node_id[id_buff-1].sec = temp_sec;
				node_id[id_buff-1].min = temp_min;
				#if 0
					write_flash_temp[id_buff*2] = temp_min;
					write_flash_temp[id_buff*2+1] = temp_sec;
					stm32_flash_write(0x80000 , (uint8_t*)&write_flash_temp, 16);
					stm32_flash_read(0x80000 , (uint8_t*)&readTest_1, 16);
				#endif

			}
		}
		break;
	case 'S': //gpio port setting

		// handleSetFunc();
		break;
	case 'G':
		//handleGetFunc();
		break;
	case 'C':
		//conf_flag = 1;
		//handleCTX();
		break;
	case 'R':
		//handleReset();
		break;

	case 'M':
		switch (cmdBuffer[1]) {
		case 'C':
//                    HAL_UART_Transmit(&hlpuart1, "C", 1, 100);
			break;
		case 'E':
//                    HAL_UART_Transmit(&hlpuart1, "E", 1, 100);
			break;
		}

	default:

		break;
	}

	return 0;
}

void uart2_receive_func(void) {
	if (uart_flag == 1)
	{
		if (recvUartPkt() > 0)
		{
			handleUart2Pkt();
			uart_flag = 0;
		}
	}
}

