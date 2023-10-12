/*
 * flash.h
 *
 *  Created on: 2022. 4. 11.
 *      Author: luckyman
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f4xx_hal.h"

//extern int stm32_flash_read(uint32_t addr , uint8_t *buf, uint16_t size);
//extern int stm32_flash_read(long offset , uint8_t *buf, uint16_t size);
//extern int stm32_flash_write(long offset, const uint8_t*buf ,  uint16_t size);

extern int stm32_flash_read(long offset , uint8_t *buf, uint64_t size);
extern int stm32_flash_erase(uint32_t offset, uint64_t size);
extern int stm32_flash_write(long offset, const uint8_t* buf ,  uint64_t size);


#endif /* FLASH_H_ */
