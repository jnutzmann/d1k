/********************************************************************
d1k_can.h

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

#include "d1k.h"
#include "stm32f4xx_can.h"

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define CAN_PACKET_ID_MASK_ALLOW_ALL  ((CANPacketIDMask_t)0xFFFFFFFF)

#define D1K_CAN_DEFAULT_BAUDRATE 125000

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef uint32 CANPacketIDMask_t;
typedef uint32 CANPacketId_t;

/**
 * CAN Packet Handler
 * @param packet - new packet to be handled.
 */
typedef void (*CANRXHandlerFxn)(CanRxMsg * packet);

typedef struct
{
	CANPacketIDMask_t  mask;
	CANPacketId_t      idAfterMask;
	CANRXHandlerFxn    callback;
} CANRXEntry_t;

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

void d1k_CAN_Init            ( CAN_TypeDef * canModule, uint32 baudRate);
void d1k_CAN_SendPacket      ( CAN_TypeDef * canModule, CanTxMsg * packet );
void d1k_CAN_SendPacket_ISR  ( CAN_TypeDef * canModule, CanTxMsg * packet );
void d1k_CAN_RegisterHandler ( CAN_TypeDef * canModule, CANRXEntry_t * entry );

#endif /* CAN_H_ */
