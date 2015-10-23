/********************************************************************
can.c

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

#include "can.h"
#include "FreeRTOS.h"
#include "led.h"
#include "misc.h"
#include "queue.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "string.h"

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define CAN_TX_BUFFER_DEPTH		    (64)
#define CAN_MAX_PACKET_HANDLER		(32)

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static CANRXEntry_t can1_rx_table[CAN_MAX_PACKET_HANDLER];
static CANRXEntry_t can2_rx_table[CAN_MAX_PACKET_HANDLER];

static uint32_t can1_rx_packet_handler_count = 0;
static uint32_t can2_rx_packet_handler_count = 0;

static xQueueHandle can1_tx_queue, can2_tx_queue;

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void can_dispatch_rx(CAN_TypeDef *can_module, CanRxMsg *packet);

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
void can_init(CAN_TypeDef *can_module, uint32_t baud_rate)
{
	// TODO: order of can perph init matters here!  We should do some sort of check.

	RCC_ClocksTypeDef RCC_ClocksStatus;
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	if ( can_module == CAN1 )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // turn on CAN1 clock
		can1_tx_queue = xQueueCreate(CAN_TX_BUFFER_DEPTH,sizeof(CanTxMsg));
	}
	else if ( can_module == CAN2 )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); // turn on CAN2 clock
		can2_tx_queue = xQueueCreate(CAN_TX_BUFFER_DEPTH,sizeof(CanTxMsg));
	}

	CAN_InitTypeDef can_init_struct;
	CAN_StructInit(&can_init_struct);

	// SYNC SEG : 1TQ
	// BS1		: 8TQ
	// BS2		: 5TQ

	can_init_struct.CAN_BS1 = CAN_BS1_8tq;
	can_init_struct.CAN_BS2 = CAN_BS2_5tq;
	can_init_struct.CAN_SJW = CAN_SJW_1tq;
	can_init_struct.CAN_Prescaler = RCC_ClocksStatus.PCLK1_Frequency /
							baud_rate / (3 + can_init_struct.CAN_BS1 + can_init_struct.CAN_BS2);
	can_init_struct.CAN_Mode = CAN_Mode_Normal;
	can_init_struct.CAN_TXFP = ENABLE;
	can_init_struct.CAN_ABOM = ENABLE;
	CAN_Init(can_module, &can_init_struct);

	// For now, create a filter that passes all messages.
	// TODO: Autogenerate these?
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = (can_module == CAN1) ? 0 : 14;
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
	CAN_ITConfig(can_module, CAN_IT_FMP0, ENABLE);

	// Set up the NVIC for the receive interrupt.
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = (can_module == CAN1) ? CAN1_RX0_IRQn : CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the transmit interrupt as well.  Note that the transmit interrupt will only be enabled
	// when the buffer becomes full in can_send_packet.
	NVIC_InitStructure.NVIC_IRQChannel = (can_module == CAN1) ? CAN1_TX_IRQn : CAN2_TX_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * Sends a CAN packet.  Must NOT be used from an ISR.
 * @param canModule - CAN module to use (CAN1 or CAN2)
 * @param packet - Packet to be sent.
 */
void can_send_packet (CAN_TypeDef * can_module, CanTxMsg * packet )
{
	if ( CAN_Transmit(can_module,packet) == CAN_TxStatus_NoMailBox )
	{
		// TODO: What should we set this timeout to?
		xQueueSend( (can_module==CAN1) ? can1_tx_queue : can2_tx_queue, packet, 0 );

		CAN_ITConfig(can_module, CAN_IT_TME, ENABLE);
	}
}

/**
 * Sends a CAN packet from an ISR
 * @param canModule - CAN module to use (CAN1 or CAN2)
 * @param packet - Packet to be sent.
 */
void can_send_packet_isr(CAN_TypeDef *can_module, CanTxMsg * packet )
{
	if (CAN_Transmit(can_module,packet) == CAN_TxStatus_NoMailBox)
	{
		xQueueSendFromISR((can_module ==CAN1) ? can1_tx_queue : can2_tx_queue, packet, NULL );

		CAN_ITConfig(can_module,CAN_IT_TME,ENABLE);
	}
}

/**
 * Register a packet handler.  Note: more than one packet handler can be assigned
 * to an ID.
 * @param canModule - CAN module to associate the handler with (CAN1 or CAN2).
 * @param entry - Entry to add to the list.
 */
void can_register_handler(CAN_TypeDef *can_module, CANRXEntry_t * entry)
{
	if ( can_module == CAN1 )
	{
		memcpy( &(can1_rx_table[can1_rx_packet_handler_count]), entry, sizeof(CANRXEntry_t) );
		can1_rx_packet_handler_count++;
	}
	else
	{
		memcpy( &(can2_rx_table[can2_rx_packet_handler_count]), entry, sizeof(CANRXEntry_t) );
		can2_rx_packet_handler_count++;
	}
}


/****************************************************************************
 * Private Functions
 ***************************************************************************/

static void can_dispatch_rx(CAN_TypeDef *can_module, CanRxMsg *packet)
{
	CANRXEntry_t * table = (can_module == CAN1) ? can1_rx_table : can2_rx_table;
	uint32_t handlerCount = (can_module == CAN1) ? can1_rx_packet_handler_count : can2_rx_packet_handler_count;

	// Note: This purposely allows more than one callback to be registered for
	// a given packet.

	for (uint16_t i = 0; i < handlerCount; i++ )
	{
		// TODO: support Extended IDs?
		if ( (packet->StdId & table[i].mask ) == table[i].id_after_mask)
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
	led_on(LED_CAN);
	CanRxMsg packet;
	CAN_Receive( CAN1, CAN_FIFO0, &packet );
	can_dispatch_rx(CAN1, &packet);
	led_off(LED_CAN);
}

void CAN2_RX0_IRQHandler ( void )
{
	led_on(LED_CAN);
	CanRxMsg packet;
	CAN_Receive( CAN2, CAN_FIFO0, &packet );
	can_dispatch_rx(CAN2, &packet);
	led_off(LED_CAN);
}

void CAN1_TX_IRQHandler ( void )
{
	led_on(LED_CAN);

	CanTxMsg toSend;

	if ( xQueueReceiveFromISR(can1_tx_queue,&toSend,NULL) == pdTRUE )
	{
		if ( CAN_Transmit(CAN1,&toSend) == CAN_TxStatus_NoMailBox )
		{
			// For some reason, we failed again.  Push it back onto the front.
			xQueueSendToFrontFromISR(can1_tx_queue,&toSend,NULL);
		}
	}
	else
	{
		// There are no more messages left in the software buffer.
		// Disable this interrupt.
		CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);
	}

	led_off(LED_CAN);
}

void CAN2_TX_IRQHandler ( void )
{
	led_on(LED_CAN);

	CanTxMsg toSend;

	if ( xQueueReceiveFromISR(can2_tx_queue,&toSend,NULL) == pdTRUE )
	{
		if ( CAN_Transmit(CAN2,&toSend) == CAN_TxStatus_NoMailBox )
		{
			// For some reason, we failed again.  Push it back onto the front.
			xQueueSendToFrontFromISR(can2_tx_queue,&toSend,NULL);
		}
	}
	else
	{
		// There are no more messages left in the software buffer.
		// Disable this interrupt.
		CAN_ITConfig(CAN2,CAN_IT_TME,DISABLE);
	}

	led_off(LED_CAN);
}



