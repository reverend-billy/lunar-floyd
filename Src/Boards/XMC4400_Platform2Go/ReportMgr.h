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
  *    This function retrieves the information for the BB_BatteryStatus message.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_BatteryStatusRSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_DCDCStatusRSP message.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_DCDCStatusRSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_CellVoltage1to4RSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_CellVoltage1to4RSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_CellVoltage5to8RSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_CellVoltage5to8RSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_CellVoltage9to12RSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_CellVoltage9to12RSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_CellTempRSP message.
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_CellTempRSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function retrieves the information for the BB_DCDCTemperaturesRSP
  *    message. 
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/8/2021: Function created (EJH)
  *                                                               
*/
void ReportMgr_MessageRouter_BB_DCDCTemperaturesRSP(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif
