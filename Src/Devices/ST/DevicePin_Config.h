/*******************************************************************************
// Device Pin Definition
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
// Platform Includes
// Other Includes
#include <stdint.h> // For uint8_t type
#include "stm32l1xx_hal.h" // Defines the port type


/*******************************************************************************
// Public Constants
*******************************************************************************/


/*******************************************************************************
// Public Types
*******************************************************************************/

// Define port enumeration used for configuration
typedef enum 
{
	STM32_PORT_A,
	STM32_PORT_B,
	STM32_PORT_C,
	STM32_PORT_D,
	STM32_PORT_H,
} STM32_PORT_t;

// This is the pin definition values used by the XMCLib.  Each processor library
// will have a diffent way of specifying these.
typedef struct
{
	// STM32 HAL port definition
   //uint32_t  port;
	GPIO_TypeDef *port;
	// STM32 uses uint32_t for defining the pin for a specific port but
   // The pin should be be specified as GPIO_PIN_0 - GPIO_PIN_15
   uint32_t pin;
} DevicePin_Config_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/


#ifdef __cplusplus
}
#endif
