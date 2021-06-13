/*******************************************************************************
// Core Event Manager Configuration
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
#include "Lunar_EventMgr.h" // Event Mgr Types
#include "Lunar_EventMgr_Config.h" // List of system errors
// Platform Includes
// Other Includes
#include "Control.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

// Table that defines a list of Events and their corresponding Event Callbacks
static const Lunar_EventMgr_Table_Item_t Lunar_EventMgr_eventConfigTable[] =
{
   // {Event, Event Callback }
   { LUNAR_EVENTMGR_CONFIG_EVENT_Critical_Error, Control_Fault }
};

#ifdef __cplusplus
extern "C"
}
#endif
