/*******************************************************************************
// Software Timer Library
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Lunar_SoftTimerLib.h"
// Platform Includes
#include "Lunar_MathLib.h"
// Other Includes
#include "Timebase_Drv.h"
#include <stdint.h> // Integer values


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// The timer duration that represents a stopped timer
#define TIMER_STOPPED_VALUE (UINT32_MAX)


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/


/*******************************************************************************
// Private Variable Definitions
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

// Initialize a software timer object
void Lunar_SoftTimerLib_Init(Lunar_SoftTimerLib_Timer_t *const timer)
{
   // Validate the given parameter
   if (timer != 0)
   {
      // Set the duration to the stopped value
      timer->ticksRemaining = TIMER_STOPPED_VALUE;

      // Set the last update time to 0
      timer->lastUpdateTimestamp = 0;
   }
}


// Start a countdown software timer
void Lunar_SoftTimerLib_StartTimer(Lunar_SoftTimerLib_Timer_t *const timer, const uint32_t durationMilliseconds)
{
   // Validate the given parameter
   if (timer != 0)
   {
      // Make sure that the given milliseconds in in bounds
      if (durationMilliseconds <= LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS)
      {
         // We're updating the remaining ticks, store the current timestamp
         timer->lastUpdateTimestamp = Timebase_Drv_GetCurrentTickCount();

         // The remaining ticks is Milliseconds * Ticks Per Second.
         // Max value: Max milliseconds (86400000) * Max Ticks/ms (49) = 4,233,600,000.
         timer->ticksRemaining = durationMilliseconds * TIMEBASE_DRV_NUM_TICKS_PER_MILLISECOND;
      }
   }
}


// Start a countup software timer
void Lunar_SoftTimerLib_StartTimerMeasurement(Lunar_SoftTimerLib_Timer_t *const timer)
{
   // A measurement timer is started by just starting a timer for the max duration
   // We can then determine how much time has passed from the remaining time
   Lunar_SoftTimerLib_StartTimer(timer, LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS);
}


// Stop a timer
void Lunar_SoftTimerLib_StopTimer(Lunar_SoftTimerLib_Timer_t *const timer)
{
   // Validate the given parameter
   if (timer != 0)
   {
      // Set the duration to the stopped value
      timer->ticksRemaining = TIMER_STOPPED_VALUE;
   }
}


// See if the timer is running
bool Lunar_SoftTimerLib_IsTimerRunning(const Lunar_SoftTimerLib_Timer_t *const timer)
{
   // Timer is running if timer object is valid and tick count is not stopped value (0xFFFFFFFF)
   return((timer != 0) && (timer->ticksRemaining != TIMER_STOPPED_VALUE));
}


// Update the timer and see if it has expired
// Note a stopped will never be expired
bool Lunar_SoftTimerLib_IsTimerExpired(Lunar_SoftTimerLib_Timer_t *const timer)
{
   // Assume expired
   bool isExpired = false;

   // Validate the given parameter
   if (timer != 0)
   {
      // The basic is rule is stopped timers cannot be expired.
      // So first, we validate the timer is running.
      if (Lunar_SoftTimerLib_IsTimerRunning(timer))
      {
         // Timer is running, get the current time in ticks.
         // We store it locally so all calculations are done on the same value
         // since it can be changed by the interrupt at any time.
         uint32_t currentTickCount = Timebase_Drv_GetCurrentTickCount();

         // We need to calculate the elapsed time since we were last called.
         // See if any ticks have elapsed
         if (currentTickCount != timer->lastUpdateTimestamp)
         {
            uint32_t elapsedTicks = 0U;

            // First we have to check to see if the tick count has wrapped
            if (currentTickCount >= timer->lastUpdateTimestamp)
            {
               // Systick has not wrapped, just subtract
               elapsedTicks = (currentTickCount - timer->lastUpdateTimestamp);
            }
            else
            {
               // The systick has wrapped, determine how much time was left before wrap and
               // add to current time
               elapsedTicks = (TIMEBASE_DRV_MAX_TICK_VALUE - timer->lastUpdateTimestamp) + currentTickCount;
            }

            // Now we have the number of elapsed ticks, we can store the current time as the last update
            timer->lastUpdateTimestamp = currentTickCount;

            // Subtract the elapsed ticks from the remaining
            timer->ticksRemaining = Lunar_MathLib_SubtractSaturateUint32(timer->ticksRemaining, elapsedTicks);

            // Determine if we are expired (Remaining == 0)
            isExpired = (timer->ticksRemaining == 0);
         }
         // else no time has expired, not expired, nothing to update
      }
   }

   // Finally, return the result
   return(isExpired);
}


// Get the remaining time in milliseconds for a countdown timer
uint32_t Lunar_SoftTimerLib_GetRemainingTimeMilliseconds(const Lunar_SoftTimerLib_Timer_t *const timer)
{
   uint32_t remainingMilliseconds = 0U;

   // Validate the given parameter
   if (timer != 0)
   {
      // The basic is rule is stopped timers cannot be expired.
      // So first, we validate the timer is running.
      if (Lunar_SoftTimerLib_IsTimerRunning(timer))
      {
         // Timer is running, convert the remaining time to milliseconds
         remainingMilliseconds = timer->ticksRemaining / TIMEBASE_DRV_NUM_TICKS_PER_MILLISECOND;
      }
      else
      {
         // Not running, so remaining time is 0
         remainingMilliseconds = 0;
      }
   }

   // Finally, return the result
   return(remainingMilliseconds);
}


// Get the elapsed time for a countup timer
uint32_t Lunar_SoftTimerLib_GetElapsedTimeMilliseconds(Lunar_SoftTimerLib_Timer_t *const timer)
{
   // Default to no time elapsed
   uint32_t elapsedMillisconds = 0U;

   // Check if the timer is running. A stopped timer will not have any elapsed time.
   if (Lunar_SoftTimerLib_IsTimerRunning(timer))
   {
      // Call IsTimerExpired to update the remaining tick count
      Lunar_SoftTimerLib_IsTimerExpired(timer);

      // Since timer was started with the max duration, the elapsed time is just the difference between max and remaining
      // We must subtract ticks before division to prevent rounding error since we only want to return the number of fully elapsed milliseconds
      elapsedMillisconds = ((uint32_t)LUNAR_SOFTTIMERLIB_MAX_DURATION_TICKS - timer->ticksRemaining) / (uint32_t)TIMEBASE_DRV_NUM_TICKS_PER_MILLISECOND;
   }

   // Return the elapsed time -- 0 if timer was not running
   return(elapsedMillisconds);
}


// Calaculatetotal milliseconds from a given number of hours, minutes and seconds
uint32_t Lunar_SoftTimerLib_ConvertTimeToMilliseconds(const uint16_t hoursToConvert, const uint16_t minutesToConvert, const uint16_t secondsToConvert)
{
   // Init return value to 0
   uint32_t timerMilliseconds = 0U;

   // Calculate the number of total minutes
   // Use saturated add to prevent wrapping
   uint32_t timerMinutes = Lunar_MathLib_AddSaturateUint32((hoursToConvert * 60U), minutesToConvert);
   // Calculate the total number of seconds
   uint32_t timerSeconds = Lunar_MathLib_AddSaturateUint32((timerMinutes * 60U), secondsToConvert);

   // Convert the total minutes to milliseconds
   // Just make sure that it isn't over the maximum software timer limit to prevent overflow
   if (timerSeconds < LUNAR_SOFTTIMERLIB_MAX_DURATION_SECONDS)
   {
      // Will not overflow, continue
      // Convert seconds to milliseconds
      timerMilliseconds = timerSeconds * 1000U;
   }
   else
   {
      // Will overflow, just return the max limit for the software timer
      timerMilliseconds = (uint32_t)LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS;
   }

   // Return the total milliseconds
   return(timerMilliseconds);
}


// Convert milliseconds to into duration components
void Lunar_SoftTimerLib_ConvertMillisecondsToDuration(Lunar_SoftTimerLib_TimeDuration_t *const duration, const uint32_t millisecondsToConvert)
{
   // Validate the parameters
   if (duration != 0)
   {
      // Store total number of milliseconds
      duration->totalMilliseconds = millisecondsToConvert;
      // 3600000 milliseconds in an hour
      duration->hours = (uint16_t)(millisecondsToConvert / LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_HOUR);
      // 60000 milliseconds in a minute
      duration->minutes = (uint16_t)((millisecondsToConvert % LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_HOUR) / LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_MINUTE);
      // 1000 milliseconds in a second
      duration->seconds = (uint16_t)(millisecondsToConvert % LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_MINUTE) / LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_SECOND;
      // 1000 milliseconds in a second
      duration->milliseconds = (uint16_t)(millisecondsToConvert % LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_SECOND);
   }
}


// Round a given time to the specified interval
void Lunar_SoftTimerLib_ConvertMillisecondsToRoundedDuration(Lunar_SoftTimerLib_TimeDuration_t *const duration, const uint32_t millisecondsToConvert, const uint32_t roundToMillisecondInterval)
{
   // Validate the parameters
   if (duration != 0)
   {
      // Store the rounding interval for local use
      uint32_t tmpRoundInterval = roundToMillisecondInterval;

      // Make sure we are not trying to round to more than one week to prevent overflow
      if (roundToMillisecondInterval > LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS)
      {
         // Clip to the max allowed value
         tmpRoundInterval = LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS;
      }

      // Round up to the nearest interval by adding 1 less than the rounding interval to the given interval
      // Using milliseconds as ticks and a max of 32 days keeps from overflowing a uint32
      uint32_t tmpMilliseconds = (millisecondsToConvert + tmpRoundInterval - 1) / tmpRoundInterval;

      // Multiply by rounding interval again to get the total rounded milliseconds
      tmpMilliseconds = tmpMilliseconds * tmpRoundInterval;

      // Now convert this number to a duration
      Lunar_SoftTimerLib_ConvertMillisecondsToDuration(duration, tmpMilliseconds);
   }
}

