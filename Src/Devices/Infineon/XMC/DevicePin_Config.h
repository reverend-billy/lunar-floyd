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
#include "xmc_gpio.h" // Defines the port type


/*******************************************************************************
// Public Constants
*******************************************************************************/


/*******************************************************************************
// Public Types
*******************************************************************************/

// This is the pin definition values used by the XMCLib.  Each processor library
// will have a diffent way of specifying these.
typedef struct
{
	// XMCLib port definition
	// It will be a pointer to the register for the the port
   XMC_GPIO_PORT_t *port;
	// XMCLib just uses uint8_t for defining the pin for a specific port
	// The API will often use the value for shifting to build a mask
   uint8_t pin;
} DevicePin_Config_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/


#ifdef __cplusplus
}
#endif
