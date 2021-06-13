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
// Constant Configuration Variable Declarations
*******************************************************************************/

// This table defines the configuration for each GPIO pin used by the common GPIO
// driver.  Each pin will be initialized in the GPIO_Drv_Init function.
static const GPIO_Drv_ConfigItem_t gpioConfigTable[GPIO_DRV_CHANNEL_Count] =
{
   // LED 1
   {
      .direction = GPIO_DRV_DIRECTION_OUTPUT,
      .devicePin = {
         .port = XMC_GPIO_PORT0,
         .pin = 6
      },
      .gpioConfig.gpioOutput = {
         .isActiveHigh = true,
         .initialEnableState = false
      }
   },
   // LED 2
   {
      .direction = GPIO_DRV_DIRECTION_OUTPUT,
      .devicePin =
      {
         .port = XMC_GPIO_PORT0,
         .pin = 7
      },
      .gpioConfig.gpioOutput =
      {
         .isActiveHigh = true,
         .initialEnableState = false
      }
   }
};


#ifdef __cplusplus
extern "C"
}

#endif

