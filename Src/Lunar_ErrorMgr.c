/*******************************************************************************
// Core Error Manager
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
#include "Lunar_ErrorMgr.h"
#include "Lunar_ErrorMgr_Config.h"
#include "Lunar_ErrorMgr_ConfigTable.h"
// Platform Includes
#include "Lunar_EventMgr.h"
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Private Constants
*******************************************************************************/

/** This is the number of critical errors defined in the Critical
  * Errors array. Note that this value must match the number of
  * critical errors in the array.                                
*/
#define LUNAR_ERRORMGR_CRITICAL_ERROR_COUNT (sizeof(Lunar_ErrorMgr_criticalErrorConfigTable)/sizeof(Lunar_ErrorMgr_Error_t))

// This is the number of persistent errors that are expected to remain after a reboot
#define LUNAR_ERRORMGR_PERSISTENT_ERROR_COUNT (sizeof(Lunar_ErrorMgr_persistentErrorConfigTable)/sizeof(Lunar_ErrorMgr_Error_t))

// This defines the number of bytes it takes to fully bit-pack all of the error flags.
#define LUNAR_ERRORMGR_NUM_BYTES_TO_BIT_PACK_ERRORS (((uint8_t)LUNAR_ERRORMGR_ERROR_Count + 7)/8)


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure hold the private data for this module
typedef struct
{
   // This array holds all of the errors
   bool errors[LUNAR_ERRORMGR_ERROR_Count];
} Lunar_ErrorMgr_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// This is the status structure for the Error Mgr module.
static Lunar_ErrorMgr_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    This function checks the given enumerated error value to see
  *    if it is defined as a Critical Error for the system.
  * Parameters:
  *    error :  The enumerated error that is to be queried
  * Returns:
  *    bool - Value representing the Critical Error State for the
  *    given error
  * Return Value List:
  *    true :            The given error is defined as Critical
  *    false :           The given error is not defined as Critical
  *    active History :  * 6/6/2021\: Function created (EJH)
  *                                                                
*/
static bool IsCriticalError(const Lunar_ErrorMgr_Error_t error);


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// This function return true if the given error is a critical error
static bool IsCriticalError(const Lunar_ErrorMgr_Error_t error)
{
	// Assume no ciriticl error
   bool foundCriticalError = false;

   // Loop through the array of critical errors and see if any are flagged
   for (uint8_t i = 0U; i < LUNAR_ERRORMGR_CRITICAL_ERROR_COUNT; i++)
   {
      if (error == Lunar_ErrorMgr_criticalErrorConfigTable[i])
      {
         // We have found a critical error
         // Set the return value to true
         foundCriticalError = true;
         // Exit the for loop
         break;
      }
   }

   // Return the critical error status
   return(foundCriticalError);
}


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize the Error Manager
void Lunar_ErrorMgr_Init(void)
{
   // Clear all flags.
   Lunar_ErrorMgr_ClearAllErrors();
}


// Set the state of the specified error
void Lunar_ErrorMgr_SetErrorState(const Lunar_ErrorMgr_Error_t error, const bool errorState)
{
   // Verify that the error does not exceed the maximum error value
   if (error < LUNAR_ERRORMGR_ERROR_Count)
   {
      // If the state of this error is changing, continue
      if (status.errors[(uint8_t)error] != errorState)
      {
         // Error state is changing
         // We have a valid error index, set the value
         status.errors[(uint8_t)error] = errorState;

         // See if this a critical error being set
         if ((errorState) && (IsCriticalError(error)))
         {
            // Trigger the event
            Lunar_EventMgr_HandleEvent(LUNAR_EVENTMGR_CONFIG_EVENT_Critical_Error);
         }
      }
   }
}

// Query the state of the given error
bool Lunar_ErrorMgr_GetErrorState(const Lunar_ErrorMgr_Error_t error)
{
   // Initialize the return value to no error
   bool errorState = false;

   // Verify that the error does not exceed the maximum error value
   if (error < LUNAR_ERRORMGR_ERROR_Count)
   {
      // We have a valid error index, return the value
      errorState = status.errors[(uint8_t)error];
   }

   // Finally, return the result
   return(errorState);
}

// Determine if any errors are set
bool Lunar_ErrorMgr_DoAnyErrorsExist(void)
{
   // Assume failure
   bool errorsFound = false;

   // Loop through all error flags to see if any are true
   for (uint8_t i = 0U; i < (uint8_t)LUNAR_ERRORMGR_ERROR_Count; i++)
   {
      if (Lunar_ErrorMgr_GetErrorState((Lunar_ErrorMgr_Error_t)i))
      {
         errorsFound = true;
         // found an error, no need to keep looking
         break;
      }
   }

   return(errorsFound);
}

// Determine if any Critical Errors are set
bool Lunar_ErrorMgr_GetCriticalErrorStatus(void)
{
	// Assume failure
   bool errorState = false;

   // Loop through the array of critical errors and see if any are flagged
   for (uint8_t i = 0U; i < LUNAR_ERRORMGR_CRITICAL_ERROR_COUNT; i++)
   {
      if (Lunar_ErrorMgr_GetErrorState(Lunar_ErrorMgr_criticalErrorConfigTable[i]))
      {
         // We have found a critical error
         // Set the return value to true
         errorState = true;
         // Exit the for loop
         break;
      }
   }

   // Return the critical error status
   return(errorState);
}

// Clear all errors
void Lunar_ErrorMgr_ClearAllErrors(void)
{
   // Loop through the error values and initialize the values to false
   for (uint8_t i = 0U; i < (uint8_t)LUNAR_ERRORMGR_ERROR_Count; i++)
   {
      // Set the flag to false (non-error state)
      status.errors[i] = false;
   }
}

// Get the errors for the specified byte range
uint8_t Lunar_ErrorMgr_GetBitPackedErrors(uint8_t byteIndex)
{
   // Range check the specified byte index.  If it is larger than the max number
   // required to fully pack all of the bits, then clip it to the highest byte.
   if (byteIndex > ((uint8_t)LUNAR_ERRORMGR_NUM_BYTES_TO_BIT_PACK_ERRORS - 1))
   {
      byteIndex = (uint8_t)LUNAR_ERRORMGR_NUM_BYTES_TO_BIT_PACK_ERRORS - 1;
   }

   // This variable specifies which flag index will be packed into bit 0 of the
   // bit-packed error byte.
   uint8_t bit0FlagIndex = byteIndex * 8U;
   // This variable specifies which flag index will be packed into bit 7 of the
   // bit-packed error byte.
   uint8_t bit7FlagIndex = bit0FlagIndex + 7U;

   // Make sure the High bit flag index does not go past the last error flag
   // Not needed if we are using 8 errors or less
#if LUNAR_ERRORMGR_ERROR_Count > 8U
   if (bit7FlagIndex > (uint8_t)LUNAR_ERRORMGR_ERROR_Count)
   {
      bit7FlagIndex = (uint8_t)LUNAR_ERRORMGR_ERROR_Count - 1U;
   }

#endif

   // Init Error Flag Byte to 0
   uint8_t errorFlags = 0U;

   // Pack the flags as bits.  To do this, start at the high flag and loop through
   // to the low flag, shifting in a 1 each time a flag is set.
   for (uint8_t i = bit0FlagIndex; i <= bit7FlagIndex; i++)
   {
      // If the current flag is set, OR in a 1 at the current bit location
      if (Lunar_ErrorMgr_GetErrorState((Lunar_ErrorMgr_Error_t)i))
      {
         errorFlags |= (uint8_t)(1U << (i - bit0FlagIndex));
      }
   }

   // return the error flags
   return(errorFlags);
}



/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/

// Fetch the current state of the given error
void Lunar_ErrorMgr_MessageRouter_GetErrorState(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the command data.
   typedef struct
   {
      // Index of the error to get
      Lunar_ErrorMgr_Error_t errorIndex;
   } Command_t;

   // This structure defines the format of the response.
   typedef struct
   {
      // Error index whose state was retrieved.
      Lunar_ErrorMgr_Error_t errorIndex;
      // State of the specified error.
      bool errorState;
   } Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), sizeof(Response_t)))
   {
      // Cast the command buffer as the command type
      Command_t *command = (Command_t *)message->commandParams.data;

      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      // Return the given error index
      response->errorIndex = command->errorIndex;

      // Fetch the error state for the given error
      response->errorState = Lunar_ErrorMgr_GetErrorState(response->errorIndex);


      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// Set/Clear a the state of the given error
void Lunar_ErrorMgr_MessageRouter_SetErrorState(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the command data.
   typedef struct
   {
      // Index of the error to set.
      Lunar_ErrorMgr_Error_t errorIndex;
      // State to set for the specified error.
      bool bState;
   } Command_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), 0))
   {
      // Cast the command buffer as the command type
      Command_t *command = (Command_t *)message->commandParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
      Lunar_ErrorMgr_SetErrorState(command->errorIndex, command->bState);


      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }
}

// Query if any errors exist in the system
void Lunar_ErrorMgr_MessageRouter_DoErrorsExist(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Status of errors
      bool bErrorsExist;
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
      response->bErrorsExist = Lunar_ErrorMgr_DoAnyErrorsExist();


      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// Clear all errors in the system
void Lunar_ErrorMgr_MessageRouter_ClearAllErrors(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // No command or response params for this message.

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
      Lunar_ErrorMgr_ClearAllErrors();

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }
}

// Get error flags in a bit-packed byte
void Lunar_ErrorMgr_MessageRouter_GetAllErrors(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------
   // This structure defines the format of the response.
   typedef struct
   {
      // Bit-packed array of errors
      // Byte 0, Bit 0 is the very first error
      uint8_t errorByte[((uint8_t)LUNAR_ERRORMGR_ERROR_Count + 7U)/8U];
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

      // Populate error bits
      for (uint8_t i = 0U; i < (uint8_t)LUNAR_ERRORMGR_NUM_BYTES_TO_BIT_PACK_ERRORS; i++)
      {
         response->errorByte[i] = Lunar_ErrorMgr_GetBitPackedErrors(i);
      }

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

