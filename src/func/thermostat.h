/********************************************************************
thermostat.h

Copyright (c) 2016, Jonathan Nutzmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

********************************************************************/

#ifndef THERMOSTAT_H
#define THERMOSTAT_H


/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stdint.h"
#include "stm32f4xx.h"

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef struct {
	float setpoint;
	float hysteresis;
	uint8_t temperature_count;
	float* temperatures;
	FunctionalState state;
} Thermostat_t;


/****************************************************************************
 * Prototypes
 ***************************************************************************/

 FunctionalState thermostat_control_heat(Thermostat_t* config);


#endif