/*
 * tdma_mgt.c
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */



#include "main.h"
#include "tdma_mgt.h"
#include "tdma_timings.h"
#include "tdma_frames.h"

static uint8_t slots[DATA_SLOT_MAX];

void tdma_mgt_init(void) {
    int16_t i;

    for (i=0;i<DATA_SLOT_MAX;i++) {
        slots[i] = 0x0;
    }
}

int16_t tdma_mgt_attach(uint8_t node) {
    int16_t i, free=-1;

    node = node&HEADER_ADDR_MASK;

    // first, see if this node exists in the table
    for (i=0;i<DATA_SLOT_MAX;i++) {
        if (GET_ADDR(slots[i])==node) {
            // Found! Return the slot number
            return i+1;
        } else if ((slots[i]==0) && (free==-1)) {
            // Slot free, store the index
            free=i;
        }
    }
    // it's a new node
    // if there is some space, insert it
    if (free>=0) {
        slots[free]=node;
    }
    // return the index
    return free+1;
}

uint8_t tdma_mgt_getaddr(int16_t slot) {
    if (0 < slot && slot <= DATA_SLOT_MAX)
        return GET_ADDR(slots[slot - 1]);
    return 0x0;
}
