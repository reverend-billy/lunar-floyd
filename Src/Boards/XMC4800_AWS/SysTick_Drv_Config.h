/*******************************************************************************
// SysTick Driver Configuration
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
#include <stdint.h> // For integer types


/*******************************************************************************
// Public Constants
*******************************************************************************/

// Defines the SysTick IRQ for the device
#define SYSTICK_DRV_IRQ SysTick_IRQn

/** Defines the IRQ priority used for the SysTick Note that the
  * XMC1400 does not use priority grouping                     
*/
#define SYSTICK_DRV_IRQ_PRIORITY (63U)

/** Defines the IRQ priority used for the SysTick
  * XMC4000 series support grouping
*/
#define SYSTICK_DRV_IRQ_SUBPRIORITY  (0U)

/** Defines the SysTick IRQ specified by the device specific
  * startup code                                            
*/
#define SYSTICK_DRV_IRQ_HANDLER SysTick_Handler


/*******************************************************************************
// Public Types
*******************************************************************************/


/*******************************************************************************
// Public Variables
*******************************************************************************/

// Defines the 32-bit SysTick value to prevent the overhead of an extra function
// every time the SysTick value is accessed  Note that this does not cause any
// conflicts since only the SysTick module will modify this value.
// SysTick_Drv_GetCurrentTickCount is provided, if a method is preferred.
extern volatile uint32_t SysTick_Drv_sysTickCount;


#ifdef __cplusplus
extern "C"
}
#endif

