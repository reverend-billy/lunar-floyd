/*******************************************************************************
// GPIO Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/


// Module Includes
#include "GPIO_Drv.h" // Driver API
#include "GPIO_Drv_Config.h" // Defines channel enumeration
#include "GPIO_Drv_ConfigTable.h" // Implementation Configuration
// Platform Includes
// Other Includes
#include "xmc_gpio.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/

#define PIN_HIGH_VALUE (1U)


/*******************************************************************************
// Private Types
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Current output enable state for each configured pin
   // Only used for outputs since inputs are read directly
   bool outputEnableState[GPIO_CHANNEL_Count];
} GPIO_Drv_t;


/*******************************************************************************
// Private Variables
*******************************************************************************/

// The variable used for holding all internal data for this module.
static GPIO_Drv_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    Function to start the timer for a scheduled function.
  * Parameters:
  *    channel - The configured GPIO pin that is being queried
  *    functionIndex - Index into the Lunar_Scheduler_configTable
  * Returns:
  *    XMC_GPIO_OUTPUT_LEVEL_t - The XMCLib output level based on the requested
  *    enableState and pin configuration.
  * Return Value List: 
 *    XMC_GPIO_OUTPUT_LEVEL_LOW - XMCLib Reset (0x10000U)
 *    XMC_GPIO_OUTPUT_LEVEL_HIGH  - XMCLib Set (0x1U)
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
XMC_GPIO_OUTPUT_LEVEL_t GetXMCOutputLevel(const GPIO_Channel_t channel, const bool enableState);


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize all configured pins
void GPIO_Drv_Init(void)
{
   // Variable required for calling XMCLib GPIO functions
   XMC_GPIO_CONFIG_t xmcGPIOConf;
	const GPIO_Config_t *gpioConfig;
	
   // Loop through each GPIO entry and configure it
   for (uint8_t channel = 0U; channel < GPIO_CHANNEL_Count; channel++)
   {	
		// Store the channel configuration for easier access
		gpioConfig = &gpioConfigTable[channel];

		// Clear the configuration structure for each pass through the loop
      memset( &xmcGPIOConf, 0, sizeof(XMC_GPIO_CONFIG_t) );
   
		// Determine direction for this pin
		if (gpioConfig->direction == GPIO_DIRECTION_OUTPUT)
		{
			// Configure as output push/pull
			xmcGPIOConf.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;

         // Determine the correct output value based on the intial configuration state 
         xmcGPIOConf.output_level = GetXMCOutputLevel((GPIO_Channel_t)channel, gpioConfig->gpioConfig.gpioOutput.initialEnableState);

         // Store the new level
         status.outputEnableState[channel] = gpioConfig->gpioConfig.gpioOutput.initialEnableState;

#if UC_FAMILY == XMC4
			// XMC400 has Output Strength - Default 0
			xmcGPIOConf.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE;
#endif
	
			// Parameters set, init the pin
			XMC_GPIO_Init(gpioConfig->devicePin.port, gpioConfig->devicePin.pin, &xmcGPIOConf);
		}
      else
      {
         // Configure as tri-state input. We may want to add additional modes
         xmcGPIOConf.mode = XMC_GPIO_MODE_INPUT_TRISTATE;

#if UC_FAMILY == XMC1
			// XMC1000 has input hysteresis - Default 0
			xmcGPIOConf.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD;
#endif

         // Parameters set, init the pin
         XMC_GPIO_Init(gpioConfig->devicePin.port, gpioConfig->devicePin.pin, &xmcGPIOConf);
      }
	}
}

// Read the current state of the given pin
bool GPIO_Drv_Read(const GPIO_Channel_t channel)
{
   bool enableState = false;

   // Verify the given channel is valid
   if (channel < GPIO_CHANNEL_Count)
   {
		// Store the channel configuration for easier access
		const GPIO_Config_t *gpioConfig = &gpioConfigTable[channel];
		
		// Verify the pin is configured for input
		if (gpioConfig->direction == GPIO_DIRECTION_INPUT)
		{
         // XMC GPIO returns 1 if pin is set, 0 otherwise
         enableState = (XMC_GPIO_GetInput(gpioConfig->devicePin.port, gpioConfig->devicePin.pin) == PIN_HIGH_VALUE);
		}
      else
      {
         // This is an output, just return the current output state
         enableState = status.outputEnableState[channel];
      }
	}

   // Return the pin state
   return(enableState);
}

// Writes the desired value to the given pin
void GPIO_Drv_Write(const GPIO_Channel_t channel, bool enableState)
{
   // Verify the given channel is valid
   if (channel < GPIO_CHANNEL_Count)
   {
		// Store the channel configuration for easier access
		const GPIO_Config_t *gpioConfig = &gpioConfigTable[channel];
		
		// Verify the pin is configured for output
		if (gpioConfig->direction == GPIO_DIRECTION_OUTPUT)
		{
			// Pin valid and set as output, write the new value
   		XMC_GPIO_SetOutputLevel(gpioConfig->devicePin.port, gpioConfig->devicePin.pin, GetXMCOutputLevel(channel, enableState));

         // Store the new output state
         status.outputEnableState[channel] = enableState;
		}
   }
}

// Toggle the output state of the given pin
void GPIO_Drv_Toggle(const GPIO_Channel_t channel)
{
   // Verify the given channel is valid
   if (channel < GPIO_CHANNEL_Count)
   {
		// Store the channel configuration for easier access
		const GPIO_Config_t *gpioConfig = &gpioConfigTable[channel];
		
		// Verify the pin is configured for output
		if (gpioConfig->direction == GPIO_DIRECTION_OUTPUT)
		{
			// Toggle the given pin for the given channel
			XMC_GPIO_ToggleOutput(gpioConfig->devicePin.port, gpioConfig->devicePin.pin);

         // Store the toggled value
         status.outputEnableState[channel] = !status.outputEnableState[channel];
		}
   }
}


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Determine the corect output level for the XMCLib function
XMC_GPIO_OUTPUT_LEVEL_t GetXMCOutputLevel(const GPIO_Channel_t channel, const bool enableState)
{
   // Init the output to low
   XMC_GPIO_OUTPUT_LEVEL_t outputLevel = XMC_GPIO_OUTPUT_LEVEL_LOW;

	// Store the channel configuration for easier access
	const GPIO_Config_t *xmcGPIOConf = &gpioConfigTable[channel];

   // AL & True  = RESET
   // AH & True  = SET
   // AL & False = SET
   // AH & False = RESET
   if (((xmcGPIOConf->gpioConfig.gpioOutput.isActiveHigh) && (enableState)) || ((!xmcGPIOConf->gpioConfig.gpioOutput.isActiveHigh) && (!enableState)))
   {
      // The XMC driver value used for high output
      outputLevel = XMC_GPIO_OUTPUT_LEVEL_HIGH;
   }
   // else output low value

   // Finally, return the result
   return(outputLevel);
}

