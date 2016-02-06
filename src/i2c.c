/********************************************************************
i2c.c

Copyright (c) 2015, Jonathan Nutzmann, Arlo Siemsen

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

#include "stdbool.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"


#include "FreeRTOS.h"
#include "semphr.h"

#include "i2c.h"

/****************************************************************************
 * Definitions
 ***************************************************************************/

#define I2C_TIMEOUT 5000

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static xSemaphoreHandle i2c_mutex[3];

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static uint8_t i2c_lock           ( I2C_TypeDef *I2Cx );
static void    i2c_unlock         ( I2C_TypeDef *I2Cx );
static uint8_t i2c_get_channel_id ( I2C_TypeDef *I2Cx );

/****************************************************************************
 * Public Functions
 ***************************************************************************/

void i2c_init( GPIODefStruct_t* sda_pin, GPIODefStruct_t* scl_pin,
               I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_init )
{
  RCC_AHB1PeriphClockCmd( sda_pin->RCC_AHB1Periph, ENABLE );
  RCC_AHB1PeriphClockCmd( scl_pin->RCC_AHB1Periph, ENABLE );

  GPIO_InitTypeDef gpio_init_struct = {
      .GPIO_Pin = sda_pin->GPIO_Pin,
      .GPIO_Mode = GPIO_Mode_AF,
      .GPIO_PuPd = GPIO_PuPd_NOPULL,
      .GPIO_OType = GPIO_OType_OD,
      .GPIO_Speed = GPIO_Speed_50MHz
  };

  GPIO_Init( sda_pin->GPIOx, &gpio_init_struct );

  gpio_init_struct.GPIO_Pin = scl_pin->GPIO_Pin;
  GPIO_Init( scl_pin->GPIOx, &gpio_init_struct );

  GPIO_PinAFConfig(sda_pin->GPIOx, sda_pin->GPIO_PinSource, GPIO_AF_I2C1);
  GPIO_PinAFConfig(scl_pin->GPIOx, scl_pin->GPIO_PinSource, GPIO_AF_I2C1);

	// Init the I2C mutex
	i2c_mutex[i2c_get_channel_id(I2Cx)] = xSemaphoreCreateMutex();

	// Enable the I2C clock.
	if      (I2Cx == I2C1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	else if (I2Cx == I2C2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	else if (I2Cx == I2C3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

  I2C_DeInit(I2Cx);
  I2C_Cmd(I2Cx, ENABLE);

	I2C_Init(I2Cx, I2C_init);
}

/**
 * i2c_start - starts an i2c transaction
 * @param I2Cx - I2C module
 * @param address - I2C address (needs to be << 1 bit for some reason)
 * @param direction - either I2C_Direction_Receiver or I2C_Direction_Transmitter
 */
bool i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
  if (!i2c_lock(I2Cx)) return false;

  uint32_t timeout = I2C_TIMEOUT;

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) {
    timeout--;
    if (timeout == 0) return false;
  };

  return i2c_restart(I2Cx, address, direction);
}

/**
 * Issues a start condition and
 * transmits the slave address + R/W bit
 * @param I2Cx
 * @param address
 * @param direction - I2C_Direction_Tranmitter or I2C_Direction_Receiver
 */
bool i2c_restart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
	// Send I2C1 START condition
	I2C_GenerateSTART(I2Cx, ENABLE);

  uint32_t timeout = I2C_TIMEOUT;

  // Check slave acked the start
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    timeout--;
    if (timeout == 0) return false;
  }

	// Send slave Address for write
	I2C_Send7bitAddress(I2Cx, address << 1, direction);

  timeout = I2C_TIMEOUT;

	// check for proper ack
	if(direction == I2C_Direction_Transmitter)
	{
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      timeout--;
      if (timeout == 0) return false;
    }
	}
	else if(direction == I2C_Direction_Receiver)
	{
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      timeout--;
      if (timeout == 0) return false;
    }
	}
  return true;
}


uint16_t i2c_read_bytes(I2C_TypeDef *I2Cx, uint8_t address, uint8_t *buf,
                  uint16_t bytes)
{
  if (!i2c_start( I2Cx, address, I2C_Direction_Receiver )) return 0;

  uint16_t count = 0;

  // receive everything but the last byte which needs to be NAKd
  while(count < (bytes - 1))
  {
    if (!i2c_read(I2Cx, ENABLE, &buf[count]))
    {
      i2c_stop( I2Cx );
      return count;
    }
    count++;
  }

  if (i2c_read(I2Cx, DISABLE, &buf[count]))
  {
    count++;
  }

  i2c_stop( I2Cx );
  return count;
}

/**
 * i2c_read_ack - reads one byte and requests another (ACK)
 * @param I2Cx - I2C module
 * @return data byte
 */
uint8_t i2c_read( I2C_TypeDef* I2Cx, FunctionalState ack, uint8_t* data )
{
	// enable acknowledge of received data
	I2C_AcknowledgeConfig(I2Cx, ack);

  uint32_t timeout = I2C_TIMEOUT;

	// wait until one byte has been received
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE)) // I2C_FLAG_RXNE
  {
    timeout--;
    if (timeout == 0) {
      return false;
    }
  }

	// read data from I2C data register and return data byte
	*data = I2C_ReceiveData(I2Cx);
	return true;
}

/**
 * i2c_stop - releases the bus and unlock
 * @param I2Cx - i2c module
 */
void i2c_stop( I2C_TypeDef* I2Cx )
{
	// Send I2C1 STOP Condition
	I2C_GenerateSTOP( I2Cx, ENABLE );
	i2c_unlock(I2Cx);
}

/**
 * i2c_write
 * @param I2Cx - I2C module
 * @param data - data byte
 */
void i2c_write( I2C_TypeDef* I2Cx, uint8_t data )
{
	I2C_SendData(I2Cx, data);

	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

static uint8_t i2c_get_channel_id(I2C_TypeDef *I2Cx)
{
	if      (I2Cx == I2C1) return 0;
	else if (I2Cx == I2C2) return 1;
	else 				   return 2;
}

static uint8_t i2c_lock(I2C_TypeDef *I2Cx)
{
	return xSemaphoreTake(i2c_mutex[i2c_get_channel_id(I2Cx)], 1000);
}

static void i2c_unlock(I2C_TypeDef *I2Cx)
{
	xSemaphoreGive(i2c_mutex[i2c_get_channel_id(I2Cx)]);
}
