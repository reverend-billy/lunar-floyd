/*******************************************************************************
// LED Manager Configuration
*******************************************************************************/ 
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Include
#include "LEDMgr.h"
// Platform Includes
#include "Lunar_ErrorMgr.h"
// Other Includes
#include "xmc_gpio.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

// This table defines the number of LED flashes that will be used for the
// specified errors.
// Note: The priority is defined by order of this table.  Starting from the
// top, the first error flag that is set will define the blink pattern.  If
// a higher priority error then occurs, the pattern will change to the new
// error pattern.  Flash code of 1 is hard to tell from standard blinking, 
// so it is not used
static const LEDMgr_FlashCodeItem_t ledMgrFlashCodeTable[] =
{
   // Error, Number of Flashes
   // {NOTE_1_BLINK_NOT_USED, 1}
   { LUNAR_ERRORMGR_ERROR_BATTERY_FAULT, 2 },
   { LUNAR_ERRORMGR_ERROR_VOLTAGE_LOW, 3 }
};


#ifdef __cplusplus
extern "C"
}
#endif

