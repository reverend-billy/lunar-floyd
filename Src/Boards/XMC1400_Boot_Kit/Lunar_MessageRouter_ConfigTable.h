/*******************************************************************************
// Message Router Module Configuration
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
#include "Lunar_MessageRouter.h"
// Platform Includes
#include "Lunar_ErrorMgr_MessageTable.h"
#include "Lunar_Main_MessageTable.h"
#include "Lunar_Serial_MessageTable.h"
// Other Includes
#include "Control_MessageTable.h"
#include "LEDMgr_MessageTable.h"
#include "RTC_Drv_MessageTable.h"


/*******************************************************************************
// Message Router Module Configuration
*******************************************************************************/

// This table provides a list of the message handler tables for each module in
// the system.
static const Lunar_MessageRouter_ConfigItem_t messageRouterConfigTable[] =
{
   // {Module ID, Command Table, #Commands in Command Table }
   { 0x01, Lunar_Main_messageTable, sizeof(Lunar_Main_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   { 0x02, Lunar_ErrorMgr_messageTable, sizeof(Lunar_ErrorMgr_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   { 0x03, LEDMgr_messageTable, sizeof(LEDMgr_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   { 0x04, Lunar_Serial_messageTable, sizeof(Lunar_Serial_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   { 0x05, Control_messageTable, sizeof(Control_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   // { 0x06, ADC_messageTable, sizeof(ADC_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   // { 0x07, PWM_messageTable, sizeof(PWM_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   // { 0x08, TemperatureMonitor_messageTable, sizeof(TemperatureMonitor_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t) },
   { 0x09, RTC_Drv_messageTable, sizeof(RTC_Drv_messageTable) / sizeof(Lunar_MessageRouter_CommandTableItem_t)},
};

#ifdef __cplusplus
extern "C"
}
#endif

