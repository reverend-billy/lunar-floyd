/*******************************************************************************
// Control Message Router Commands
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
#include "Control.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
// Other Includes
#include <stddef.h>


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module.
static const Lunar_MessageRouter_CommandTableItem_t Control_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 0x01, Control_MessageRouter_GetState },
   { 0x02, Control_MessageRouter_SetState }
};

// Control has a second message table just to align the commands with CAN
// This makes testing easier during development for IDs and command handling
// to be shared.
static const Lunar_MessageRouter_CommandTableItem_t Control_messageTableCAN[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 0x01, Control_MessageRouter_INV_CommandCMD },
};


#ifdef __cplusplus
extern "C"
}
#endif

