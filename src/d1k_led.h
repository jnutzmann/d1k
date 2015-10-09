/********************************************************************
d1k_led.h

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

#ifndef D1K_LED_H
#define D1K_LED_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "d1k.h"

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define D1K_LED_RED             (13)
#define D1K_LED_BLUE            (14)
#define D1K_LED_YELLOW          (15)

#define MAX_LED_COUNT			(16)

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef uint8 d1k_LED_ID_t;

/**
 * These are purposes that can be assigned to LEDs to give D1K control of their state.
 */
typedef enum {
	D1K_LED_PURPOSE_CAN = 0,
	D1K_LED_PURPOSE_ERROR,
	D1K_LED_PURPOSE_APPLICATION,
	D1K_NUM_PURPOSES
} d1k_LEDPurpose_t;

typedef struct {
	d1k_LEDPurpose_t purpose;
	uint32_t GPIO_Pin;
	GPIO_TypeDef* GIOPx;
	uint32_t GPIO_Clock;
	uint32 onTime;
	uint32 offTime;
} d1k_LEDInitStruct_t;

/****************************************************************************
 * Public Prototypes
 ***************************************************************************/

void d1k_LED_InitDefault ( void );
void d1k_LED_Init        ( d1k_LED_ID_t LEDId, d1k_LEDInitStruct_t* led );

void d1k_LED_On    ( d1k_LED_ID_t n );
void d1k_LED_Off   ( d1k_LED_ID_t n );
void d1k_LED_Toggle( d1k_LED_ID_t n );
bool d1k_LED_Flash ( d1k_LED_ID_t n, uint32 onTime, uint32 offTime );

void d1k_LED_OnPurpose    ( d1k_LEDPurpose_t n );
void d1k_LED_OffPurpose   ( d1k_LEDPurpose_t n );
bool d1k_LED_FlashPurpose ( d1k_LEDPurpose_t n, uint32 onTime, uint32 offTime );


#endif
