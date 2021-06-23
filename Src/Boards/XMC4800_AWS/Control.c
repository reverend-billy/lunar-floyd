/*******************************************************************************
// Control
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
#include "Control.h"
#include "Control_Config.h" // Control States
// Platform Includes
#include "GPIO_Drv.h"
#include "Lunar_ErrorMgr.h"
#include "Lunar_EventMgr.h"
#include "Lunar_MessageRouter.h"
#include "Reset_Drv.h"
// Other Includes
#include "ReportMgr.h"
#include <stdbool.h>


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure holds the private information for this module
typedef struct
{
   /*This is the enumerated value that contains the current state
   */
   Control_State_t deviceState;
} Control_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static Control_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialization function for the module.  After this function is called, it
// is assumed that the module is ready for use
void Control_Init(void)
{
   // Default to idle state for now
   status.deviceState = CONTROL_STATE_STANDBY;
}

// Scheduled control function
void Control_Update(void)
{
   // TODO
	
   // Currently we just set the LED status based on error state
	// This is should be handled in a better way as the control is defined
	GPIO_Drv_Write(GPIO_DRV_CHANNEL_LED_FAULT, Lunar_ErrorMgr_DoAnyErrorsExist());
}

// Enable and disable the control
void Control_SetState(const Control_State_t newState)
{
   // See if the state is changing
   if (newState != status.deviceState)
   {
      // Verify the new state
      // Faults are set through the error manager and are not set directly
      if (newState < CONTROL_STATE_Count)
      {
         // Do not change state if there are any errors
         if (!Lunar_ErrorMgr_DoAnyErrorsExist())
         {
            // If previous state was running, consider this new state disable event
            if (status.deviceState == CONTROL_STATE_MPPT)
            {
               // This is a disable state, store it
               status.deviceState = newState;

               // Trigger the Enable State Change event
               Lunar_EventMgr_HandleEvent(LUNAR_EVENTMGR_CONFIG_EVENT_Control_Disabled);
            }
            else
            {
               // No fault and not disabling, just store the new state
               status.deviceState = newState;
            }
         }
      }
   }
}

// Return the enumerated state used for reporting
Control_State_t Control_GetState(void)
{
   Control_State_t currentState;

   // Check for errors
   if (Lunar_ErrorMgr_DoAnyErrorsExist())
   {
      // Errors, consider this as fault state
      currentState = CONTROL_STATE_FAULT;
   }
   else
   {
      // Just return the current state
      currentState = status.deviceState;
   }

   return(currentState);
}

// Event handler for fault
void Control_Fault(void)
{
   // TODO
}


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/

// Get the device state
void Control_MessageRouter_GetState(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Current enable state of the control module
      uint8_t deviceState;
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
      response->deviceState = (uint8_t)Control_GetState();

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// Set the device state
void Control_MessageRouter_SetState(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the commnad
   typedef struct
   {
      // New enable state to be set
      uint8_t state;
   } Command_t;


   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), 0))
   {
      // Cast the command buffer as the command type.
      Command_t *command = (Command_t *)message->commandParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
      Control_SetState((Control_State_t)command->state);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }
}

// INV_CommandCMD
void Control_MessageRouter_INV_CommandCMD(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_CommandCMD_t Command_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), 0))
   {
      // Cast the command buffer as the command type.
      Command_t *command = (Command_t *)message->commandParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
     
      // Shutdown, State and Reset are combined into one command
      // Check Shutdown first. Shutdown if non-zero
      //      Bits=08.  [ 0     , 255    ]  Unit:''
      if (command->INV_RapidShutdown != 0)
      {		
			// TODO - Rapid Shutdown
	  }
      // Check Reset next. Reset if non-zero
      //      Bits=08.  [ 0     , 255    ]  Unit:''
	  else if (command->INV_ResetReq != 0)
      {
         // Just call the platform reset function
         Reset_Drv_Execute();
      }
      else
	  {
	     // INV_L1L2InPhaseReq, not currently implemented
         // No reset requested, check for state change
         // We can call this each time since Control will only act if the state changes
         //      Bits=08.  [ 0     , 255    ]  Unit:''
         Control_SetState((Control_State_t)command->INV_StateReq);
      }

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }
}

