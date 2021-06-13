/*******************************************************************************
// Real-time Clock Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
#include "RTC_Drv.h"
#include "RTC_Drv_ConfigTable.h"
// Platform Includes
#include "Lunar_MessageRouter.h"
#include "Platform.h"
// Other Includes
#include "Reset_Drv.h"
#include "xmc_rtc.h"
#include "xmc_scu.h"
#include <stdint.h>
#include "Timebase_Drv.h"

/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// The number of seconds between Unix Epoch 1/1/1970 and Y2K Epoch 1/1/2000
#define EPOCH_TIME_OFFSET_S 946684800
// The number of seconds between XMC Std Time 1/1/1900 and Unix Epoch 1/1/1970
#define STD_TIME_UNIX_TIME_OFFSET_S 2208988800
// RTC year offset is 1900 (Ex. 100 = Year 2000)
#define XMC_RTC_YEAR_OFFSET (1900U)


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
   // Used to track the module enable state
   bool enableState;
} RTC_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static RTC_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    This function returns the prescaler used by the XMC RTC peripheral.
  * Returns: 
  *    uint32_t - 32-bit prescaler value used directly by the XMC RTC peripheral
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
static uint32_t GetPrescaler(void) PLATFORM_UNUSED;

/** Description:
  *    This function set the prescaler used by the XMC RTC peripheral.
  * Parameters: 
  *   prescaler - Prescaler value used directly by the XMC RTC peripheral
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
static void SetPrescaler(const uint16_t prescaler) PLATFORM_UNUSED;

/** Description:
  *    Returns the Epoch used by the RTC (1/1/1900) in the format used by
  *    the common time.h library.
  * Returns: 
 *     time_t - Epoch time in Unix seconds (Seconds past 1/1/1970)
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
static time_t GetEpoch(void) PLATFORM_UNUSED;

/** Description:
  *    Returns the 1/1/2000 Epoch in the format used by
  *    the common time.h library.
  * Returns: 
 *     time_t - 1/1/2000 epoch time in Unix seconds (Seconds past 1/1/1970)
  * History: 
  *    * 6/6/2021: Function created (EJH)
  *                                                              
*/
static time_t GetY2KEpoch(void) PLATFORM_UNUSED;


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Change the RTC prescaler
static void SetPrescaler(const uint16_t prescaler)
{

   // Stop the RTC before changing the prescaler
   RTC_Drv_SetEnableState(false);

   // Set the new prescaler value
   XMC_RTC_SetPrescaler(prescaler);

   // Restart the RTC
   RTC_Drv_SetEnableState(true);
}


// Query the RTC prescaler
static uint32_t GetPrescaler(void)
{
   // Just read the prescaler from the RTC module
   return(XMC_RTC_GetPrescaler());
}


// Get Epoch used by RTC (1/1/1900) in the common time.h library format
// This is the device-specific implementation of the time.h function
// Date/time in unix secs past 1-Jan-70
static time_t GetEpoch(void)
{
   // Time structure defined in <time.h>
   struct tm tm;

   // Epoch is 1/1/1900
   tm.tm_isdst = -1;
   tm.tm_yday = 0;
   tm.tm_wday = 0;
   tm.tm_year = XMC_RTC_YEAR_OFFSET;
   tm.tm_mon = 0;
   tm.tm_mday = 0;
   tm.tm_hour = 0;
   tm.tm_min = 0;
   tm.tm_sec = 0;

   // Call <time.h> to convert to time_t value
   return(mktime(&tm));
}


// Get Epoch for 1/1/2000
static uint32_t GetY2KEpoch(void)
{
   return(GetEpoch() + EPOCH_TIME_OFFSET_S);
}


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/


// This is the device-specific implementation of the time.h function
// Date/time in unix secs past 1-Jan-70
time_t time(time_t *timer)
{
   // Standard <time.h> structure
   struct tm tm;

   // Fetch the time from XMC library
   // The XMC_RTC_GetTimeStdFormat function gets the time values from TIM0, TIM1 registers.
   // See the structure ::XMC_RTC_TIME_t for the valid range of time value parameters. <br>
   // For days the valid range is (1 - Actual days of month), year (since 1900) and
   // daysinyear (0 -365).
   XMC_RTC_GetTimeStdFormat(&tm);

   // <time.h> structure
   // date/time in unix secs past 1-Jan-70
   time_t unixTime = mktime(&tm);

   // See if a given time object was given
   if (timer != 0)
   {
      // Store the time in the given object
      *timer = unixTime;

      // Return the data at the given location
      return(*timer);
   }
   else
   {
      // Just return the time value
      return(unixTime);
   }
}


// Initialize and start the RTC peripheral
void RTC_Drv_Init(void)
{
   // Init the RTC to disabled
   status.enableState = false;

#if (UC_FAMILY == XMC4)
   XMC_SCU_HIB_EnableHibernateDomain();
   XMC_SCU_HIB_SetRtcClockSource(XMC_SCU_HIB_RTCCLKSRC_OSI);
#endif

   // If the RTC is in clock mode and the reset was not due to Power On Reset or Voltage Brown Out, preserve the clock time
   uint32_t resetReason = Reset_Drv_GetResetReason();

#if (UC_FAMILY == XMC4)
   if (resetReason & (XMC_SCU_RESET_REASON_PORST | XMC_SCU_RESET_REASON_PV))
#elif (UC_FAMILY == XMC1)
   if (resetReason & (XMC_SCU_RESET_REASON_PORST | XMC_SCU_RESET_REASON_MASTER))
#endif	
   {
      // Make sure the clock is stopped in order to set it
      XMC_RTC_Stop();

      // Set default time 01-01-2000 00:00
      XMC_RTC_Init(&rtcConfigTable);
   }
   else
   {
      // Make sure the RTC is enabled
      XMC_RTC_Enable();
   }

   // XMC_RTC_Enable is called by the RTC Init, so we just call Start
   RTC_Drv_SetEnableState(true);
}


// See if the RTC peripheral is active
bool RTC_Drv_GetEnableState(void)
{
   // Just return the status of the RTC module
   return(status.enableState);
}


// Start the RTC peripheral
void RTC_Drv_SetEnableState(const bool newEnableState)
{

   // See if we are changing the enable state
   if (status.enableState != newEnableState)
   {
      // State is changing, see if we are starting
      if (newEnableState)
      {
         // Start the RTC
         XMC_RTC_Start();
      }
      else
      {
         // Stop the RTC
         XMC_RTC_Stop();
      }

      // Store the new state
      status.enableState = newEnableState;
   }

}


// Get the current time in common RTC Drvier format
void RTC_Drv_GetCurrentTime(RTC_Drv_DateTime_t *const dateTime)
{
   // Get the RTC Time
   time_t secondsTime;

   // Get time from <time.h> function
   time(&secondsTime);

   // gmtime does not appear to return the correct value, so
   // call local time to convert into components
   struct tm timeStruct;

   localtime_r(&secondsTime, &timeStruct);

   // Populate the response data
   dateTime->time.seconds = timeStruct.tm_sec;
   dateTime->time.minutes = timeStruct.tm_min;
   dateTime->time.hours = timeStruct.tm_hour;
   dateTime->time.timeFormat = RTC_DRV_TIME_FORMAT_24_HOUR;
   // Days are 1-based (e.g. 1st of month = 1)
   dateTime->date.day = timeStruct.tm_mday;
   // Year is offset from 1900
   // Return to user as full year
   dateTime->date.year = timeStruct.tm_year + XMC_RTC_YEAR_OFFSET;
   // Month is 0-based (Ex: Jan = 0)
   // Send to user as 1-based
   dateTime->date.month = (RTC_Drv_Month_t)(timeStruct.tm_mon + 1);
   dateTime->date.dayOfWeek = (RTC_Drv_DayOfWeek_t)(timeStruct.tm_wday);
}


// Set the time using common RTC Driver format
void RTC_Drv_SetCurrentTime(const RTC_Drv_DateTime_t *const dateTime)
{
   // Set the RTC Time
   XMC_RTC_TIME_t time;

   // Populate the response data
   time.seconds = dateTime->time.seconds;
   time.minutes = dateTime->time.minutes;
   time.hours = dateTime->time.hours;
   // Adjust time if 12 hour/PM
   if (dateTime->time.timeFormat == RTC_DRV_TIME_FORMAT_PM)
   {
      // Add 12 hours when time is sent as 12HR and PM is selected
      time.hours += 12U;
   }

   // Days are sent as 1-based (e.g. 1st of month = 1), but stored as 0-based
   time.days = dateTime->date.day - 1;
   // Set year as a whole number, not offset
   time.year = dateTime->date.year;
   // Month is sent as 1-based (e.g. Jan = 1), but stored as 0-based
   time.month = dateTime->date.month - 1;
   // Day of the week isn't need for XMC
   time.daysofweek = dateTime->date.dayOfWeek;

   // Call XMC Lib to set the time and date
   XMC_RTC_SetTime(&time);
}


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/


// Handler for GetCurrentDateTime message
void RTC_Drv_MessageRouter_GetCurrentDateTime(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------

   // This structure defines the format of the response data
   PLATFORM_PACK_START(1)
   typedef struct
   {
      // YYYY,MM,DD,DoW, HH,MM,SS
      // Date is packed version of RTC_Drv_Date_t
      uint16_t year;
      RTC_Drv_Month_t month;
      uint8_t day;
      RTC_Drv_DayOfWeek_t dayOfWeek;
      // Time is packed version of RTC_Drv_Time_t
      uint8_t hours;
      uint8_t minutes;
      uint8_t seconds;
      RTC_Drv_TimeFormat_t timeFormat;
   } PLATFORM_ATTRIBUTE_PACKED Response_t;
   PLATFORM_PACK_END()


   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      // Get the Time from RTC Driver function
      RTC_Drv_DateTime_t dateTime;

      RTC_Drv_GetCurrentTime(&dateTime);

      // Place item in response
      // Date
      response->year = dateTime.date.year;
      response->month = dateTime.date.month;
      response->day = dateTime.date.day;
      response->dayOfWeek = dateTime.date.dayOfWeek;
      // Time
      response->hours = dateTime.time.hours;
      response->minutes = dateTime.time.minutes;
      response->seconds = dateTime.time.seconds;
      response->timeFormat = dateTime.time.timeFormat;

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}


// Handler for SetCurrentDateTime message
void RTC_Drv_MessageRouter_SetCurrentDateTime(Lunar_MessageRouter_Message_t *const message)
{
   //-----------------------------------------------
   // Command/Response Params
   //-----------------------------------------------

   // This structure defines the format of the response data
   PLATFORM_PACK_START(1)
   typedef struct
   {
      // YYYY,MM,DD,DoW, HH,MM,SS
      // Date is packed version of RTC_Drv_Date_t
      uint16_t year;
      RTC_Drv_Month_t month;
      uint8_t day;
      RTC_Drv_DayOfWeek_t dayOfWeek;
      // Time is packed version of RTC_Drv_Time_t
      uint8_t hours;
      uint8_t minutes;
      uint8_t seconds;
      RTC_Drv_TimeFormat_t timeFormat;
   } PLATFORM_ATTRIBUTE_PACKED Command_t;
   PLATFORM_PACK_END()


   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), 0))
   {
      // Cast the command buffer as the command type
      Command_t *command = (Command_t *)message->commandParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------

      RTC_Drv_DateTime_t dateTime;

      // Date
      dateTime.date.year = command->year;
      dateTime.date.month = command->month;
      dateTime.date.day = command->day;
      dateTime.date.dayOfWeek = command->dayOfWeek;
      // Time
      dateTime.time.hours = command->hours;
      dateTime.time.minutes = command->minutes;
      dateTime.time.seconds = command->seconds;
      dateTime.time.timeFormat = command->timeFormat;

      // Just call the driver set function
      RTC_Drv_SetCurrentTime(&dateTime);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }
}


