/*******************************************************************************
// Reset Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Reset_Drv.h"
// Platform Includes
// Other Includes
#include "xmc_scu.h" // Device reset functions


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Store the cause of the last reset
   uint32_t deviceResetReason;
} Reset_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/


// The variable used for holding all internal data for this module.
static Reset_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initializes the module
void Reset_Drv_Init(void)
{
   // Get the cause of reset
   status.deviceResetReason = XMC_SCU_RESET_GetDeviceResetReason();
   // Clear reset reason
   XMC_SCU_RESET_ClearDeviceResetReason();
}


// Returns a value representing the reason for the previous reset
uint32_t Reset_Drv_GetResetReason(void)
{
   // Just return the reset reason
   return(status.deviceResetReason);
}


// Reset the device immediately
void Reset_Drv_Execute(void)
{
#if UC_FAMILY == XMC1
   // Just call reset for XMC1000 devices
   XMC_SCU_RESET_AssertMasterReset();
#elif UC_FAMILY == XMC4
   // XMC4000 resets via NVIC
   NVIC_SystemReset();
#else
   #error "Invalid device faimly"
#endif
}
