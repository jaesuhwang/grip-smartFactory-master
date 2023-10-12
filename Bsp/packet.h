/*
 * packet.h
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#ifndef PACKET_H_
#define PACKET_H_

#include "ringbuf.h"


extern uint8_t uart_flag;
extern ringbuffer_t rxrb;

extern uint8_t write_flash_temp[16];
extern uint8_t read_flash_temp[16];

extern void uartRingBuff_init(void);
extern void uart2_receive_func(void);

#endif /* PACKET_H_ */
