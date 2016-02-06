/********************************************************************
thermostat.c

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

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "thermostat.h"


/****************************************************************************
 * Public Functions
 ***************************************************************************/

 FunctionalState thermostat_control_heat(Thermostat_t* config)
 {
 	float min = 1000.0f;

 	// find the min (we control off that)
 	for (uint8_t i=0; i < config->temperature_count; i++)
 	{
 		if (config->temperatures[i] < min) min = config->temperatures[i];
 	}

 	if (config->state == ENABLE)
 		return min < (config->setpoint + config->hysteresis) ? ENABLE : DISABLE;
 	else
 		return min < (config->setpoint - config->hysteresis) ? ENABLE : DISABLE;
 	
 }