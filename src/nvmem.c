/********************************************************************
nvmem.c

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

#include "nvmem.h"

/****************************************************************************
 * Private variables
 ***************************************************************************/

static NvMemWriteFxn nvm_write_fxn = NULL;
static NvMemReadFxn nvm_read_fxn = NULL;

/****************************************************************************
 * Public Functions
 ***************************************************************************/

void nvmem_init ( NvMemReadFxn read_fxn, NvMemWriteFxn write_fxn )
{
    nvm_read_fxn = read_fxn;
    nvm_write_fxn = write_fxn;
}

/**
 * Write data to NvMem.
 * @param address - Address in RAM to write to.
 * @param data - Data to write.
 * @param len - Length of the data.
 */
bool nvmem_write ( uint16_t address, void * data, size_t len )
{
    if ( nvm_write_fxn != NULL )
    {
        return nvm_write_fxn(address, data, len );
    }

    return false;
}

/**
 * Read data from NvMem.
 * @param address - Address to read from.
 * @param data - Location to put the data.
 * @param len - Length of data to read.
 */
bool nvmem_read ( uint16_t address, void * data, size_t len )
{
    if ( nvm_read_fxn != NULL )
    {
        return nvm_read_fxn(address, data, len );
    }

    return false;
}
