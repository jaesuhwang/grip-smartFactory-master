/*
 * cc1120_ref_reg.h
 *
 *  Created on: 2021. 11. 30.
 *      Author: lucky
 */

#ifndef INC_CC1120_REF_REG_H_
#define INC_CC1120_REF_REG_H_


#ifdef __cplusplus
extern "C" {
#endif

// 767MHZ
/******************************************************************************
 * INCLUDES
 */
//#include "hal_spi_rf_trxeb.h"
#include "cc112x_spi.h"

static const registerSetting_t preferredSettings[] = {
//		  {CC112X_IOCFG3,            0x11}, // IOCFG_GPIO_CFG_CS CARRIER SENSE  Carrier Sense ? assert : de-assert */
//		  {CC112X_IOCFG2,            0x26}, // IOCFG_GPIO_CFG_RXTX_OR_IDLE      assert if in rx or tx, de-assert if idle/settling (MARC_2PIN_STATUS[0])
//		  {CC112X_IOCFG1,            0xB0},
//		  {CC112X_IOCFG0,            0x46},// 0100 IOCFG_GPIO_CFG_PKT_SYNC_RXTX | IOCFG_GPIO_CFG_INVERT
		  {CC112X_IOCFG3,            0xB0}, // IOCFG_GPIO_CFG_CS CARRIER SENSE  Carrier Sense ? assert : de-assert */
		  {CC112X_IOCFG2,            0xB0}, // IOCFG_GPIO_CFG_RXTX_OR_IDLE      assert if in rx or tx, de-assert if idle/settling (MARC_2PIN_STATUS[0])
		  {CC112X_IOCFG1,            0xB0},
		  {CC112X_IOCFG0,            0x06},// 0100 IOCFG_GPIO_CFG_PKT_SYNC_RXTX | IOCFG_GPIO_CFG_INVERT

		  {CC112X_SYNC_CFG1,     	 0x0B},
		  {CC112X_DCFILT_CFG,    	 0x1C},
		  {CC112X_PREAMBLE_CFG1,     0x18},
		  {CC112X_PREAMBLE_CFG0,     0x2A},
//------------------- 		BAND WIDTH   -----------------

//	  {CC112X_IQIC,          0x46},		// BW 200KHZ
//	  {CC112X_CHAN_BW,       0x01},   	// BW 200KHZ

//	  {CC112X_IQIC,          0x46},		// BW 100KHZ
//	  {CC112X_CHAN_BW,       0x02},   	// BW 100KHZ

//	  {CC112X_IQIC,          0x46},		// BW 50KHZ
//	  {CC112X_CHAN_BW,       0x04},     // BW 50KHZ

	  {CC112X_IQIC,          0x46},		// BW 40KHZ
	  {CC112X_CHAN_BW,       0x05},     // BW 40KHZ

//	  {CC112X_IQIC,          0xC6},		// BW 25KHZ
//	  {CC112X_CHAN_BW,       0x08},     // BW 25KHZ

//--------------------------------------------------------
//----------------------    SYMBOLATE     ----------------

//	  	{CC112X_SYMBOL_RATE2,  0xA9},	// 	100kps
//		{CC112X_SYMBOL_RATE1,  0x99},	//  100kps
//		{CC112X_SYMBOL_RATE0,  0x9A},	// 	100kps

//	  {CC112X_SYMBOL_RATE2,  0x99},		// 50kps
//	  {CC112X_SYMBOL_RATE1,  0x99},		// 50kps
//	  {CC112X_SYMBOL_RATE0,  0x9A},		// 50kps


//	  {CC112X_SYMBOL_RATE2,  0x89},		//  25kps
//	  {CC112X_SYMBOL_RATE1,  0x99},		//  25kps
//	  {CC112X_SYMBOL_RATE0,  0x9A},		//  25kps

	  {CC112X_SYMBOL_RATE2,  0x84},		// 20kps
	  {CC112X_SYMBOL_RATE1,  0x7A},		// 20kps
	  {CC112X_SYMBOL_RATE0,  0xE1},		// 20kps

//	  {CC112X_SYMBOL_RATE2,  0x7E},		// 15kps
//	  {CC112X_SYMBOL_RATE1,  0xB8},		// 15kps
//	  {CC112X_SYMBOL_RATE0,  0x52},		// 15kps


//		  {CC112X_SYMBOL_RATE2,  0x74},		// 	10kps
//		  {CC112X_SYMBOL_RATE1,  0x7A},		//  10kps
//		  {CC112X_SYMBOL_RATE0,  0xE1},		// 	10kps

//		  {CC112X_SYMBOL_RATE2,  0x64},		// 	5kps
//		  {CC112X_SYMBOL_RATE1,  0x7A},		//  5kps
//		  {CC112X_SYMBOL_RATE0,  0xE1},		// 	5kps

//		  {CC112X_SYMBOL_RATE2,  0x60},		// 	4kps
//		  {CC112X_SYMBOL_RATE1,  0x62},		//  4kps
//		  {CC112X_SYMBOL_RATE0,  0x4E},		// 	4kps

//		  {CC112X_SYMBOL_RATE2,  0x50},		// 	2kps
//		  {CC112X_SYMBOL_RATE1,  0x6E},		//  2kps
//		  {CC112X_SYMBOL_RATE0,  0x42},		// 	2kps

//-----------------------------------------------------------------------------
		  {CC112X_MDMCFG0,       0x05},
		  {CC112X_AGC_REF,       0x20},
		  {CC112X_AGC_CS_THR,    0x19},
		  {CC112X_AGC_CFG1,      0xA9},
		  {CC112X_AGC_CFG0,      0xCF},
		  {CC112X_FIFO_CFG,      0x03},
		  {CC112X_SETTLING_CFG,  0x03},
		  {CC112X_FS_CFG,        0x12},
		  {CC112X_PKT_CFG2,      0x04},
		  {CC112X_PKT_CFG1,      0x05},
		  {CC112X_PKT_CFG0,      0x20},
		  {CC112X_PA_CFG2,       0x4F},
		  {CC112X_PA_CFG1,       0x56},
		  {CC112X_PA_CFG0,       0x1C},
		  {CC112X_PKT_LEN,       0xFF},
		  {CC112X_IF_MIX_CFG,    0x00},
		  {CC112X_FREQOFF_CFG,   0x22},
		  {CC112X_FREQ2,         0x75},
		  {CC112X_FREQ1,         0x80},
		  {CC112X_FREQ0,         0x00},
		  {CC112X_FS_DIG1,       0x00},
		  {CC112X_FS_DIG0,       0x5F},
		  {CC112X_FS_CAL0,       0x0E},
		  {CC112X_FS_DIVTWO,     0x03},
		  {CC112X_FS_DSM0,       0x33},
		  {CC112X_FS_DVC0,       0x17},
		  {CC112X_FS_PFD,        0x50},
		  {CC112X_FS_PRE,        0x6E},
		  {CC112X_FS_REG_DIV_CML,0x14},
		  {CC112X_FS_SPARE,      0xAC},
		  {CC112X_XOSC5,         0x0E},
		  {CC112X_XOSC3,         0xC7},
		  {CC112X_XOSC1,         0x07},
};

#ifdef  __cplusplus
}
#endif

#endif /* INC_CC1120_REF_REG_H_ */
