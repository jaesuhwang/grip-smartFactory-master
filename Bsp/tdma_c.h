/*
 * tdma_c.h
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */

#ifndef INC_TDMA_C_H_
#define INC_TDMA_C_H_

#include "tdma_timings.h"
#include "tdma_frames.h"

#define MAIN_PROC_BEACON	0x01
#define MAIN_PROC_DATA		0x02
#define MAIN_PORC_CONTROL	0x04
#define STX 0x02
#define ETX 0x03


typedef struct 
{
    uint8_t sec;
    uint8_t min
}Node_sleep_time_t;



//extern Node_sleep_time_t node_id[9];  // why 9 ? id 0번 헷갈림 1번부터 쓸거임

//extern Node_sleep_tim_t node_id[9];  // why 9 ? id 0번 헷갈림 1번부터 쓸거임

extern footer_t footer;
extern data_msg_t data_msg;

extern uint8_t src;
/**
 * This node MAC address
 */
extern uint8_t node_addr;

/**
 * Initialize the MAC layer. Look for a coordinator,
 * send an attach request, wait for an ACK.
 */
void mac_init(uint8_t channel);

#define MAC_PAYLOAD_SIZE PAYLOAD_LENGTH_MAX
#define MAC_SLOT_NUMBER DATA_SLOT_MAX

/**
 * a structure containing all relevant information a on slot
 */
typedef struct {
    /**
     * The node address conrresponding to this slot.
     */
    uint8_t addr;
    /**
     * This is the lates received data from this slot.
     * It should be read only when ready is set to 1;
     */
    uint8_t data[MAC_PAYLOAD_SIZE];
    /**
     * Flag indicating that new data has been placed in the data buffer.
     * Set it to 0 when you have read it. New data won't be copied
     * if this flag is not cleared.
     * When this flag is set by the MAC layer, the CPU will be woken up
     * if it is in a LPM.
     */
    volatile uint8_t ready;
} slot_t;

/**
 * A variable containing all informations on all the slots
 */
extern slot_t mac_slots[MAC_SLOT_NUMBER];

/**
 * Function that registers a callback function that will be called
 * when new data has been received. The argument of the callback
 * function will be the slot number available.
 * \param cb the callback function to register
 */
void mac_set_new_data_cb(uint16_t (*cb)(int16_t slot));

void set_rx(void);
uint16_t slot_data(void);
uint16_t slot_control(void);
uint16_t beacon_send(void);
uint16_t beacon_sent(void);

extern uint16_t slotAlarmDly;
extern uint8_t slotAlarmReq;


#endif /* INC_TDMA_C_H_ */
