/*******************************************************************************
// LED Manager
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
#include "GPIO_Drv.h"
#include "Lunar_ErrorMgr.h"
#include "Lunar_MessageRouter.h"
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// This type is used to define the number of flashes for a specific error.
typedef struct
{
   // Enumerated error for this flash code
   Lunar_ErrorMgr_Error_t error;
   // Flash count for this error.
   uint8_t numFlashes;
} LEDMgr_FlashCodeItem_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function defines the module initialization.  The GPIO output
  *    for the LED is setup and a software timer is started.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void LEDMgr_Init(void);


/** Description:
  *    The periodic function called by the Scheduler that determines the correct
  *    flash pattern to be displayed based on the system status.
  *  History:
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void LEDMgr_Update(void);


/*******************************************************************************
// Message Router Function Declarations
*******************************************************************************/

/** Description:
  *    This function retrieves the current LED flash code that is being
  *    displayed.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void LEDMgr_MessageRouter_GetFlashCode(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif

