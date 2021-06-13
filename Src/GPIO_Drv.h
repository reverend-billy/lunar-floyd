/*******************************************************************************
// GPIO Driver
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
#include "GPIO_Drv_Config.h" // Defines the GPIO channel enumeration
// Platform Includes
#include "DevicePin_Config.h"
// Other Includes
#include <stdbool.h> // Boolean type


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Enumeration to defines the direction of the GPIO pin used for configuration
typedef enum 
{
   // Pin is to be configured as an input
   GPIO_DRV_DIRECTION_INPUT,
   // Pin is to be configured as an output
   GPIO_DRV_DIRECTION_OUTPUT,
   // This defines the total number of enumerated GPIO Direction values
   GPIO_DRV_DIRECTION_Count
} GPIO_Drv_Direction_t;

// Enumeration to defines the direction of the GPIO pin used for configuration
typedef enum 
{
   // Pin has no pull mode (floating)
   GPIO_DRV_PULLMODE_NONE,
   // Pin is configured with pullup
   GPIO_DRV_PULLMODE_UP,
   // Pin is configured with pulldown
   GPIO_DRV_PULLMODE_DOWN,
   // This defines the total number of enumerated GPIO Pull Modes
   GPIO_DRV_PULLMODE_Count
} GPIO_Drv_PullMode_t;

// Structure to define the configuration options for a GPIO input
typedef struct
{
   // Mode for pullup/pulldown configuration
   GPIO_Drv_PullMode_t pullMode;
   // Not implemented
   uint16_t debounceCount;
} GPIO_Drv_InputConfig_t;

// Structure to define the configuration options for a GPIO output
typedef struct
{
   // Defines if this pin should be configured as active high or active low
   bool isActiveHigh;
   /** Defines the initial state for the pin upon initialization.
     * \Note that this value is inverted by the driver for active
     * low pins.                                                 
   */
   bool initialEnableState;
} GPIO_Drv_OutputConfig_t;

// This is the structure for each entry in the GPIO configuration table
typedef struct
{
   // Defines the input or output state for the pin
   GPIO_Drv_Direction_t direction;

   // The provides a structure to define the device-specific port and pin.
   // This prevents requiring device driver references in this structure directly.
   DevicePin_Config_t devicePin;

   // Configuration settings specific to either GPIO input or output configuration
   union
   {
		// Options when pin is configured as input
      GPIO_Drv_InputConfig_t gpioInput;
		/// Option when pin is configured as output
      GPIO_Drv_OutputConfig_t gpioOutput;
   } gpioConfig;
} GPIO_Drv_ConfigItem_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the hardware specific SysTick for the device.
  *    Typically the SysTick is used for the system Timebase used for software
  *    timers and function scheduling.  Some devices may be able to read
  *    directly from a SysTick register.  
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void GPIO_Drv_Init(void);


/** Description:
  *    This function toggles the specified GPIO output.  The current output 
  *    state is tracked by the driver. The pins are referenced by the configured
  *    GPIO driver enumeration.
  * Parameters: 
  *    channel - The configured GPIO pin that is to be toggled.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void GPIO_Drv_Toggle(const GPIO_Drv_Channel_t channel);


/** Description:
  *    This function toggles the specified GPIO output.  The current output 
  *    state is tracked by the driver. The pins are referenced by the configured
  *    GPIO driver enumeration.  The enableState will be inverted if the pin is
  *    configured as active low.
  * Parameters: 
  *    channel - The configured GPIO pin that is to be toggled.
  * Returns: 
  *    bool - The current state of the given pin.
  * Return Value List: 
  *    true - Configured active state
  *    false - Configured inactive state
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
bool GPIO_Drv_Read(const GPIO_Drv_Channel_t channel);


/** Description:
  *    This function writes to the specified GPIO output.  The current output 
  *    state is tracked by the driver. The pins are referenced by the configured
  *    GPIO driver enumeration.  The enableState will be inverted if the pin is
  *    configured as active low.
  * Parameters: 
  *    channel - The configured GPIO pin that is to be toggled.
  *    enableState - The desired output state to be written. (true: Configured
  *    active state, false: Configured inactive state)
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void GPIO_Drv_Write(const GPIO_Drv_Channel_t channel, const bool enableState);


#ifdef __cplusplus
}
#endif

