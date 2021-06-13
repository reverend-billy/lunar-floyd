/*******************************************************************************
// UART Driver Configuration
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
#include "DevicePin_Config.h"
// Other Includes
#include <stdint.h> // Interget types
#include "xmc_gpio.h"
#include "xmc_scu.h"
#include "xmc_uart.h"
#include "xmc_usic.h"


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/

// The UART Channel Index for the IRQ9/10 handlers
// Specifying here allows the handler IRQs to be linked with the channel index in one spot

// Primary UART is used for built-in communication on the development board
#define UART0_CONFIG_TX_CHANNEL_INDEX  UART_DRV_CHANNEL_DEBUG
// XMC_UART0_CH1 TX uses IRQ10
#define UART0_CONFIG_TX_HANDLER        IRQ_Hdlr_91
// XMC_UART0_CH1 RX uses IRQ9
#define UART0_CONFIG_RX_HANDLER        IRQ_Hdlr_90

/** Used to denote an unused input source in the configuration
  * table                                                     
*/
#define INPUT_SOURCE_INVALID (UINT8_MAX)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Defines the enumeration used to specify the desired UART in driver functions.
typedef enum
{
   // UART Primary Channel - Debug
   UART_DRV_CHANNEL_DEBUG,
   /** The total number of UART channels configured Note that this
     * must match the uartConfigTable.                            
   */
   UART_DRV_CHANNEL_Count
} UART_Drv_Channel_t;

// Defines the structure that defines Input MUX for XMCLib UART driver
typedef struct
{
   /*XMCLib Mux input definition (Ex. XMC_USIC_CH_INPUT_DX0)
   */
   XMC_USIC_CH_INPUT_t input;
   /*XMCLib Mux source definition (Ex. USIC0_C1_DX0_P1_3)
   */
   uint8_t source;
} UART_Drv_ChannelInputSource_t;

// Defines the structure used for each entry it the UART Driver Config table
typedef struct
{
   /*XMCLib USIC register configuration definition
   */
   XMC_USIC_CH_t *channel;
   /*XMCLib UART initialization structure Defines baud rate, etc.
   */
   XMC_UART_CH_CONFIG_t channelConfig;
   /*RX Port/Pin definition
   */
   DevicePin_Config_t rxPin;
   /** XMCLib GPIO Configuration structure for RX pin GPIO Driver is
     * not used since there are so many differences between UART and
     * GPIO                                                         
   */
   XMC_GPIO_CONFIG_t rxConfig;
   /*TX Port/Pin definition
   */
   DevicePin_Config_t txPin;
   /** XMCLib GPIO Configuration structure for TX pin GPIO Driver is
     * not used since there are so many differences between UART and
     * GPIO                                                         
   */
   XMC_GPIO_CONFIG_t txConfig;
   // Input mux configuration for DX0
   UART_Drv_ChannelInputSource_t inputSourceDX0;
   // Input mux configuration for DX1
   // Use to activate additional Input MUXs to select input pin (if not used, set to: XMC_INPUT_INVALID)
   UART_Drv_ChannelInputSource_t inputSourceDX1;
   // Input mux configuration for DX2
   // Use to activate additional Input MUXs to select input pin (if not used, set to: XMC_INPUT_INVALID)
   UART_Drv_ChannelInputSource_t inputSourceDX2;
   // Input mux configuration for DX3
   // Use to activate additional Input MUXs to select input pin (if not used, set to: XMC_INPUT_INVALID)
   UART_Drv_ChannelInputSource_t inputSourceDX3;
   /*CMSIS-compatible IRQ definition for RX
   */
   IRQn_Type rxIrqNum;
   /*CMSIS-compatible IRQ definition for TX
   */
   IRQn_Type txIrqNum;
   /** XMC Service Request Number for RX -- Required for IRQ routing
     * \Note this must align with the rxInterruptControlSource
     * definition                                                   
   */
   uint32_t rxIRQServiceRequest;
   /** XMC Service Request Number for TX -- Required for IRQ routing
     * \Note this must align with the rxInterruptControlSource
     * definition                                                   
   */
   uint32_t txIRQServiceRequest;
   // RX IRQ priority used for this channel
   uint32_t rxIRQPriority;
   // TX IRQ priority used for this channel
   uint32_t txIRQPriority;
   // XMC1400 requires mapping of XMC interrupt sources to Cortex-M0 sources
#if UC_SERIES == XMC14
   // XMC interrupt source used for TX of this channel (Ex. XMC_SCU_IRQCTRL_USIC0_SR1_IRQ10)
   XMC_SCU_IRQCTRL_t txInterruptControlSource;
   // XMC interrupt source used for RX of this channel (Ex. XMC_SCU_IRQCTRL_USIC0_SR1_IRQ9)
   XMC_SCU_IRQCTRL_t rxInterruptControlSource;
#endif
} UART_Drv_ConfigItem_t;


#ifdef __cplusplus
extern "C"
}
#endif

