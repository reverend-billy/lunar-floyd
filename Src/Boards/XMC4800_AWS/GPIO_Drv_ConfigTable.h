/*******************************************************************************
// GPIO Driver Configuration
*******************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
#include "GPIO_Drv.h"
#include "GPIO_Drv_Config.h" // Defines channel enumeration
// Platform Includes
#include "DevicePin_Config.h"
// Other Includes
#include "xmc_gpio.h"


/*******************************************************************************
// Constant Configuration Variables
*******************************************************************************/

// This table defines the configuration for each GPIO pin used by the common GPIO
// driver.  Each pin will be initialized in the GPIO_Drv_Init function.
static const GPIO_Config_t gpioConfigTable[GPIO_CHANNEL_Count] =
{
   // LED 1
   {
      .direction = GPIO_DIRECTION_OUTPUT,
      .devicePin = {
         .port = XMC_GPIO_PORT4,
         .pin = 0
      },
      .gpioConfig.gpioOutput = {
         .isActiveHigh = true,
         .initialEnableState = false
      }
   },
   // LED 2
   {
      .direction = GPIO_DIRECTION_OUTPUT,
      .devicePin =
      {
         .port = XMC_GPIO_PORT4,
         .pin = 1
      },
      .gpioConfig.gpioOutput =
      {
         .isActiveHigh = true,
         .initialEnableState = true
      }
   }
};


#ifdef __cplusplus
extern "C"
}

#endif

