/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#if   (MODBUS_ENABLE==1)
	#include "tim.h"
#endif
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "max17048.h"
#include "flash.h"
#include "tdma_c.h"
#include "packet.h"

#include "nFlash.h"
#include "fs.h"

#if (MODBUS_ENABLE==1)
  #include "mb.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#if (MODBUS_ENABLE==1)
  uint8_t MODEBUS_SET_REQ;
#endif
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char* p, int len)
{
	HAL_UART_Transmit(&huart1, p, len, 10);
	return len;
}

uint16_t mac_new_data(int16_t slot);
volatile int16_t mac_new_dataslot;

uint16_t mac_new_data(int16_t slot) {
    mac_new_dataslot = slot;
    // return 1 to wake the cpu up
    return 1;
}


#if (MODBUS_ENABLE==1)
  extern uint16_t REG_HOLD_BUF[];
  ModBus_t modeBusSetting;
#endif

void read_data_check();
void write_data_check();


ModBus_t fsModeBVus;
USER_FLASH_USER_t userFlashSetting;


uint8_t exGpioPortSts[6];

/* USER CODE END 0 */

/**
  * @brief  The application entry point. 
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //uint16_t bat;
  //HAL_StatusTypeDef hal_ok;
  fs_t fs;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

#if (MODBUS_ENABLE==1)
  stm32_flash_read(0xC00A0, (uint8_t*)&modeBusSetting, sizeof(ModBus_t));
  read_data_check();
#endif



#if (MODBUS_ENABLE==1)
  REG_HOLD_BUF[0x0101] = modeBusSetting.addr;
  REG_HOLD_BUF[0x0102] = modeBusSetting.baudRate;
#endif

#if 1

 

#endif 
  //stm32_flash_read(0x80000 , &readTest, 4);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
#if   (MODBUS_ENABLE==1)
  MX_TIM7_Init();
  MX_USART2_UART_Init();
#endif  
  
  flashInit();
  fsInit();

#if (MODBUS_ENABLE==1)
  if ( modeBusSetting.baudRate == 0 )
  {
	  eMBInit(MB_RTU, modeBusSetting.addr , 0 , 9600, MB_PAR_NONE);
  }
  else if (modeBusSetting.baudRate == 1 )
  {
	  eMBInit(MB_RTU, modeBusSetting.addr , 0 , 14400, MB_PAR_NONE);
  }
  else if (modeBusSetting.baudRate == 2 )
  {
	  eMBInit(MB_RTU, modeBusSetting.addr , 0 , 19200, MB_PAR_NONE);
  }
  else if (modeBusSetting.baudRate == 3 )
  {
	  eMBInit(MB_RTU, modeBusSetting.addr , 0 , 115200, MB_PAR_NONE);
  }

  eMBEnable();
#endif

  /* USER CODE BEGIN 2 */

  uartRingBuff_init();

  //Voltameter_Port_Init();
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);

  if (fsFileOpen(&fs, "userSet") == true)
  {
	  fsFileRead(&fs, (uint8_t*)&userFlashSetting, sizeof(USER_FLASH_USER_t));

	  fsFileRewind(&fs);
	  fsFileClose(&fs);

//        cliPrintf("R]Rear down limit:%ld \r\n",motor_sts.MotorPos[1].motor_end_rear_ps);
  }
  else
  {
	  //cliPrintf("mtr_pos open faile\r\n");
  }
//  	  if (fsFileOpen(&fs, "mtr_pos") == true)
//          {
//  		  	  fsModeBVus.addr = 1;
//  		  	  fsModeBVus.baudRate =3535;
//  		  	  fsModeBVus.magicNum =2;
//
//              fsFileWrite(&fs, &fsModeBVus, sizeof(ModBus_t));
//              fsFileClose(&fs);
//          }
//    if ( motor_sts.MotorPos[0].motor_start_ps == 0 && motor_sts.MotorPos[0].motor_end_ps == 0 &&
//        motor_sts.MotorPos[1].motor_start_ps == 0 && motor_sts.MotorPos[1].motor_end_ps == 0 )
//    {
//        if (fsFileOpen(&fs, "mtr_pos") == true)
//        {
//            landing_LeftUpLimit        = motor_sts.MotorPos[0].motor_start_ps   = 10000;                                                           //++ Storing variables on respective array indexes
//            landing_LeftDownLimit      = motor_sts.MotorPos[0].motor_end_ps     = 30500;                                                                   //++ Storing variables on respective array indexes
//            landing_LeftRearDownLimit  = motor_sts.MotorPos[0].motor_end_rear_ps= 35500;                                                                   //++ Storing variables on respective array indexes
//            landing_LeftCurPos         = motor_sts.MotorPos[0].motor_current_ps = 20000;
//
//            landing_RightUpLimit       = motor_sts.MotorPos[1].motor_start_ps   = 10000;
//            landing_RightDownLimit     = motor_sts.MotorPos[1].motor_end_ps     = 30500;
//            landing_RightRearDownLimit = motor_sts.MotorPos[1].motor_end_rear_ps= 35500;
//            landing_RightCurPos        = motor_sts.MotorPos[1].motor_current_ps = 20000;
//
//            fsFileWrite(&fs, &motor_sts, sizeof(Motor_stats_t));
//            fsFileClose(&fs);
//            cliPrintf("L]St ch ,%ld \r\n",motor_sts.MotorPos[0].motor_start_ps);
//            cliPrintf("L]En ch ,%ld \r\n",motor_sts.MotorPos[0].motor_end_ps);
//            cliPrintf("L]Du ch ,%ld \r\n",motor_sts.MotorPos[0].motor_end_rear_ps);
//            cliPrintf("L]Cu ch ,%ld \r\n",motor_sts.MotorPos[0].motor_current_ps);
//
//            cliPrintf("R]St ch ,%ld \r\n",motor_sts.MotorPos[1].motor_start_ps);
//            cliPrintf("R]En ch ,%ld \r\n",motor_sts.MotorPos[1].motor_end_ps);
//            cliPrintf("R]Du ch ,%ld \r\n",motor_sts.MotorPos[1].motor_end_rear_ps);
//            cliPrintf("R]Cu ch ,%ld \r\n",motor_sts.MotorPos[1].motor_current_ps);
//            cliPrintf("init mot encoder..Success.\r\n");
//        }
//    }


#if 1
	mac_init(0);
	mac_set_new_data_cb(mac_new_data);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uart2_receive_func();
#if (MODBUS_ENABLE==1)
	  eMBPoll();
	  write_data_check();
#endif    
#if 0
	  for ( int i = 0 ; i < 255; i++)
	  {
		  hal_ok =  HAL_I2C_IsDeviceReady(&hi2c1, i, 2, 50);
		  if ( hal_ok ==  HAL_OK )
			  while(1);
	  }
#endif
	  //bat = Voltameter_Port_Get_Battery_Electric_Quantity();
	  //HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void read_data_check()
{
#if (MODBUS_ENABLE==1)
  if (modeBusSetting.magicNum != 0x1234 || modeBusSetting.baudRate > 4 || modeBusSetting.baudRate < 0 || modeBusSetting.addr >128 || modeBusSetting.addr ==0 ) // 초기값셋팅
  {
    modeBusSetting.addr = 1;
    modeBusSetting.baudRate = 0;
    modeBusSetting.magicNum = 0x1234;
    stm32_flash_write(0xC00A0 , (uint8_t*)&modeBusSetting, sizeof(ModBus_t));
  }
#endif
}

void write_data_check()
{
#if (MODBUS_ENABLE==1)
  int ret ;

  if ( MODEBUS_SET_REQ == 1)
  {
	  stm32_flash_erase(0xC00A0, sizeof(ModBus_t));

	  modeBusSetting.magicNum = 0x1234;
	  ret = stm32_flash_write(0xC00A0 , (uint8_t*)&modeBusSetting, sizeof(ModBus_t));
	  if ( ret == -1 )
	  {
		  ret = stm32_flash_write(0xC00A0 , (uint8_t*)&modeBusSetting, sizeof(ModBus_t));
	  }

	  MODEBUS_SET_REQ = 0;
  }
#endif
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
