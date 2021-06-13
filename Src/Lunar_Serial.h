/*******************************************************************************
// Core Serial Protocol Module
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
#include "Lunar_MessageRouter.h"
#include "GPIO_Drv.h"
#include "Platform.h"
#include "UART_Drv.h"
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Encoding type enumeration
typedef enum
{
   // Protocol uses binary encoding for protocol data
   SERIAL_ENCODING_BINARY,
   // Protocol uses ASCII-coded hex encoding for protocol data   
   SERIAL_ENCODING_ASCII_CODED_HEX
} Lunar_Serial_Encoding_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the serial module and calls the
  *    UART driver initialization function for each configure port.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
void Lunar_Serial_Init(void);

/** Description:
  *    This is the scheduled update function that will check the receive buffer
  *    for a complete command.  If found, the corresponding command will be
  *    executed via a call to the Message Router module.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Serial_Update(void);

/** Description:
  *    Reset the TX/RX statistics for the given port.  
  * Parameters: 
  *    channel - The configured UART port that is to be reset
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Serial_ResetStats(const UART_Drv_Channel_t channel);

/** Description:
  *    This function is called to transmit data to the given UART. The data that
  *    is passed into this function will be placed in the circular transmit  
  *    buffer.  If we are not currently transmitting, the TX ISR must be started
  *    by writing the first byte. After the first byte, each byte from the
  *    circular buffer is sent via the transmit interrupt until the buffer
  *    is empty.
  *    NOTE:  Never send data with more than TX_BUFFER_SIZE values, or the data will
  *           wrap around the circular TX buffer.
  * Parameters: 
  *    channel - The configured UART pin that is to be written
  *    data - A pointer to the data that is to be sent
  *    length - The number of bytes in the data buffer that is to be sent
  *    encoding - Specifies the encoding to be used when placing data in the
  *    transmit buffer.  If ASCII, each byte will be converted (Ex. ASCII
  *    encoded Hex, 0x0F = "0F")
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Serial_Send(const UART_Drv_Channel_t channel, uint8_t *const data, const uint8_t length, const Lunar_Serial_Encoding_t encoding) PLATFORM_NON_NULL;

/** Description:
  *    This is the command handler used for querying serial TX/RX statistics for
  *    a given port. 
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
void Lunar_Serial_MessageRouter_GetSerialStatistics(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This is the command handler used for resetting serial TX/RX statistics
  *    for a given port.
  *    Parameters: 
  *       message :  A pointer to a common Message Router message object. The
  *       response is expected to be placed in this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
void Lunar_Serial_MessageRouter_ResetSerialStatistics(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

#ifdef __cplusplus
extern "C"
}
#endif

