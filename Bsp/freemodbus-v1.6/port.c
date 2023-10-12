//port.c
#include "mb.h"
#include "mbport.h"

#include "main.h"
#include "tdma_c.h"
// 十路输入寄存器
#define REG_INPUT_SIZE  400
uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];


// 十路保持寄存器
#define REG_HOLD_SIZE   400
uint16_t REG_HOLD_BUF[REG_HOLD_SIZE];


// 十路线圈
#define REG_COILS_SIZE 10
uint8_t REG_COILS_BUF[REG_COILS_SIZE] = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1};


// 十路离散量
#define REG_DISC_SIZE  10
uint8_t REG_DISC_BUF[REG_DISC_SIZE] = {1,1,1,1,0,0,0,0,1,1};


/// CMD4命令处理回调函数
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    USHORT usRegIndex = usAddress - 1;

    // 非法检测
    if((usRegIndex + usNRegs) > REG_INPUT_SIZE)
    {
        return MB_ENOREG;
    }

    // 循环读取
    while( usNRegs > 0 )
    {
        *pucRegBuffer++ = ( unsigned char )( REG_INPUT_BUF[usRegIndex] >> 8 );
        *pucRegBuffer++ = ( unsigned char )( REG_INPUT_BUF[usRegIndex] & 0xFF );
        usRegIndex++;
        usNRegs--;
    }

    // 模拟输入寄存器被改变
    //for(usRegIndex = 0; usRegIndex < REG_INPUT_SIZE; usRegIndex++)
    //{
    //    REG_INPUT_BUF[usRegIndex]++;
    //}

    return MB_ENOERR;
}
extern ModBus_t modeBusSetting;
extern uint8_t MODEBUS_SET_REQ;
extern Node_sleep_time_t node_id[];
/// CMD6、3、16命令处理回调函数
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    USHORT usRegIndex = usAddress - 1;
    uint8_t check = 0;
    // 非法检测
    if((usRegIndex + usNRegs) > REG_HOLD_SIZE)
    {
        return MB_ENOREG;
    }

    // 写寄存器
    if(eMode == MB_REG_WRITE)
    {
        while( usNRegs > 0 )
        {
            REG_HOLD_BUF[usRegIndex] = (pucRegBuffer[0] << 8) | pucRegBuffer[1];
            if (usRegIndex == 0x0101 )
            {
            	modeBusSetting.addr =REG_HOLD_BUF[usRegIndex];
            	check = 1;
            }
            if ( usRegIndex == 0x0102 )
            {
            	modeBusSetting.baudRate =REG_HOLD_BUF[usRegIndex];
            	check = 1;
            }

            if ( usRegIndex == 0x0104 )
            {

            	uint8_t nodeidVal = (uint8_t)REG_HOLD_BUF[0x0103];
            	if ( nodeidVal > 0 )
            	{
            		node_id[nodeidVal-1].min = (uint8_t)((REG_HOLD_BUF[0x0104] & 0xFF00)>> 8);
					node_id[nodeidVal-1].sec = (uint8_t)(REG_HOLD_BUF[0x0104]) & 0xFF;
            	}
            }
            pucRegBuffer += 2;
            usRegIndex++;
            usNRegs--;
        }
        if ( check == 1)
        	MODEBUS_SET_REQ = 1;
    }

    // 读寄存器
    else
    {
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( REG_HOLD_BUF[usRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( REG_HOLD_BUF[usRegIndex] & 0xFF );
            usRegIndex++;
            usNRegs--;
        }
    }

    return MB_ENOERR;
}

/// CMD1、5、15命令处理回调函数
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    USHORT usRegIndex   = usAddress - 1;
    UCHAR  ucBits       = 0;
    UCHAR  ucState      = 0;
    UCHAR  ucLoops      = 0;

    // 非法检测
    if((usRegIndex + usNCoils) > REG_COILS_SIZE)
    {
        return MB_ENOREG;
    }

    if(eMode == MB_REG_WRITE)
    {
        ucLoops = (usNCoils - 1) / 8 + 1;
        while(ucLoops != 0)
        {
            ucState = *pucRegBuffer++;
            ucBits  = 0;
            while(usNCoils != 0 && ucBits < 8)
            {
                REG_COILS_BUF[usRegIndex++] = (ucState >> ucBits) & 0X01;
                usNCoils--;
                ucBits++;
            }
            ucLoops--;
        }
    }
    else
    {
        ucLoops = (usNCoils - 1) / 8 + 1;
        while(ucLoops != 0)
        {
            ucState = 0;
            ucBits  = 0;
            while(usNCoils != 0 && ucBits < 8)
            {
                if(REG_COILS_BUF[usRegIndex])
                {
                    ucState |= (1 << ucBits);
                }
                usNCoils--;
                usRegIndex++;
                ucBits++;
            }
            *pucRegBuffer++ = ucState;
            ucLoops--;
        }
    }

    return MB_ENOERR;
}

/// CMD2命令处理回调函数
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    USHORT usRegIndex   = usAddress - 1;
    UCHAR  ucBits       = 0;
    UCHAR  ucState      = 0;
    UCHAR  ucLoops      = 0;

    // 非法检测
    if((usRegIndex + usNDiscrete) > REG_DISC_SIZE)
    {
        return MB_ENOREG;
    }

    ucLoops = (usNDiscrete - 1) / 8 + 1;
    while(ucLoops != 0)
    {
        ucState = 0;
        ucBits  = 0;
        while(usNDiscrete != 0 && ucBits < 8)
        {
            if(REG_DISC_BUF[usRegIndex])
            {
                ucState |= (1 << ucBits);
            }
            usNDiscrete--;
            usRegIndex++;
            ucBits++;
        }
        *pucRegBuffer++ = ucState;
        ucLoops--;
    }

    // 模拟离散量输入被改变
    for(usRegIndex = 0; usRegIndex < REG_DISC_SIZE; usRegIndex++)
    {
        REG_DISC_BUF[usRegIndex] = !REG_DISC_BUF[usRegIndex];
    }

    return MB_ENOERR;
}
