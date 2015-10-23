/********************************************************************
led.h

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

#ifndef LED_H
#define LED_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stdbool.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define MAX_LED_COUNT			(16)

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef uint8_t LED_ID_t;

/**
 * These are purposes that can be assigned to LEDs to give D1K control of their state.
 */
typedef enum {
	LED_CAN = 0,
	LED_ERROR,
	LED_COUNT_CORE
} LEDPurpose_t;

typedef struct {
	uint32_t GPIO_Pin;
	GPIO_TypeDef* GPIOx;
	uint32_t GPIO_Clock;
	uint32_t on_time;
	uint32_t off_time;
} LEDInitStruct_t;

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

bool led_init(LED_ID_t led_id, LEDInitStruct_t *led);

void led_on(LED_ID_t n);
void led_off(LED_ID_t n);
void led_toggle(LED_ID_t n);
bool led_flash(LED_ID_t n, uint32_t on_time, uint32_t off_time);

#endif
