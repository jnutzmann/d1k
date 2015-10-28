/********************************************************************
i2c.h

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

#ifndef I2C_H
#define I2C_H

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

void i2c_init    ( I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2CInit );

void i2c_restart ( I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction );
void i2c_start   ( I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction );

uint8_t i2c_read_ack  ( I2C_TypeDef* I2Cx );
uint8_t i2c_read_nack ( I2C_TypeDef* I2Cx );
void    i2c_write     ( I2C_TypeDef* I2Cx, uint8_t data );

void i2c_stop ( I2C_TypeDef* I2Cx );

#endif /* I2C_H_ */
