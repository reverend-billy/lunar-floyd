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
#include <string.h>
#include "stm32l1xx_hal.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/

// Value return by vendor API when pin is set
#define PIN_HIGH_VALUE (GPIO_PIN_SET)


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
  *    GPIO_PinState - The STM32 output level based on the requested
  *    enableState and pin configuration.
  * Return Value List: 
 *    GPIO_PIN_RESET - STM32 Reset (0)
 *    GPIO_PIN_SET - STM32 Set (1) 
 *  History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
GPIO_PinState GetSTM32OutputLevel(const GPIO_Channel_t channel, const bool enableState);


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize all configured pins
void GPIO_Drv_Init(void)
{
   // Variable required for calling STM32 HAL GPIO functions
   GPIO_InitTypeDef GPIO_InitStruct = {0};
	const GPIO_Config_t *gpioConfig;
	
   // Loop through each GPIO entry and configure it
   for (uint8_t channel = 0U; channel < GPIO_CHANNEL_Count; channel++)
   {	
		// Store the channel configuration for easier access
		gpioConfig = &gpioConfigTable[channel];

		// Clear the configuration structure for each pass through the loop
      memset( &GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
  
      // GPIO Ports Clock Enable
      if (gpioConfig->devicePin.port == GPIOA)
      {
         // Enable Port A clock
         __HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if (gpioConfig->devicePin.port == GPIOB)
		{
         // Enable Port B clock
         __HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if (gpioConfig->devicePin.port == GPIOC)
		{
         // Enable Port C clock
         __HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if (gpioConfig->devicePin.port == GPIOD)
		{
         // Enable Port D clock
         __HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if (gpioConfig->devicePin.port == GPIOH)
		{
         // Enable Port H clock
          __HAL_RCC_GPIOH_CLK_ENABLE();
		}
		// else Unknown port

		// Determine direction for this pin
		if (gpioConfig->direction == GPIO_DIRECTION_OUTPUT)
		{
         // Set the pin based on the configured value
         GPIO_InitStruct.Pin = gpioConfig->devicePin.pin;

			// Configure as output push/pull
         GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
         GPIO_InitStruct.Pull = GPIO_NOPULL;
         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

         // Determine the correct output value based on the intial configuration state 
         GPIO_Drv_Write((GPIO_Channel_t)channel, gpioConfig->gpioConfig.gpioOutput.initialEnableState);

         // Store the new level
         status.outputEnableState[channel] = gpioConfig->gpioConfig.gpioOutput.initialEnableState;

			// Parameters set, init the pin
         HAL_GPIO_Init(gpioConfig->devicePin.port, &GPIO_InitStruct);
		}
      else
      {
         // Configure as input. We may want to add additional modes
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

         // Set pullup/pulldown based on configuration		
         switch (gpioConfig->gpioConfig.gpioInput.pullMode)
         {
            case GPIO_PULLMODE_UP:
               GPIO_InitStruct.Pull = GPIO_PULLUP;
               break;
            case GPIO_PULLMODE_DOWN:
               GPIO_InitStruct.Pull = GPIO_PULLDOWN;
               break;
            default:
               GPIO_InitStruct.Pull = GPIO_NOPULL;
         }

         // Parameters set, init the pin
         HAL_GPIO_Init(gpioConfig->devicePin.port, &GPIO_InitStruct);
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
         // STM32 driver return GPIO_PIN_SET when high or GPIO_PIN_RESET when low         
         enableState = (HAL_GPIO_ReadPin(gpioConfig->devicePin.port, gpioConfig->devicePin.pin) == PIN_HIGH_VALUE);
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
         HAL_GPIO_WritePin(gpioConfig->devicePin.port, gpioConfig->devicePin.pin, GetSTM32OutputLevel(channel, enableState));

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
         HAL_GPIO_TogglePin(gpioConfig->devicePin.port, gpioConfig->devicePin.pin);

         // Store the toggled value
         status.outputEnableState[channel] = !status.outputEnableState[channel];
		}
   }
}


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Determine the correct output level for the XMCLib function
GPIO_PinState GetSTM32OutputLevel(const GPIO_Channel_t channel, const bool enableState)
{
   // Init the output to low
   GPIO_PinState outputLevel = GPIO_PIN_RESET;

	// Store the channel configuration for easier access
	const GPIO_Config_t *gpioConf = &gpioConfigTable[channel];

   // AL & True  = RESET
   // AH & True  = SET
   // AL & False = SET
   // AH & False = RESET
   if (((gpioConf->gpioConfig.gpioOutput.isActiveHigh) && (enableState)) || ((!gpioConf->gpioConfig.gpioOutput.isActiveHigh) && (!enableState)))
   {
      // The STM32 driver value used for high output
      outputLevel = GPIO_PIN_SET;
   }
   // else output low value

   // Finally, return the result
   return(outputLevel);
}

