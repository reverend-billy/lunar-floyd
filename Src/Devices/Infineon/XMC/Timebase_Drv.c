/*******************************************************************************
// Timebase Module
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Timebase_Drv.h"
#include "Timebase_Drv_Config.h"
// Platform Includes
#include "SysTick_Drv.h"
#include "SysTick_Drv_Config.h"
// Other Includes
#include <stdint.h>
#include <time.h>



/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/


/*******************************************************************************
// Private Prototypes
*******************************************************************************/


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Timebase_Drv_Init not needed since SysTick is used for the timebase in this implementation
// However, an _clock_init is required to call the clock() function from 
// the standard time.h library
void _clock_init(void)
{
	// Nothing to do, SysTick is reset to 0 for this implementation
}

// Required clock function returns the general timebase used by time.h
// Note that the value returned must match the value configured by the
// __CLK_TCK definition in the Timebase driver
// If no value is definition, centiseconds are used by default
// It is expected that this value will wrap, as required
clock_t clock(void)
{
   // Just return the SysTick value
	return(SysTick_Drv_sysTickCount);
}

// Returns current Timebase tick value
uint32_t Timebase_Drv_GetCurrentTickCount(void)
{
   // Simply return the current SysTick value
   // We don't really gain anything by adding an extra function call to SysTick_Drv_GetCurrentTickCount
   return(SysTick_Drv_sysTickCount);
}

