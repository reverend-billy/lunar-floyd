/*******************************************************************************
// CAN Driver
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
#include "CAN_Drv_Config.h" // For channel enumeration
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
  *    This function initializes the hardware CAN channels for the device. This
  *    is primarily used for board-to-board communication on the internal bus.
  * History: 
  *    * 6/18/2021: Function created (EJH)
  *                                                              
*/
void CAN_Drv_Init(void);

/** Description:
  *    This is the scheduled update function that will check the message object
  *    for a new command.  If found, the corresponding command will be
  *    executed via a call to the Message Router module.
  * History: 
  *    * 6/18/2021: Function created (EJH)
  *                                                              
*/
void CAN_Drv_Update(void);

/** Description:
  *    This function is called to transmit a single data packet over CAN.
  *    The data that is passed into this function will be placed into the message object
  *    for sending on the bus.
  * Parameters: 
  *    channel - The configured CAN channel that is to be written
  *    data - A pointer to the data that is to be sent
  *    length - The number of bytes in the data buffer that is to be sent
  * History: 
  *    * 6/18/2021: Function created (EJH)
  *                                                              
*/
void CAN_Drv_TransmitPacket(const CAN_Drv_Channel_t channel, uint8_t *const data, uint8_t const length) PLATFORM_NON_NULL;

/** Description:
  *    This function is called to receive a single data packet from the CAN
  *    message object. The data that is passed into this function will be placed into 
  *    the Message Router object for processing.
  * Parameters: 
  *    channel - The configured CAN channel that is to be written
  *    data - A pointer where the data is to be placed.  Not that this buffer must be 
  *    large enough to hold an entire CAN data message (8 bytes).
  *    length - The pointer where the received number of bytes is to be placed.
  * Returns:
  *    bool - The result of the receive operation
  * Return Value List:
  *    true - A message was received successfully and the data was placed in the given buffer.
  *    false - No mesasage was received.
  * History: 
  *    * 6/18/2021: Function created (EJH)
  *                                                              
*/
bool CAN_Drv_ReceivePacket(const CAN_Drv_Channel_t channel, uint8_t *const data, uint8_t *const length) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif

