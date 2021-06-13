/*******************************************************************************
// Core Circular Buffer Library
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_CircularBufferLib.h"
// Platform Includes
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/


/*******************************************************************************
// Private Prototypes
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

// Initialize the circular buffer structure with the given buffer
void Lunar_CircularBufferLib_Init(Lunar_CircularBufferLib_t *const circularBuffer, uint8_t *const dataBuffer, const uint16_t bufferSize)
{
   // Verify the given parameters
   if (circularBuffer != 0)
   {
      // Initialize the Circular TX Buffer.
      circularBuffer->indexRead = 0;
      circularBuffer->indexWrite = 0;
      circularBuffer->size = 0;
      circularBuffer->inUse = false;
      // Set up the pointer to the data buffer.
      if (dataBuffer != 0)
      {
         // Buffer is valid, store it
         circularBuffer->data = dataBuffer;
         // Store the size of the buffer
         circularBuffer->maxSize = bufferSize;
      }
      else
      {
         // Buffer is not valid, just make buffer null with size 0 to prevent use
         circularBuffer->maxSize = 0;
         circularBuffer->data = (void *)0;
      }
   }
}


// Place a single byte into the given circular buffer
bool Lunar_CircularBufferLib_Enqueue(Lunar_CircularBufferLib_t *const circularBuffer, const uint8_t byteToStore)
{
   bool bSuccess = false;

   // Check for NULL pointer.
   if (circularBuffer != 0)
   {
      // Check the semaphore to see if the buffer is already in use.
      if (!circularBuffer->inUse)
      {
         // Set the semaphore.
         circularBuffer->inUse = true;

         // Add the byte to the circular receive buffer at the write index.
         circularBuffer->data[circularBuffer->indexWrite] = byteToStore;

         // Increment the write index.
         circularBuffer->indexWrite++;

         // Wrap write index if necessary.
         if (circularBuffer->indexWrite >= circularBuffer->maxSize)
         {
            circularBuffer->indexWrite = 0;
         }

         // Increment the size.
         circularBuffer->size++;

         // If buffer overflows, remove oldest data
         if (circularBuffer->size > circularBuffer->maxSize)
         {
            // Increment the read index (because the write passed the read index).
            circularBuffer->indexRead++;
            // Decrement the size of the data in the buffer.
            circularBuffer->size--;
         }

         // Clear the semaphore.
         circularBuffer->inUse = false;

         // Data enqueued successfully.
         bSuccess = true;
      }
   }

   // Return the success of the operation.
   return(bSuccess);
}


// Retreive a single byte from the given circular buffer
bool Lunar_CircularBufferLib_Dequeue(Lunar_CircularBufferLib_t *const circularBuffer, uint8_t *const byteRetreived)
{
   bool bSuccess = false;

   // Check for NULL pointers
   if ((circularBuffer != 0) && (byteRetreived != 0))
   {
      // Check the semaphore to see if the buffer is already in use
      if (!circularBuffer->inUse)
      {
         // See if there is data in the buffer
         if (circularBuffer->size > 0)
         {
            // Set the semaphore
            circularBuffer->inUse = true;

            // Get the byte at the read index.
            *byteRetreived = circularBuffer->data[circularBuffer->indexRead];

            // Move to the next byte in the buffer
            circularBuffer->indexRead++;
            // Decrement the size of the data in the buffer
            circularBuffer->size--;

            // Wrap the read index when we hit the maximum size
            if (circularBuffer->indexRead >= circularBuffer->maxSize)
            {
               // Just wrap the index back to the start index
               circularBuffer->indexRead = 0;
            }

            // Clear the semaphore
            circularBuffer->inUse = false;

            // Data dequeued successfully
            bSuccess = true;
         }
      }
   }

   // Return the status of the operation.
   return(bSuccess);
}


