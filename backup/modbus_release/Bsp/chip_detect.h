/*
 * chip_detect.h
 *
 *  Created on: 2021. 12. 1.
 *      Author: lucky
 */

#ifndef INC_CHIP_DETECT_H_
#define INC_CHIP_DETECT_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 * INCLUDES
 */


/******************************************************************************
 * CONSTANTS
 */

/* Chip type constants */
#define CHIP_TYPE_CC1101                0x1101

#define CHIP_TYPE_CC110L                0x0110
#define CHIP_TYPE_CC113L                0x0113
#define CHIP_TYPE_CC115L                0x0115

#define CHIP_TYPE_CC1101_CC1190         0x0190  // CC1101 + CC1190 = 0190
#define CHIP_TYPE_CC1120_CC1190         0x2090  // CC1120 + CC1190 = 2090
#define CHIP_TYPE_CC1120                0x1120
#define CHIP_TYPE_CC1121                0X1121
#define CHIP_TYPE_CC1125                0x1125
#define CHIP_TYPE_CC1175                0x1175
#define CHIP_TYPE_CC1200                0x1200
#define CHIP_TYPE_CC1201                0x1201
#define CHIP_TYPE_CC2500                0x2500
#define CHIP_TYPE_NONE                  0x0000

/* Manual chip select constants*/
#define CC1101_SELECTED                 0x1101
#define CC1120_SELECTED                 0x1120
#define CC1101_CC1190_SELECTED          0x0190
#define CC1120_CC1190_SELECTED          0x2090
/******************************************************************************
 * Prototypes
 */

typedef struct
{
	uint16_t deviceName;
	uint16_t id;
	uint8_t  ver;
	uint8_t  xoscFreq;

}radioChipType_t;


/******************************************************************************
 * PROTOTYPES
 */

/* Will populate the radioChipType struct when called */
uint8_t trxDetectChipType(radioChipType_t *pRadioChipType);

#ifdef  __cplusplus
}
#endif


#endif /* INC_CHIP_DETECT_H_ */
