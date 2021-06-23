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
   { (uint8_t)BB_BatteryStatusRSP_CANID, ReportMgr_MessageRouter_BB_BatteryStatusRSP },
   { (uint8_t)BB_DCDCStatusRSP_CANID, ReportMgr_MessageRouter_BB_DCDCStatusRSP },
   { (uint8_t)BB_CellVoltage1to4RSP_CANID, ReportMgr_MessageRouter_BB_CellVoltage1to4RSP },
   { (uint8_t)BB_CellVoltage5to8RSP_CANID, ReportMgr_MessageRouter_BB_CellVoltage5to8RSP },
   { (uint8_t)BB_CellVoltage9to12RSP_CANID, ReportMgr_MessageRouter_BB_CellVoltage9to12RSP },
   { (uint8_t)BB_CellTempRSP_CANID, ReportMgr_MessageRouter_BB_CellTempRSP },
   { (uint8_t)BB_DCDCTemperaturesRSP_CANID, ReportMgr_MessageRouter_BB_DCDCTemperaturesRSP },
};

#ifdef __cplusplus
extern "C"
}
#endif
