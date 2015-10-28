/********************************************************************
can.h

Copyright (c) 2014, Jonathan Nutzmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
********************************************************************/

#ifndef CAN_H
#define CAN_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stm32f4xx_can.h"

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define CAN_PACKET_ID_MASK_ALLOW_ALL  ((CANPacketIDMask_t)0xFFFFFFFF)

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef uint32_t CANPacketIDMask_t;
typedef uint32_t CANPacketId_t;

/**
 * CAN Packet Handler
 * @param packet - new packet to be handled.
 */
typedef void (*CANRXHandlerFxn)(CanRxMsg * packet);

typedef struct
{
	CANPacketIDMask_t  mask;
	CANPacketId_t      id_after_mask;
	CANRXHandlerFxn    callback;
} CANRXEntry_t;

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

void can_init(CAN_TypeDef *can_module, uint32_t baud_rate);
void can_send_packet(CAN_TypeDef *can_module, CanTxMsg *packet);
void can_send_packet_isr(CAN_TypeDef *can_module, CanTxMsg *packet);
void can_register_handler(CAN_TypeDef *can_module, CANRXEntry_t *entry);

#endif /* CAN_H */
