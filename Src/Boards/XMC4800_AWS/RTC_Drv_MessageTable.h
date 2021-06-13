/*******************************************************************************
// Real-time Clock Driver Message Router Commands
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
#include "RTC_Drv.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
// Other Includes


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module
static const Lunar_MessageRouter_CommandTableItem_t RTC_Drv_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 1, RTC_Drv_MessageRouter_GetCurrentDateTime },
   { 2, RTC_Drv_MessageRouter_SetCurrentDateTime },
};

#ifdef __cplusplus
extern "C"
}
#endif
