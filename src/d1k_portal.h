/********************************************************************
d1k_portal.h

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

#ifndef D1K_PORTAL_H
#define D1K_PORTAL_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "d1k.h"

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef void (*d1kPortalFxn)( int argc, char ** argv );

/****************************************************************************
 * Public Function Prototypes
 ***************************************************************************/

void d1k_portal_Init             ( void );
void d1k_portal_RegisterFunction ( char* cmd, d1kPortalFxn fxn);


#endif /* D1K_PORTAL_H_ */
