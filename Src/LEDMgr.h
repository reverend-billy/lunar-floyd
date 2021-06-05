/*******************************************************************************
// Core LED Manager
*******************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
// Includes
*******************************************************************************/
// Platform Includes
// Other Includes
#include "GPIO_Drv.h"
#include <stdint.h>


/*******************************************************************************
// Public Types
*******************************************************************************/

// This type is used to define the number of flashes for a specific error.
typedef struct
{
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
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void LEDMgr_Init(void);


/** Description:
  *    The periodic function called by the schedule that determines the correct
  *    flash pattern to be displayed based on the system status.
  *  History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void LEDMgr_Update(void);

#ifdef __cplusplus
extern "C"
}
#endif

