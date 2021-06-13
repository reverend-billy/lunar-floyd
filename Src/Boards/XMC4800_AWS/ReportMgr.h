/*******************************************************************************
// Report Manager
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
#include "ReportMgr_CAN.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Platform.h"
// Other Includes
#include <stdint.h>


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
  *    This function defines the module initialization. 
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void ReportMgr_Init(void);

/** Description:
  *    This is the periodic scheduled function that builds and sends out reports
  *    for the system.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void ReportMgr_Update(void);


/*******************************************************************************
// Message Router Function Declarations
*******************************************************************************/

/** Description:
  *    This function retrieves the information for the INV_L2NStatusRSP message.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_INV_L2NStatusRSP(Lunar_MessageRouter_Message_t *const message);

/** Description:
  *    This function retrieves the information for the INV_L2LStatusRSP message.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_INV_L2LStatusRSP(Lunar_MessageRouter_Message_t *const message);


/** Description:
  *    This function retrieves the information for the INV_DCBusStatusRSP message.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_INV_DCBusStatusRSP(Lunar_MessageRouter_Message_t *const message);


/** Description:
  *    This function retrieves the information for the INV_TemperaturesRSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_INV_TemperaturesRSP(Lunar_MessageRouter_Message_t *const message);


/** Description:
  *    This function retrieves the information for the INV_StateRSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_INV_StateRSP(Lunar_MessageRouter_Message_t *const message);


#ifdef __cplusplus
extern "C"
}
#endif
