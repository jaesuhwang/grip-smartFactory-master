/*
 * pinDef.h
 *
 *  Created on: 2022. 3. 23.
 *      Author: luckyman
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "stm32f4xx_hal.h"

extern long ch2hex(char *hex, uint8_t size);
extern void hex2ch(uint8_t hex, char *chBuff);

//extern config_data configs;
//int read_configuration_data(config_data *c);



#endif /* GLOBAL_H_ */

