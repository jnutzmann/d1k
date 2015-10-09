/********************************************************************
d1k_can.c

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

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "d1k.h"
#include "string.h"
#include "d1k_can.h"
#include "d1k_led.h"

#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "queue.h"


/****************************************************************************
 * Definitions
 ***************************************************************************/

#define CAN_TX_BUFFER_DEPTH		    (64)
#define CAN_MAX_PACKET_HANDLER		(32)

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static CANRXEntry_t can1RXTable[CAN_MAX_PACKET_HANDLER];
static CANRXEntry_t can2RXTable[CAN_MAX_PACKET_HANDLER];

static uint32 can1RXPacketHandlerCount = 0;
static uint32 can2RXPacketHandlerCount = 0;

static xQueueHandle can1TXQueue, can2TXQueue;

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void d1k_CAN_DispatchRx( CAN_TypeDef * canModule, CanRxMsg * packet );

/****************************************************************************
 * Public Functions
 ***************************************************************************/

/**
 * Inits the specified CAN module.
 * @param canModule - CAN module to be inited (CAN1 or CAN2).
 * @param baudRate - Baud rate in Hz.
 * @param apb1Freq - APB1 clock frequency in HZ.
 * @param RXTable - Receive table with definitions of packets to be handled and their
 * corresponding handlers.
 */
void d1k_CAN_Init ( CAN_TypeDef * canModule, uint32 baudRate)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	if ( canModule == CAN1 )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // turn on CAN1 clock
		can1TXQueue = xQueueCreate(CAN_TX_BUFFER_DEPTH,sizeof(CanTxMsg));
	}
	else if ( canModule == CAN2 )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); // turn on CAN2 clock
		can2TXQueue = xQueueCreate(CAN_TX_BUFFER_DEPTH,sizeof(CanTxMsg));
	}

	CAN_InitTypeDef canInit;
	CAN_StructInit(&canInit);

	// SYNC SEG : 1TQ
	// BS1		: 8TQ
	// BS2		: 5TQ

	canInit.CAN_BS1 = CAN_BS1_8tq;
	canInit.CAN_BS2 = CAN_BS2_5tq;
	canInit.CAN_SJW = CAN_SJW_1tq;
	canInit.CAN_Prescaler = RCC_ClocksStatus.PCLK1_Frequency / baudRate / (3 + canInit.CAN_BS1 + canInit.CAN_BS2);
	canInit.CAN_Mode = CAN_Mode_Normal;
	canInit.CAN_TXFP = ENABLE;
	canInit.CAN_ABOM = ENABLE;
	CAN_Init(canModule, &canInit);

	// For now, create a filter that passes all messages.
	// TODO: Autogenerate these?
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = (canModule == CAN1) ? 0 : 14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// Right now, we will only use FIFO0 for receiving packets.  Enable the interrupt.
	// Also, enable the transmit mailbox empty interrupt in case the software buffer is used.
	// TODO: RX FIFO overrun interrupt?
	// TODO: Bus error interrupts?
	// TODO: Add second FIFO?
	CAN_ITConfig(canModule, CAN_IT_FMP0, ENABLE);

	// Set up the NVIC for the receive interrupt.
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = (canModule == CAN1) ? CAN1_RX0_IRQn : CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the transmit interrupt as well.  Note that the transmit interrupt will only be enabled
	// when the buffer becomes full in d1k_CAN_SendPacket.
	NVIC_InitStructure.NVIC_IRQChannel = (canModule == CAN1) ? CAN1_TX_IRQn : CAN2_TX_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * Sends a CAN packet.  Must NOT be used from an ISR.
 * @param canModule - CAN module to use (CAN1 or CAN2)
 * @param packet - Packet to be sent.
 */
void d1k_CAN_SendPacket ( CAN_TypeDef * canModule, CanTxMsg * packet )
{
	if ( CAN_Transmit(canModule,packet) == CAN_TxStatus_NoMailBox )
	{
		// TODO: What should we set this timeout to?
		xQueueSend( (canModule==CAN1) ? can1TXQueue : can2TXQueue, packet, 0 );

		CAN_ITConfig(canModule,CAN_IT_TME,ENABLE);
	}
}

/**
 * Sends a CAN packet from an ISR
 * @param canModule - CAN module to use (CAN1 or CAN2)
 * @param packet - Packet to be sent.
 */
void d1k_CAN_SendPacket_ISR ( CAN_TypeDef * canModule, CanTxMsg * packet )
{
	if ( CAN_Transmit(canModule,packet) == CAN_TxStatus_NoMailBox )
	{
		xQueueSendFromISR( (canModule==CAN1) ? can1TXQueue : can2TXQueue, packet, NULL );

		CAN_ITConfig(canModule,CAN_IT_TME,ENABLE);
	}
}

/**
 * Register a packet handler.  Note: more than one packet handler can be assigned
 * to an ID.
 * @param canModule - CAN module to associate the handler with (CAN1 or CAN2).
 * @param entry - Entry to add to the list.
 */
void d1k_CAN_RegisterHandler( CAN_TypeDef * canModule, CANRXEntry_t * entry )
{
	if ( canModule == CAN1 )
	{
		memcpy( &(can1RXTable[can1RXPacketHandlerCount]), entry, sizeof(CANRXEntry_t) );
		can1RXPacketHandlerCount++;
	}
	else
	{
		memcpy( &(can2RXTable[can2RXPacketHandlerCount]), entry, sizeof(CANRXEntry_t) );
		can2RXPacketHandlerCount++;
	}
}


/****************************************************************************
 * Private Functions
 ***************************************************************************/

static void d1k_CAN_DispatchRx( CAN_TypeDef * canModule, CanRxMsg * packet )
{
	CANRXEntry_t * table = (canModule == CAN1) ? can1RXTable : can2RXTable;
	uint32 handlerCount = (canModule == CAN1) ? can1RXPacketHandlerCount : can2RXPacketHandlerCount;

	// Note: This purposely allows more than one callback to be registered for
	// a given packet.

	for (uint16 i = 0; i < handlerCount; i++ )
	{
		// TODO: support Extended IDs?
		if ( (packet->StdId & table[i].mask ) == table[i].idAfterMask )
		{
			table[i].callback(packet);
		}
	}
}

/****************************************************************************
 * Interrupt Service Routines
 ***************************************************************************/

void CAN1_RX0_IRQHandler ( void )
{
	d1k_LED_OnPurpose(D1K_LED_PURPOSE_CAN);
	CanRxMsg packet;
	CAN_Receive( CAN1, CAN_FIFO0, &packet );
	d1k_CAN_DispatchRx( CAN1, &packet );
	d1k_LED_OffPurpose(D1K_LED_PURPOSE_CAN);
}

void CAN2_RX0_IRQHandler ( void )
{
	d1k_LED_OnPurpose(D1K_LED_PURPOSE_CAN);
	CanRxMsg packet;
	CAN_Receive( CAN2, CAN_FIFO0, &packet );
	d1k_CAN_DispatchRx( CAN2, &packet );
	d1k_LED_OffPurpose(D1K_LED_PURPOSE_CAN);
}

void CAN1_TX_IRQHandler ( void )
{
	d1k_LED_OnPurpose(D1K_LED_PURPOSE_CAN);

	CanTxMsg toSend;

	if ( xQueueReceiveFromISR(can1TXQueue,&toSend,NULL) == pdTRUE )
	{
		if ( CAN_Transmit(CAN1,&toSend) == CAN_TxStatus_NoMailBox )
		{
			// For some reason, we failed again.  Push it back onto the front.
			xQueueSendToFrontFromISR(can1TXQueue,&toSend,NULL);
		}
	}
	else
	{
		// There are no more messages left in the software buffer.
		// Disable this interrupt.
		CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);
	}

	d1k_LED_OffPurpose(D1K_LED_PURPOSE_CAN);
}

void CAN2_TX_IRQHandler ( void )
{
	d1k_LED_OnPurpose(D1K_LED_PURPOSE_CAN);

	CanTxMsg toSend;

	if ( xQueueReceiveFromISR(can2TXQueue,&toSend,NULL) == pdTRUE )
	{
		if ( CAN_Transmit(CAN2,&toSend) == CAN_TxStatus_NoMailBox )
		{
			// For some reason, we failed again.  Push it back onto the front.
			xQueueSendToFrontFromISR(can2TXQueue,&toSend,NULL);
		}
	}
	else
	{
		// There are no more messages left in the software buffer.
		// Disable this interrupt.
		CAN_ITConfig(CAN2,CAN_IT_TME,DISABLE);
	}

	d1k_LED_OffPurpose(D1K_LED_PURPOSE_CAN);
}




