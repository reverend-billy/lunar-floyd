/*******************************************************************************
// CAN Driver Configuration
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
#include "CAN_Drv_Config.h" // Defines channel enumeration
// Platform Includes
#include "DevicePin_Config.h" // Port/Pin definition
// Other Includes
#include "xmc_can.h"
#include "xmc_gpio.h"
#include "xmc_scu.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

// This table defines the configuration for each UART channelused by the
// common UART driver.  Each cahnnel will be initialized in the
// UART_Drv_Init function.
static const CAN_Drv_ConfigItem_t canConfigTable[CAN_DRV_CHANNEL_Count] =
{
   // Primary CAN Channel (Board-to-Board)
   {
      .channel = CAN_NODE1,
      .channelIndex = 1,
      .channelConfig = {
        .can_frequency = (uint32_t)1.44E8, 
        .baudrate      = (uint32_t)(500 * 1000),   
        .sample_point  = (uint16_t)(80 * 100),
        .sjw           = (uint16_t)1
      },
      .rxPin = {
         .port = XMC_GPIO_PORT4,
         .pin = 8
      },
      .rxConfig = {
         .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
         .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
      },
      .txPin = {
         .port = XMC_GPIO_PORT4,
         .pin = 9
      },
      .txConfig = {
         .mode = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
      },
      .input = XMC_CAN_NODE_RECEIVE_INPUT_RXDCC,
   },
};


#ifdef __cplusplus
extern "C"
}
#endif

