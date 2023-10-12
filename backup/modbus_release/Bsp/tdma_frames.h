/*
 * tdma_frames.h
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */

#ifndef INC_TDMA_FRAMES_H_
#define INC_TDMA_FRAMES_H_

#include "main.h"

typedef struct {
    uint8_t length, // length of the packet
            fctl; // frame control
} header_t;

#define HEADER_LENGTH sizeof(header_t)
/*
 * FCTL format:
 * | 7 6 5 4 3 2 1 0 |
 * |  type  |  addr  |
 * | frame  |  src   |
 *
 */
#define HEADER_TYPE_MASK 0xF0
#define HEADER_ADDR_MASK 0x0F
#define HEADER_SET_TYPE(hdr, type) hdr.fctl= \
        ((hdr.fctl&HEADER_ADDR_MASK)|(type&HEADER_TYPE_MASK))
#define HEADER_GET_TYPE(hdr) (hdr.fctl&HEADER_TYPE_MASK)
#define HEADER_SET_ADDR(hdr, addr) hdr.fctl= \
        ((hdr.fctl&HEADER_TYPE_MASK)|(addr&HEADER_ADDR_MASK))
#define HEADER_GET_ADDR(hdr) (hdr.fctl&HEADER_ADDR_MASK)

//#define BEACON_PACKET_SIZE_MAX 16
#define BEACON_PACKET_SIZE_MAX 18

typedef struct {
    uint8_t rssi, crc;
} footer_t;

#define FOOTER_LENGTH sizeof(footer_t)

typedef struct {
    header_t hdr;
    uint8_t seq;
    uint8_t ctl;
    uint8_t data[BEACON_PACKET_SIZE_MAX];
} beacon_msg_t;

#define BEACON_LENGTH sizeof(beacon_msg_t)
#define BEACON_TYPE 0x10

typedef struct {
    header_t hdr;
    uint8_t ctl;
} control_msg_t;

#define CONTROL_LENGTH sizeof(beacon_msg_t)
#define CONTROL_TYPE       0x20
#define CONTROL_ATTACH_REQ 0xA0
#define CONTROL_ATTACH_OK  0xB0
#define CONTROL_ATTACH_ERR 0xC0

#define CONTROL_SET_TYPE(msg, type) msg.ctl=((msg.ctl&0x0F)|(type&0xF0))
#define CONTROL_GET_TYPE(msg) (msg.ctl&0xF0)
#define CONTROL_SET_ADDR(msg, data) msg.ctl=((msg.ctl&0xF0)|(data&0x0F))
#define CONTROL_GET_ADDR(msg) (msg.ctl&0x0F)

#define PACKET_SIZE_MAX 16

#define PAYLOAD_LENGTH_MAX PACKET_SIZE_MAX-(HEADER_LENGTH+FOOTER_LENGTH)

typedef struct {
    header_t hdr;
    uint8_t payload[PAYLOAD_LENGTH_MAX];
} data_msg_t;

#define DATA_LENGTH sizeof(data_msg_t)
#define DATA_TYPE 0x30


#endif /* INC_TDMA_FRAMES_H_ */
