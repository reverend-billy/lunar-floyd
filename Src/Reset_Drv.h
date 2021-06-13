/*******************************************************************************
// Reset Driver
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
  *    This function defines the module initialization. The previous
  *    reset reason will be stored for later query by the Reset_GetResetReason
  *    function.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Reset_Drv_Init(void);

/** Description:
  *    This function will reset the device as soon as possible.  The
  *    implementation depends on the hardware.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Reset_Drv_Execute(void);

/** Description:
  *    This function returns the reason the previous reset.
  * Returns:
  *    uint32_t - A 32-bit value define the system-specific reason for the
  *    previous device reset.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                 
*/
uint32_t Reset_Drv_GetResetReason(void);


#ifdef __cplusplus
extern "C"
}
#endif

