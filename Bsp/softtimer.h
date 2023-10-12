/*
 * softtimer.h
 *
 *  Created on: 2022. 2. 14.
 *      Author: lucky
 */

#ifndef INC_SOFTTIMER_H_
#define INC_SOFTTIMER_H_

#include "main.h"
#include <stdbool.h>


#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL 0
#endif

#define _HW_DEF_SW_TIMER_MAX       1// HW_SWTIMER_MAX_CH

typedef int16_t             swtimer_handle_t;


#define ONE_TIME            1
#define LOOP_TIME           2

bool swtimerInit(void);
void swtimerSet  (uint8_t TmrNum, uint32_t TmrData, uint8_t TmrMode, void (*Fnct)(void *),void *arg);
void swtimerStart(uint8_t TmrNum);
void swtimerStop (uint8_t TmrNum);
void swtimerReset(uint8_t TmrNum);
void swtimerISR(void);


int16_t  swtimerGetHandle(void);
uint32_t swtimerGetCounter(void);

#endif /* INC_SOFTTIMER_H_ */
