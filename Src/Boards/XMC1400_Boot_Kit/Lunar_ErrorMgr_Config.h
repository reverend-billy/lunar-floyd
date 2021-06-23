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
// Other Includes
#include <stddef.h>


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/

/** This constant defines whether the error manager will persist
  * errors to non-volatile memory. Setting this value to true =
  * enabled, false = disabled.                                  
*/
#define ERRORMGR_FLASH_STORAGE_ENABLED (false)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// This enumeration defines the available errors in the system.
typedef enum
{
   // This is the error when voltage drops below the allowed minimum voltage
   LUNAR_ERRORMGR_ERROR_VOLTAGE_LOW,

   // This error means the watchdog timer has occurred in the software
   LUNAR_ERRORMGR_ERROR_WATCHDOG_TIMEOUT,

   // This is a generic error for testing faults
   LUNAR_ERRORMGR_ERROR_BATTERY_FAULT,
	
   // This error means the RAM has been detected as invalid by the software
   LUNAR_ERRORMGR_ERROR_RAM_CHECK_FAILED,

   // This is the number of errors defined by the enumerated type
   LUNAR_ERRORMGR_ERROR_Count,

   // This error defines an error that will never appear in the bit-packed array
   // It is primarily a placeholder for empty critical and persistent error tables
   LUNAR_ERRORMGR_ERROR_None
} Lunar_ErrorMgr_Error_t;


#ifdef __cplusplus
extern "C"
}
#endif

