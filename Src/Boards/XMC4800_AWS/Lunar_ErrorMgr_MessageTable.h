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
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Lunar_ErrorMgr.h"
// Other Includes
#include <stddef.h>


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module.
static const Lunar_MessageRouter_CommandTableItem_t Lunar_ErrorMgr_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 0x01, Lunar_ErrorMgr_MessageRouter_GetErrorState },
   { 0x02, Lunar_ErrorMgr_MessageRouter_SetErrorState },
   { 0x03, Lunar_ErrorMgr_MessageRouter_DoErrorsExist },
   { 0x04, Lunar_ErrorMgr_MessageRouter_ClearAllErrors },
   { 0x05, Lunar_ErrorMgr_MessageRouter_GetAllErrors }
};

#ifdef __cplusplus
extern "C"
}
#endif

