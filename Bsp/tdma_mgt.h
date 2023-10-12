/*
 * tdma_mgt.h
 *
 *  Created on: 2022. 2. 9.
 *      Author: lucky
 */

#ifndef INC_TDMA_MGT_H_
#define INC_TDMA_MGT_H_

#include "main.h"

#define ADDR_MASK 0x0F
#define GET_ADDR(u) (u&ADDR_MASK)

/**
 * Initialize the slot management mechanism
 */
void tdma_mgt_init(void);

/**
 * Proceed an attach request
 * \param node_addr the node address requesting attachement
 * \return the slot number allocated, or -1 if not possible
 */
int16_t tdma_mgt_attach(uint8_t node_addr);

/**
 * Get the node address associated with a slot ID.
 * \param slot the slot number
 * \return the corresponding node address
 */
uint8_t tdma_mgt_getaddr(int16_t slot);

#endif /* INC_TDMA_MGT_H_ */
