#include "gpio.h"

void gpio_init_output( const GPIODefStruct_t *pin, GPIOOType_TypeDef otype, 
		       GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed )
{
  // Build the init struct off the parameters.
  GPIO_InitTypeDef gpio_init_struct = {
    .GPIO_Mode = GPIO_Mode_OUT,
    .GPIO_OType = otype,
    .GPIO_PuPd = pupd,
    .GPIO_Speed = speed,
    .GPIO_Pin = pin->GPIO_Pin
  };

  // Enable the clock for the GPIO bank (in case it is not already enabled)
  RCC_AHB1PeriphClockCmd( pin->RCC_AHB1Periph, ENABLE );

  // Initialize the pin.
  GPIO_Init( pin->GPIOx, &gpio_init_struct );
}

