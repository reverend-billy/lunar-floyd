/*******************************************************************************
// Core Error Manager Message Router Commands
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


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// An enumerated list of all user defined events in the system
typedef enum
{
   // Event when control is enabled or disabled
   LUNAR_EVENTMGR_CONFIG_EVENT_Control_Disabled,
   // Event for any critical error
   LUNAR_EVENTMGR_CONFIG_EVENT_Critical_Error,
   // Value representing the number of events in the enumeration
   LUNAR_EVENTMGR_CONFIG_EVENT_Count,
   // This event is primarily a placeholder for placing an unused event in the config table
   LUNAR_EVENTMGR_CONFIG_EVENT_None
} Lunar_EventMgr_Config_Event_t;

#ifdef __cplusplus
extern "C"
}
#endif
