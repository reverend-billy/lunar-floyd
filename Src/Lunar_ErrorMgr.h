/*******************************************************************************
// Core Error Manager
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
#include "Lunar_ErrorMgr_Config.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Platform.h"
// Other Includes
#include <stdbool.h>


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the Error Manager module.  After calling this
  *    function, it is assumed, the module is ready for use.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_ErrorMgr_Init(void);


/** Description:
  *    This function sets the specified error state to the given value.
  * Parameters: 
  *     error - The enumerated error to be set
  *     errorState - The state to be set for the given error. true: error set,
  *     false: error cleared
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_ErrorMgr_SetErrorState(const Lunar_ErrorMgr_Error_t error, const bool errorState);


/** Description:
  *    This function gets the current error state for the given
  *    error.
  * Parameters:
  *    error :  The enumerated error to be queried
  * Returns:
  *    bool - The current state of the given error
  * Return Value List:
  *    true :   Error is currently active
  *    false :  Error is not currently active
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                            
*/
bool Lunar_ErrorMgr_GetErrorState(const Lunar_ErrorMgr_Error_t error);


/** Description:
  *    This function checks to see if there are any errors currently active in
  *    the system.
  *  Returns:
  *     bool - Value representing the state of all errors in the system.
  *  Return Value List:
  *     true - At least one error is currently active
  *     false - No error is not currently active
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_ErrorMgr_DoAnyErrorsExist(void);


/** Description:
  *    This function checks to see if there are currently any errors active that
  *    are defined as Critical Errors for the system.
  *  Returns:
  *     bool - Value representing the Critical Error State of the system
  *  Return Value List:
  *     true - At least one Critical Error is currently active
  *     false - No Critical Error is currently active
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_ErrorMgr_GetCriticalErrorStatus(void);


/** Description:
  *    This function clears all active errors in the system
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_ErrorMgr_ClearAllErrors(void);


/** Description:
  *    This function This function return a bit-packed list of all
  *    error states in the system. Each error is represented as a
  *    single bit -- starting with Error 0 at bit 0, etc. For
  *    \example, if index 0 is given, the first 8 error flags
  *    (Errors 0-7) will be packed into the returned byte. Index 1
  *    packs errors 8-15, and so on. If an index is given that is
  *    too large, the highest byte is returned.
  * Parameters:
  *    byteIndex :  The byte offset to be retrieved. Ex\: byteIndex 0
  *                 retrieves Errors 0\-7, byteIndex retrieves Errors
  *                 8\-15, etc.
  * Returns:
  *    uint8_t - The bit-packed error flags. Each bit represents one
  *    Boolean error flag.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                  
*/
uint8_t Lunar_ErrorMgr_GetBitPackedErrors(uint8_t byteIndex);


/*******************************************************************************
// Message Router Function Declarations
*******************************************************************************/

/** Description:
  *    This function retrieves the current state of a given error.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Lunar_ErrorMgr_MessageRouter_GetErrorState(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function sets (or clears) the state of a given error.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Lunar_ErrorMgr_MessageRouter_SetErrorState(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function checks to see if there are any errors currently active in
  *    the system.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Lunar_ErrorMgr_MessageRouter_DoErrorsExist(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    This function clears all active errors in the system.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Lunar_ErrorMgr_MessageRouter_ClearAllErrors(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


/** Description:
  *    Command to retrieve all errors as bit-packed bytes.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                               
*/
void Lunar_ErrorMgr_MessageRouter_GetAllErrors(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif
