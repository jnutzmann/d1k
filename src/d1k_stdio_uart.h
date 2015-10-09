/********************************************************************
d1k_stdio_uart.h

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

#ifndef STDIO_UART_H
#define STDIO_UART_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "d1k.h"
#include "stdio.h"


/****************************************************************************
 * Public Function Prototypes
 ***************************************************************************/

void d1k_STDIO_UART_Init ( uint32 baudRate );
void d1k_STDIO_UART_DeInit ( void );
void d1k_STDIO_UART_Send (const uint8_t *buf, size_t len);

#endif /* STDIO_CAN_H_ */




