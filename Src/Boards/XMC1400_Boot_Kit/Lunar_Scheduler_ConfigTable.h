/*******************************************************************************
// Core Scheduler Configuration
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
#include "Lunar_Scheduler.h"
// Platform Includes
#include "Lunar_Serial.h"
// Other Includes
#include "Control.h"
#include "LEDMgr.h"
#include "CAN_Drv.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

/** This is the scheduler table -- it contains an entry for each
  * scheduled function. The entries are Lunar_Scheduler_ConfigItem_t
  * structure, containing an interval in seconds and milliseconds
  * and specifies the function to be called.                     
*/
static const Lunar_Scheduler_ConfigItem_t schedulerConfigTable[] =
{
   // { s, ms, Pointer To Scheduled Function }
   // { 0, 5, Watchdog_Update},
   { 0,   10, Lunar_Serial_Update },
   { 0,   50, Control_Update },
   { 0,   50, LEDMgr_Update },
   { 5,    0, CAN_Drv_Update },
};

#ifdef __cplusplus
extern "C"
}
#endif


