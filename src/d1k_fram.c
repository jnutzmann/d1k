/********************************************************************
d1k_fram.c

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
#include "stdlib.h"
#include "stdio.h"

#include "FreeRTOS.h"

#include "d1k_i2c.h"
#include "d1k_portal.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define FRAM_I2C_SPEED_HZ	(400000)

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void d1k_fram_portalRead ( int argc, char** argv );
static void d1k_fram_portalWrite( int argc, char** argv );


/****************************************************************************
 * Global Variables
 ***************************************************************************/

static I2C_TypeDef* framI2Cx = NULL;
static uint8		framDeviceAddress = 0;

/****************************************************************************
 * Public Functions
 ***************************************************************************/
/**
 * Initialize the FRAM.
 * @param I2Cx - I2C Channel.
 * @param framSize - Size of the FRAM in bytes.
 * @param deviceAddress - Physical address of the memory (set in hardware).
 */
void d1k_fram_Init( I2C_TypeDef* I2Cx, uint32 framSize, uint8 deviceAddress )
{
	I2C_InitTypeDef I2CInit;
	I2C_StructInit(&I2CInit);

	I2CInit.I2C_Ack = I2C_Ack_Enable;
	I2CInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2CInit.I2C_ClockSpeed = FRAM_I2C_SPEED_HZ;
	I2CInit.I2C_DutyCycle = I2C_DutyCycle_2;
	I2CInit.I2C_Mode = I2C_Mode_I2C;
	I2CInit.I2C_OwnAddress1 = 0x00;

	d1k_i2c_Init( I2Cx, &I2CInit );

	framI2Cx = I2Cx;
	framDeviceAddress = deviceAddress;

	d1k_portal_RegisterFunction("framRead",  d1k_fram_portalRead );
	d1k_portal_RegisterFunction("framWrite", d1k_fram_portalWrite);
}

/**
 * Write data to FRAM.
 * @param address - Address in RAM to write to.
 * @param data - Data to write.
 * @param len - Length of the data.
 */
void d1k_fram_Write( uint16_t address, void * data, size_t len )
{
	// Start a write
	d1k_i2c_Start( framI2Cx, 0xA0 | (0x07 & framDeviceAddress),
				   I2C_Direction_Transmitter);

	// Send the memory address
	d1k_i2c_Write( framI2Cx, (address >> 8) & 0xFF  );
	d1k_i2c_Write( framI2Cx, address & 0xFF );

	// send the data
	for( uint16 i = 0; i < len; i++ )
	{
		d1k_i2c_Write(framI2Cx, ((uint8_t*)data)[i] );
	}

	d1k_i2c_Stop( framI2Cx );
}

/**
 * Read data from FRAM.
 * @param address - Address to read from.
 * @param data - Location to put the data.
 * @param len - Length of data to read.
 */
void d1k_fram_Read( uint16_t address, void * data, size_t len )
{
	// Start a write to select the address
	d1k_i2c_Start( framI2Cx, 0xA0 | (0x07 & framDeviceAddress),
				   I2C_Direction_Transmitter);

	// Send the memory address
	d1k_i2c_Write( framI2Cx, (address >> 8) & 0xFF  );
	d1k_i2c_Write( framI2Cx, address & 0xFF );

	// Restart in read mode
	d1k_i2c_Restart( framI2Cx, 0xA1 | (0x07 & framDeviceAddress),
				   I2C_Direction_Receiver);

    for (uint16 i = 0; i < len; i++)
    {
    	// Nack on the last byte
    	if ( i == len-1 )
    	{
    		((uint8*) data)[i] = d1k_i2c_ReadNack( framI2Cx );
    	}
    	else
    	{
    		((uint8*) data)[i] = d1k_i2c_ReadAck( framI2Cx );
    	}
    }

    d1k_i2c_Stop( framI2Cx );
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

static void d1k_fram_portalRead( int argc, char** argv )
{
	// command, address, length
	if (argc == 3)
	{
		long address = strtol(argv[1],NULL,0);
		long len     = strtol(argv[2],NULL,0);

		if (address < 0 || len < 0)
		{
			printf("Error.  Address and/or length cannot be negative.\n");
			return;
		}

		if (address > 0x3FFF || len > 0xFF )
		{
			printf("Error.  Address and/or length too large.\n");
			return;
		}

		uint8* data = malloc(len);

		d1k_fram_Read(address,data,len);

		printf("\nAddress: 0x%lX: ",address);

		for (int i = 0; i < len; i++)
		{
			printf("0x%X ",data[i]);
		}

		printf("\n");

		free(data);
	}
	else
	{
		printf("Invalid usage.  Need address and length arguments.\n");
	}
}

static void d1k_fram_portalWrite( int argc, char** argv )
{
	// command, address, data
	if (argc == 3)
	{
		long address = strtol(argv[1],NULL,0);
		long data     = strtol(argv[2],NULL,0);

		if (address < 0 )
		{
			printf("Error.  Address cannot be negative.\n");
			return;
		}

		if (address > 0x3FFF )
		{
			printf("Error.  Address too large.\n");
			return;
		}

		uint8 d = 0xFF & data;

		d1k_fram_Write( address, &d, 1);
	}
	else
	{
		printf("Invalid usage.  Need address and length arguments.\n");
	}
}

