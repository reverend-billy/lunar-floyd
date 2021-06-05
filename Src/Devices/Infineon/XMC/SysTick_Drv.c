/*******************************************************************************
// SysTick Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "SysTick_Drv.h"
#include "SysTick_Drv_Config.h"
// Platform Includes
#include "Timebase_Drv_Config.h"
// Other Includes
#include <stdint.h>
#include "xmc_scu.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/


/*******************************************************************************
// Private Types
*******************************************************************************/


/*******************************************************************************
// Private Variables
*******************************************************************************/

// 32-bit system tick counter used for software timers
// In the current implementation, this value is read by the Timebase module
// to prevent the overhead of calling a function.
volatile uint32_t SysTick_Drv_sysTickCount = 0U;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize module
void SysTick_Drv_Init(void)
{
   // Initialize SysTick interrupt - XMC4000 uses priority grouping
#if (UC_FAMILY == XMC1)
   // Set priority value for XMC1000 devices
   NVIC_SetPriority(SYSTICK_DRV_IRQ, SYSTICK_DRV_IRQ_PRIORITY);
#elif (UC_FAMILY == XMC4)
   // Set NVIC Priority for XMC400 devices
   NVIC_SetPriority(SYSTICK_DRV_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SYSTICK_DRV_IRQ_PRIORITY, SYSTICK_DRV_IRQ_SUBPRIORITY));
#endif

   // Set the SysTick Period to timebase setting of 1ms
   SysTick_Config(SystemCoreClock / TIMEBASE_DRV_NUM_TICKS_PER_SECOND);

   // Initialize the global systick variable
   SysTick_Drv_sysTickCount = 0U;
}


// Get the current SysTick value
uint32_t SysTick_Drv_GetCurrentTickCount(void)
{
   // Simply return the current SysTick value
   return(SysTick_Drv_sysTickCount);
}


/*******************************************************************************
// Interrupt Handler
*******************************************************************************/

// SysTick Interrupt
void SYSTICK_DRV_IRQ_HANDLER(void)
{
   // Simply increment the SysTick counter by 1
   SysTick_Drv_sysTickCount++;
}
