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
// Platform Includes
#include "DevicePin_Config.h"
// Other Includes
#include "ReportMgr_CAN.h"
#include <stdint.h> // Interget types
#include "xmc_can.h"
#include "xmc_gpio.h"
#include "xmc_scu.h"


/*******************************************************************************
// Public Constant Definitions
*******************************************************************************/

// Configure CAN message ID target->host.
#define CAN_TX_MSG_ID          (INV_StateRSP_CANID | 0x80000000)
// Configure number of bytes in the target->host CAN message.
#define CAN_TX_MAX_DATA        (INV_StateRSP_DLC)

// Configure CAN message ID host->target.
#define CAN_RX_MSG_ID          (BB_BatteryStatusRSP_CANID | 0x80000000)
// Configure number of bytes in the host->target CAN message.
#define CAN_RX_MAX_DATA        (BB_BatteryStatusRSP_DLC)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Defines the enumeration used to specify the desired UART in driver functions.
typedef enum
{
   // CAN Primary Channel
   CAN_DRV_CHANNEL_PRIMARY,
   /** The total number of CAN channels configured Note that this
     * must match the uartConfigTable.                            
   */
   CAN_DRV_CHANNEL_Count
} CAN_Drv_Channel_t;


// Defines the structure used for each entry it the UART Driver Config table
typedef struct
{
   /*XMCLib CAN register configuration definition
   */
   CAN_NODE_TypeDef *channel;
   /*Index that corresponds to the channel (Ex. Node0=0, Node1=1)
   */
   uint8_t channelIndex;
   /*XMCLib UART initialization structure Defines baud rate, etc.
   */
   XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t channelConfig;
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
   // Defines the possible receive inputs. 
   XMC_CAN_NODE_RECEIVE_INPUT_t input;
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
} CAN_Drv_ConfigItem_t;


#ifdef __cplusplus
extern "C"
}
#endif

