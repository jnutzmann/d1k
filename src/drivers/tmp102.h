/********************************************************************
tmp102.h

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

#ifndef TMP102_H
#define TMP102_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "i2c.h"
#include "stdlib.h"
#include "stdbool.h"

/****************************************************************************
 * Public Functions
 ***************************************************************************/

bool tmp102_read_temp(I2C_TypeDef *I2Cx, uint8_t address, float *temperature);

#endif
