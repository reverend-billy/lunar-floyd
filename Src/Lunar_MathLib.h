/*******************************************************************************
// Core Math Timer Library
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
// Other Includes
#include <stdint.h>


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
  *    Calculates the sum of two values without wrapping. If the sum
  *    is greater than UINT32_MAX, the result will be saturated to
  *    UINT32_MAX (0xFFFFFFFF)
  * Parameters:
  *    value1 :  The first addend for the addition operation.
  *    value2 :  The second addend for the addition operation.
  * Returns:
  *    uint32_t - The calculated sum (value1 + value2)
  * Return Value List:
  *    UINT32_MAX :     The sum is saturated if it cannot be
  *                     contained in a 32\-bit unsigned integer.
  *    \< UINT32_MAX :  The calculated sum if it can be contained in
  *                     a 32\-bit unsigned integer.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                 
*/
uint32_t Lunar_MathLib_AddSaturateUint32(uint32_t const value1, uint32_t const value2);

/** Description:
  *    Calculates the difference between two 32-bit unsinged values
  *    without wrapping. If value2 \>= value1, the result will be 0.
  * Parameters:
  *    value1 :  The minuend for the subtraction operation.
  *    value2 :  The subtrahend for the subtraction operation.
  * Returns:
  *    uint32_t - The calculated difference (value1 - value2)
  * Return Value List:
  *    0 :     \Returns 0 if value2 \>= value1 to prevent wrapping
  *    \> 0 :  \Returns non\-zero value if value2 \< value1
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                 
*/
uint32_t Lunar_MathLib_SubtractSaturateUint32(const uint32_t value1, const uint32_t value2);


#ifdef __cplusplus
extern "C"
}
#endif



