/*******************************************************************************
// Initialization Module
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_InitMgr.h"
#include "Lunar_InitMgr_ConfigTable.h" // Include only once
// Platform Includes
// Other Includes
#include <stdbool.h>
#include <stdint.h>


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// Determines the number of initialization functions in the configuration table.
// This value is automatically computed at compilation based on the size of the
// table.
#define NUM_INIT_FUNTIONS (sizeof(initMgrConfigTable)/sizeof(Lunar_InitMgr_ConfigItem_t))


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Tracks if the module has been initialized.
   bool isInitialized;
} InitMgr_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static InitMgr_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize all configurated modules
void Lunar_InitMgr_Execute(void)
{
   // Note that we have not compelte initialization
   status.isInitialized = false;

   // Loop through the table and execute each function
   for (uint32_t i = 0U; i < NUM_INIT_FUNTIONS; i++)
   {
      // Verify the function pointer
      if (initMgrConfigTable[i].initFunction != 0)
      {
         // Function pointer is valid, call it
         initMgrConfigTable[i].initFunction();
      }
   }

   // Note that the module initialization is complete
   status.isInitialized = true;
}


// Returns the initialiation state for the system
bool Lunar_InitMgr_GetInitializationState(void)
{
   // Just return the stored state
   return(status.isInitialized);
};

