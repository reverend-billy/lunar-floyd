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
// Constant Configuration Variable Declarations
*******************************************************************************/

// Configuration data for identifying the firmware and product
static const Lunar_Main_ConfigItem_t mainConfigTable = 
{
   // Version Info
   .version = {
      .major = 0,
      .minor = 3,
      .build = 0,
   },
   .productId = 4800U,
	// 1 = Release Configuration, 0 = Debug Configuration
#ifdef RELEASE_TARGET	
	.releaseTarget = 1,
#else
	.releaseTarget = 0,
#endif	
   .productName = {
      .stringValue = "XMC4800 AWS         "
   }
};


#ifdef __cplusplus
extern "C"
}
#endif

