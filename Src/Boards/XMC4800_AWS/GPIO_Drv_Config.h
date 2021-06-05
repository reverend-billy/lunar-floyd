/*******************************************************************************
// GPIO Driver Configuration
*******************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
// Public Types
*******************************************************************************/

// Defines the enumeration of the configured GPIO pins
// This must match the entries in the gpioConfigTable
typedef enum
{
   // This pin is used for device status and flashing error codes
   GPIO_LED1,
   // A second LED used for development
   GPIO_LED2,
   // This defines the total number of enumerated GPIO values
   GPIO_CHANNEL_Count
} GPIO_Channel_t;

#ifdef __cplusplus
extern "C"
}
#endif


