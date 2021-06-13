/*******************************************************************************
// Core Error Manager Configuration
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
#include "Lunar_ErrorMgr_Config.h"
// Other Includes


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

// This constant array defines the errors that will be considered by the 
// Error Manager module to be critical errors.
static const Lunar_ErrorMgr_Error_t Lunar_ErrorMgr_criticalErrorConfigTable[] =
{
   LUNAR_ERRORMGR_ERROR_WATCHDOG_TIMEOUT,
   LUNAR_ERRORMGR_ERROR_VOLTAGE_LOW,
   LUNAR_ERRORMGR_ERROR_RAM_CHECK_FAILED,
};

/** This table defines the list of errors that persist across
  * reboots
  * Note:
  *    Persistent storage is not currently implemented       
*/
static const Lunar_ErrorMgr_Error_t Lunar_ErrorMgr_persistentErrorConfigTable[] =
{
   LUNAR_ERRORMGR_ERROR_None
};

#ifdef __cplusplus
extern "C"
}
#endif


