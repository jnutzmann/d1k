/********************************************************************
d1k_stdio_can.c

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

#include "d1k_stdio_can.h"
#include "d1k_stdio.h"
#include "d1k_can.h"
#include "d1k_led.h"
#include "FreeRTOS.h"
#include "task.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define DEBUG_STDIO_INVAID_ID	( (CANPacketId_t) 0x700)

#define DEBUG_STDIO_PACKET_TIMOUT_MS	( 50 )

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void d1k_STDIO_CAN_ReadPacketHandler( CanRxMsg * packet);
static void d1k_STDIO_CAN_Task ( void * pvParameters );

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static FunctionalState canStdioEnabled = DISABLE;
static CAN_TypeDef* canStdioModule = NULL;

static xTaskHandle canStdioTask;

static portTickType xLastPacketSent = 0;

static CanTxMsg pendingPacket = {
		.StdId = DEBUG_STDIO_INVAID_ID,
		.IDE = CAN_Id_Standard,
		.RTR = CAN_RTR_Data,
		.DLC = 0
};

// Grab the specific debug packet.
static CANRXEntry_t canStdioReadEntry = {
		.mask = CAN_PACKET_ID_MASK_ALLOW_ALL,
		.idAfterMask = DEBUG_STDIO_INVAID_ID,
		.callback = d1k_STDIO_CAN_ReadPacketHandler
};

/****************************************************************************
 * Public Functions
 ***************************************************************************/

/**
 * Initializes the CAN STDIO module.  This will enable the use of CAN for printf and
 * incoming characters to the D1K portal.
 * @param canModule - CAN module to listen on.
 * @param debugSource - 8 bit ID of the board to uniquely identify boards on the network.
 */
void d1k_STDIO_CAN_Init( CAN_TypeDef * canModule, uint32 txAddr, uint32 rxAddr )
{
	canStdioModule = canModule;
	canStdioEnabled = ENABLE;

	canStdioReadEntry.idAfterMask = rxAddr;
	d1k_CAN_RegisterHandler(canModule,&canStdioReadEntry);

	pendingPacket.StdId = txAddr;
	xTaskCreate(d1k_STDIO_CAN_Task, "CANIO", 1024, NULL, 2, &canStdioTask);
}

/**
 * Disables the CAN STDIO module.
 */
void d1k_STDIO_CAN_DeInit( void )
{
	canStdioEnabled = DISABLE;
	// TODO: De-register handler?
}

// TODO: this is definitely not thread safe.  Is printf?
/**
 * Sends a STDIO buffer out the CAN module.
 * @param buf - Buffer to send.
 * @param len - Number of characters in the buffer.
 */
void d1k_STDIO_CAN_Send(const uint8_t *buf, size_t len)
{
	if (canStdioEnabled == DISABLE)
	{
		return;
	}

	for ( ; len > 0; len--)
	{
		if ( pendingPacket.DLC < 8 )
		{
			pendingPacket.Data[pendingPacket.DLC++] = *(buf++);
		}

		if (pendingPacket.DLC == 8)
		{
			// Use the ISR method to be safe.
			// TODO: does this have any side-effects?
			d1k_CAN_SendPacket_ISR(canStdioModule, &pendingPacket);

			// Reset the length.
			pendingPacket.DLC = 0;

			xLastPacketSent = xTaskGetTickCountFromISR();
		}
	}
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

static void d1k_STDIO_CAN_Task ( void * pvParameters )
{
	sint32 timeRemainingUntilTimout = 0;

	while(1)
	{
		timeRemainingUntilTimout = DEBUG_STDIO_PACKET_TIMOUT_MS - (xTaskGetTickCount() - xLastPacketSent);

		if (timeRemainingUntilTimout > 0)
		{
			vTaskDelay(timeRemainingUntilTimout);
		}

		// If we have data to send, send it.
		if (pendingPacket.DLC > 0)
		{
			d1k_CAN_SendPacket_ISR(canStdioModule, &pendingPacket);

			// Reset the length.
			pendingPacket.DLC = 0;
		}

		xLastPacketSent = xTaskGetTickCount();

		vTaskDelay(DEBUG_STDIO_PACKET_TIMOUT_MS);
	}

}

static void d1k_STDIO_CAN_ReadPacketHandler( CanRxMsg * packet )
{
	d1k_stdio_queueReceived(&(packet->Data[0]),packet->DLC);
}


