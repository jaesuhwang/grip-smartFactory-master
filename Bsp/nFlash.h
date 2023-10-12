/*
 * nFlash.h
 *
 *  Created on: 2023. 10. 10.
 *      Author: luckyman
 */

#ifndef NFLASH_H_
#define NFLASH_H_

#include "main.h"
#include <stdbool.h>



bool flashInit(void);
bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length);




#endif /* NFLASH_H_ */
