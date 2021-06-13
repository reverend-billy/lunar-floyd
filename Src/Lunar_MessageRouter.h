/*******************************************************************************
// Core Message Router Module
*******************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Incclues
// Platform Includes
#include "Platform.h"
// Other Includes
#include <stdbool.h>
#include <stdint.h>


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

//-----------------------------------------------
// Response Codes
//-----------------------------------------------

/** This enumeration defines the Lunar response codes that may be
  * generated when processing messages.                          
*/
typedef enum
{
   // No error
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_None,
   // Invalid Module ID
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidModuleID,
   // Invalid Command ID
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidCommandID,
   // Invalid command length
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidCommandLength,
   // Invalid response length
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidResponseLength,
   // Invalid Checksum
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InvalidChecksum,
   // Internal Error
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_InternalError,
   // Number of Response Codes
   LUNAR_MESSAGEROUTER_RESPONSE_CODE_Count
} Lunar_MessageRouter_ResponseCode_t;


//-----------------------------------------------
// Message Definitions
//-----------------------------------------------

// This type defines a message header
typedef struct
{
   // Unique Identifier for a destination software module
   uint8_t moduleID;

   // Identifier for a specific command in the destination software module
   uint8_t commandID;

   // Optional - Used to match responses with commands (sequencing)
   uint8_t messageID;
} Lunar_MessageRouter_MessageItemHeader_t;

// This type defines the buffer for the actual message data
typedef struct
{
   // Maximum length of the data buffer (assigned by module that owns the actual buffer)
   uint8_t maxLength;

   // Length of the data that is currently in the buffer
   uint8_t length;

   // Pointer to the actual buffer
   uint8_t *data;
} Lunar_MessageRouter_MessageItemBuffer_t;

// This type defines the complete Message structure common to all 
// Message Router functions -- composed of Command and Response
typedef struct
{
   // Command Header
   Lunar_MessageRouter_MessageItemHeader_t header;

   // Command received from external source
   Lunar_MessageRouter_MessageItemBuffer_t commandParams;

   // Response built by internal module
   Lunar_MessageRouter_MessageItemBuffer_t responseParams;

   // The response for this message
   Lunar_MessageRouter_ResponseCode_t responseCode;
} Lunar_MessageRouter_Message_t;


//-----------------------------------------------
// Command Table Definitions
//-----------------------------------------------
// This type defines a function pointer that is used in the message table for 
// each module.  The handler is the destination for an incoming message.
// Command parameters (if any) are contained in the passed message structure.
// Responses (if any) are expected to be set in the passed message structure before 
// the handler completes.  The message router will handle packetizing the response
// and sending the response to the sender.
typedef void (*Lunar_MessageRouter_MessageHandler_t)(Lunar_MessageRouter_Message_t *const message);

// This is an item in the command table for a module.  This associates a
// message handler with a command ID.
// functions of a module.
typedef struct
{
   // Command ID
   uint8_t commandID;

   // Message Handler Function
   const Lunar_MessageRouter_MessageHandler_t messageHandler;
} Lunar_MessageRouter_CommandTableItem_t;


//-----------------------------------------------
// Module Table Definitions
//-----------------------------------------------
// This is an item in the module table.  This associates a command table with a
// module ID.
typedef struct
{
   // Module ID
   uint8_t moduleID;

   // Command Table
   const Lunar_MessageRouter_CommandTableItem_t *commandTable;

   // Number of Commands in the Command Table
   uint8_t numCommands;
} Lunar_MessageRouter_ConfigItem_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function accepts a Message Router populated with the complete
  *    Message information and a header for the Response.  This information is
  *    used to route the Message to the corresponding handler.  The handlers are
  *    expected to be define in the module's Message Table.  The Response header
  *    will be used if  the destination module wishes to respond to have the
  *    response sent back to the originating driver.
  * Parameters:
  *    message - Pointer to the Message Object to be processed
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_MessageRouter_ProcessMessage(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function checks the given message object to verify that
  *    the command length matches the expected value.
  * Parameters:
  *    message :      Pointer to the Message Object containing the
  *                   information to be verified.
  *    commandSize :  The expected size of the command parameters in
  *                   the message object.
  * Returns:
  *    bool - The result of the size verification.
  * Return Value List:
  *    true :   The length of the command data in the given message
  *             object matches the desired length
  *    false :  The length does not match the expected value or the
  *             given message object was not valid.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
bool Lunar_MessageRouter_VerifyCommandSize(Lunar_MessageRouter_Message_t *const message, const uint8_t commandSize);

/** Description:
  *    This function checks the given message object to verify that
  *    a response of the specified size will fit.
  * Parameters:
  *    message :       Pointer to the Message Object containing the
  *                    information to be verified.
  *    responseSize :  The desired size for the response
  * Returns:
  *    bool - The result of the size verification.
  * Return Value List:
  *    true :   The given response size will fit in the message
  *             response
  *    false :  The given response length is too large for the
  *             message or the given message object was not valid.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
bool Lunar_MessageRouter_VerifyResponseSize(Lunar_MessageRouter_Message_t *const message, const uint8_t responseSize);

/** Description:
  *    This function checks the given message object to verify that no
  *    parameters were given.
  * Parameters: 
  *    message - Pointer to the Message Object containing the information to be 
  *    verified.
  * Returns: 
  *    bool - The result of the size verification.
  * Return Value List: 
  *    true - No command parameters were given in the message object
  *    false - Command parameters were sent unexpectedly for this message or the
  *    given message object was not valid.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_MessageRouter_VerifyNoCommandParameters(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function checks the given message object to verify that a response
  *    with no parameters will fit. This primarily added for consistency.
  * Parameters: 
  *    message - Pointer to the Message Object containing the information to be 
  *    verified.
  * Returns: 
  *    bool - The result of the size verification.
  * Return Value List: 
  *    true - A response with no parameters will fit in the given message
  *    object.
  *    false - The given message object was not valid
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_MessageRouter_VerifyNoResponseParameters(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function assists with verifying both the command and
  *    command and response sizes.
  * Parameters:
  *    message :       Pointer to the Message Object containing the
  *                    information to be verified.
  *    commandSize :   The expected size of the command parameters in
  *                    the message object.
  *    responseSize :  The desired size for the response
  * Returns:
  *    bool - The result of the size verification.
  * Return Value List:
  *    true :   The length of the command data in the given message
  *             object matches the desired length and the response
  *             size will fit in the message object.
  *    false :  Either the command or response size verification
  *             failed or the given message object was not valid.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                  
*/
bool Lunar_MessageRouter_VerifyParameterSizes(Lunar_MessageRouter_Message_t *const message, const uint8_t commandSize, const uint8_t responseSize);

/** Description:
  *    This function assists with setting the size of the response data in the
  *    given message object.
  * Parameters:
  *    message - Pointer to the Message Object containing the information to be
  *    verified. 
  *    responseSize - The desired size for the response
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_MessageRouter_SetResponseSize(Lunar_MessageRouter_Message_t *const message, const uint8_t responseSize);


#ifdef __cplusplus
extern "C"
}
#endif
