/*
 * cc1120.h
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */

#ifndef INC_CC1120_H_
#define INC_CC1120_H_

#include "stm32f4xx_hal.h"

#include "cc1120_gpio.h"
#include "cc112x_spi.h"

/**
 * \name RXOFF mode configuration constants
 * @{
 */
#define CC1120_RXOFF_MODE_IDLE     0x00
#define CC1120_RXOFF_MODE_FSTXON   0x01 /* freq synth on, ready to Tx */
#define CC1120_RXOFF_MODE_TX       0x02
#define CC1120_RXOFF_MODE_STAY_RX  0x03

/**
 * \name TXOFF mode configuration constants
 * @{
 */
#define CC1120_TXOFF_MODE_IDLE     0x00
#define CC1120_TXOFF_MODE_FSTXON   0x01 /* freq synth on, ready to Tx */
#define CC1120_TXOFF_MODE_STAY_TX  0x02
#define CC1120_TXOFF_MODE_RX       0x03


/**
 * \name Automatic calibration configuration constants
 * @{
 */
#define CC1101_AUTOCAL_NEVER             0x00
#define CC1101_AUTOCAL_IDLE_TO_TX_RX     0x01
#define CC1101_AUTOCAL_TX_RX_TO_IDLE     0x02
#define CC1101_AUTOCAL_4TH_TX_RX_TO_IDLE 0x03


/**
 * \name Data whitening configuration constants
 * @{
 */
#define CC1120_DATA_WHITENING_ENABLE  0x1
#define CC1120_DATA_WHITENING_DISABLE 0x0


/**
 * \brief register a callback function for GDO0 interrupt
 * \param cb a function pointer
 */
void cc1120_gdo0_register_callback(uint16_t (*cb)(void));

/**
 * \brief register a callback function for GDO2 interrupt
 * \param cb a function pointer
 */
void cc1120_gdo2_register_callback(uint16_t (*cb)(void));

extern uint16_t (*gdo0_cb)(void);
extern uint16_t (*gdo2_cb)(void);



#define SPI1_NSS_HIGH() 									\
do {														\
	HAL_GPIO_WritePin(CC1120_NSS_PORT, CC1120_NSS_PIN, GPIO_PIN_SET);		\
}while(0)													\

#define SPI1_NSS_LOW()										\
do {														\
	HAL_GPIO_WritePin(CC1120_NSS_PORT, CC1120_NSS_PIN, GPIO_PIN_RESET);	\
}while(0)													\

void cc112x_hw_rst(void);
void cc1120_init(void);
void cc1120_cmd_idle(void);

/**
 * \brief flush RX FIFO
 */
#define cc1120_cmd_flush_rx() \
		CC1120_SpiCmdStrobe(CC112X_SFRX)

/**
 * \brief flush TX FIFO
 */
#define cc1120_cmd_flush_tx() \
		CC1120_SpiCmdStrobe(CC112X_SFTX)

/**
 * \brief calibrate frequency synthetizer
 */
void cc1120_cmd_calibrate(void);


/**
 * \brief does nothing, update status byte
 */
#define cc1120_cmd_nop() \
		CC1120_SpiCmdStrobe(CC112X_SNOP)


/**
 * \brief enable rx.
 */
#define cc1120_cmd_rx() \
		CC1120_SpiCmdStrobe(CC112X_SRX)


/**
 * \brief enable tx. if in rx with cca enabled, go to tx if channel clear
 */
#define cc1120_cmd_tx() \
		CC1120_SpiCmdStrobe(CC112X_STX)


/**
 * \brief enter power down
 */
#define cc1120_cmd_pwd() \
		CC1120_SpiCmdStrobe(CC112X_SPWD)



/**
 * \brief Set the behavior after a packet RX
 * \param cfg the configuration value
 */
#define cc1120_cfg_rxoff_mode(cfg) do { \
  uint8_t reg; \
  CC1120_SpiReadReg(CC112X_RFEND_CFG1, &reg, 1); \
  reg = (reg & 0xCF) | (((cfg) << 4) & 0x30); \
  CC1120_SpiWriteReg(CC112X_RFEND_CFG1, &reg,1); \
} while (0)


/**
 * \brief Set the behavior after packet TX
 * \param cfg the configuration value
 */
#define cc1120_cfg_txoff_mode(cfg) do { \
  uint8_t reg; \
  CC1120_SpiReadReg(CC112X_RFEND_CFG0,&reg, 1); \
  reg = (reg & 0xCF) | (((cfg) << 4) & 0x30); \
  CC1120_SpiWriteReg(CC112X_RFEND_CFG0, &reg,1); \
} while (0)


/**
 * \brief Set auto calibration policy
 * \param cfg the configuration value
 */
#define cc1120_cfg_fs_autocal(cfg) do { \
  uint8_t reg; \
  CC1120_SpiReadReg(CC112X_SETTLING_CFG,&reg , 1 ); \
  reg = (reg & 0xE7) | (((cfg) << 3) & 0x18); \
  CC1120_SpiWriteReg(CC112X_SETTLING_CFG, &reg, 1); \
} while (0)


/**
 * \brief turn data whitening on/off
 * \param cfg the configuration value
 */
#define cc1120_cfg_white_data(cfg) do { \
  uint8_t reg; \
  CC1120_SpiReadReg(CC112X_PKT_CFG1,&reg, 1); \
  reg = (reg & 0xBF) | (((cfg) << 6) & 0x40); \
  CC1120_SpiWriteReg(CC112X_PKT_CFG1, &reg,1); \
} while (0)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t CC1120_SpiCmdStrobe(uint8_t cmd);
uint8_t CC1120_8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint8_t len);
uint8_t CC1120_16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len);
void CC1120_ReadWriteBurstSingle(uint16_t addr,uint8_t *pData,uint8_t len);
uint8_t CC1120_SpiWriteReg(uint16_t addr, uint8_t *pData, uint8_t len);
uint8_t CC1120_SpiReadReg(uint16_t addr, uint8_t *pData, uint8_t len);
uint8_t CC1120_SpiWriteTxFifo(uint8_t *pData, uint8_t len);
uint8_t CC1120_SpiReadRxFifo(uint8_t *pData, uint8_t len);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t send_packet(uint8_t * sendBuffer);
//uint8_t send_packet2(uint8_t * sendBuffer, uint8_t len);
void send_packet2(uint8_t * sendBuffer, uint8_t len);
void registerConfig(void);
void GetPartAndVersion();
uint8_t CC1120_setFrequency(float centre);
void manualCalibration();
void perCC112xSetOutputPower(uint8_t index);


extern volatile uint32_t slotAlarmTime;

#endif /* INC_CC1120_H_ */
