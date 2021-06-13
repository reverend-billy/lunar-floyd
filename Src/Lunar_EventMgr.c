/*******************************************************************************
// Core Event Manager
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
#include "Lunar_EventMgr.h"
#include "Lunar_EventMgr_Config.h"
#include "Lunar_EventMgr_ConfigTable.h"
// Platform Includes
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Private Constants
*******************************************************************************/

//This defines the number of entries in the Event Manager Configuration Table.                                                            
#define NUM_EVENT_HANDLERS (sizeof(Lunar_EventMgr_eventConfigTable)/sizeof(Lunar_EventMgr_Table_Item_t))


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize the module
void Lunar_EventMgr_Init(void)
{
   // Nothing to do
}

// Trigger each event handler for the given event
void Lunar_EventMgr_HandleEvent(const Lunar_EventMgr_Config_Event_t eventToTrigger)
{
   // Validate the event is valid
   if (eventToTrigger < LUNAR_EVENTMGR_CONFIG_EVENT_Count)
   {
      // Event is valid
      // Loop through every entry in the table and check for a match
      // For every match, call the callback
      for (uint8_t i = 0U; i < NUM_EVENT_HANDLERS; i++)
      {
         // Check the current item for a match
         if (Lunar_EventMgr_eventConfigTable[i].eventValue == eventToTrigger)
         {
            // Event matches, see if the callback is valid
            if (Lunar_EventMgr_eventConfigTable[i].callbackFunction != 0)
            {
               // Handler is valid, call it
               Lunar_EventMgr_eventConfigTable[i].callbackFunction();
            }
            // Continue checking the rest of the table
         }
      }
   }

}

