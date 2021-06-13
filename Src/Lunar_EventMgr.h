/*******************************************************************************
// Core Event Manager
// Description: 
// Module that serves as a loosely coupled clearing house for events
// throughout the system.  This allows modules to only know about this module
// instead of knowing about each other.
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
#include "Lunar_EventMgr_Config.h"
// Platform Includes
// Other Includes
#include <stddef.h>


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Function definition for event handler callbacks
typedef void (*Lunar_EventMgr_Event_Handler_t)(void);

// Structure defining each entry in the Config Table for this module
typedef struct
{
   // Event
   Lunar_EventMgr_Config_Event_t eventValue;

   // Event Callback
   const Lunar_EventMgr_Event_Handler_t callbackFunction;
} Lunar_EventMgr_Table_Item_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the Error Manager module.  After calling this
  *    function, it is assumed, the module is ready for use.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_EventMgr_Init(void);


/** Description:
  *    This function searches the configured event table for all
  *    entries matching the given event. For each matching event,
  *    the corresponding event handler will be called. Note that all
  *    handlers will be called immediately and care should be taken
  *    when designing the system to prevent long running event
  *    handlers.
  * Parameters:
  *    eventToTrigger :  The enumerated event to be triggered
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
void Lunar_EventMgr_HandleEvent(const Lunar_EventMgr_Config_Event_t eventToTrigger);


#ifdef __cplusplus
extern "C"
}
#endif
