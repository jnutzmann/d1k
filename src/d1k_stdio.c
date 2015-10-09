/********************************************************************
d1k_stdio.c

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
#include <stdlib.h>
#include "d1k_led.h"
#include "FreeRTOS.h"
#include "queue.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define RECEIVE_QUEUE_LENGTH		(512)

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static xQueueHandle stdioReceiveQueue = NULL;

/****************************************************************************
 * Public functions
 ***************************************************************************/

/**
 * Inits the D1K STDIO module.
 */
void d1k_stdio_Init( void )
{
	stdioReceiveQueue = xQueueCreate(RECEIVE_QUEUE_LENGTH,sizeof(uint8_t));
}

/**
 * Waits on new character to arrive on the STDIO input channel.
 * @param buf - address to put the received character.
 * @return pdTRUE if character returned successfully.  Should never be pdFALSE.
 */
portBASE_TYPE d1k_stdio_WaitForReceived(uint8* buf)
{
	return xQueueReceive(stdioReceiveQueue,buf,portMAX_DELAY);
}

/**
 * Queues a received buffer of characters.
 * @param buf - Buffer of characters to queue.
 * @param len - Number of characters in buffer.
 */
void d1k_stdio_queueReceived( uint8_t* buf, size_t len )
{
	if (stdioReceiveQueue != NULL)
	{
		while (len > 0)
		{
			xQueueSendFromISR(stdioReceiveQueue,buf,NULL);
			buf++;
			len--;
		}
	}
}

// TODO: Restructure to make more efficient?  Should be able to directly
//       send 1 character without all the fxn calls.


/**
 * Send a string out STDIO.  MIGHT NOT WORK!!!!
 * @param str - Null terminated string to send.
 */
void d1k_stdio_PutStr( const uint8_t* str )
{
	size_t len = 0;
	const uint8_t* c = str;

	// TODO: use strlen
	while (c++ != 0) len++;

	__write(1,str,len);
}

/**
 * Send a character out STDIO.
 * @param c - character to send.
 */
void d1k_stdio_PutCh( const uint8_t c )
{
	__write(1,&c,1);
}

/**
 * Writes a buffer out the STDIO channel.  Used by printf.
 * For direct use, use d1k_stdio_PutStr(...) or d1k_stdio_PutCh(...).
 * @param handle - STDIO handle.
 * @param buf - Character buffer.
 * @param len - Number of characters in the buffer.
 * @return Number of characters sent.
 */
size_t __write(int handle, const uint8_t *buf, size_t len)
{

	if ((handle == -1) || (buf == NULL))
	{
		return 0;
	}

	if (handle != 1 && handle != 2)
	{
		return -1;
	}

	d1k_STDIO_CAN_Send(buf,len);

	return len;
}


