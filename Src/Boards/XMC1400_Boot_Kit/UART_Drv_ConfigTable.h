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

// This table defines the configuration for each UART channelused by the
// common UART driver.  Each cahnnel will be initialized in the
// UART_Drv_Init function.
static const UART_Drv_ConfigItem_t uartConfigTable[UART_DRV_CHANNEL_Count] =
{
   // Primary Debug Channel (PC  Host)
   {
      .channel = XMC_UART0_CH1,
      .channelConfig = {
         .baudrate = 115200U,
         .data_bits = 8U,
         .frame_length = 8U,
         .stop_bits = 1U,
         .oversampling = 16U,
         .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
      },
      .rxPin = {
         .port = XMC_GPIO_PORT1,
         .pin = 3
      },
      .rxConfig = {
         .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
         .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
      },
      .txPin = {
         .port = XMC_GPIO_PORT1,
         .pin = 2
      },
      .txConfig = {
         .mode = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
      },
      .inputSourceDX0 = {
         .input = XMC_USIC_CH_INPUT_DX0,
         .source = USIC0_C1_DX0_P1_3
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
      .rxIrqNum = IRQ9_IRQn,
      .txIrqNum = IRQ10_IRQn,
      .rxIRQServiceRequest = 0,
      .txIRQServiceRequest = 1,
      .rxIRQPriority = 3,
      .txIRQPriority = 3,
      .txInterruptControlSource = XMC_SCU_IRQCTRL_USIC0_SR1_IRQ10,
      .rxInterruptControlSource = XMC_SCU_IRQCTRL_USIC0_SR0_IRQ9
   },
   // Secondary UART Channel
   {
      .channel = XMC_UART1_CH0,
      .channelConfig = {
         .baudrate = 57600U,
         .data_bits = 8U,
         .frame_length = 8U,
         .stop_bits = 1U,
         .oversampling = 16U,
         .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
      },
      .rxPin = {
         //Was P0.2
         .port = XMC_GPIO_PORT3,
         .pin = 3
      },
      .rxConfig = {
         .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
         .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
      },
      .txPin = {
         // Was P0.3
         .port = XMC_GPIO_PORT3,
         .pin = 4
      },
      .txConfig = {
         //.mode = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
         .mode = (XMC_GPIO_MODE_t)XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
         .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
      },
      .inputSourceDX0 = {
         .input = XMC_USIC_CH_INPUT_DX0,
         //.source = USIC1_C0_DX0_P0_2
         .source = USIC1_C0_DX0_P3_3
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
      .rxIrqNum = IRQ11_IRQn,
      .txIrqNum = IRQ12_IRQn,
      .rxIRQServiceRequest = 2,
      .txIRQServiceRequest = 3,
      .rxIRQPriority = 3,
      .txIRQPriority = 3,
      .txInterruptControlSource = XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12,
      .rxInterruptControlSource = XMC_SCU_IRQCTRL_USIC1_SR2_IRQ11

   },
};


#ifdef __cplusplus
extern "C"
}
#endif

