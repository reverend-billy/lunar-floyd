/*******************************************************************************
// Initialization Module
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
#include <stdbool.h>

/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/


// The type definition used for callback function in the Init Table
typedef void (*Lunar_InitMgr_Function_t)(void);

// Structure that defines each entry in the Initialization Table.
// Each entry defines a function to be called during initialization.
typedef struct
{
   // The function to be called for this module
   const Lunar_InitMgr_Function_t initFunction;
} Lunar_InitMgr_ConfigItem_t;



/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function will call each initialization, in order, as defined in the
  *    configuration table for this module.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Lunar_InitMgr_Execute(void);

/** Description:
  *    This function retrieves the current status of initialization
  *    for all module collectively.
  * Returns:
  *    bool - The initialization state for the module
  * Return Value List:
  *    true :   All module initialization is complete.
  *    false :  Some modules may be uninitialized.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                
*/
bool Lunar_InitMgr_GetInitializationState(void);


#ifdef __cplusplus
extern "C"
}
#endif

