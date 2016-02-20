/********************************************************************
rtc.h

Copyright (c) 2015, Jonathan Nutzmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
********************************************************************/

#ifndef RTC_H
#define RTC_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stdint.h"


/****************************************************************************
 * Typedefs
 ***************************************************************************/


/****************************************************************************
 * Public Functions
 ***************************************************************************/

void rtc_init( void );
void rtc_set( uint32_t time );
uint32_t rtc_get( void );

#endif /* RTC_H */
