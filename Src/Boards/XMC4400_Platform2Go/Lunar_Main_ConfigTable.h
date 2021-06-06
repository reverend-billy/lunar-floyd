/*******************************************************************************
// Core Main Configuration
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
#include "Lunar_Main.h"
// Platform Includes
// Other Includes


/*******************************************************************************
// Constant Configuration Variables
*******************************************************************************/

// Configuration data for identifying the firmware and product
static const Lunar_Main_ConfigItem_t mainConfig = 
{
   // Version Info
   .version = {
      .major = 0,
      .minor = 2,
      .build = 0,
   },
   .productId = 4400U,
   .productName = {
      .stringValue = "XMC4400 Platform2Go "
   }
};


#ifdef __cplusplus
extern "C"
}
#endif

