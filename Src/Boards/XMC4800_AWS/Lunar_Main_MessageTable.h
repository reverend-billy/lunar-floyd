/*******************************************************************************
// Core Main Message Router Commands
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
#include "Lunar_Main.h"
// Platofrm Includes
#include "Lunar_MessageRouter.h"
// Other Includes


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module. The primary purpose
// is to link each Command ID to its corresponding message handler function
static const Lunar_MessageRouter_CommandTableItem_t Lunar_Main_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 0x01, Lunar_Main_MessageRouter_GetApplicationVersion },
   { 0x02, Lunar_Main_MessageRouter_GetProductID },
   { 0x03, Lunar_Main_MessageRouter_GetProductName },
   { 0x04, Lunar_Main_MessageRouter_Reset },
   { 0x05, Lunar_Main_MessageRouter_GetResetReason },
   { 0x06, Lunar_Main_MessageRouter_GetUptimeMillseconds },
};

#ifdef __cplusplus
extern "C"
}
#endif
