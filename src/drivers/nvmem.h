/********************************************************************
fram.h

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

#ifndef NVMEM_H
#define NVMEM_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "drivers/i2c.h"
#include "stdlib.h"

/****************************************************************************
 * Public Functions
 ***************************************************************************/

void fram_Init  ( I2C_TypeDef* I2Cx, uint32_t framSize, uint8_t deviceAddress );
void fram_Write ( uint16_t address, void * data, size_t len );
void fram_Read  ( uint16_t address, void * data, size_t len );


#endif /* NVMEM_H */
