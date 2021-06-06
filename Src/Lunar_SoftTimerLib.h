/*******************************************************************************
// Software Timer Library
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
#include <stdbool.h> // Boolean type
#include <stdint.h> // Integer types


/*******************************************************************************
// Public Constants
*******************************************************************************/

// The maximum duration for a timer. We allow for 32 days to hold a month and
// this protects for overflow in time conversion routines
#define LUNAR_SOFTTIMERLIB_MAX_DURATION_DAYS (32)
// 1 second converted to milliseconds
#define LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_SECOND (1000U)
// 1 minute converted to milliseconds
#define LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_MINUTE (LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_SECOND * 60)
// 1 hour converted to milliseconds
#define LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_HOUR   (LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_MINUTE * 60)
// 1 day converted to milliseconds
#define LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_DAY    (LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_HOUR * 24)
// Maximum timer duration converted to milliseconds.  Max uint32_t of 0xFFFFFFFF is reserved
#define LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS (LUNAR_SOFTTIMERLIB_MAX_DURATION_DAYS * LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_DAY)
// Maximum timer duration converted to seconds.
#define LUNAR_SOFTTIMERLIB_MAX_DURATION_SECONDS (LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS/LUNAR_SOFTTIMERLIB_MILLISECONDS_PER_SECOND)
// Maximum timer duration converted to Timebase ticks
#define LUNAR_SOFTTIMERLIB_MAX_DURATION_TICKS (LUNAR_SOFTTIMERLIB_MAX_DURATION_MILLISECONDS * TIMEBASE_DRV_NUM_TICKS_PER_MILLISECOND)


/*******************************************************************************
// Public Types
*******************************************************************************/

// Structure that represents a timer object
typedef struct
{
   // The timestamp when the timer was last update
   uint32_t lastUpdateTimestamp;
   // The number of ticks remaining before the timer is expired
   uint32_t ticksRemaining;
} Lunar_SoftTimerLib_Timer_t;

/** Structure that defines a time duration consisting of hours,
  * minutes and second components. This is primarily used to
  * allow functions to pass a single pointer rather than
  * requiring each component to be passed. This is especially
  * beneficial so that a function may return a duration structure
  * pointer.                                                     
*/
typedef struct
{
   // Hours portion of the duration
   uint16_t hours;
   // Minutes portion of the duration
   uint16_t minutes;
   // Seconds portion of the duration
   uint16_t seconds;
   // Milliseconds portion of the duration
   uint16_t milliseconds;
   // The total number of milliseconds in hours, minutes, seconds
   // This is maintained to prevent recalculation
   uint32_t totalMilliseconds;
} Lunar_SoftTimerLib_TimeDuration_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function initialized the given timer structure.  The timer will be
  *    marked as stopped.
  * Parameters: 
  *    timer - A pointer to a common software timer structure.  
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void Lunar_SoftTimerLib_Init(Lunar_SoftTimerLib_Timer_t *timer);

/** Description:
  *    Start a countdown timer with the specified duration. Note that the value
  *    cannot be 0xFFFFFFFF since it is reserved for noting when a timer is
  *    stopped.  It is required that the timer be periodically checked for
  *    expiration by calling Lunar_SoftTimerLib_IsTimerExpired
  * Parameters: 
  *    timer - A pointer to the common software timer structure to be started.
  *    durationMilliseconds - The total number of milliseconds for timer.
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void Lunar_SoftTimerLib_StartTimer(Lunar_SoftTimerLib_Timer_t *timer, const uint32_t durationMilliseconds);

/** Description:
  *    Stops the given timer.  Note that a stopped timer can never be expired.
  * Parameters: 
  *    timer - A pointer to the common software timer structure to be started.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void Lunar_SoftTimerLib_StartTimerMeasurement(Lunar_SoftTimerLib_Timer_t *timer);

/** Description:
  *    Start a count up (measurement) timer. Use
  *    Lunar_SoftTimerLib_GetElapsedTimeMilliseconds to determine
  *    how much time has elapsed.
  * Parameters:
  *    timer :  A pointer to the common software timer structure to be
  *             stopped.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                                   
*/
void Lunar_SoftTimerLib_StopTimer(Lunar_SoftTimerLib_Timer_t *timer);

/** Description:
  *    Checks to see if a given timer is running. Note that expired timers are
  *    still consider to be running until Lunar_SoftTimerLib_StopTimer is
  *    called.
  * Parameters: 
  *    timer - A pointer to the common software timer structure to be checked.
  * Returns:
  *    bool - The current running status of the given timer
  * Return Value List: 
  *    true - The given timer is not stopped and is still running.
  *    false - The given timer is stopped.
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
bool Lunar_SoftTimerLib_IsTimerRunning(const Lunar_SoftTimerLib_Timer_t *timer);

/** Description:
  *    Checks to see if a given timer is expired. Note that the timer must be
  *    stopped or it will continue to report that it is expired. A stopped timer
  *    will never be expired.  Care must be taken to make sure the timer is
  *    checked for expiration faster than the time it takes for the Timebase to 
  *    wrap. A good rule of thumb is to check for expiration at least daily to 
  *    maintain the timer.  When this cannot be done, real-time clock or alarm 
  *    functionality may be a better fit. 
  * Parameters: 
  *    timer - A pointer to the common software timer structure to be checked.
  * Returns:
  *    bool - The expiration status of the given timer
  * Return Value List: 
  *    true - The given timer is not stopped and the duration has expiration. 
  *    false - The given timer is running and the duration has not expired. 
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
bool Lunar_SoftTimerLib_IsTimerExpired(Lunar_SoftTimerLib_Timer_t *timer);

/** Description:
  *    Returns the remaining time in milliseconds for a running timer.  Since a
  *    stopped timer does not have remaining time, 0 will be
  *    returned.
  * Parameters: 
  *    timer - A pointer to the common software timer structure to be queried.
  * Returns:
  *    uint32_t - The remaining timer duration in milliseconds.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
uint32_t Lunar_SoftTimerLib_GetRemainingTimeMilliseconds(Lunar_SoftTimerLib_Timer_t *const timer);

/** Description:
  *    \Returns the elapsed time for a running count up
  *    (measurement) timer
  * Parameters:
  *    timer :  A pointer to the common software timer structure to be
  *             queried.
  * Returns:
  *    uint32_t - The total number of milliseconds elapsed since the
  *    timer was started.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                                   
*/
uint32_t Lunar_SoftTimerLib_GetElapsedTimeMilliseconds(Lunar_SoftTimerLib_Timer_t *timer);

/**
  * Description:
  *    Convert a given hours, minutes and seconds to a total number of
  *    milliseconds.
  * Parameters:
  *     hoursToConvert - The hours portion of the duration to be converted.
  *     minutesToConvert - The minutes portion of the duration to be converted.
  *     secondsToConvert - The seconds portion of the duration to be converted.
  * Returns:
  *    uint32_t - The calculated total number of milliseconds for the given
  *    duration components.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
uint32_t Lunar_SoftTimerLib_ConvertTimeToMilliseconds(const uint16_t hoursToConvert, const uint16_t minutesToConvert, const uint16_t secondsToConvert);

/** Description:
  *    Convert a given number of milliseconds to a time duration
  *    structure.
  * Parameters:
  *    duration :               Pointer to a time duration structure
  *                             where the result of the conversion is
  *                             to be placed.
  *    millisecondsToConvert :  The given number of milliseconds to
  *                             be converted into duration
  *                             components.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                                  
*/
void Lunar_SoftTimerLib_ConvertMillisecondsToDuration(Lunar_SoftTimerLib_TimeDuration_t *duration, const uint32_t millisecondsToConvert);

/** Description:
  *    This will convert a given time in milliseconds to a time
  *    duration while rounding up to the nearest given interval.
  *    This is best used for displaying time -- especially when
  *    counting down. This prevents displaying zeroes until all
  *    milliseconds are expired. The max rounding interval is
  *    limited to 7 days
  *    (Lunar_SoftTimerLib_MAX_ROUNDING_MILLISECONDS) to prevent
  *    overflow. Use Lunar_SoftTimerLib_MILLISECONDS_PER_x (HOUR,
  *    MINUTE, DAY, SECOND) to pass as the rounding interval.
  * Example:
  *    1hr, 1min, 1sec with rounding interval of 1 minute (60000)
  *    would round to 1hr, 2mins, 0sec, 0ms 1hr, 1min, 1sec with
  *    rounding interval of 15 minutes (150000) would round to 1hr,
  *    15mins, 0sec, 0ms 1hr, 1min, 1sec with rounding interval of 1
  *    hour (3600000) would round to 2hrs, 0sec, 0ms
  * Parameters:
  *    duration :                    Pointer to a time duration
  *                                  structure where the result of
  *                                  the calculation is to be placed.
  *    millisecondsToConvert :       The number of milliseconds that
  *                                  is to be rounded and placed in
  *                                  the given duration object.
  *    roundToMillisecondInterval :  The interval to be used for the
  *                                  rounding calculation.
  * History:
  *    * 05/1/2021 : Function created (EJH)
  *                                                                  
*/
void Lunar_SoftTimerLib_ConvertMillisecondsToRoundedDuration(Lunar_SoftTimerLib_TimeDuration_t *duration, const uint32_t millisecondsToConvert, const uint32_t roundToMillisecondInterval);

#ifdef __cplusplus
extern "C"
}
#endif

