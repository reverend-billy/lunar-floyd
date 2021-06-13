/*******************************************************************************
// GPIO Driver Configuration
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
#include "UART_Drv_Config.h" // Defines channel enumeration
// Platform Includes
#include "DevicePin_Config.h" // Port/Pin definition
// Other Includes
#include "xmc_gpio.h"
#include "xmc_scu.h"
#include "xmc_usic.h"


/*******************************************************************************
// Constant Configuration Variable Declarations
*******************************************************************************/

/** This table defines the configuration for each UART channel
  * used by the common UART driver. Each channel will be
  * initialized in the UART_Drv_Init function.                
*/
static const UART_Drv_ConfigItem_t uartConfigTable[UART_DRV_CHANNEL_Count] =
{
   // Primary Debug Channel (PC  Host)
   {
      .channel = XMC_UART1_CH0,
      .channelConfig = {
         .baudrate = 115200U,
         .data_bits = 8U,
         .frame_length = 8U,
         .stop_bits = 1U,
         .oversampling = 16U,
         .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
      },
      .rxPin = {
         .port = XMC_GPIO_PORT0,
         .pin = 4
      },
      .rxConfig = {
         .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
         .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
      },
      .txPin = {
         .port = XMC_GPIO_PORT0,
         .pin = 5
      },
      .txConfig = {
         .mode = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
         .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
      },
      .inputSourceDX0 = {
         .input = XMC_USIC_CH_INPUT_DX0,
         .source = USIC1_C0_DX0_P0_4
      },
      .inputSourceDX1 = {
         .source = INPUT_SOURCE_INVALID
      },
      .inputSourceDX2 = {
         .source = INPUT_SOURCE_INVALID
      },
      .inputSourceDX3 = {
         .source = INPUT_SOURCE_INVALID
      },
      .rxIrqNum = USIC1_0_IRQn, // 90 = RX
      .txIrqNum = USIC1_1_IRQn, //91 = TX
      .rxIRQServiceRequest = 0,
      .txIRQServiceRequest = 1,
      .rxIRQPriority = 64,
      .txIRQPriority = 64,
   },
};


#ifdef __cplusplus
extern "C"
}
#endif

