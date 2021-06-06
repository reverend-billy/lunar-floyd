/*******************************************************************************
// Core Main
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_Main.h"
#include "Lunar_Main_ConfigTable.h"
// Platform Includes
#include "Lunar_InitMgr.h"
#include "Lunar_Scheduler.h"
#include "Lunar_SoftTimerLib.h"
// Other Includes
#include "Platform.h"
#include "Reset_Drv.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h> // Used for memcpy
#include "Timebase_Drv.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/

// Use sizeof for compile time length computation of the product name string
// Note that a constant C string will be the length of the string plus a null terminating character
#define PRODUCT_NAME_LENGTH (sizeof(mainConfig.productName.stringValue))


/*******************************************************************************
// Private Types
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   /** Software timer object used for measuring uptime. This will be
     * reset at each reboot and is primarily used as an easy way to
     * verify that watchdog or other resets have not occurred
     * unexpectedly during testing. Note that the timer will
     * eventually wrap and is not designed for measuring very long
     * durations.                                                   
   */
   Lunar_SoftTimerLib_Timer_t uptimeTimer;

   /** Tracks if the module has been initialized. This prevents any
     * confusion with initialization and allows the module to be
     * initialized by the user or will be handled automatically when
     * the Execute function is called.                              
   */
   bool isInitialized;
} Main_Status_t;


/*******************************************************************************
// Private Variables
*******************************************************************************/

// The variable used for holding all internal data for this module.
static Main_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Module initialization
void Lunar_Main_Init(void)
{
   // Start a timer measurement for measuring the uptime
   Lunar_SoftTimerLib_Init(&status.uptimeTimer);
   Lunar_SoftTimerLib_StartTimerMeasurement(&status.uptimeTimer);

   // Note that the module has been initialized
   status.isInitialized = true;
}

// Start the module
void Lunar_Main_Execute(void)
{
   // TODO -- consider disabling interrupts prior to initialization
   
   // Simply loops through each module in the Init Config table
   // and calls the corresponding Init function
   Lunar_InitMgr_Execute();

   // Call Init, if needed
   if (!status.isInitialized)
   {
      // Init the main module if it has not been called already.  This currently
      // only provides a software timer that counts the uptime.
      // This is done after initialzing the other modules so that we can start a
      // software timer.
      Lunar_Main_Init();
   }

   // Start the system scheduler
   // *** NOTE THIS WILL NOT RETURN ***
   Lunar_Scheduler_Execute();
}

