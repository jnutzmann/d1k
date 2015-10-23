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

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef bool (*NvMemWriteFxn) ( uint16_t address, void * data, size_t len, void* nvmem_config );
typedef bool (*NvMemReadFxn) ( uint16_t address, void * data, size_t len, void* nvmem_config );

/****************************************************************************
 * Public Functions
 ***************************************************************************/




#endif /* NVMEM_H */
