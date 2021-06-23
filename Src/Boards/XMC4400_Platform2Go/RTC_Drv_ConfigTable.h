/*******************************************************************************
// Real-time Clock Driver Configuration
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
#include "xmc_rtc.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

// The configuration values specific to the device-specific RTC Driver implementation
// Since this is hardware specific the XMC time values are used to eliminate
// unncessary conversion
XMC_RTC_CONFIG_t rtcConfigTable = {
    // The default value for the prescalar with the 32.768kHz crystal (or the internal clock) is 0x7FFF for a time interval of 1 sec
	.prescaler = 0x7FFFU,
	.time = {
         // Initialize to the Y2K Epoch (1/1/200)
			.seconds = 0U,
			.minutes = 0U,
			.hours = 0U,
         .days = 0U, // Days are 0-based (e.g. 1st of month = 0)
			.year = 2000U,
			.month = XMC_RTC_MONTH_JANUARY,
         // Jan 1, 2000 was a Saturday
			.daysofweek = XMC_RTC_WEEKDAY_SATURDAY
		}
};


#ifdef __cplusplus
extern "C"
}
#endif

