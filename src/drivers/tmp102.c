/********************************************************************
tmp102.c

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

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "tmp102.h"

/****************************************************************************
 * Public Functions
 ***************************************************************************/

bool tmp102_read_temp(I2C_TypeDef *I2Cx, uint8_t address, float *temperature)
{
  uint8_t data[2];

  if(i2c_read_bytes(I2Cx, address, data, 2) == 2)
  {
    int16_t temp = data[1] | (data[0] << 8);
    temp >>= 4;

    *temperature = (float)temp / 16.0f + 273.15f;

    return true;
  }
  else
  {
    return false;
  }
}
