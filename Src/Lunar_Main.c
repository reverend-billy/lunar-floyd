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
// Private Constant Definitions
*******************************************************************************/

// Use sizeof for compile time length computation of the product name string
// Note that a constant C string will be the length of the string plus a null terminating character
#define PRODUCT_NAME_LENGTH (sizeof(mainConfigTable.productName.stringValue))


/*******************************************************************************
// Private Type Declarations
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
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static Main_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
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


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/

// Get version information
void Lunar_Main_MessageRouter_GetApplicationVersion(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Major version number (1 in v1.2.3)
      uint8_t major;
      // Minor version number (2 in v1.2.3)
      uint8_t minor;
      // Build number (3456 in v1.2.3456)
      uint16_t build;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
      // Set Major.Minor to 0.0 for Debug Targets
      if (mainConfigTable.releaseTarget)
      {
         // Release build, use version
         response->major = PLATFORM_MIN(mainConfigTable.releaseTarget, mainConfigTable.version.major);
         response->minor = mainConfigTable.version.minor;
      }
      else
      {
         // Debug Build, override version
         response->major = 0;
         response->minor = 0;
      }
      // Build is always included
      response->build = mainConfigTable.version.build;

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// Get product identification string
void Lunar_Main_MessageRouter_GetProductID(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // 32-bit Product ID
      uint32_t productID;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
      // Just store the 32-bit product ID
      response->productID = mainConfigTable.productId;

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }

}


// Get product identification string
void Lunar_Main_MessageRouter_GetProductName(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // Response params not used -- copy string directly

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, PRODUCT_NAME_LENGTH))
   {
      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      // Copy each byte to the response buffer
      for (int i = 0; i < PRODUCT_NAME_LENGTH; i++)
      {
         message->responseParams.data[i] = (uint8_t)mainConfigTable.productName.stringValue[i];
      }

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, PRODUCT_NAME_LENGTH);
   }

}

// Get the reason for the last reset
void Lunar_Main_MessageRouter_GetResetReason(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response
   typedef struct
   {
      uint32_t resetReason;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
      response->resetReason = Reset_Drv_GetResetReason();

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// Reset the system
void Lunar_Main_MessageRouter_Reset(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, 0))
   {
      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      // Set the response length before calling reset
      // Depedning on the implementation, the response may not go out
      Lunar_MessageRouter_SetResponseSize(message, 0);

      // Just call the platform reset function
      Reset_Drv_Execute();
   }
}

// Get time since last reboot
void Lunar_Main_MessageRouter_GetUptimeMillseconds(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Elapsed time
      uint32_t uptimeMilliseconds;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
		
      //xxx response->uptimeMilliseconds = Lunar_SoftTimerLib_GetElapsedTimeMilliseconds(&status.uptimeTimer);
		response->uptimeMilliseconds = clock();

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

