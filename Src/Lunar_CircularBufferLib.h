/*******************************************************************************
// Core Circular Buffer Library
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

// Structure that defines all parameters for managing a single circular buffer
typedef struct
{
   // Index where the next byte is to be read from the buffer.
   uint16_t indexRead;
   // Index where the next byte is to be added to the buffer.
   uint16_t indexWrite;
   // Size of data in the buffer (bytes).
   uint16_t size;
   // Max size of the buffer (bytes).
   uint16_t maxSize;
   // Pointer to the data buffer.
   uint8_t *data;
   // Semaphore used to access the data.
   bool inUse;
} Lunar_CircularBufferLib_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the given circular buffer structure.  The given
  *    buffer will be set and the length stored.
  * Parameters: 
  *    circularBuffer - A pointer to the circular buffer structure to be
  *    intialized.
  *    dataBuffer - The data buffer to be used by this circular buffer
  *    bufferSize - The total size of given buffer
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void Lunar_CircularBufferLib_Init(Lunar_CircularBufferLib_t *const circularBuffer, uint8_t *dataBuffer, const uint16_t bufferSize) PLATFORM_NON_NULL;

/** Description:
  *    Adds the specified data to the specified circular buffer. All
  *    indices associated with the circular buffer are updated
  *    accounting for buffer wrapping, etc.
  * Parameters:
  *    circularBuffer :  Pointer to the circular buffer.
  *    byteToStore :     Data byte to add to the circular buffer.
  * Returns:
  *    bool: The result of adding the given data to the circular
  *    buffer.
  * Return Value List:
  *    true :   The given data was successfully added to the circular
  *             buffer.
  *    false :  The given data could not be stored in the circular
  *             buffer. You may wait for the buffer to shrink and
  *             then try again.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_CircularBufferLib_Enqueue(Lunar_CircularBufferLib_t *const circularBuffer, const uint8_t byteToStore) PLATFORM_NON_NULL;

/** Description:
  *    Retrieves the next available byte from the specified circular
  *    buffer. All indices associated with the circular buffer are
  *    updated accounting for buffer wrapping, etc.                  
  * Parameters:
  *    circularBuffer :  Pointer to the circular buffer.
  *    byteRetreived  : Pointer where retrieved byte will be stored.
  * Returns:
  *    bool: The result of the dequeu operation
  * Return Value List:
  *    true: Byte was dequeued successfully and stored in the given location
  *    false: No data was dequeued
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool Lunar_CircularBufferLib_Dequeue(Lunar_CircularBufferLib_t *const circularBuffer, uint8_t *const byteRetreived) PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif

