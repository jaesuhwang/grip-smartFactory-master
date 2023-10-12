/*
 * tdma_c.c
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */

#include <stdlib.h>
#include <string.h>
#include "tdma_c.h"
#include "tdma_timings.h"
#include "tdma_frames.h"
#include "tdma_mgt.h"

#include "cc1120.h"
#include "softtimer.h"
#include "usart.h"


// ALARMS
#define ALARM_SLOTS  TIMERB_ALARM_CCR0
// 시간셋팅.
Node_sleep_time_t node_id[10];  // why 9 ? id 0번 헷갈림 1번부터 쓸거임


uint8_t node_addr=0x0;

//#define LED_RED_OFF()	\
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET)
//#define LED_RED_ON()	\
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET)
//
//#define LED_BLUE_OFF()	\
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_RESET)
//#define LED_BLUE_ON()	\
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,GPIO_PIN_SET)
//
//#define LED_WHITE_OFF()	\
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET)
//#define LED_WHITE_ON()	\
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET)

/* GLOBAL VARIABLES */
slot_t mac_slots[DATA_SLOT_MAX];

/* STATIC VARIABLES */
// frames
beacon_msg_t beacon_msg;
static control_msg_t control_msg;
data_msg_t data_msg;
footer_t footer;

// times
static uint16_t beacon_eop_time;


/* STATIC PROTOTYPES */
void slot_alarm(void *arg);
//uint16_t slot_alarm(void );
//static void set_rx(void);
uint16_t slot_data(void);

//static uint16_t slot_control(void);
uint16_t slot_control(void);
//uint16_t beacon_send(void);
//static uint16_t beacon_sent(void);

// other
static uint16_t slot_count;
static uint16_t (*new_data_cb)(int16_t);

char SendToPcBuffer[64];


void mac_init(uint8_t channel) {
	int i;
	uint8_t regCh;
	swtimer_handle_t h_timeSlot_timer;
	// configure the radio
	cc1120_init();
	cc1120_cmd_idle();

	//cc1120_cfg_white_data(CC1120_DATA_WHITENING_ENABLE);

	// go to RX after TX
	cc1120_cfg_rxoff_mode(CC1120_RXOFF_MODE_STAY_RX);
	cc1120_cfg_txoff_mode(CC1120_TXOFF_MODE_RX);
	//cc1120_cfg_txoff_mode(CC1120_TXOFF_MODE_RX);

	// initialize the timerB, with the beacon perdiod
	//cc1120_cfg_fs_autocal(CC1101_AUTOCAL_NEVER);
	//CC1120_SpiReadReg(CC112X_SETTLING_CFG, &regCh, 1);

	swtimerInit();
	h_timeSlot_timer = swtimerGetHandle();
	swtimerSet(h_timeSlot_timer, 100, LOOP_TIME, slot_alarm, NULL  );
	swtimerStart(h_timeSlot_timer);

//	sSoftwareTimer.Start(TimeSlot.timeSlotId, 30);
//	sSoftwareTimer.Stop(TimeSlot.timeSlotId);

//	timerB_init();
//	timerB_set_alarm_from_now(ALARM_SLOTS, 312, 327);
//	timerB_register_cb(ALARM_SLOTS, slot_alarm);
	//HAL_TIM_Base_Start_IT(&htim4);
	// set IDLE state, flush everything
	cc1120_cmd_idle();
	cc1120_cmd_flush_rx();
	cc1120_cmd_flush_tx();


// configure irq

	//cc1120_gdo0_int_clear();
	//cc1120_gdo2_int_clear();
	__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI1_IRQn);

	// configure irq
	/* EXTI interrupt init*/
//	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	// configure the beacon frame
	beacon_msg.hdr.length = BEACON_LENGTH-1;
	HEADER_SET_ADDR(beacon_msg.hdr, node_addr);
	HEADER_SET_TYPE(beacon_msg.hdr,BEACON_TYPE);
	beacon_msg.seq=0;
    #if 1
	for ( int i = 0 ; i < BEACON_PACKET_SIZE_MAX ; i++)
	{
		beacon_msg.data[i] = 0;
	}  // 여기네
    #endif 
    	// initialize the slot management service
	tdma_mgt_init();
	// reset slot count
	slot_count = -1;

	// init the data slots
	for (i=0;i<DATA_SLOT_MAX;i++) {
		mac_slots[i].ready=0;
		mac_slots[i].addr=0;
	}

	// reset the callback
	new_data_cb = 0x0;

}

void mac_set_new_data_cb(uint16_t (*cb)(int16_t)) {
    new_data_cb = cb;
}


void set_rx(void) {
    // idle, flush
    cc1120_cmd_idle();
    cc1120_cmd_flush_rx();
    cc1120_cmd_flush_tx();
    // set RX
    cc1120_cmd_rx();

    // clear interrupt
    //cc1120_gdo0_int_clear();
    //cc1120_gdo2_int_clear();
	//__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI9_5_IRQn);
	//__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI15_10_IRQn);
}

volatile uint16_t slotTimeIniit;
//uint16_t slot_alarm(void ) {
void slot_alarm(void *arg) {
    slot_count++;
   // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
#if 1
//    beacon_send();
    if (slot_count>CTRL_SLOT)
    {
        slot_count = BEACON_SLOT;
        //LED_BLUE_ON();
        beacon_send();
    }
    else if (slot_count<=DATA_SLOT_MAX)
    {
    	//if ( MainProcReq == 0)
    	cc1120_gdo0_register_callback(slot_data);

    }
    else
    {
    	//cc1120_gdo0_register_callback(slot_control);
    }
#endif
}



void sleep_time_set(void)
{
    uint8_t temp_id = 1;
    #if 0
    for(int i = 0 ; i < BEACON_PACKET_SIZE_MAX ; i = i+2)
    {
        beacon_msg.data[i] = node_id[temp_id].min;
        beacon_msg.data[i+1] = node_id[temp_id].sec;
        temp_id++;
    }
    #endif 
    beacon_msg.data[0] = node_id[0].min;  // 각 ID sleep time  => id 1 min
    beacon_msg.data[1] = node_id[0].sec;  // => id 1 sec
    beacon_msg.data[2] = node_id[1].min;  // => id 2 min
    beacon_msg.data[3] = node_id[1].sec;
    beacon_msg.data[4] = node_id[2].min;
    beacon_msg.data[5] = node_id[2].sec;
    beacon_msg.data[6] = node_id[3].min;
    beacon_msg.data[7] = node_id[3].sec;
    beacon_msg.data[8] = node_id[4].min;
    beacon_msg.data[9] = node_id[4].sec;
    beacon_msg.data[10] = node_id[5].min;
    beacon_msg.data[11] = node_id[5].sec;
    beacon_msg.data[12] = node_id[6].min;
    beacon_msg.data[13] = node_id[6].sec;
    beacon_msg.data[14] = node_id[7].min;
    beacon_msg.data[15] = node_id[7].sec;
    beacon_msg.data[16] = node_id[8].min;
    beacon_msg.data[17] = node_id[8].sec;
     
}
uint16_t beacon_send(void) {
//    LED_RED_ON();
//    LED_GREEN_OFF();
//    LED_BLUE_OFF();
	uint8_t statusByte;
    // flush
#if 1 // tx off and 일시
    cc1120_cmd_idle();

    cc1120_cmd_flush_rx();
    cc1120_cmd_flush_tx();
#endif
    // calibrate
    //cc1120_cmd_calibrate();

    // config IRQ
	 cc1120_gdo0_register_callback(beacon_sent);
	//__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI9_5_IRQn);
	//HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	 __HAL_GPIO_EXTI_CLEAR_FLAG(EXTI1_IRQn);
    //__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI_IRQn);
    //cc1120_gdo0_int_clear();
    //cc1120_gdo0_int_enable();

    // start TX
    //cc1120_cmd_tx();
    //put frame in FIFO

    //cc1120_fifo_put((uint8_t*)&beacon_msg, beacon_msg.hdr.length+1);
    //PacketSend(destAddr, srcAddr, frame)

    //CC1120_SpiWriteTxFifo((uint8_t*)&beacon_msg, beacon_msg.hdr.length+1);

    //PacketSend2((uint8_t*)&beacon_msg);
    //CC1120_SpiWriteTxFifo((uint8_t*)&beacon_msg, beacon_msg.hdr.length+1); //
    CC1120_SpiReadReg(CC112X_MARC_STATUS1, &statusByte, 1);
	if (statusByte == 0x07 ||  statusByte == 0x08)
	{
		//Flush FIFO
		cc1120_cmd_flush_tx();
	}
	else
	{
		//cc1120_gdo0_register_callback(beacon_sent);
			//__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI0_IRQn);
		//printf("0x%02x\r\n", beacon_msg.ctl  );


        sleep_time_set();
		// Strobe TX to send packet
		send_packet2((uint8_t*)&beacon_msg, beacon_msg.hdr.length);
	}
	//LED_BLUE_OFF();
	beacon_msg.ctl=0;
	//beacon_msg.data=0;



    //printf("***\r\n hdr l %02x\r\n hdr fct %02x\r\n seq %02x\r\n ctl %02x\r\n data %02x ***\r\n",beacon_msg.hdr.length,beacon_msg.hdr.fctl,beacon_msg.seq,beacon_msg.ctl, beacon_msg.data);

    return 0;
}

uint16_t beacon_sent(void) {
    //beacon_eop_time = timerB_time();
    beacon_msg.seq++;

    cc1120_gdo0_register_callback(slot_data);

    return 0;
}


#define RX_FIFO_ERROR       0x11

uint8_t src;
uint16_t slot_data(void) {
    uint8_t len;
    uint16_t now;
    uint8_t rxBytes;
    uint8_t rxBytes2;
    uint8_t marcState;
    uint8_t headerType;
    uint8_t msg_type;
    uint8_t footerp[2];
    //now = timerB_time();
    //LED_WHITE_ON();
    // test CRC and bytes in FIFO
    //len = cc1120_status_rxbytes();
    CC1120_SpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);
	//CC1120_SpiReadReg(CC112X_NUM_RXBYTES, &rxBytes2, 1);
    len = rxBytes;

    // check fifo length
    if (len==0) {
        // empty packet
        //~ printf("empty");
        return 0;
    } else if (len>64) {
        // overflow, flush
        //~ printf("over");
        set_rx();
        return 0;
    }

    // Read MARCSTATE to check for RX FIFO error
//    CC1120_SpiReadReg(CC112X_MARCSTATE, &marcState, 1);
//
//    if((marcState & 0x1F) == RX_FIFO_ERROR) {
//
//    	CC1120_SpiCmdStrobe(CC112X_SFRX);
//    	return 0;
//    }


    // get length, and check
    CC1120_SpiReadRxFifo((uint8_t*)&data_msg.hdr.length, 1);
    if ( data_msg.hdr.length > (DATA_LENGTH-1))
    {
    	set_rx();
    	return 0;
    }

    // length is good
    // get data
    CC1120_SpiReadRxFifo((uint8_t*)&data_msg+1, data_msg.hdr.length);
    // get status
    CC1120_SpiReadRxFifo((uint8_t*)&footer, FOOTER_LENGTH);
    //printf("id %02x| ",data_msg.hdr.fctl &0x0F);
    // check CRC
	if ( (footer.crc&0x80)==0 ) {
		// bad crc, exit
		//~ printf("crc");

		//printf("crc fail\r\n");
		//printf("crc\r\n");
		return 0;
	}

	// check type, destination
	if (HEADER_GET_TYPE(data_msg.hdr) != DATA_TYPE) {
		//~ printf("not_data");
		return 0;
	}

	// check source corresponds to timeslot
	src = HEADER_GET_ADDR(data_msg.hdr);

//	if (tdma_mgt_getaddr(slot_count)!=src) {
//		// src doesn't match slot
//		return 0;
//	}
//	printf("%02x%02X", STX,data_msg.hdr.length );
	//for ( int i = 0 ; i < PAYLOAD_LENGTH_MAX; i++)
	//{
//		printf("%02x",data_msg.payload[i]);
	//}
	//printf("\r\n");
//	SendToPcBuffer[0] = STX;
//	SendToPcBuffer[1] = data_msg.hdr.length ;
//	SendToPcBuffer[2] = src;
//	memcpy(SendToPcBuffer+3 , data_msg.payload, data_msg.hdr.length);
//	SendToPcBuffer[15] = footer.rssi;
//	SendToPcBuffer[16] = ETX;
	//  sprintf(SendToPcBuffer, "%c%d%d%d%d%d%d%d%d%d%d%d%d%d%d%c", 	STX,	// stx
	 																			
	//  																				data_msg.hdr.length,
	//  																				data_msg.payload[0],  //
	//  																				data_msg.payload[1],
	//  																				data_msg.payload[2],
	//  																				data_msg.payload[3],
	//  																				data_msg.payload[4],
	//  																				data_msg.payload[5],
	//  																				data_msg.payload[6],
	//  																				data_msg.payload[7],
	//  																				data_msg.payload[8],
	//  																				data_msg.payload[9],
	//  																				data_msg.payload[10],
	//  																				data_msg.payload[11],
	//  																				footer.rssi,
	//  																				ETX
	//  										);

    send_to_pc(); // JKU

    
	//HAL_UART_Transmit(&huart1, SendToPcBuffer, 33, 10);
	//printf("\r\n");
	// check data has been read
	if (mac_slots[slot_count-1].ready==0) {
		//memcpy(mac_slots[slot_count-1].data, data_msg.payload, MAC_PAYLOAD_SIZE);
		mac_slots[slot_count-1].ready = 1;

		//printf("slot_count=%d\r\n",slot_count-1);
		// wake the CPU up by returning 1

		return 1;
	}
	cc1120_cmd_rx();
	//cc1120_cmd_rx
	//set_rx();
    return 0;
}


uint16_t slot_control(void) {
    uint8_t len, src;
    uint8_t rxBytes;
    uint16_t now;
    uint8_t marcState;
    // test bytes in FIFO
    //len = cc1101_status_rxbytes();
    LED_RED_ON();
    CC1120_SpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);
    len = rxBytes;
//    now = timerB_time();
//    printf("cont t = %d\r\n", now  );
    // check fifo length
    if (len==0) {
        // empty packet
        //~ printf("empty");
        return 0;
    } else if (len>64) {
        // overflow, flush
        //~ printf("over");
        set_rx();
        return 0;
    }

    CC1120_SpiReadReg(CC112X_MARCSTATE, &marcState, 1);

	if((marcState & 0x1F) == RX_FIFO_ERROR) {

		CC1120_SpiCmdStrobe(CC112X_SFRX);
		return 0;
	}

	CC1120_SpiReadRxFifo(&control_msg.hdr.length, 1);

    // get length, and check
//    cc1101_fifo_get(&control_msg.hdr.length, 1);

    if (control_msg.hdr.length<(CONTROL_LENGTH-1)) {
        // length too small, download packet and return
        //cc1101_fifo_get((uint8_t*)&control_msg+1, control_msg.hdr.length);
//        cc1101_fifo_get((uint8_t*)&footer, FOOTER_LENGTH);
        //~ printf("small");
        return 0;
    } else if (control_msg.hdr.length>(CONTROL_LENGTH-1)) {
        // length too big, can't empty, flush
        //~ printf("big");
        set_rx();
        return 0;
    }

   // length is good
   // get data+status
    CC1120_SpiReadRxFifo((uint8_t*)&control_msg+1, CONTROL_LENGTH-1);
    CC1120_SpiReadRxFifo((uint8_t*)&footer, FOOTER_LENGTH);

    // check CRC
    if ( (footer.crc&0x80)==0 ) {
        // bad crc, exit
        //~ printf("crc");
        return 0;
    }

    src = HEADER_GET_ADDR(control_msg.hdr);
    // it's valid data for me, check content
    if (CONTROL_GET_TYPE(control_msg) == CONTROL_ATTACH_REQ) {
        int16_t slot;
        slot = tdma_mgt_attach(src);
        if (slot>0) {
            CONTROL_SET_TYPE(beacon_msg, CONTROL_ATTACH_OK);
            CONTROL_SET_ADDR(beacon_msg, src);
            //beacon_msg.data = (uint8_t)slot;
            mac_slots[slot-1].addr = src;
        } else {
            CONTROL_SET_TYPE(beacon_msg, CONTROL_ATTACH_ERR);
            CONTROL_SET_ADDR(beacon_msg, src);
            //beacon_msg.data = 0xFF;
        }
    }
    LED_RED_OFF();
    return 0;
}

uint16_t sync_detected(void) {

    return 0;
}

