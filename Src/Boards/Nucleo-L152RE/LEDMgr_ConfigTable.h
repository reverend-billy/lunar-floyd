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
// Other Includes
#include "stm32l1xx_hal.h"


/*******************************************************************************
// Constant Configuration Variables
*******************************************************************************/

// This table defines the number of LED flashes that will be used for the
// specified errors.
// Note: The priority is defined by order of this table.  Starting from the
// top, the first error flag that is set will define the blink pattern.  If
// a higher priority error then occurs, the pattern will change to the new
// error pattern.  Flash code of 1 is hard to tell from standard blinking, 
// so it is not used
static const LEDMgr_FlashCodeItem_t ledFlashCodeTable[] =
{
   // Error, Number of Flashes
   // {NOTE_1_BLINK_NOT_USED, 1}
   { 0 }
};

#ifdef __cplusplus
extern "C"
}
#endif

