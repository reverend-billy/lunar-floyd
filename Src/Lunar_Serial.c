/*******************************************************************************
// Core Serial Protocol Module
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_Serial.h"
// Platform Includes
#include "Lunar_CircularBufferLib.h"
#include "Lunar_MessageRouter.h"
// Other Includes
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "UART_Drv.h" // For UART API
#include "UART_Drv_Config.h" // For UART channel enumeration


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// Uncomment to constantly send data during every update loop
// This is just used for debugging serial port
//#define DEBUG_SEND_CONSTANT_DATA

// Size of the local circular buffer used for transmitting data (Max 255 in Send function)
#define TX_BUFFER_SIZE (128)

// Size of the local circular buffer used for receiving data.
#define RX_BUFFER_SIZE (128)

// Address used to identifying messages intended for any device
#define BROADCAST_ADDRESS (0xFFU)

//-----------------------------------------------
// Command/Response Constants
//-----------------------------------------------

// This byte indicates the start of an incoming command.
#define COMMAND_START_BYTE ('<')
// Module ID is at the start of the message -- immediately after the start byte
#define MODULE_ID_OFFSET (0)

// Number of bytes used to denote the length of the data portion
#define DATA_LENGTH_SIZE (1)

// Defines if we are using addressing at the beginning of the message
#define NUM_ADDRESS_BYTES (0)

// This defines the length of a command header in bytes
// Address, if enabled
// Module ID
// Command ID
// Message ID
// Data Length (Part of data buffer, not header)
#define COMMAND_HEADER_SIZE (NUM_ADDRESS_BYTES + sizeof(Lunar_MessageRouter_MessageItemHeader_t) + DATA_LENGTH_SIZE)

// 2 ASCII characters per byte ("FF")
#define HEX_CHARS_PER_BYTE (2)

/** This defines the length of a command header in ASCII-coded
  * hex.                                                      
*/
#define COMMAND_HEADER_SIZE_HASCII (HEX_CHARS_PER_BYTE * COMMAND_HEADER_SIZE)

// This defines the maximum length of command data in bytes.
#define COMMAND_DATA_MAX_SIZE (48)

/** This defines the maximum length of command data in
  * ASCII-coded hex.                                  
*/
#define COMMAND_DATA_MAX_SIZE_HASCII (HEX_CHARS_PER_BYTE * COMMAND_DATA_MAX_SIZE)

// This defines the size of the CRC value used in the message, when enabled
// 2 bytes for CRC-16
#define NUM_CRC_BYTES (0)

// This defines the length of a command footer in bytes
// The footer is only the CRC
#define COMMAND_FOOTER_SIZE (NUM_CRC_BYTES)
/** This defines the length of a command header in ASCII-coded
  * hex.                                                      
*/
#define COMMAND_FOOTER_SIZE_HASCII (HEX_CHARS_PER_BYTE * COMMAND_FOOTER_SIZE)

// This defines the maximum command size in bytes
#define COMMAND_MAX_SIZE (COMMAND_HEADER_SIZE + COMMAND_DATA_MAX_SIZE + COMMAND_FOOTER_SIZE)
/*This defines the maximum command size in ASCII-coded hex.
*/
#define COMMAND_MAX_SIZE_HASCII (HEX_CHARS_PER_BYTE * COMMAND_MAX_SIZE)

// This is the stop byte used for all incoming commands.
#define COMMAND_STOP_BYTE_1 ('\r')
/*This is an alternate stop byte used for incoming commands.
*/
#define COMMAND_STOP_BYTE_2 ('\n')


//-----------------------------------------------
// Response Packet
//-----------------------------------------------

// This is the start byte used for all outgoing responses.
#define RESPONSE_START_BYTE ('>')

// This defines the length of a response header in bytes -- same as command
// Byte 1: Module ID
// Byte 2: Command ID
// Byte 3: Message ID
// Byte 4: Data Length
#define RESPONSE_HEADER_SIZE (COMMAND_HEADER_SIZE)
/** This defines the length of a response header in ASCII-coded
  * hex.                                                       
*/
#define RESPONSE_HEADER_SIZE_HASCII (HEX_CHARS_PER_BYTE * RESPONSE_HEADER_SIZE)

// This defines the maximum length of response data in bytes
#define RESPONSE_DATA_MAX_SIZE (48)

/** This defines the maximum length of response data in
  * ASCII-coded hex                                    
*/
#define RESPONSE_DATA_MAX_SIZE_HASCII (HEX_CHARS_PER_BYTE * RESPONSE_DATA_MAX_SIZE)

// This defines the length of a response footer in bytes
// There currently is no footer for debug port
#define RESPONSE_FOOTER_SIZE (0)
/** This defines the length of a response header in ASCII-coded
  * hex.                                                       
*/
#define RESPONSE_FOOTER_SIZE_HASCII (HEX_CHARS_PER_BYTE * RESPONSE_FOOTER_SIZE)

// This defines the maximum response size in bytes
#define RESPONSE_MAX_SIZE (RESPONSE_HEADER_SIZE + RESPONSE_DATA_MAX_SIZE + RESPONSE_FOOTER_SIZE)
/*This defines the maximum response size in ASCII-coded hex.
*/
#define RESPONSE_MAX_SIZE_HASCII (HEX_CHARS_PER_BYTE * RESPONSE_MAX_SIZE)

// This is the stop byte used for all outgoing responses.
#define RESPONSE_STOP_BYTE ('\r')


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// Structure to hold the ASCII hex data when retrieving a valid command from the
// RX circular buffer
typedef struct
{
   // Denotes if the start byte has been found while searching for a complete message
   bool isStartByteFound;
   // Size of the data buffer
   uint16_t dataBufferLen;
   // Buffer used for storing the complete ASCII command during processing
   uint8_t data[COMMAND_MAX_SIZE_HASCII];
} ASCIICommandItem_t;

// Holds statistics on TX/RX data and messages
// Note this structure needs to be packed properly since it is reused in the GetSerialStatistics method
typedef struct
{
   // RX byte count for the port
   // Incremented after each call to UART_ReadByte
   uint32_t numBytesReceived;
   // TX byte count for the port
   // Incremented for when Data is sent to the Serial Protocol module
   uint32_t numBytesSent;
   // Number of complete message received
   // Incremented after a complete command is found during the
   // scheduled update loop
   uint32_t numMessagesReceived;
   /** The number of complete messages sent Incremented at each call
     * to the Send Response function                                
   */
   uint32_t numMessagesSent;
} TxRxStatistics_t;

// Structure to hold buffers and data for each port
typedef struct
{
   // This is the message structure for the message that must be
   // populated and sent to the message router for routing to the
   // destination software module.
   Lunar_MessageRouter_Message_t currentMessage;

   // This buffer is allocated for command data and is pointed to by the
   // command proc message structure.
   uint8_t commandBuffer[COMMAND_DATA_MAX_SIZE];

   // This buffer is allocated for response data and is pointed to by the
   // command proc message structure.
   uint8_t responseBuffer[RESPONSE_DATA_MAX_SIZE];

   // This is the information for assembling the next command as we
   // dequeue bytes from the UART driver
   // The data in this buffer is ASCII data that must be converted to binary
   // before sending on to the command processor.
   ASCIICommandItem_t asciiCommand;

   /** The address for this device on this port For simplicity in
     * the driver, this initializes to BROADCAST_ADDRESS if
     * addressing is not used                                    
   */
   uint8_t deviceAddress;

   // Stats for transmit and receive data
   TxRxStatistics_t statistics;
} PortData_t;

// This structure holds the private information for this module
typedef struct
{
   // Create a status object for each port used
   PortData_t portData[UART_DRV_CHANNEL_Count];
} Lunar_Serial_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static Lunar_Serial_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    This function searches the circular buffer for the given channel to find
  *    a complete command to be processed. (All data between Start and Stop
  *    characters.) Note that it may take mutiple calls to this function to
  *    locate a complete message.
  * Parameters: 
  *    channel : The enumerated channel value for which this function will search for a command.
  *    asciiCommand : The located command in ASCII format
  * Returns: 
  *    bool: The result of the command search
  * Return Value List:
  *    true: Command found and placed into the ASCII command buffer for
  *    processing 
  *    false: No command found
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
static bool FindNextCommand(const UART_Drv_Channel_t channel, ASCIICommandItem_t *const asciiCommand);

/** Description:
  *    This function packetizes the given message response as ASCII-coded hex data and add the
  *    data to the outgoing transmit buffer;
  * Parameters: 
  *    channel : The enumerated channel value used for sending this message.
  *    message : A pointer to the Message Router object defining the message to be sent.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
static void SendResponseAsciiHex(const UART_Drv_Channel_t channel, Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function converts one or two ASCII-coded hex cahracters to a 16-bit
  *    value.
  * Parameters: 
  *    hexCharacters    : pointer to the buffer of ASCII character to be converted.
  *    numHexCharacters : The number of characters in the given buffer.
  * Returns: 
  *    uint16_t: The converted numeric value of the given ASCII-coded hex value.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
static uint16_t ConvertAsciiHexStringToNumeric(const uint8_t *const hexCharacters, const uint8_t numHexCharacters);

/** Description:
  *    This function takes a character '0' - 'F' and converts it to its hex equivalent 
  *    ('F' becomes 0x0F)
  * Parameters: 
  *    charToConvert: The character to be to be converted.
  * Returns: 
  *    uint8_t: The Hex representation of the given character.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
static uint8_t ConvertAsciiHexCharToNumeric(const uint8_t charToConvert);

/** Description:
  *    This function converts a 16-bit value to an ASCII-coded hex string
  *    and stores it in the given buffer. NOTE: The given buffer must be as long
  *    as the specified length
  * Parameters: 
  *    destinationBuffer: The pointer to the buffer where the conversion is to be stored
  *    desiredLength: The number of nibbles to be converted (Max 4)
  *    valueToConvert: The value that is to be converted
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                 
*/
static void ConvertNumericToAsciiHexString(uint8_t *const destinationBuffer, const uint8_t desiredLength, const uint16_t valueToConvert);


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Search circular buffer for the next command
static bool FindNextCommand(const UART_Drv_Channel_t channel, ASCIICommandItem_t *const asciiCommand)
{
   // Start with no command found
   bool wasCommandFound = false;

   // Verify the channel is valid
   if ((asciiCommand != 0) && (channel < UART_DRV_CHANNEL_Count))
   {
      // Init to null char
      uint8_t tmpByte = 0U;

      // Get all bytes from the circular RX buffer
      // Note this reads from the buffer not the port so it does not block.
      while ((!wasCommandFound) && (UART_Drv_ReadByte(channel, &tmpByte)))
      {
         // Increase the number of bytes received for this channel
         status.portData[channel].statistics.numBytesReceived += sizeof(tmpByte);

         // See if the current byte is a command "Start" byte.
         if (tmpByte == COMMAND_START_BYTE)
         {
            // Store the flag so if the buffer only contains the first half of the
            // message, we will continue next time.
            asciiCommand->isStartByteFound = true;
            // Always reset the size when a start byte is found.  If
            // the start byte of the next message is received before
            // the stop byte of the previous message, then the previous
            // message will be ignored.
            asciiCommand->dataBufferLen = 0;
         }
         else if ((tmpByte == COMMAND_STOP_BYTE_1) || (tmpByte == COMMAND_STOP_BYTE_2))
         {
            // Complete command found: clear start byte flag
            asciiCommand->isStartByteFound = false;
            // Mark that we have found a command (which will exit the loop)
            wasCommandFound = true;
         }
         else if (asciiCommand->isStartByteFound)
         {
            // If we have room for the next byte of the command...
            if (asciiCommand->dataBufferLen < (uint16_t)COMMAND_MAX_SIZE_HASCII)
            {
               // Add byte to command buffer and increment size
               asciiCommand->data[asciiCommand->dataBufferLen++] = tmpByte;
            }
            // Otherwise, clear the command buffer and send an error...
            else
            {
               // Reset command buffer
               asciiCommand->isStartByteFound = false;
               asciiCommand->dataBufferLen = 0;
            }
         }
         // else, byte is not part of a valid message.  Throw it away
      }
   }

   return(wasCommandFound);
}


// Send message response using hex encoding
static void SendResponseAsciiHex(const UART_Drv_Channel_t channel, Lunar_MessageRouter_Message_t *const message)
{
   // Make sure the given channel is valid
   if (channel < UART_DRV_CHANNEL_Count)
   {
      // Check for NULL pointer
      if (message != 0)
      {
         // Message structure is valid.
         // Make sure the response data buffer is valid.
         if (message->responseParams.data != 0)
         {
            // Response data appears to be valid, so send the HASCII response.
            // Start Byte
            uint8_t temp = (uint8_t)RESPONSE_START_BYTE;

            Lunar_Serial_Send(channel, &temp, 1, SERIAL_ENCODING_BINARY);
#if (NUM_ADDRESS_BYTES > 0)
            // Send address - 0 is the master
            Lunar_Serial_Send(channel, 0, 1, SERIAL_ENCODING_ASCII_CODED_HEX);
#endif
            // ModID
            Lunar_Serial_Send(channel, &message->header.moduleID, 1, SERIAL_ENCODING_ASCII_CODED_HEX);
            // CmdID
            Lunar_Serial_Send(channel, &message->header.commandID, 1, SERIAL_ENCODING_ASCII_CODED_HEX);
            // MsgID
            Lunar_Serial_Send(channel, &message->header.messageID, 1, SERIAL_ENCODING_ASCII_CODED_HEX);
            // Length
            Lunar_Serial_Send(channel, &message->responseParams.length, 1, SERIAL_ENCODING_ASCII_CODED_HEX);
            // Buffer
            Lunar_Serial_Send(channel, message->responseParams.data, message->responseParams.length, SERIAL_ENCODING_ASCII_CODED_HEX);
            // Stop Byte
            temp = (uint8_t)RESPONSE_STOP_BYTE;
            Lunar_Serial_Send(channel, &temp, 1, SERIAL_ENCODING_BINARY);

            // Increment the number of messages sent
            status.portData[channel].statistics.numMessagesSent++;
         }
      }
   }
}


// Convert a series of ASCII-coded hex values to a single 16-bit numeric value
static uint16_t ConvertAsciiHexStringToNumeric(const uint8_t *const hexCharacters, const uint8_t numHexCharacters)
{
   // Default to 0 result
   uint16_t resultValue = 0U;
   uint8_t tmpByte = '0';
   const uint8_t *nextByte;

   // Check for NULL pointer.  If it is NULL return 0 for the hex value.
   if (hexCharacters != 0)
   {
      // Point to the beginning of the data
      nextByte = hexCharacters;

      // Since this returns a uint16_t, we can only convert a maximum
      // of 4 characters.  If a length > 4 is specified, only do the
      // first 4 characters.
      uint8_t tmpNumHexCharacters = numHexCharacters;

      if (tmpNumHexCharacters > 4)
      {
         tmpNumHexCharacters = 4;
      }

      // Number of bits to shift
      uint8_t shiftValue = 0U;

      // Loop through each character
      // Use 1-based indexing
      for (uint8_t i = 1U; i <= tmpNumHexCharacters; i++)
      {
         // Convert next ASCII character to its HEX equivalent
         tmpByte = ConvertAsciiHexCharToNumeric(*nextByte);

         // We need to shift the HEX character to it's correct position:
         // (i.e.) 7 in 0xF7FF must be shifted left 8 positions from zero
         // to be in the right spot.
         shiftValue = 4 * (tmpNumHexCharacters - i);
         resultValue += ((uint16_t)tmpByte) << shiftValue;

         // Move to the next byte for the next pass through loop
         nextByte++;
      }
   }

   // Finally, return the converted value
   return(resultValue);
}


// Convert ASCII-coded hex value to 8-bit numeric representation
static uint8_t ConvertAsciiHexCharToNumeric(const uint8_t charToConvert)
{
   uint8_t value = 0U;

   // 'A' - 'F'
   if ((charToConvert > 0x40) && (charToConvert < 0x47))
   {
      value = (charToConvert - 0x37);
   }
   // 'a' - 'f'
   else if ((charToConvert > 0x60) && (charToConvert < 0x67))
   {
      value = (charToConvert - 0x57);
   }
   // '0' - '9'
   else if ((charToConvert > 0x2F) && (charToConvert < 0x3A))
   {
      value = (charToConvert - 0x30);
   }

   // else, invalid hex character -- return 0

   return(value);
}


// Convert value to ASCII-coded hex
static void ConvertNumericToAsciiHexString(uint8_t *const destinationBuffer, const uint8_t desiredLength, const uint16_t valueToConvert)
{
   // Store the given value locally to allow modification during conversion
   uint16_t tmpValue = valueToConvert;

   // Check for NULL pointer.
   if (destinationBuffer != 0)
   {
      // The MSB of the value to convert will go in buffer
      // position 0, and the LSB will go in buffer position
      // length-1.  Start at the LSB position.
      uint8_t bufferPosition = desiredLength - 1;

      // Variable used for the current nibble being converted -- init to 0
      uint8_t tmpNibble = 0U;
      // Converted nibble -- init to null character
      uint8_t tmpASCIIChar = 0U;

      // Loop through the number of characters requested and build the ASCII string
      for (uint8_t i = 0U; i < desiredLength; i++)
      {
         // Mask off the lowest nibble in the value we are converting
         tmpNibble = (uint8_t)(tmpValue & (uint16_t)(0x000F));

         // Convert to ASCII
         // 0 - 1
         if (tmpNibble < 10)
         {
            tmpASCIIChar = tmpNibble + 0x30;
         }
         // A - F
         else if (tmpNibble < 16)
         {
            // character is 11-15 since it was masked to one nibble
            tmpASCIIChar = tmpNibble + 0x37;
         }
         else
         {
            // Added so that if statement has a default case
            tmpASCIIChar = 0;
         }

         // Put the ASCII value in the next buffer position
         // Destination is buffer address
         *(destinationBuffer + bufferPosition) = tmpASCIIChar;

         // Move the buffer position to the left
         bufferPosition--;

         // Shift the value so the next nibble is in the lowest position
         tmpValue >>= 4;
      }
   }
}


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Initialize all configured serial ports
void Lunar_Serial_Init(void)
{
   // Init the UART driver
   UART_Drv_Init();

   for (uint8_t portIndex = 0; portIndex < UART_DRV_CHANNEL_Count; portIndex++)
   {
      // Init the port information and buffers
      memset(&status.portData[portIndex], 0, sizeof(PortData_t));

      // Always start with the broadcast address
      status.portData[portIndex].deviceAddress = BROADCAST_ADDRESS;
   }
}

// Scheduled update loop for processing messages
void Lunar_Serial_Update(void)
{
   // Loop through each port and check for new commands
   for (uint8_t channel = 0U; channel < UART_DRV_CHANNEL_Count; channel++)
   {
#ifdef DEBUG_SEND_CONSTANT_DATA
      // Constantly send data during every update loop
      // This is just used for debugging serial port
      Lunar_Serial_Send((UART_Drv_Channel_t)channel, "*", 1, SERIAL_ENCODING_BINARY);
#endif

      //-----------------------------------------------
      // Process RX Data
      //-----------------------------------------------
      // Store the command object for easy access
      ASCIICommandItem_t *asciiCommand = &(status.portData[channel].asciiCommand);

      // Look for a valid command in the circular RX buffer
      if (FindNextCommand((UART_Drv_Channel_t)channel, asciiCommand))
      {
         // A complete command was received, now we need to populate the standard
         // message structure with the data in this command.

         // Store the message object for easy access
         Lunar_MessageRouter_Message_t *const message = &(status.portData[channel].currentMessage);

         // Make sure the length of the command is at least long enough to
         // contain a complete HASCII command header.  The data in the Next Command
         // buffer is HASCII, so compare it to the HASCII length of the command header.
         if (asciiCommand->dataBufferLen >= ((uint16_t)COMMAND_HEADER_SIZE_HASCII + (uint16_t)COMMAND_FOOTER_SIZE_HASCII))
         {
            // Init the message to no error
            status.portData[channel].currentMessage.responseCode = LUNAR_MESSAGEROUTER_RESPONSE_CODE_None;

            //-----------------------------------------------
            // Parse Header
            //-----------------------------------------------

            // Populate the command header.  This tells the Message Router
            // how to route the command to the destination module.

            // We start at the first byte
            uint8_t tmpIndex = (uint8_t)0U;
            // 2 hex character per byte
            uint8_t tmpCharacterCount = (uint8_t)HEX_CHARS_PER_BYTE;

#if (NUM_ADDRESS_BYTES > 0)
            // Extract the Destination Address
            // Note size has been verified above to be at least Address +  Message Header + Data Length
            uint8_t destinationAddress = (uint8_t)AsciiToHex(&(asciiCommand->data[tmpIndex]), tmpCharacterCount);

            // Move to the next byte
            tmpIndex += tmpCharacterCount;
#else

            // Addressing is not used, just set address to the broadcast address (0xFF)
            uint8_t destinationAddress = (uint8_t)BROADCAST_ADDRESS;
#endif

            // Verify this message is intended for us
            // If addressing is not used, our address will be the broadcast address and the message is accepted
            if ((destinationAddress == BROADCAST_ADDRESS) || (destinationAddress == status.portData[channel].deviceAddress))
            {
               // Extract everything and verify the CRC

               // This message is for us, continue and extract the Module ID
               message->header.moduleID = (uint8_t)ConvertAsciiHexStringToNumeric(&(asciiCommand->data[tmpIndex]), tmpCharacterCount);

               // Move to the next byte for CMD ID
               tmpIndex += tmpCharacterCount;
               message->header.commandID = (uint8_t)ConvertAsciiHexStringToNumeric(&(asciiCommand->data[tmpIndex]), tmpCharacterCount);

               // Move to the next byte for MSG ID
               tmpIndex += tmpCharacterCount;
               message->header.messageID = (uint8_t)ConvertAsciiHexStringToNumeric(&(asciiCommand->data[tmpIndex]), tmpCharacterCount);

               //-----------------------------------------------
               // Initialize Command Buffer
               //-----------------------------------------------

               // Move to the next byte for DATA LENGTH
               tmpIndex += tmpCharacterCount;
               // Assign the command buffer
               message->commandParams.data = status.portData[channel].commandBuffer;
               // Set the max size to prevent other modules from overwriting the bounds of the data buffer.
               message->commandParams.maxLength = (uint8_t)COMMAND_DATA_MAX_SIZE;
               // Get the length byte
               message->commandParams.length = (uint8_t)ConvertAsciiHexStringToNumeric(&(asciiCommand->data[tmpIndex]), tmpCharacterCount);

               //-----------------------------------------------
               // Initialize Response Buffer
               //-----------------------------------------------

               // Setup the buffer for the response
               message->responseParams.data = status.portData[channel].responseBuffer;
               message->responseParams.maxLength = (uint8_t)RESPONSE_DATA_MAX_SIZE;
               message->responseParams.length = 0U;

               //-----------------------------------------------
               // Process Command
               //-----------------------------------------------

               // Increment the number of messages received since this command will at least generate some sort of response message
               status.portData[channel].statistics.numMessagesReceived++;

               // Verify the length
               // The length in the command buffer is what was specified in the command
               // and represents the number of hex bytes are in the data field after converting
               // from HASCII.  The sNextCommand buffer is still in HASCII, so we need to
               // convert the length in the command buffer to HASCII by multiplying by 2.
               if (asciiCommand->dataBufferLen == (COMMAND_HEADER_SIZE_HASCII + (HEX_CHARS_PER_BYTE * message->commandParams.length) + COMMAND_FOOTER_SIZE_HASCII))
               {
                  // Length is correct.
                  // Now ensure the length is within the bounds of the data buffer
                  // before we convert the HASCII bytes to binary and copy them to the
                  // buffer.  This will prevent buffer overflow.
                  if (message->commandParams.length <= message->commandParams.maxLength)
                  {
                     // Convert each byte in the data field from HASCII to hex.
                     for (uint8_t i = 0U; i < message->commandParams.length; i++)
                     {
                        // Convert the next data byte from HASCII to hex and store in
                        // the command data buffer.
                        status.portData[channel].commandBuffer[i] = (uint8_t)ConvertAsciiHexStringToNumeric(&(asciiCommand->data[(uint8_t)COMMAND_HEADER_SIZE_HASCII + HEX_CHARS_PER_BYTE * i]), HEX_CHARS_PER_BYTE);
                     }

                     // Process message
                     Lunar_MessageRouter_ProcessMessage(message);
                     // Increment the number of messages sent
                     // Send the response out the serial port.
                     SendResponseAsciiHex((UART_Drv_Channel_t)channel, message);
                  }
                  else
                  {
                     // The specified length is longer than our available command buffer size.
                     // Do not process this command, just send a response with the same
                     // header, with a length of 0 and no data.
                     SendResponseAsciiHex((UART_Drv_Channel_t)channel, message);
                  }
               }
               else
               {
                  // The specified length is incorrect.
                  // Do not process this message, just send a response with the same
                  // header, with a length of 0 and no data.
                  SendResponseAsciiHex((UART_Drv_Channel_t)channel, message);
               }
            }
         }

         // Command has been processed, remove it.
         asciiCommand->dataBufferLen = 0U;
      }
   }
}


// Clear statistics for the given port
void Lunar_Serial_ResetStats(const UART_Drv_Channel_t channel)
{
   // Verify the port is valid
   if (channel < UART_DRV_CHANNEL_Count)
   {
      // Port is valid, reset everything to 0
      memset(&(status.portData[channel].statistics), 0, sizeof(TxRxStatistics_t));
   }
}


// Send encoded data to the given port
void Lunar_Serial_Send(const UART_Drv_Channel_t channel, uint8_t *const data, const uint8_t dataLength, const Lunar_Serial_Encoding_t outputEncoding)
{
   // Verify the channel index is valid
   if (channel < UART_DRV_CHANNEL_Count)
   {
      // Make sure the length of the data we want to send will fit in the TX buffer -- max 255 bytes
      if ((data != 0) && (dataLength < (uint8_t)TX_BUFFER_SIZE))
      {
         // Output buffer for converting each byte to hex -- only used for Hex encoding
         uint8_t tmpOutputBuffer[HEX_CHARS_PER_BYTE];

         // If this message is a response message, convert the TxBuffer data into
         // ASCII encoded hex or HASCII (i.e. 0x0F = "0F").
         switch (outputEncoding)
         {
            case SERIAL_ENCODING_ASCII_CODED_HEX:
               // Convert each data byte to HASCII and put in TX Buffer
               // UART_Write implements a circular buffer so we do not have to wait
               for (uint8_t i = 0U; i < dataLength; i++)
               {
                  // Convert the byte to it's ASCII form.
                  ConvertNumericToAsciiHexString(tmpOutputBuffer, sizeof(tmpOutputBuffer), *(data + i));
                  // Add the two characters to the tx buffer.
                  UART_Drv_Write(channel, tmpOutputBuffer, sizeof(tmpOutputBuffer));
                  // Increment the bytes sent by 2 bytes
                  status.portData[channel].statistics.numBytesSent += sizeof(tmpOutputBuffer);
               }

               break;
            case SERIAL_ENCODING_BINARY:
            default:
               // Otherwise, data the specified message directly in the TX Buffer
               UART_Drv_Write(channel, data, dataLength);
               // Increase bytes sent by dataLength
               status.portData[channel].statistics.numBytesSent += dataLength;
               break;
         }
      }
   }
}


// Message Router function to reutrn statistics
void Lunar_Serial_MessageRouter_GetSerialStatistics(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------

   // This structure defines the format of the command
   typedef struct
   {
      // UART channel index being requested
      uint8_t channelIndex;
   } Command_t;

   // This structure defines the format of the response
   typedef struct
   {
      // Items from Statistics_t
      uint32_t numBytesSent;
      uint32_t numBytesReceived;
      uint32_t numMessagesSent;
      uint32_t numMessagesReceived;
      uint32_t msSinceLastMessageReceived;
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

      // Verify the index is valid
      if (command->channelIndex < UART_DRV_CHANNEL_Count)
      {
         // Port is valid, store the statistics object for easy access
         TxRxStatistics_t *tmpStatistics = &(status.portData[command->channelIndex].statistics);

         // Just store each of the items for the given port
         response->numBytesSent = tmpStatistics->numBytesSent;
         response->numBytesReceived = tmpStatistics->numBytesReceived;
         response->numMessagesSent = tmpStatistics->numMessagesSent;
         response->numMessagesReceived = tmpStatistics->numMessagesReceived;
         // TODO - Calculate time since last message
         response->msSinceLastMessageReceived = 0;
      }

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

