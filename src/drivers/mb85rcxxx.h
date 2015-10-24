/********************************************************************
mb85rcxxx.h

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

#ifndef MC85RC_H
#define MC85RC_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "i2c.h"
#include "stdlib.h"

/****************************************************************************
 * Public Functions
 ***************************************************************************/

void mb85rcxx_init ( I2C_TypeDef *I2Cx, uint32_t size, uint8_t device_address );

#endif /* NVMEM_H */
