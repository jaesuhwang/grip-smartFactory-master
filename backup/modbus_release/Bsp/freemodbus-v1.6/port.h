//port.h/*
/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <inttypes.h>
#include "cmsis_gcc.h"

#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   __disable_irq()
#define EXIT_CRITICAL_SECTION( )    __enable_irq()

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

//��ʱ��������ͨ�����ã���ʹ��huart1���������ָĳ�1����ʱ��ͬ��
#define Modbus_Rtu_serial huart2  //ModbusͨѶ�Ĵ���
#define Modbus_Rtu_huart_number USART2  //ModbusͨѶ�Ĵ���
#define Modbus_Rtu_IRQHandler USART2_IRQHandler //�����ж�
#define Modbus_Rtu_IRQ USART2_IRQn //��־λ

#define Modbus_Rtu_timer htim7 //modbusʹ�õĶ�ʱ��
#define Modbus_Rtu_timer_number TIM7 //modbusʹ�õĶ�ʱ��
#define Modbus_Rtu_timer_prescaler 8400-1//Ԥ����ϵ����Ҫ��ʱ50us
#define Modbus_Rtu_timer_IRQHandler TIM7_IRQHandler

#endif
