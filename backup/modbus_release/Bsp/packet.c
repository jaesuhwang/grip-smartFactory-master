/*
 * packet.c
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#include <stdio.h>
#include <stdlib.h>
#include "packet.h"
#include "tdma_c.h"
#include "flash.h"
#define RX_BUFFERSIZE 128

uint8_t uart_flag;

uint32_t used;

ringbuffer_t rxrb;
uint8_t rxdata[RX_BUFFERSIZE];
uint8_t cmdBuffer[RX_BUFFERSIZE];
extern Node_sleep_time_t node_id[10];

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
	uint8_t c, i = 0, l = 0;
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
	case 0x02: // TEST
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
		break;
	case 'S':
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

