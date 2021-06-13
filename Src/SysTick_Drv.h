/*******************************************************************************
// SysTick Driver
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
#include <stdint.h> // Integer types


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the hardware specific SysTick for the device.
  *    Typically the SysTick is used for the system Timebase used for software
  *    timers and function scheduling.  Some devices may be able to read
  *    directly from a SysTick register.  
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void SysTick_Drv_Init(void);

/** Description:
  *    This function retrieves the current value of the SysTick.  Note that this
  *    may be either  a variable or register depending on the device.
  * Returns: 
  *    uint32_t - The current value of the 32-bit SysTick
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
uint32_t SysTick_Drv_GetCurrentTickCount(void);

#ifdef __cplusplus
extern "C"
}
#endif

