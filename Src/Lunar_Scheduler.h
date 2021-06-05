/*******************************************************************************
// Core Scheduler
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
// Public Constants
*******************************************************************************/


/*******************************************************************************
// Public Types
*******************************************************************************/

// This is the type definition for all scheduled functions.
typedef void (*Lunar_Scheduler_Function_t)(void);

// This is the structure for each scheduler entry. Interval is
// the number of scheduler ticks (milliseconds) in which this
// function is to be called. ScheduledFunction is the address
// of the scheduled function.
// Each entry should have an Interval, and a pointer to the
// function to be called.
typedef struct
{
   // Seconds portion of the interval in which the specified function
   // is to be called
   uint16_t intervalSeconds;

   // Millisecond portion of the interval in which the specified function
   // is to be called
   uint16_t intervalMilliseconds;

   // This is the pointer to the function that is being scheduled.
   const Lunar_Scheduler_Function_t scheduledFunction;
} Lunar_Scheduler_Item_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the Scheduler module and starts a
  *    software timer for each configured module.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                               
*/
void Lunar_Scheduler_Init(void);


/** Description:
  *    Entry point for the scheduler module.  Note that function never returns.
 *     This function will loop through the configured schedule table and check
 *     the corresponding timer for expiration.  When a timer for a scheduled
 *     item is expired, the function pointer for that scheduled item is 
 *     executed.
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void Lunar_Scheduler_Execute(void);


#ifdef __cplusplus
extern "C"
}
#endif

