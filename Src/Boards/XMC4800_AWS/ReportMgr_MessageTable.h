/*******************************************************************************
// Report Manager Message Router Commands
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
#include "ReportMgr.h"
#include "ReportMgr_CAN.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
// Other Includes


/*******************************************************************************
// Message Router Table
*******************************************************************************/

// This table provides a list of commands for this module. The primary purpose
// is to link each Command ID to its corresponding message handler function
static const Lunar_MessageRouter_CommandTableItem_t ReportMgr_messageTable[] =
{
   // {Command ID, Message Handler Function Pointer}
   // Note that the Command IDs are currently aligned with the auto-generated ReportMgr_CAN.h file
   // Only the LSB of the CAN ID is used for the Command ID
   { (uint8_t)INV_L2NStatusRSP_CANID, ReportMgr_MessageRouter_INV_L2NStatusRSP },
   { (uint8_t)INV_L2LStatusRSP_CANID, ReportMgr_MessageRouter_INV_L2LStatusRSP },
   { (uint8_t)INV_DCBusStatusRSP_CANID, ReportMgr_MessageRouter_INV_DCBusStatusRSP },
   { (uint8_t)INV_TemperaturesRSP_CANID, ReportMgr_MessageRouter_INV_TemperaturesRSP },   
   { (uint8_t)INV_StateRSP_CANID, ReportMgr_MessageRouter_INV_StateRSP },
};

#ifdef __cplusplus
extern "C"
}
#endif
