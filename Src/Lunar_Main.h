/*******************************************************************************
// Core Main
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

/** The allowed length for the <link Lunar_Main_ProductName_t, product name>
  * string in the <link mainConfigTable, Main configuration table>.              
*/
#define LUNAR_MAIN_PRODUCT_NAME_LENGTH (20U)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

/** This structure defines the firmware version used internally
  * for the entire build. It is intended that semantic versioning
  * used to define compatibility with other firmware versions.
  * See https://semver.org/                                      
*/
typedef struct
{
   // Major: The major version number should be incremented for each change that
   // breaks compatibility. Ex: The "1" in "Version 1.2.3456"
   uint8_t major;
   // Minor: The minor version number should be incremented for feature changes but
   // does not break compatibility with previous versions of the same Major release.
   // Ex: The "2" in "Version 1.2.3456"
   uint8_t minor;
   // Build: The build is incremented for each new release. It does not affect compatibility.
   // Ex: The "3456" in "Version 1.2.3456"
   uint16_t build;
} Lunar_Main_Version_t;

// A unique 32-bit identifier used to differentiate the product.
// This is intended to allow for easy numerical comparison by a host device.
typedef uint32_t Lunar_Main_ProductId_t;

/** Firmware Version ID String
  * * --------------------------------------------- *
  * This string may be used to provide additional details about
  * the firmware that the version number can't provide. This is
  * intended to provide an easier string identification method
  * for users. This is placed in a structure to so that
  * sizeof(struct.name) returns a proper array size rather than
  * pointer size.
  * Note:
  *    Make sure that the data buffer used for the Message Router
  *    response is large enough to hold the string you choose.   
*/
typedef struct
{
   /** This string may be used to provide additional details about
     * the firmware that the version number can't provide. This is
     * intended to provide an easier string identification method
     * for users. This is placed in a structure to so that
     * Lunar_Main_ProductId_t can be passed by reference and the
     * user can use sizeof(productName-\>stringValue) to return the
     * proper array size rather than pointer size.
     * Note:
     *    Make sure that the data buffer used for the Message Router
     *    response is large enough to hold the string you choose.   
   */
   uint8_t stringValue[LUNAR_MAIN_PRODUCT_NAME_LENGTH];
} Lunar_Main_ProductName_t;

// Structure to define group the product identification information to make
// the config file easier to read.
typedef struct
{
   // Version Info
   Lunar_Main_Version_t version;
   // Product Id (Numeric)
   Lunar_Main_ProductId_t productId;
   // Release Target
   uint8_t releaseTarget;
   // Product Name (String)
   Lunar_Main_ProductName_t productName;
} Lunar_Main_ConfigItem_t;


/*******************************************************************************
// Public Function Declarations
*******************************************************************************/

/** Description:
  *    This function defines the module initialization. Note that an explicit
  *    call to this function is optional if Lunar_Main_Execute is called.
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Main_Init(void);

/** Description:
  *    This function will initialize and start the system.  Typically this is
  *    the only function required to start the core firmware. 
  * History: 
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Main_Execute(void);


/*******************************************************************************
// Message Router Function Declarations
*******************************************************************************/

/** Description:
  *    This function retrieves the current firmware version
  *    information for the module.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                               
*/
void Lunar_Main_MessageRouter_GetApplicationVersion(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function retrieves the 32-bit product identifier
  *    for the module. This is primarily used to provide a mechanism 
  *    for an attached device to verify the connected device without
  *    string comparisons.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                
*/
void Lunar_Main_MessageRouter_GetProductID(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function retrieves the firmware identification string
  *    for the module. This is primarily used to provide an easily
  *    readable description of the firmware to possibly
  *    differentiate between multiple devices connected to the same
  *    host.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                
*/
void Lunar_Main_MessageRouter_GetProductName(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function retrieves the reason for the previous reset.
  *    This is typically hardware dependent based on the
  *    implementation of the Reset driver.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                               
*/
void Lunar_Main_MessageRouter_GetResetReason(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function will perform as reset for the firmware.  This is typically
  *    hardware dependent based on the implementation of the Reset driver.
  * Parameters: 
  *    message - A pointer to a common Message Router message object. Note
  *    that no response is expected from this function since the reset may occur
  *    immediately.
  *  History:
  *    * 5/1/2021: Function created (EJH)
  *                                                              
*/
void Lunar_Main_MessageRouter_Reset(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

/** Description:
  *    This function retrieves the reason the current uptime for the
  *    system. This will be reset at each reboot and is primarily
  *    used as an easy way to verify that watchdog or other resets
  *    have not occurred unexpectedly during testing. Note that the
  *    timer will eventually wrap and is not designed for measuring
  *    very long durations.
  * Parameters:
  *    message :  A pointer to a common Message Router message
  *               object. The response is expected to be placed in
  *               this object.
  * History:
  *    * 5/1/2021: Function created (EJH)
  *                                                                 
*/
void Lunar_Main_MessageRouter_GetUptimeMillseconds(Lunar_MessageRouter_Message_t *const message) PLATFORM_NON_NULL;

#ifdef __cplusplus
extern "C"
}
#endif
