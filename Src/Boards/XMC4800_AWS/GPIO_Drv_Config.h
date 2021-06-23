/*******************************************************************************
// GPIO Driver Configuration
*******************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Defines the enumeration of the configured GPIO pins
// This must match the entries in the gpioConfigTable
typedef enum
{
   // This pin is used for device status and flashing error codes
   GPIO_DRV_CHANNEL_LED_STATUS,
   // LED enabled to denote fault status
   GPIO_DRV_CHANNEL_LED_FAULT,
   // This defines the total number of enumerated GPIO values
   GPIO_DRV_CHANNEL_Count
} GPIO_Drv_Channel_t;


#ifdef __cplusplus
extern "C"
}
#endif


