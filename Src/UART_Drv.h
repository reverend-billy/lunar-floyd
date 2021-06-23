/*******************************************************************************
// UART Driver
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
#include "UART_Drv_Config.h" // For channel enumeration
// Platform Includes
#include "Platform.h"
// Other Includes
#include <stdbool.h> // Boolean types
#include <stdint.h> // Integer types


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the hardware UART channels for the device. This
  *    is primarily used for serial communication with a host device.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void UART_Drv_Init(void);

/** Description:
  *    Function to write a series of bytes to the given UART.  The data is
  *    placed into the circular buffer and the first byte is sent to the UART if
  *    the port is idle.
  * Parameters: 
  *    channel: The given channel to which the data is to be written.
  *    data: The buffer containing the data to be sent.
  *    length: The number of bytes to be written.  Note that the given data
  *    buffer MUST contain at least this number of bytes.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                   
*/
void UART_Drv_Write(const UART_Drv_Channel_t channel, uint8_t *const data, const uint8_t length) PLATFORM_NON_NULL;

/** Description:
  *    Function to retrieve a single byte from the circular buffer.  This is 
  *    primarily used for locating the next command to be processed.
  * Parameters: 
  *    channel:  The given channel from which to be read
  *    byteRead: The destination buffer for the retrieved byte.
  * Returns:
  *    bool: The result of the read request
  * Return Value List:
  *    true: A byte was located and placed into the given buffer
  *    false: No data was located for the given channel
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                   
*/
bool UART_Drv_ReadByte(const UART_Drv_Channel_t channel, uint8_t *byteRead) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif

