/********************************************************************
d1k_stdio.h

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

#ifndef D1K_STDIO_H
#define D1K_STDIO_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stdio.h"
#include "d1k.h"
#include "FreeRTOS.h"


/****************************************************************************
 * Public Function Prototypes
 ***************************************************************************/

void          d1k_stdio_queueReceived   ( uint8_t* buf, size_t len );
portBASE_TYPE d1k_stdio_WaitForReceived (uint8* buf);
void          d1k_stdio_Init            ( void );

void          d1k_stdio_PutStr ( const uint8_t* str );
void          d1k_stdio_PutCh  ( const uint8_t c );

size_t __write(int handle, const uint8_t *buf, size_t len);

#endif /* D1K_STDIO_H_ */
