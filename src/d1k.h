/********************************************************************
d1k.h

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

#ifndef D1K_H
#define D1K_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stm32f4xx.h"

//#include "d1k_can.h"
//#include "d1k_led.h"
//#include "d1k_portal.h"
//#include "d1k_stdio.h"
//#include "d1k_stdio_can.h"

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef uint8_t  uint8;
typedef int8_t   sint8;
typedef uint16_t uint16;
typedef int16_t  sint16;
typedef uint32_t uint32;
typedef int32_t  sint32;

typedef uint8	 bool;

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define true  (1)
#define false (0)

#define NVIC_SetPriorityGrouping ERROR_Use_NVIC_PriorityGroupConfig_instead

#endif /* D1K_H_ */
