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

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "i2c.h"


/****************************************************************************
 * Global Variables
 ***************************************************************************/

static xSemaphoreHandle i2c_mutex[3];

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static uint8_t i2c_lock(I2C_TypeDef *I2Cx);
static void    i2c_unlock(I2C_TypeDef *I2Cx);
static uint8_t i2c_get_channel_id(I2C_TypeDef *I2Cx);

/****************************************************************************
 * Public Functions
 ***************************************************************************/

void i2c_init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_init)
{
	// Init the I2C mutex
	i2c_mutex[i2c_get_channel_id(I2Cx)] = xSemaphoreCreateMutex();

	// Enable the I2C clock.
	if      (I2Cx == I2C1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	else if (I2Cx == I2C2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	else if (I2Cx == I2C3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	I2C_Init(I2Cx, I2C_init);

	// enable I2C
	I2C_Cmd(I2Cx, ENABLE);
}

/**
 * Issues a start condition and
 * transmits the slave address + R/W bit
 * @param I2Cx
 * @param address
 * @param direction - I2C_Direction_Tranmitter or I2C_Direction_Receiver
 */
void i2c_restart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{

	// Send I2C1 START condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2Cx, address, direction);

	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(direction == I2C_Direction_Transmitter)
	{
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver)
	{
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/**
 * i2c_start - starts an i2c transaction
 * @param I2Cx - I2C module
 * @param address - I2C address (needs to be << 1 bit for some reason)
 * @param direction - either I2C_Direction_Receiver or I2C_Direction_Transmitter
 */
void i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
	i2c_lock(I2Cx);

	// wait until I2C1 is not busy anymore
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	i2c_restart(I2Cx, address, direction);
}

/**
 * i2c_read_ack - reads one byte and requests another (ACK)
 * @param I2Cx - I2C module
 * @return data byte
 */
uint8_t i2c_read_ack(I2C_TypeDef* I2Cx)
{
	// enable acknowledge of received data
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	// wait until one byte has been received
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);

	return data;
}

/**
 * i2c_read_nack - read one bytes and do not request another (NACK)
 * @param I2Cx - i2c module
 * @return data byte
 */
uint8_t i2c_read_nack( I2C_TypeDef* I2Cx )
{
	// disabe acknowledge of received data
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	// wait until one byte has been received
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);

	return data;
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
