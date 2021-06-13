/*******************************************************************************
// Core Message Router Module
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Lunar_MessageRouter_ConfigTable.h"
// Other Includes


/*******************************************************************************
// Constants
*******************************************************************************/

// The total number of modules define in the Message Router configuration
#define NUM_MESSAGEROUTER_MODULES (sizeof(messageRouterConfigTable)/sizeof(Lunar_MessageRouter_ConfigItem_t))


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Lookup and execute the correspongin handler for a message
void Lunar_MessageRouter_ProcessMessage(Lunar_MessageRouter_Message_t *const message)
{
   // Always initialize the length of the response buffer to zero
   message->responseParams.length = 0;

   // Assume module not found
   message->responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidModuleID;

   // Loop through the module table and find the correct module ID
   for (uint32_t i = 0U; i < NUM_MESSAGEROUTER_MODULES; i++)
   {
      // See if the module ID is found in the table
      if (messageRouterConfigTable[i].moduleID == message->header.moduleID)
      {
         // Module was valid, assume command not found
         message->responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidCommandID;

         // Loop through the table and find the correct command ID
         for (uint8_t j = 0U; j < messageRouterConfigTable[i].numCommands; j++)
         {
            // See if the command ID is found in the table
            if (messageRouterConfigTable[i].commandTable[j].commandID == message->header.commandID)
            {
               // Command ID found, note that the message is valid up to this point
               message->responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_None;

               // Send the message to the massage handler, if it is not NULL
               if (messageRouterConfigTable[i].commandTable[j].messageHandler != 0)
               {
                  // Function is not NULL, so call it
                  messageRouterConfigTable[i].commandTable[j].messageHandler(message);
               }

               // Exit the command loop
               break;
            }
         }

         // Exit the module loop
         break;
      }
   }
}


// Verify the expected size of the command parameters
bool Lunar_MessageRouter_VerifyCommandSize(Lunar_MessageRouter_Message_t *const message, const uint8_t commandSize)
{
   bool isValid = false;

   // Verify the message pointer
   if (message != 0)
   {
      // Message is valid, verify that the command parameters are the expected size
      if (message->commandParams.length == commandSize)
      {
         // Parameter size matches
         isValid = true;
      }
      else
      {
         // The command buffer does not match the expected value, set the response code.
         message->responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidCommandLength;
      }
   }

   return(isValid);
}


// Verify the given command parameter size is 0
bool Lunar_MessageRouter_VerifyNoCommandParameters(Lunar_MessageRouter_Message_t *const message)
{
   return(Lunar_MessageRouter_VerifyCommandSize(message, 0));
}


// Verify that a Response with the given size can be placed in the message object
bool Lunar_MessageRouter_VerifyResponseSize(Lunar_MessageRouter_Message_t *const message, const uint8_t responseSize)
{
   bool isValid = false;

   // Verify the message pointer is valid
   if (message != 0)
   {
      // Verify that the response will fit
      if (responseSize <= message->responseParams.maxLength)
      {
         // Response size fits
         isValid = true;
      }
      else
      {
         // The response buffer is too small, set the response code.
         message->responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidResponseLength;
      }
   }

   return(isValid);
}


// Verify that a Response with no parameters can be placed in the message object
bool Lunar_MessageRouter_VerifyNoResponseParameters(Lunar_MessageRouter_Message_t *const message)
{
   return(Lunar_MessageRouter_VerifyResponseSize(message, 0));
}


// Verify both command and response sizes match expected values
bool Lunar_MessageRouter_VerifyParameterSizes(Lunar_MessageRouter_Message_t *const message, const uint8_t commandSize, const uint8_t responseSize)
{
   // Default to failed
   bool wasSuccessful = false;

   // Verify the message pointer is valid
   if (message != 0)
   {
      // Verify the length of the command (error response set, if necessary)
      if (Lunar_MessageRouter_VerifyCommandSize(message, commandSize))
      {
         // Verify that the response will fit  (error response set, if necessary)
         if (Lunar_MessageRouter_VerifyResponseSize(message, responseSize))
         {
            // Success
            wasSuccessful = true;
         }
      }
   }

   // Return the success result
   return(wasSuccessful);
}


// Set the response length
void Lunar_MessageRouter_SetResponseSize(Lunar_MessageRouter_Message_t *const message, const uint8_t responseSize)
{
   // Verify the message pointer is valid
   if (message != 0)
   {
      // Simply set the length, note that it is expected the buffer has been checked before processing the message
      message->responseParams.length = responseSize;
   }
}

