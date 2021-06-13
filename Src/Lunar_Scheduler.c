/*******************************************************************************
// Core Scheduler
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_Scheduler.h"
#include "Lunar_Scheduler_ConfigTable.h"
// Platofrm Includes
#include "Lunar_MathLib.h"
#include "Lunar_SoftTimerLib.h"
// Other Includes
#include <stdbool.h>


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

/** This is the number of scheduled functions define in the
  * Scheduler configuration table                          
*/
#define NUM_SCHEDULED_FUNCTIONS (sizeof(schedulerConfigTable)/sizeof(Lunar_Scheduler_ConfigItem_t))


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Enable state for the scheduler module
   bool enableState;

   // The following array allocates a timer object for each scheduled item.
   Lunar_SoftTimerLib_Timer_t schedulerItemTimer[NUM_SCHEDULED_FUNCTIONS];
} Lunar_Scheduler_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static Lunar_Scheduler_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    Function to start the timer for a scheduled function.
  * Parameters:
  *    functionIndex - Index into the Lunar_Scheduler_configTable
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
static void StartTimer(uint8_t functionIndex);


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Function to start the timer for the function at the given index
static void StartTimer(uint8_t timerIndex)
{
   // Verify the timer index is valid
   if (timerIndex < (uint8_t)NUM_SCHEDULED_FUNCTIONS)
   {
      // Calculate the desired interval in milliseconds and start the timer
      Lunar_SoftTimerLib_StartTimer(&status.schedulerItemTimer[timerIndex], Lunar_MathLib_AddSaturateUint32((uint32_t)schedulerConfigTable[timerIndex].intervalSeconds * 1000, (uint32_t)schedulerConfigTable[timerIndex].intervalMilliseconds));
   }
}


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize the scheduler module
void Lunar_Scheduler_Init(void)
{
   // Loop through each timer in the array and initialize
   for (uint8_t i = 0U; i < NUM_SCHEDULED_FUNCTIONS; i++)
   {
      Lunar_SoftTimerLib_Init(&status.schedulerItemTimer[i]);
   }
}


// Scheduler entry point
void Lunar_Scheduler_Execute(void)
{
   // Enable the scheduler
   status.enableState = true;

   // Loop through and start all of the timers
   for (uint8_t i = 0U; i < NUM_SCHEDULED_FUNCTIONS; i++)
   {
      StartTimer(i);
   }

   // Loop through the schedule table and call the functions when they have
   // expired
   while (status.enableState)
   {
      // Loop through and check each timer for expiration
      // If expired, call the function
      for (uint8_t i = 0U; i < NUM_SCHEDULED_FUNCTIONS; i++)
      {
         // Check the timer to see if it is expired
         if (Lunar_SoftTimerLib_IsTimerExpired(&status.schedulerItemTimer[i]))
         {
            // The timer is expired

            // Restart the timer -- the time is measured from the start of the
            // function to the start of the next time it is called
            StartTimer(i);

            // Finally, call the function
            schedulerConfigTable[i].scheduledFunction();
         }
      }
   }
}





