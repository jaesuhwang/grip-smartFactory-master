//porttime.c
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

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "tim.h"
#include <stdio.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    Modbus_Rtu_timer.Instance = Modbus_Rtu_timer_number;
    Modbus_Rtu_timer.Init.Prescaler = Modbus_Rtu_timer_prescaler;								// 50us��һ����
    Modbus_Rtu_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	Modbus_Rtu_timer.Init.Period = usTim1Timerout50us - 1;					// usTim1Timerout50us * 50��Ϊ��ʱ�����ʱ��
    Modbus_Rtu_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    Modbus_Rtu_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&Modbus_Rtu_timer) != HAL_OK)
    {
        return FALSE;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&Modbus_Rtu_timer, &sClockSourceConfig) != HAL_OK)
    {
        return FALSE;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&Modbus_Rtu_timer, &sMasterConfig) != HAL_OK)
    {
        return FALSE;
    }

	__HAL_TIM_CLEAR_FLAG(&Modbus_Rtu_timer, TIM_FLAG_UPDATE);              // �����һ�¶�ʱ�����жϱ��,��ֹʹ���жϺ�ֱ�Ӵ����ж�
    __HAL_TIM_ENABLE_IT(&Modbus_Rtu_timer, TIM_IT_UPDATE);					// ʹ�ܶ�ʱ�������ж�
    return TRUE;
}

inline void vMBPortTimersEnable(  )
{
    __HAL_TIM_SET_COUNTER(&Modbus_Rtu_timer, 0);		// ��ռ�����
    __HAL_TIM_ENABLE(&Modbus_Rtu_timer);				// ʹ�ܶ�ʱ��
}

inline void vMBPortTimersDisable(  )
{
    __HAL_TIM_DISABLE(&Modbus_Rtu_timer);				// ���ܶ�ʱ��
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

/// ��ʱ��4�жϷ������
void Modbus_Rtu_timer_IRQHandler(void)
{
    if(__HAL_TIM_GET_FLAG(&Modbus_Rtu_timer, TIM_FLAG_UPDATE))			// �����жϱ�Ǳ���λ
    {
        __HAL_TIM_CLEAR_FLAG(&Modbus_Rtu_timer, TIM_FLAG_UPDATE);		// ����жϱ��
        prvvTIMERExpiredISR();								// ֪ͨmodbus3.5���ַ��ȴ�ʱ�䵽
    }
}
