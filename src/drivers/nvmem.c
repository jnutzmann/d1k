/********************************************************************
fram.c

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

/****************************************************************************
 * Includes
 ***************************************************************************/
#include "nvmem.h"
#include "stdio.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define FRAM_I2C_SPEED_HZ	(400000)


/****************************************************************************
 * Global Variables
 ***************************************************************************/

static I2C_TypeDef* fram_I2Cx = NULL;
static uint8_t		fram_device_address = 0;

/****************************************************************************
 * Public Functions
 ***************************************************************************/
/**
 * Initialize the FRAM.
 * @param I2Cx - I2C Channel.
 * @param framSize - Size of the FRAM in bytes.
 * @param deviceAddress - Physical address of the memory (set in hardware).
 */
void nvmem_init(I2C_TypeDef *I2Cx, uint32_t fram_size, uint8_t device_address)
{
	I2C_InitTypeDef i2c_init_struct;
	I2C_StructInit(&i2c_init_struct);

	i2c_init_struct.I2C_Ack = I2C_Ack_Enable;
	i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_init_struct.I2C_ClockSpeed = FRAM_I2C_SPEED_HZ;
	i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
	i2c_init_struct.I2C_OwnAddress1 = 0x00;

	i2c_init( I2Cx, &i2c_init_struct);

	fram_I2Cx = I2Cx;
	fram_device_address = device_address;
}

/**
 * Write data to FRAM.
 * @param address - Address in RAM to write to.
 * @param data - Data to write.
 * @param len - Length of the data.
 */
void nvmem_write( uint16_t address, void * data, size_t len )
{
	// Start a write
	i2c_start(fram_I2Cx, 0xA0 | (0x07 & fram_device_address),
				   I2C_Direction_Transmitter);

	// Send the memory address
	i2c_write(fram_I2Cx, (address >> 8) & 0xFF  );
	i2c_write(fram_I2Cx, address & 0xFF );

	// send the data
	for( uint16_t i = 0; i < len; i++ )
	{
		i2c_write(fram_I2Cx, ((uint8_t*)data)[i] );
	}

	i2c_stop(fram_I2Cx);
}

/**
 * Read data from FRAM.
 * @param address - Address to read from.
 * @param data - Location to put the data.
 * @param len - Length of data to read.
 */
void nvmem_read( uint16_t address, void * data, size_t len )
{
	// Start a write to select the address
	i2c_start(fram_I2Cx, 0xA0 | (0x07 & fram_device_address),
				   I2C_Direction_Transmitter);

	// Send the memory address
	i2c_write(fram_I2Cx, (address >> 8) & 0xFF  );
	i2c_write(fram_I2Cx, address & 0xFF );

	// Restart in read mode
	i2c_restart(fram_I2Cx, 0xA1 | (0x07 & fram_device_address),
				   I2C_Direction_Receiver);

    for (uint16_t i = 0; i < len; i++)
    {
    	// Nack on the last byte
    	if ( i == len-1 )
    	{
    		((uint8_t*) data)[i] = i2c_read_nack(fram_I2Cx);
    	}
    	else
    	{
    		((uint8_t*) data)[i] = i2c_read_ack(fram_I2Cx);
    	}
    }

    i2c_stop(fram_I2Cx);
}
