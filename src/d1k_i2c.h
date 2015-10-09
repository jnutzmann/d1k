/********************************************************************
d1k_i2c.h

Copyright (c) 2014, Jonathan Nutzmann, Arlo Siemsen

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
********************************************************************/

#ifndef D1K_I2C_H
#define D1K_I2C_H

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "d1k.h"

void d1k_i2c_Init    ( I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2CInit );

void d1k_i2c_Restart ( I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction );
void d1k_i2c_Start   ( I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction );

uint8_t d1k_i2c_ReadAck  ( I2C_TypeDef* I2Cx );
uint8_t d1k_i2c_ReadNack ( I2C_TypeDef* I2Cx );
void d1k_i2c_Write       ( I2C_TypeDef* I2Cx, uint8_t data );

void d1k_i2c_Stop ( I2C_TypeDef* I2Cx );

#endif /* D1K_I2C_H_ */
