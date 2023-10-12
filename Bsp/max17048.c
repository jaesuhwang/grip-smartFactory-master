/*
 * max17048.c
 *
 *  Created on: 2022. 4. 10.
 *      Author: luckyman
 */

#include "max17048.h"
#include "i2c.h"

#include "stm32f4xx_hal.h"

#define BAT_STAT_GPIO_Port GPIOB
#define BAT_STAT_Pin	GPIO_PIN_9

/** Private variables --------------------------------------------------------*/
static uint16_t Battery_Soc = 0; // 배터리 비율
static uint16_t Battery_Voltage = 0;// 배터리 전압
static uint8_t Send_Val_Buf[2];
static uint8_t Reg_Val_Buf[2];
/** Private function prototypes ----------------------------------------------*/
#if ENABLE_MAX17048_DEBUG
static void Display_Alarm_Info(uint8_t Alarm_Value);
#endif
static void I2C_State_Reset(void);
static void Power_On_Reset(void);
static uint16_t Read_Vcell_Update(uint32_t Block_Time);
static uint16_t Read_Soc_Update(uint32_t Block_Time);


// 배터리 비정상상태 복구
static inline void I2C_State_Reset(void)
{
  __HAL_I2C_DISABLE(&hi2c1);
  HAL_Delay(1);
  __HAL_I2C_ENABLE(&hi2c1);
}


#if ENABLE_MAX17048_DEBUG
static void Display_Alarm_Info(uint8_t Alarm_Value)
{
  if(Alarm_Value & REG_BIT0)
  {
    printf("POWER ON IC NO CONFIG.\r\n");
  }
  if(Alarm_Value & REG_BIT1)
  {
    printf("VCELL HIGH THAN ALRT.VALRTMAX.\r\n");
  }
  if(Alarm_Value & REG_BIT2)
  {
    printf("VCELL LOW THAN ALRT.VALRTMIN.\r\n");
  }
  if(Alarm_Value & REG_BIT3)
  {
    printf("VOLTAGE RESET.\r\n");
  }
  if(Alarm_Value & REG_BIT4)
  {
    printf("SOC LOW THAN CONFIG.ATHD.\r\n");
  }
  if(Alarm_Value & REG_BIT5)
  {
    printf("SOC CHANGE VALUE ARRIVE %%1.\r\n");
  }
  if(Alarm_Value & REG_BIT6)
  {
    printf("ENABLE VOLTAGE RESET ALARM.\r\n");
  }
}
#endif

// 파워 온 리셋
static void Power_On_Reset(void)
{
	HAL_StatusTypeDef hal_ok;

  // power on
  uint8_t WriteData[3] = {REG_CMD, 0x54, 0x00};
  hal_ok = HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, WriteData, 3, 100);

  /*Wait*/
  HAL_Delay(100);


  // 시작
  WriteData[0] = REG_MODE;
  WriteData[1] = 0x00;
  WriteData[2] |= REG_BIT6;

  // 절전 모드 활성화
  WriteData[2] |= REG_BIT5;
  hal_ok = HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, WriteData, 3, 100);

}

// 배터리 알람상태 지우기

static void Alarm_State_Reset(void)
{
  uint8_t RegData = 0;
  uint8_t RecData[2] = {0};
  uint8_t SendData[3] = {0};

  // 알람상태 정보 읽기
  RegData = REG_STATUS;
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
#if ENABLE_MAX17048_DEBUG
  printf("Status REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
  Display_Alarm_Info(RecData[0]);
#endif
  // 알람 정보 지우기
  SendData[0] = RegData;
  SendData[1] = (RecData[0]&(~0x7F));
  SendData[2] = RecData[1];
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, SendData, 3, 100);
#if ENABLE_MAX17048_DEBUG
  printf("Set Status REG:%02X:VAL:%02X%02X.\r\n", RegData, SendData[1], SendData[2]);
#endif
#if ENABLE_MAX17048_DEBUG
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
  printf("After Status REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
#endif

  // 구성 정보 읽기
  RegData = REG_CONFIG;
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
#if ENABLE_MAX17048_DEBUG
  printf("Config REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
#endif
  /*경보 임계값 설정 BIT0-4는 범위를 1-32%로 설정하고 10%를 설정한 다음 32-10으로 설정할 수 있습니다.*/
  RecData[1] = ((RecData[1]&0xE0)|(32-10));
  /*경보 상태 비트 BIT5 지우기*/
  RecData[1] &= (~REG_BIT5);
  SendData[0] = RegData;
  SendData[1] = RecData[0];
  SendData[2] = RecData[1];
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, SendData, 3, 100);
#if ENABLE_MAX17048_DEBUG
  printf("Set Config REG:%02X:VAL:%02X%02X.\r\n", RegData, SendData[1], SendData[2]);
#endif
#if ENABLE_MAX17048_DEBUG
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
  printf("After Config REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
#endif

  // 버전 정보 읽기
#if ENABLE_MAX17048_DEBUG
  RegData = REG_VERSION;
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
  printf("Version REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
#endif
}

//MAX17048 Vcell 전압 값 78.125uV/Vcell 읽기
static uint16_t Read_Vcell_Update(uint32_t Block_Time)
{
  Send_Val_Buf[0] = REG_V_CELL;
  if(Block_Time == 0)
  {
    if(HAL_OK != HAL_I2C_Master_Transmit_IT(&hi2c1, DEV_ADDR_WRITE, Send_Val_Buf, 1))
    {
      return Battery_Voltage;
    }
  }
  else if(Block_Time > 0)
  {
    if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, Send_Val_Buf, 1, Block_Time))
    {
      return Battery_Voltage;
    }
    if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, Reg_Val_Buf, 2, Block_Time))
    {
      return Battery_Voltage;
    }

    Battery_Voltage = (Reg_Val_Buf[0]<<8)+Reg_Val_Buf[1];
    Battery_Voltage = (uint16_t)(((double)Battery_Voltage * 78.125)/1000000);
    return Battery_Voltage;
  }
  return Battery_Voltage;
}


// MAX17048 1%/256의 SOC(배터리 충전 상태) 값 읽기
static uint16_t Read_Soc_Update(uint32_t Block_Time)
{
  Send_Val_Buf[0] = REG_SOC;
  if(Block_Time == 0)
  {
    if(HAL_OK != HAL_I2C_Master_Transmit_IT(&hi2c1, DEV_ADDR_WRITE, Send_Val_Buf, 1))
    {
      return Battery_Soc;
    }
  }
  else if(Block_Time > 0)
  {
    if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, Send_Val_Buf, 1, Block_Time))
    {
      I2C_State_Reset();
      return Battery_Soc;
    }
    if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, Reg_Val_Buf, 2, Block_Time))
    {
      return Battery_Soc;
    }
    uint16_t Battery_Soc_Temp = (Reg_Val_Buf[0]<<8) + Reg_Val_Buf[1];
    Battery_Soc_Temp /= 256;
    Battery_Soc = Battery_Soc_Temp > 100?100:Battery_Soc_Temp;
    return Battery_Soc;
  }
  return Battery_Soc;
}


// I2C 전송 완료 콜백
void Voltameter_Port_I2C_MasterTxCpltCallback(void)
{
  /*TODO:READ*/
  HAL_I2C_Master_Receive_IT(&hi2c1, DEV_ADDR_READ, Reg_Val_Buf, 2);
}

// I2C 수신 완료 콜백
void Voltameter_Port_I2C_MasterRxCpltCallback(void)
{
  /*TODO:UPDATE VAL*/
  if(Send_Val_Buf[0] == REG_V_CELL)
  {
    uint16_t Battery_Voltage_Temp = (Reg_Val_Buf[0]<<8)+Reg_Val_Buf[1];
    Battery_Voltage_Temp = (uint16_t)(((double)Battery_Voltage_Temp * 78.125)/1000000);
    Battery_Voltage = Battery_Voltage_Temp;
  }
  else if(Send_Val_Buf[0] == REG_SOC)
  {
    uint16_t Battery_Soc_Temp = (Reg_Val_Buf[0]<<8) + Reg_Val_Buf[1];
    Battery_Soc_Temp /= 256;
    Battery_Soc = Battery_Soc_Temp > 100?100:Battery_Soc_Temp;
  }
}

// 배터리 전원 및 전압 정보 얻기
uint16_t Voltameter_Port_Get_Battery_Electric_Quantity(void)
{
  Read_Soc_Update(10);
  Read_Vcell_Update(0);
  return Battery_Soc;
}


// 배터리 충전 상태 가져오기
uint8_t Voltameter_Port_Get_Battery_Charge_State(void)
{

  if(HAL_GPIO_ReadPin(BAT_STAT_GPIO_Port, BAT_STAT_Pin) == GPIO_PIN_SET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void Voltameter_Port_OnOff_Sleep(uint8_t Sleep_En)
{
  uint8_t RegData = 0;
  uint8_t RecData[2] = {0};
  uint8_t SendData[3] = {0};

  // 구성 정보 읽기
  RegData = REG_CONFIG;
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);
#if ENABLE_MAX17048_DEBUG
  printf("Config REG:%02X:VAL:%02X%02X.\r\n", RegData, RecData[0], RecData[1]);
#endif

  SendData[1] = RegData;
  SendData[1] = RecData[0];
  SendData[2] = RecData[1];

  if(Sleep_En == 1)
  {
	  /* 절전 모드로 이동 config.sleep = 1*/
    SendData[1] |= REG_BIT7;
  }
  else
  {
	  /*최대 절전 모드 종료 config.sleep = 0*/
    SendData[1] &= ~REG_BIT7;
  }

  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, SendData, 3, 100);
}



// 배터리 모니터링 인터페이스 초기화
void Voltameter_Port_Init(void)
{
  /*上电复位MAX17048*/
  Power_On_Reset();

  /*报警复位*/
  Alarm_State_Reset();
}



// 배터리 보상에 대해
// 칩에는 온도 보상 영역이 있지만 온도 감지 영역은 배터리 온도이므로 허용되지 않을 수 있습니다.
#define RCOMP0                  0x97/**< 温度补偿值*/
static int8_t Rcomp = RCOMP0;
/**
  ******************************************************************
  * @brief   온도 보상
  * @param   [in]Temperatur 배터리 온도.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-16
  ******************************************************************
  */
void MAX17048_Driver_Compensate(float Temperature)
{
  uint8_t RegData = 0;
  uint8_t RecData[2] = {0};
  uint8_t SendData[3] = {0};

  if(Temperature > 20)
  {
    Rcomp = (int8_t)(RCOMP0 + (Temperature - 20) * -0.5);
  }
  else
  {
    Rcomp = (int8_t)(RCOMP0 + (Temperature - 20) * -5.0);
  }

  //구성데이터 읽기
  RegData = REG_CONFIG;
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, &RegData, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, DEV_ADDR_READ, RecData, 2, 100);

  // 온도 보상 RCOMP 설정
  SendData[0] = RegData;
  SendData[1] = (uint8_t)Rcomp;
  SendData[2] = RecData[1];
  HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDR_WRITE, SendData, 3, 100);
}


// 90%-100% 배터리 영역에 대한 수동 보정


// 충전시간 기록
typedef struct rectification
{
  uint32_t Start_Time_Sec;
  // uint32_t End_Time_Sec;
  // uint32_t Next_Time_Sec;
  // uint32_t Elapsed;
  uint16_t Compensation_Soc;
}RECTIFICATION_Typedef_t;

/*디바이스 기준 측정 전력*/
#define DEVICE_REF_POWER        150   /**< 150mA/h*/
#define BATTERY_CAPACITY        1000  /**< 1000mAh*/
#define CAN_USE_MIN_TOTAL       (BATTERY_CAPACITY*60/DEVICE_REF_POWER)


//MAX17048의 SOC(Battery state of charge) 값 수정 및 보상
//static uint16_t Soc_Compensation(uint16_t Current_Soc)
//{
//  static uint16_t last_soc_val = 0;
//  uint16_t range_num = Current_Soc - 90;
//
//  // 보상이 필요 하지 않음
//  if(Current_Soc == 100)
//  {
//    return Current_Soc;
//  }
//
//  /*충전 중 보상동작 수행*/
//  if(last_soc_val != Current_Soc && Current_Soc >= 90 && MAX17048_Driver_Get_Battery_Charge_State() == 1)
//  {
//    E_Record[range_num].Start_Time_Sec = Timer_Port_Get_Current_Time(TIMER_SEC);
//    E_Record[range_num].Compensation_Soc = Current_Soc;
//    last_soc_val = Current_Soc;
//    return Current_Soc;
//  }
//
//  if(last_soc_val == Current_Soc && Current_Soc >= 90 && MAX17048_Driver_Get_Battery_Charge_State() == 1)
//  {
//	  /*배터리 값은 90% 영역, 보상 */
//    if(range_num < 2)
//    {
//      if((Timer_Port_Get_Current_Time(TIMER_SEC) - E_Record[range_num].Start_Time_Sec) > 60*30)
//      {
//        E_Record[range_num].Start_Time_Sec = Timer_Port_Get_Current_Time(TIMER_SEC);
//        E_Record[range_num].Compensation_Soc += 1;
//      }
//    }
//    else
//    {
//      uint32_t ref_time = E_Record[range_num].Start_Time_Sec - E_Record[range_num-1].Start_Time_Sec;
//      uint32_t elapsed_time = Timer_Port_Get_Current_Time(TIMER_SEC) - E_Record[range_num].Start_Time_Sec;
//      if(elapsed_time > ref_time && elapsed_time > 2*ref_time)
//      {
//        E_Record[range_num].Start_Time_Sec = Timer_Port_Get_Current_Time(TIMER_SEC);
//        E_Record[range_num-1].Start_Time_Sec = E_Record[range_num].Start_Time_Sec - ref_time;
//        E_Record[range_num].Compensation_Soc += 1;
//      }
//    }
//    E_Record[range_num].Compensation_Soc = E_Record[range_num].Compensation_Soc >= 100?100:E_Record[range_num].Compensation_Soc;
//    return E_Record[range_num].Compensation_Soc;
//  }
//
//  /*파워 값은 90% 영역에 있으며, 방전 파워는 제어됩니다*/
//  if(Current_Soc >= 90 && MAX17048_Driver_Get_Battery_Charge_State() == 0)
//  {
//    // 수정값
//    if(E_Record[range_num].Compensation_Soc > Current_Soc)
//    {
//      // 전력 소모 시간 계산
//      uint32_t elapsed_time_min = (Timer_Port_Get_Current_Time(TIMER_SEC) - E_Record[range_num].Start_Time_Sec)/60;
//      float pow = (float)elapsed_time_min/(float)CAN_USE_MIN_TOTAL;
//      E_Record[range_num].Compensation_Soc = (uint16_t)((float)E_Record[range_num].Compensation_Soc - (100.f * pow));
//      E_Record[range_num].Compensation_Soc = E_Record[range_num].Compensation_Soc < Current_Soc?Current_Soc:E_Record[range_num].Compensation_Soc;
//      return E_Record[range_num].Compensation_Soc;
//    }
//  }
//  return Current_Soc;
//}

