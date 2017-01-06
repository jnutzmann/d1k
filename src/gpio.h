/********************************************************************
gpio.h

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

#ifndef GPIO_H
#define GPIO_H

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef struct {
    uint32_t GPIO_Pin;
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_PinSource;
    uint32_t RCC_AHB1Periph;
} GPIODefStruct_t;

void gpio_init_output(
    const GPIODefStruct_t *pin,
    GPIOOType_TypeDef otype, // GPIO_OType_PP, GPIO_OType_OD
    GPIOPuPd_TypeDef pupd,   // GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN
    GPIOSpeed_TypeDef speed  //GPIO_Speed_2MHz (2, 25, 50, 100)
);

#endif
