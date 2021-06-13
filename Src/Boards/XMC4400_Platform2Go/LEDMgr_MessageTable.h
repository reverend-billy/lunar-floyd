/*******************************************************************************
// LED Manager Message Router Commands
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
#include "LEDMgr.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
// Other Includes


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module.
static const Lunar_MessageRouter_CommandTableItem_t LEDMgr_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 1, LEDMgr_MessageRouter_GetFlashCode }
};


#ifdef __cplusplus
extern "C"
}
#endif


