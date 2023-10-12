//portserial.c
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

#include "port.h"
#include "usart.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if(xRxEnable)
    {
		//
		// ���ʹ����485����оƬ,��ô�ڴ˴���485����Ϊ����ģʽ
		//
		
		// MAX485_SetMode(MODE_RECV);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);


        //GPIOA->BSRR = GPIO_PIN_0 | GPIO_PIN_1;
		

        __HAL_UART_ENABLE_IT(&Modbus_Rtu_serial, UART_IT_RXNE);		// ʹ�ܽ��շǿ��ж�
    }
    else
    {
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        __HAL_UART_DISABLE_IT(&Modbus_Rtu_serial, UART_IT_RXNE);		// ���ܽ��շǿ��ж�
    }

    if(xTxEnable)
    {
		//
		// ���ʹ����485����оƬ,��ô�ڴ˴���485����Ϊ����ģʽ
		//
		
		// MAX485_SetMode(MODE_SENT);
    	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);


		
        __HAL_UART_ENABLE_IT(&Modbus_Rtu_serial, UART_IT_TXE);			// ʹ�ܷ���Ϊ���ж�
    }
    else
    {
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        __HAL_UART_DISABLE_IT(&Modbus_Rtu_serial, UART_IT_TXE);		// ���ܷ���Ϊ���ж�
    }
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    Modbus_Rtu_serial.Instance = Modbus_Rtu_huart_number;
    Modbus_Rtu_serial.Init.BaudRate = ulBaudRate;
    Modbus_Rtu_serial.Init.StopBits = UART_STOPBITS_1;
    Modbus_Rtu_serial.Init.Mode = UART_MODE_TX_RX;
    Modbus_Rtu_serial.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    Modbus_Rtu_serial.Init.OverSampling = UART_OVERSAMPLING_16;
    switch(eParity)
    {
    // ��У��
    case MB_PAR_ODD:
        Modbus_Rtu_serial.Init.Parity = UART_PARITY_ODD;
        Modbus_Rtu_serial.Init.WordLength = UART_WORDLENGTH_9B;			// ����żУ������λΪ9bits
        break;

    // żУ��
    case MB_PAR_EVEN:
        Modbus_Rtu_serial.Init.Parity = UART_PARITY_EVEN;
        Modbus_Rtu_serial.Init.WordLength = UART_WORDLENGTH_9B;			// ����żУ������λΪ9bits
        break;

    // ��У��
    default:
        Modbus_Rtu_serial.Init.Parity = UART_PARITY_NONE;
        Modbus_Rtu_serial.Init.WordLength = UART_WORDLENGTH_8B;			// ����żУ������λΪ8bits
        break;
    }
    return HAL_UART_Init(&Modbus_Rtu_serial) == HAL_OK ? TRUE : FALSE;
}

BOOL xMBPortSerialPutByte( CHAR ucByte )
{
    Modbus_Rtu_huart_number->DR = ucByte;
    return TRUE;
}

BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = (Modbus_Rtu_huart_number->DR & (uint16_t)0x00FF);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

void Modbus_Rtu_IRQHandler(void)//�����жϽ��շ���
{
#if 0
	//if(__HAL_UART_GET_IT_SOURCE(&Modbus_Rtu_serial, UART_IT_RXNE)!= RESET)
	//{
		__HAL_UART_CLEAR_FLAG(&Modbus_Rtu_serial,UART_IT_RXNE);
	//	prvvUARTRxISR();//�����ж�
	}
	if(__HAL_UART_GET_IT_SOURCE(&Modbus_Rtu_serial, UART_IT_TXE)!= RESET) 
	{
		__HAL_UART_CLEAR_FLAG(&Modbus_Rtu_serial,UART_IT_TXE);
		prvvUARTTxReadyISR();//�����ж�
	}
#endif

	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&Modbus_Rtu_serial, UART_FLAG_RXNE);
    uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&Modbus_Rtu_serial, UART_IT_RXNE);
  
    if((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        prvvUARTRxISR();//�����ж�
        __HAL_UART_CLEAR_PEFLAG(&Modbus_Rtu_serial);    
        return;
    }
    
    if((__HAL_UART_GET_FLAG(&Modbus_Rtu_serial, UART_FLAG_TXE) != RESET) &&(__HAL_UART_GET_IT_SOURCE(&Modbus_Rtu_serial, UART_IT_TXE) != RESET)) {
		prvvUARTTxReadyISR();//�����ж�
        return ;
    }

  	// HAL_NVIC_ClearPendingIRQ(Modbus_Rtu_IRQ);
  	HAL_UART_IRQHandler(&Modbus_Rtu_serial);
}
