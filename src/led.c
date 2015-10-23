/********************************************************************
led.c

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

#include "led.h"
#include "FreeRTOS.h"
#include "string.h"
#include "task.h"

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static LEDInitStruct_t leds [MAX_LED_COUNT];
static xTaskHandle flash_handles[MAX_LED_COUNT];
static bool tasks_inited = false;
static uint32_t inited_leds = 0;

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void led_flash_task ( void *pvParameters );

/****************************************************************************
 * Public Functions
 ***************************************************************************/

/**
 * Initializes LED for use.
 * @param LEDId - ID assigned to the LED.  Must be unique for each LED registered.
 * @param led - LED init structure.
 */
bool led_init ( LED_ID_t led_id, LEDInitStruct_t *led )
{
	// Clean up the array so that we know if tasks have been created.
	if ( !tasks_inited )
	{
		for (int i = 0; i < MAX_LED_COUNT; i++)
		{
			flash_handles[i] = NULL;
		}

		tasks_inited = true;
	}

    if ( led_id > MAX_LED_COUNT ) return false;

	GPIO_InitTypeDef gpio_init_struct;

	// Enable the GPIO_LED Clock
	RCC_AHB1PeriphClockCmd( led->GPIO_Clock, ENABLE );

	// Configure the GPIO_LED pin
	gpio_init_struct.GPIO_Pin = led->GPIO_Pin;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( led->GPIOx, &gpio_init_struct );
    led_off( led_id );

	memcpy( &(leds[led_id]), led, sizeof(LEDInitStruct_t) );

    inited_leds |= 1 << led_id;

    // Set up the flash, or turn it off.
	led_flash( led_id, led->on_time, led->off_time );

    return true;
}

/**
 * Turns LED on.
 * @param n - LED Unique ID.
 */
void led_on ( LED_ID_t n )
{
    if( !(( n < MAX_LED_COUNT ) && ( 1<<n & inited_leds )) ) return;
	leds[n].GPIOx->BSRRL = leds[n].GPIO_Pin;
}

/**
 * Turns LED off.
 * @param n - LED Unique ID.
 */
void led_off ( LED_ID_t n )
{
    if( !(( n < MAX_LED_COUNT ) && ( 1<<n & inited_leds )) ) return;
	leds[n].GPIOx->BSRRH = leds[n].GPIO_Pin;
}

/**
 * Toggles LED.
 * @param n - LED Unique ID.
 */
void led_toggle ( LED_ID_t n )
{
    if( !(( n < MAX_LED_COUNT ) && ( 1<<n & inited_leds )) ) return;

	if ( leds[n].GPIOx->ODR & leds[n].GPIO_Pin )
	{
		leds[n].GPIOx->BSRRH = leds[n].GPIO_Pin;
	}
	else
	{
		leds[n].GPIOx->BSRRL = leds[n].GPIO_Pin;
	}
}


/**
 * Flashes an LED.
 * @param n - Unique ID of the LED to flash.
 * @param onTime - Time the LED is on (in ms)
 * @param offTime - Time the LED is off (in ms)
 * @return TRUE if parameters are valid and the flash task is running, otherwise false.
 */
bool led_flash ( LED_ID_t n, uint32_t on_time, uint32_t off_time )
{
    if( !(( n < MAX_LED_COUNT ) && ( 1<<n & inited_leds )) ) return false;

	if ( off_time > 0 && on_time > 0 )
	{
		leds[n].off_time = off_time;
		leds[n].on_time = on_time;

		// If we have not yet created the task, create it.
		if ( flash_handles[n] == NULL )
		{
			xTaskCreate( led_flash_task, "LEDFLSH", 256, &(leds[n]), 1, &(flash_handles[n]) );
			return true;
		}

		// If a task is already created, resume it.
		else if ( eTaskGetState( flash_handles[n] ) == eSuspended ) {
			xTaskResumeFromISR( flash_handles[n] );
			return true;
		}

		// Task is already running, just update it.
		else
		{
			return true;
		}
	}
	else
	{
		// If a task is running, it will suspend itself.
		leds[n].on_time = 0;
		leds[n].off_time = 0;
		led_off(n);
	}

	return false;
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

/**
 * Task to flash LEDs
 * @param pvParameters - Unique ID of the LED to flash.
 */
static void led_flash_task ( void *pvParameters )
{
	LEDInitStruct_t * led = (LEDInitStruct_t *)pvParameters;

	while (1)
	{
		if ( led->on_time == 0 || led->off_time == 0 )
		{
			vTaskSuspend( NULL );
		}

		led->GPIOx->BSRRL = led->GPIO_Pin;

		vTaskDelay( led->on_time );

		if ( led->on_time == 0 || led->off_time == 0 )
		{
			vTaskSuspend( NULL );
		}

		led->GPIOx->BSRRH = led->GPIO_Pin;

		vTaskDelay( led->off_time );
	}
}
