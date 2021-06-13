/*******************************************************************************
// Timebase Driver Configuration
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
// Platform Includes
// Other Includes

/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/

// The maximum allowable SysTick (simply max unsigned 32-bit value)
#define TIMEBASE_DRV_MAX_TICK_VALUE (UINT32_MAX)

// Keil default for time.h is centiseconds, change to milliseconds
#define __CLK_TCK (1000U)

// The number of SysTicks for the current clock that equal 1
// second. This value must match the SysTick driver implementation.                                          
#define TIMEBASE_DRV_NUM_TICKS_PER_SECOND (__CLK_TCK)

// The number of SysTicks for the current clock that equal 1 millisecond
// Note that the maximum value is 49 to keep the Software Timer objects from overflowing
#define TIMEBASE_DRV_NUM_TICKS_PER_MILLISECOND (1)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/


#ifdef __cplusplus
extern "C"
}
#endif

