/********************************************************************
d1k_stdio_can.h

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

#ifndef STDIO_CAN_H
#define STDIO_CAN_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "d1k.h"
#include "stdio.h"

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define STDIO_CAN_GENERAL_CHANNEL	(0)

/****************************************************************************
 * Public Function Prototypes
 ***************************************************************************/

void d1k_STDIO_CAN_Init( CAN_TypeDef * canModule, uint32 txAddr, uint32 rxAddr );
void d1k_STDIO_CAN_DeInit ( void );
void d1k_STDIO_CAN_Send   (const uint8_t *buf, size_t len);

#endif /* STDIO_CAN_H_ */
