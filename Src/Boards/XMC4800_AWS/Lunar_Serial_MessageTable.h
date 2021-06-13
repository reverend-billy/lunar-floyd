/*******************************************************************************
// Core Serial Protocol Module Message Router Commands
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
#include "Lunar_Serial.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
// Other Includes
#include <stddef.h>


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module.
static const Lunar_MessageRouter_CommandTableItem_t Lunar_Serial_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   { 0x01, Lunar_Serial_MessageRouter_GetSerialStatistics },
};

#ifdef __cplusplus
extern "C"
}
#endif
