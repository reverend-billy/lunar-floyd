/*******************************************************************************
// Core Initialization Manager Configuration
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
#include "Lunar_InitMgr.h"
// Platform Includes
#include "GPIO_Drv.h"
#include "Reset_Drv.h"
#include "SysTick_Drv.h"
#include "Lunar_ErrorMgr.h"
#include "Lunar_EventMgr.h"
#include "Lunar_Main.h"
#include "Lunar_Scheduler.h"
#include "Lunar_Serial.h"
// Other Includes
#include "Control.h"
#include "LEDMgr.h"
#include "RTC_Drv.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

/** This table defines the initialization functions for each
  * module that is to be called by the InitMgr Execute function.
  * Each function will be called in order and will run until
  * completion. Note that care should be taken to prevent
  * delaying the remaining functions. It is suggested that long
  * running initialization should be handled in asynchronously. 
*/
static const Lunar_InitMgr_ConfigItem_t initMgrConfigTable[] =
{
   // { Init function }
   // Init errors first so that we can set failures
   { Lunar_ErrorMgr_Init },
   // Timebase second to make sure timing is correct
   { SysTick_Drv_Init },
   { Lunar_EventMgr_Init },
   { Reset_Drv_Init },
   // RTC driver needs Reset Drv initialzed to determine if time 
   // should be preserved
   { RTC_Drv_Init },
   { GPIO_Drv_Init },
   { Lunar_Main_Init },
   { Lunar_Scheduler_Init },
   { LEDMgr_Init },
   { Lunar_Serial_Init },
   { Control_Init },
};

#ifdef __cplusplus
extern "C"
}
#endif

