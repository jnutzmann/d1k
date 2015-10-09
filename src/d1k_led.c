/********************************************************************
d1k_led.c

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

#include "FreeRTOS.h"
#include "task.h"

#include "d1k_led.h"
#include "string.h"

/****************************************************************************
 * Defines
 ***************************************************************************/

#define D1K_LED_GPIO_PORT                   GPIOC
#define D1K_LED_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define D1K_LEDB_PIN                        GPIO_Pin_13
#define D1K_LEDR_PIN                        GPIO_Pin_14
#define D1K_LEDY_PIN                        GPIO_Pin_15

#define D1K_LEDB_PIN                        GPIO_Pin_13
#define D1K_LEDR_PIN                        GPIO_Pin_14
#define D1K_LEDY_PIN                        GPIO_Pin_15

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static d1k_LEDInitStruct_t leds [MAX_LED_COUNT];
static d1k_LED_ID_t purposes [D1K_NUM_PURPOSES];
static xTaskHandle flashHandles [MAX_LED_COUNT];
static bool tasksInited = false;

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void d1k_LED_Flash_Task ( void * pvParameters );

/****************************************************************************
 * Public Functions
 ***************************************************************************/

/**
 * Initializes LEDs to default configuration for boards used on D1.
 */
void d1k_LED_InitDefault ( void )
{
	d1k_LEDInitStruct_t l;

	l.GIOPx = D1K_LED_GPIO_PORT;
	l.GPIO_Clock = RCC_AHB1Periph_GPIOC;
	l.offTime = 0;
	l.onTime = 0;
	l.purpose = D1K_LED_PURPOSE_APPLICATION;
	l.GPIO_Pin = D1K_LEDB_PIN;
	d1k_LED_Init( D1K_LED_BLUE, &l );

	l.GPIO_Pin = D1K_LEDR_PIN;
	l.purpose = D1K_LED_PURPOSE_ERROR;
	d1k_LED_Init( D1K_LED_RED, &l );

	l.GPIO_Pin = D1K_LEDY_PIN;
	l.purpose = D1K_LED_PURPOSE_CAN;
	d1k_LED_Init( D1K_LED_YELLOW, &l );

}

/**
 * Initializes LED for use.
 * @param LEDId - ID assigned to the LED.  Must be unique for each LED registered.
 * @param led - LED init structure.
 */
void d1k_LED_Init ( d1k_LED_ID_t LEDId, d1k_LEDInitStruct_t* led )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(led->GPIO_Clock, ENABLE);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = led->GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(led->GIOPx, &GPIO_InitStructure);

	memcpy( &(leds[LEDId]), led, sizeof(d1k_LEDInitStruct_t) );

	// If set up to flash, do that.  Otherwise, turn it off.
	if (!d1k_LED_Flash(LEDId,led->onTime,led->offTime))
	{
		d1k_LED_Off( LEDId );
	}

	purposes[led->purpose] = LEDId;

	// Clean up the array so that we know if tasks have been created.
	if (!tasksInited)
	{
		for ( uint16 i = 0; i < MAX_LED_COUNT; i++ )
		{
			flashHandles[i] = NULL;
		}

		tasksInited = true;
	}
}

/**
 * Turns LED on.
 * @param n - LED Unique ID.
 */
void d1k_LED_On( d1k_LED_ID_t n )
{
	leds[n].GIOPx->BSRRL = leds[n].GPIO_Pin;
}

/**
 * Turns LED off.
 * @param n - LED Unique ID.
 */
void d1k_LED_Off( d1k_LED_ID_t n )
{
	leds[n].GIOPx->BSRRH = leds[n].GPIO_Pin;
}

/**
 * Toggles LED.
 * @param n - LED Unique ID.
 */
void d1k_LED_Toggle( d1k_LED_ID_t n )
{
	if ( leds[n].GIOPx->ODR & leds[n].GPIO_Pin )
	{
		leds[n].GIOPx->BSRRH = leds[n].GPIO_Pin;
	}
	else
	{
		leds[n].GIOPx->BSRRL = leds[n].GPIO_Pin;
	}


}


/**
 * Flashes an LED.
 * @param n - Unique ID of the LED to flash.
 * @param onTime - Time the LED is on (in ms)
 * @param offTime - Time the LED is off (in ms)
 * @return TRUE if parameters are valid and the flash task is running, otherwise false.
 */
bool d1k_LED_Flash ( d1k_LED_ID_t n, uint32 onTime, uint32 offTime )
{
	if ( offTime > 0 && onTime > 0 )
	{
		leds[n].offTime = offTime;
		leds[n].onTime = onTime;

		// If we have not yet created the task, create it.
		if ( flashHandles[n] == NULL )
		{
			xTaskCreate(d1k_LED_Flash_Task,"LEDFLSH",256,&(leds[n]),1,&(flashHandles[n]));
			return true;
		}

		// If a task is already created, resume it.
		if ( eTaskStateGet( flashHandles[n] ) == eSuspended )
		{
			xTaskResumeFromISR( flashHandles[n] );
			return true;
		}

		// Task is already running, just update it.
		return true;
	}
	else
	{
		// If a task is running, it will suspend itself.
		leds[n].onTime = 0;
		leds[n].offTime = 0;
		d1k_LED_Off(n);
	}

	return false;
}

/**
 * Turns LED on.
 * @param n - Purpose of the LED to turn on.
 */
void d1k_LED_OnPurpose( d1k_LEDPurpose_t n )
{
	d1k_LED_On(purposes[n]);
}

/**
 * Turns LED off.
 * @param n - Purpose of the LED to turn on.
 */
void d1k_LED_OffPurpose( d1k_LEDPurpose_t n )
{
	d1k_LED_Off(purposes[n]);
}

/**
 * Flashes an LED.
 * @param n - Purpose of the LED to flash.
 * @param onTime - Time the LED is on (in ms)
 * @param offTime - Time the LED is off (in ms)
 * @return TRUE if parameters are valid and the flash task is running, otherwise false.
 */
bool d1k_LED_FlashPurpose( d1k_LEDPurpose_t n, uint32 onTime, uint32 offTime )
{
	return d1k_LED_Flash( purposes[n], onTime, offTime );
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

/**
 * Task to flash LEDs
 * @param pvParameters - Unique ID of the LED to flash.
 */
static void d1k_LED_Flash_Task ( void * pvParameters )
{
	d1k_LEDInitStruct_t* led = (d1k_LEDInitStruct_t*)pvParameters;

	while (1)
	{
		if (led->onTime == 0 || led->offTime == 0)
		{
			vTaskSuspend(NULL);
		}

		led->GIOPx->BSRRL = led->GPIO_Pin;

		vTaskDelay(led->onTime);

		if (led->onTime == 0 || led->offTime == 0)
		{
			vTaskSuspend(NULL);
		}
		led->GIOPx->BSRRH = led->GPIO_Pin;
		vTaskDelay(led->offTime);
	}
}
