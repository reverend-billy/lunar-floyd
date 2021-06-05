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
// Other Includes
#include "LEDMgr.h"


/*******************************************************************************
// Constant Configuration Variables
*******************************************************************************/

/** This is the scheduler table -- it contains an entry for each
  * scheduled function. The entries are Lunar_Scheduler_Item_t
  * structure, containing an interval in seconds and milliseconds
  * and specifies the function to be called.                     
*/
static const Lunar_Scheduler_Item_t schedulerConfigTable[] =
{
   // { s, ms, Pointer To Scheduled Function }
   // { 0, 5, Watchdog_Update},
   { 0,   50, LEDMgr_Update }
};

#ifdef __cplusplus
extern "C"
}
#endif


