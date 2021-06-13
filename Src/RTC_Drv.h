/*******************************************************************************
// Real-time Clock Driver
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
#include "Lunar_MessageRouter.h"
#include "Platform.h"
// Other Includes
#include <stdint.h>


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

/** Enumeration defining months used by the RTC_Drv_DateTime_t
  * structure Note these values are 1-based. The driver
  * implementation is expected to convert this to the required
  * \internal representation.                                 
*/
typedef enum
{
   // Month value is undefined
   RTC_DRV_MONTH_UNKNOWN,
   // Jan = 1
   RTC_DRV_MONTH_JANUARY,
   // Feb = 2
   RTC_DRV_MONTH_FEBRUARY,
   // Mar = 3
   RTC_DRV_MONTH_MARCH,
   // Apr - 4
   RTC_DRV_MONTH_APRIL,
   // May  - 5
   RTC_DRV_MONTH_MAY,
   // Jun - 6
   RTC_DRV_MONTH_JUNE,
   // July - 7
   RTC_DRV_MONTH_JULY,
   /*Aug - 8
   */
   RTC_DRV_MONTH_AUGUST,
   // Sept - 10
   RTC_DRV_MONTH_SEPTEMBER,
   // Oct - 11
   RTC_DRV_MONTH_OCTOBER,
   // Nov - 11
   RTC_DRV_MONTH_NOVEMBER,
   // Dec - 12
   RTC_DRV_MONTH_DECEMBER,
   // Defines the number of enumerated month values
   RTC_DRV_Count
} RTC_Drv_Month_t;

/** Enumeration that defines the day of the week in the format
  * used by the RTC_Drv_DateTime_t structure. Note these values
  * are 0-based with Sunday as state of the week. The driver
  * implementation is expected to convert this to the required
  * \internal representation.                                  
*/
typedef enum
{
   // Sun - 0
   RTC_DRV_DAY_SUNDAY,
   // Mon - 1
   RTC_DRV_DAY_MONDAY,
   // Tues - 2
   RTC_DRV_DAY_TUESDAY,
   // Wed - 3
   RTC_DRV_DAY_WEDNESDAY,
   // Thurs - 4
   RTC_DRV_DAY_THURSDAY,
   // Fri - 5
   RTC_DRV_DAY_FRIDAY,
   // Sat - 6
   RTC_DRV_DAY_SATURDAY,
   // Defines the number of enumerated day of the week values
   RTC_DRV_DAY_Count
} RTC_Drv_DayOfWeek_t;

/** Enumeration that defines the 12/24 time format used by the
  * RTC_Drv_DateTime_t structure. 12-hour format is specified
  * with either the AM or PM enumerated value. The driver
  * implementation is expected to convert this to the required
  * \internal representation.                                 
*/
typedef enum
{
   // Denoted that a 24-hour time is represented
   RTC_DRV_TIME_FORMAT_24_HOUR,
   // Denotes that a 12-hour AM time is represented
   RTC_DRV_TIME_FORMAT_AM,
   // Denotes that a 12-hour PM time is represented
   RTC_DRV_TIME_FORMAT_PM,
   // Defines the total number of time format enumerated values
   RTC_DRV_TIME_FORMAT_Count
} RTC_Drv_TimeFormat_t;

// The common date format used by the RTC Driver
// It is expected that all implementations convert from this format to the
// correct internal representation.
typedef struct
{
   // The 4-digit numeric year format (Ex. 2000)
   uint16_t year;
   // The enumerated value representing the month
   RTC_Drv_Month_t month;
   // The 1-based day of the month
   uint8_t day;
   // The enumerated value representing the day of the week
   RTC_Drv_DayOfWeek_t dayOfWeek;
} RTC_Drv_Date_t;

// The common time format used by the RTC Driver
// It is expected that all implementations convert from this format to the
// correct internal representation.
typedef struct
{
   /** 12/24 hour value according to the setting of timeFormat. Note
     * this value is 0-based.                                       
   */
   uint8_t hours;
   // Minutes value for the time
   // Note this value is 0-based
   uint8_t minutes;
   // Seconds value for the time
   // Note this value is 0-based
   uint8_t seconds;
   /** Enumerated value the specifies the hour representation and
     * the AM/PM value for 12-hour times                         
   */
   RTC_Drv_TimeFormat_t timeFormat;
} RTC_Drv_Time_t;

// This structure combines date and time into a single common object used
// by the RTC Driver
typedef struct
{
   // Common RTC driver date
   RTC_Drv_Date_t date;
   // Common RTC driver time
   RTC_Drv_Time_t time;
} RTC_Drv_DateTime_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function defines the module initialization.  The real-time clock
  *    will be initialized and started.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void RTC_Drv_Init(void);

/** Description:
  *    This function gets the current enable state of the real-time clock.
  * Returns: 
  *    bool - The current enable state
  * Return Value List: 
  *    true - The real-time clock is currently active
  *    false - The real-time clock is currently not active
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
bool RTC_Drv_GetEnableState(void);

/** Description:
  *    This function sets the current enable state of the real-time clock.  Note
  *    that typically the clock is running upon initialization.
  * Parameters: 
  *    newEnableState - The desired enable state for the real-time clock.
  *    true=Set active, false=Set inactive
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void RTC_Drv_SetEnableState(const bool newEnableState);

/** Description:
  *    This function gets the current RTC time in the common format used by
  *    the RTC diver.
  * Parameters: 
  *     dateTime - Common RTC Date/Time structure where the current time is to
  *     be placed.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void RTC_Drv_GetCurrentTime(RTC_Drv_DateTime_t *const dateTime) PLATFORM_NON_NULL;

/** Description:
  *    This function sets the current RTC time using the common format used by
  *    the RTC diver.
  * Parameters: 
  *     dateTime - Common RTC Date/Time structure containing the current time is
  *     to be set.
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
void RTC_Drv_SetCurrentTime(const RTC_Drv_DateTime_t *const dateTime)PLATFORM_NON_NULL;

/** Description:
  *    This function retrieves the current date and time using the formated
  *    defined by the RTC_Drv_DateTime_t structure.
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
void RTC_Drv_MessageRouter_GetCurrentDateTime(Lunar_MessageRouter_Message_t *const message)PLATFORM_NON_NULL;

/** Description:
  *    This function set the current date and time using the formated defined by
  *    the RTC_Drv_DateTime_t structure.
  * Parameters: 
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                
*/
void RTC_Drv_MessageRouter_SetCurrentDateTime(Lunar_MessageRouter_Message_t *const message)PLATFORM_NON_NULL;


#ifdef __cplusplus
extern "C"
}
#endif

