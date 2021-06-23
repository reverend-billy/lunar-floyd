/*******************************************************************************
// LED Manager
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Icnludes
#include "LEDMgr.h"
#include "LEDMgr_ConfigTable.h"
// Platform Includes
#include "Lunar_ErrorMgr.h"
#include "Lunar_SoftTimerLib.h"
// Other Includes
#include "Control.h"
#include "Control_Config.h" // Control States
#include "GPIO_Drv.h"
#include <stdbool.h>
#include <stdint.h>


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// This is the "Off" state blink rate
#define LED_BLINK_TIME_OFF_MS    (100)
// This is the pause at the end of flashing an error code
#define LED_BLINK_TIME_OFF_PAUSE_MS (2000)
// This is the "Idle" state blink rate
#define LED_BLINK_TIME_IDLE_MS    (500)
// This is the LED blink rate if device is running
#define LED_BLINK_TIME_RUNNING_MS   (250)
// This is the "error" blink rate used to display flash codes
#define LED_BLINK_TIME_ERROR_MS     (250)
// This defines the length of the pause after displaying a flash code
#define LED_ERROR_END_OF_SEQUENCE_PAUSE_MS  (2000)
// The total number of flash code entries in the configuration table
#define NUM_FLASH_CODES (sizeof(ledMgrFlashCodeTable)/sizeof(LEDMgr_FlashCodeItem_t))


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Target number of flashes to display.
   uint8_t targetErrorCodeFlashes;

   // Counter used for displaying the proper number of error flashes.
   uint8_t currentFlashCount;

   // Variable that tracks the current LED output state
   bool currentLEDState;

   // Timer used to blink the led at the desired rate
   Lunar_SoftTimerLib_Timer_t timer;
} LED_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static LED_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    This function sets the LED output using the GPIO driver.
  * Parameters: 
  *    newEnableState - The desired output state for the LED. 
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
static void SetLedState(const bool newEnableState);


/** Description:
  *    This function toggles the current LED state using the GPIO driver.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
static void ToggleLedState(void);


/** Description:
  *    This function scans through the flash code table to see if any of the
  *    configured errors are set.  Note that only the first flash code located
  *    is returned.
  * Returns:
  *    uint8_t - The located flash code that should begin being shown.  If no
  *    code is found, 0 is returned.
  * Return Value List:
  *    0 - No flash code is was found. Blink normally.
  *    1 - Reserved since it is hard to distinguish between a normal blink.
  *    2+ - The desired number of flashes to be shown.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
static uint8_t CheckForFlashCode(void);


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Update the LED state using the GPIO Driver
static void SetLedState(const bool newEnableState)
{
   // See if we are chaging the state
   if (newEnableState != status.currentLEDState)
   {
      // State is changing, update it
		// Note that the GPIO driver will handle translation for active low
      GPIO_Drv_Write(GPIO_DRV_CHANNEL_LED_STATUS, newEnableState);
		// Store the new state
      status.currentLEDState = newEnableState;
   }
   // Else, no change
}

// Toggle the LED State
static void ToggleLedState(void)
{
	// Toggle based on the current state
   SetLedState(!status.currentLEDState);
}

// Check for a new flash code
static uint8_t CheckForFlashCode(void)
{
   // Initialize the flash code code to none
   uint8_t locatedFlashCode = 0U;

   // Start at the first table entry
   uint8_t tableIndex = 0U;

   // Loop through until the end of the table or we find a flash code
   while ((tableIndex < NUM_FLASH_CODES) && (locatedFlashCode == 0))
   {
      // See if the error for the current index is set
      if (Lunar_ErrorMgr_GetErrorState(ledMgrFlashCodeTable[tableIndex].error))
      {
         // We found an error, store the flash count and exit
    	  locatedFlashCode = ledMgrFlashCodeTable[tableIndex].numFlashes;
      }

      // Move to the next index
      tableIndex++;
   }

   // Return the number of flashes, if any
   return(locatedFlashCode);
}


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize the module
void LEDMgr_Init(void)
{
   //-----------------------------------------------
   // Local Variable Initialization
   //-----------------------------------------------
   // No error flashes by default
   status.targetErrorCodeFlashes = 0;
   // error flash count starts at zero.
   status.currentFlashCount = 0;
   // Initialize the timer
   Lunar_SoftTimerLib_Init(&status.timer);

   // Make sure LED is off to begin with
	// GPIO_Drv_Init will init the output to active high or low based on the config info given
	GPIO_Drv_Write(GPIO_DRV_CHANNEL_LED_STATUS, false);
	status.currentLEDState = false;	

   // Start the LED at the normal blink rate
   Lunar_SoftTimerLib_StartTimer(&status.timer, (uint32_t)LED_BLINK_TIME_IDLE_MS);
}

// Scheduled function for updating the LED
void LEDMgr_Update(void)
{
   // Blink Rate depending on current state in ms
   uint32_t blinkTimeMs = (uint32_t)LED_BLINK_TIME_IDLE_MS;

   //-----------------------------------------------
   // Error Code Blink
   //-----------------------------------------------
   if (status.targetErrorCodeFlashes > 0)
   {
      // There is an error code to display, increment the flash count on the
      // rising edge of the flash (LED on) and start the pause on the falling
      // edge (LED off) of the last pulse.

      // See if the timer has expired
      if (Lunar_SoftTimerLib_IsTimerExpired(&status.timer))
      {
         // Timer has expired, so toggle the LED
         ToggleLedState();

         // Initialize the timer up for the error flash time.
         blinkTimeMs = (uint32_t)LED_BLINK_TIME_ERROR_MS;

         // Do we still have more flashes to display?
         if (status.currentFlashCount < status.targetErrorCodeFlashes)
         {
            // We have not output all flashes yet, increment the flash count
            // each time the LED is turned on.
            if (status.currentLEDState)
            {
               status.currentFlashCount++;
            }
         }
         else
         {
            // We have output all of the LED flashes, but we must wait for the
            // LED to turn off so we get a complete last flash.  Once the LED
            // turns off, reset the flash count and then pause to make the
            // flash code discernible.
            if (!status.currentLEDState)
            {
               // Reset the flash count
               status.currentFlashCount = 0;

               // Over-ride the timer for the end of the sequence pause.
               blinkTimeMs = (uint32_t)LED_ERROR_END_OF_SEQUENCE_PAUSE_MS;

               // Check for new errors in-case things have changed since we
               // started blinking the current error code
               status.targetErrorCodeFlashes = CheckForFlashCode();
            }
         }

         // Restart the timer with the appropriate timing
         Lunar_SoftTimerLib_StartTimer(&status.timer, blinkTimeMs);
      }
   }
   //-----------------------------------------------
   // Normal LED Blink
   //-----------------------------------------------
   else
   {
      // Check to see if any exist now
      status.targetErrorCodeFlashes = CheckForFlashCode();

      // See if the control is enabled
      if (Control_GetState() == CONTROL_ACTIVE_STATE)
      {
         // Control enabled -- fast blink
         blinkTimeMs = (uint32_t)LED_BLINK_TIME_RUNNING_MS;
      }
      else
      {
         // Control disabled -- slow blink
         blinkTimeMs = (uint32_t)LED_BLINK_TIME_IDLE_MS;
      }

      // See if the timer has expired
      if (Lunar_SoftTimerLib_IsTimerExpired(&status.timer))
      {
         // Timer has expired, so toggle the LED
         ToggleLedState();

         // Restart the timer with the normal blink rate
         Lunar_SoftTimerLib_StartTimer(&status.timer, blinkTimeMs);
      }
   }
}


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/

// Get flash code in progress
void LEDMgr_MessageRouter_GetFlashCode(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Flash code currently being displayed.
      uint8_t flashCode;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      // Return the requested channel index
      response->flashCode = status.targetErrorCodeFlashes;


      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}
