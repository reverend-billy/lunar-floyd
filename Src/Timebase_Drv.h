/*******************************************************************************
// Timebase Module
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
#include "Timebase_Drv_Config.h"
// Platform Includes
// Other Includes
#include <stdint.h> // Integer types


/*******************************************************************************
// Public Constants
*******************************************************************************/


/*******************************************************************************
// Public Types
*******************************************************************************/


/*******************************************************************************
// Public Variables
*******************************************************************************/


/*******************************************************************************
// Public Prototypes
*******************************************************************************/

/** Description:
  *    This function initializes the Timebase driver used for
  *    software timers and function scheduling.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                          
*/
void Timebase_Drv_Init(void);

/** Description:
  *    This function retrieves the current value of the Timebase.  In most
  *    implementations, this will just be the SysTick value. 
  * Returns: 
  *    uint32_t - The current 32-bit timebase value of the 32-bit
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
uint32_t Timebase_Drv_GetCurrentTickCount(void);


#ifdef __cplusplus
extern "C"
}
#endif

