/*******************************************************************************
// Core Math Timer Library
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_MathLib.h"
// Platform Includes
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Private Constants
*******************************************************************************/


/*******************************************************************************
// Private Types
*******************************************************************************/


/*******************************************************************************
// Private Variables
*******************************************************************************/


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Saturated uint32_t addition
uint32_t Lunar_MathLib_AddSaturateUint32(const uint32_t value1, const uint32_t value2)
{

   // First add the two values
   uint32_t result = value1 + value2;

   // If the result is smaller than either of the two given values, we have overflowed
   // To prevent branching, we OR the result with 0xFFFFFFFF (-1) to set all bits
   // Otherwise, we'll OR with 0 and just return the sum
   result |= -(uint32_t)(result < value1);

   return(result);
}

// Saturated uin32_t subtraction
uint32_t Lunar_MathLib_SubtractSaturateUint32(const uint32_t value1, const uint32_t value2)
{
   // First, subtract the values
   uint32_t result = value1 - value2;

   // If the result is the same or less than the first given value, then we have underflowed
   // To prevent branching, we AND the result with 0xFFFFFFFF (-1) to return the original result
   // Otherwise, we'll AND with 0 to return 0
   result &= -(uint32_t)(result <= value1);

   return(result);
}

