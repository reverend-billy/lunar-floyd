/*******************************************************************************
// Control
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
#include "Control_Config.h" // Control states
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Platform.h"
// Other Includes
#include <stdbool.h>


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
  *    This function defines the module initialization. The control will begin
  *    in idle mode CONTROL_STATE_STANDBY
  *    
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Control_Init(void);


/** Description:
  *    Placeholder for the periodic Control function called by the Scheduler.
  *    
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Control_Update(void);


/** Description:
  *    Set the state of the system control.
  * Parameters: 
  *    newState - The enumerated value that defines the desired state to be set.
  *    Note that fault cannot be set directly.  Fault state is triggered by the
  *    Error Manager.
  * * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Control_SetState(const Control_State_t newState);


/** Description:
  *    Get the current state of the system control.
  * Return Value: 
  *    Control_State_t - The enumerated value that defines the current 
  *    state of the system.
  * Return Value List:
  *    CONTROL_STATE_FAULT - At least one fault is present
  *    Other - Refer to the enumerate Control_State_t type
  *  History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
Control_State_t Control_GetState(void);


/** Description:
  *    Fault handler -- used primarily for critical error events triggered by
  *    the Event Manager. 
  *  History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Control_Fault(void);


/** Description:
  *    This function retrieves the current enumerated state of the device.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Control_MessageRouter_GetState(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function sets the device state to the given enumerated value.
  *    device. 
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Control_MessageRouter_SetState(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    Message used by CAN to change the control state or reset the device
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Control_MessageRouter_BB_CommandCMD(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


#ifdef __cplusplus
}
#endif
