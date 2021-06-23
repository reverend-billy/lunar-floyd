/*******************************************************************************
// UART Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "UART_Drv.h" // Driver API
#include "UART_Drv_Config.h" // Channel enumeration
#include "UART_Drv_ConfigTable.h" // UART port configuration
// Platform Includes
#include "Lunar_CircularBufferLib.h"
// Other Includes
#include "xmc_scu.h"
#include "xmc_uart.h"
#include "xmc_usic.h"


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// Size of the local circular buffer used for transmitting data.
#define TX_BUFFER_SIZE (128)

// Size of the local circular buffer used for receiving data.
#define RX_BUFFER_SIZE (128)

// This defines the maximum length of command data in bytes.
#define COMMAND_DATA_MAX_SIZE (48)

// This defines the maximum length of response data in bytes
#define RESPONSE_DATA_MAX_SIZE (48)


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// Structure to hold the circular buffers for each port
typedef struct
{
   /*Defines all parameters for the RX circular buffer.
   */
   Lunar_CircularBufferLib_t rxCircularBuffer;

   // RX data buffer.
   uint8_t rxCircularBufferData[RX_BUFFER_SIZE];

   /*Defines all parameters for the TX circular buffer.
   */
   Lunar_CircularBufferLib_t txCircularBuffer;

   // TX data buffer.
   uint8_t txCircularBufferData[TX_BUFFER_SIZE];
} PortBuffers_t;

// This structure holds the private information for this module
typedef struct
{
   // Create one buffer object for each UART port used
   PortBuffers_t portBuffers[UART_DRV_CHANNEL_Count];
} UART_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// Status structure for this module
static UART_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    Function to retrieve the next byte from the circular buffer and place in
  *    the outgoing UART register.
  * Parameters: 
  *    channel :  The given channel corresponding to the this
  *               specific interrupt.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                   
*/
static void HandleChannelTXInterrupt(const UART_Drv_Channel_t channel);

/** Description:
  *    Function to store the received byte into the circular buffer
  *    for later processing and set up the IRQ for the next byte.
  * Parameters:
  *    channel :  The given channel corresponding to the this specific
  *               interrupt.
  * History:
  *    * 6/6/2021: Function created (EJH)
  *                                                                   
*/
static void HandleChannelRXInterrupt(const UART_Drv_Channel_t channel);


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Get data from outgoing buffer and place in TX register
static void HandleChannelTXInterrupt(const UART_Drv_Channel_t channel)
{

   // Verify the channel index
   if (channel < UART_DRV_CHANNEL_Count)
   {
      uint8_t tmpByte = 0;

      // Get next byte data from circular buffer, if any
      if (Lunar_CircularBufferLib_Dequeue(&(status.portBuffers[channel].txCircularBuffer), &tmpByte))
      {
         // Byte successfully dequeued from the TX buffer, send it
         XMC_USIC_CH_TXFIFO_PutData(uartConfigTable[channel].channel, tmpByte);
      }
   }
}

// Add data to buffer and set up next RX interrupt
static void HandleChannelRXInterrupt(const UART_Drv_Channel_t channel)
{
   // Verify the channel index
   if (channel < UART_DRV_CHANNEL_Count)
   {
      // Engueue the received byte -- place directly into the circular buffer
      Lunar_CircularBufferLib_Enqueue(&(status.portBuffers[channel].rxCircularBuffer), (uint8_t)XMC_USIC_CH_RXFIFO_GetData(uartConfigTable[channel].channel));

      // Receive another byte
      XMC_USIC_CH_RXFIFO_EnableEvent(uartConfigTable[channel].channel, (uint32_t)((uint32_t)XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | (uint32_t)XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE));
   }
}


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize all configured UART channels
void UART_Drv_Init(void)
{
   // Loop through the UART channels and init each one
   for (uint8_t channel = 0U; channel < UART_DRV_CHANNEL_Count; channel++)
   {
      // Store the channel configuration for easier access
      const UART_Drv_ConfigItem_t *uart = &uartConfigTable[channel];
      // Store the buffer object for easy access
      PortBuffers_t *portBuffer = &(status.portBuffers[channel]);

      // Initialize the Circular TX Buffer
      Lunar_CircularBufferLib_Init(&(portBuffer->txCircularBuffer), portBuffer->txCircularBufferData, sizeof(portBuffer->txCircularBufferData));
      // Initialize the Circular RX Buffer
      Lunar_CircularBufferLib_Init(&(portBuffer->rxCircularBuffer), portBuffer->rxCircularBufferData, sizeof(portBuffer->rxCircularBufferData));

      // Configure Receive pin
      XMC_GPIO_Init(uart->rxPin.port, uart->rxPin.pin, &(uart->rxConfig));
      // Initialize USIC channel in UART mode
      XMC_UART_CH_Init(uart->channel, &uart->channelConfig);

      // Set input source path
      // dx0 is UART RX: source must be set
      XMC_USIC_CH_SetInputSource(uart->channel, uart->inputSourceDX0.input, uart->inputSourceDX0.source);

      // Additional input multiplexing
      // Check if dx1 is used
      if (uart->inputSourceDX1.source != INPUT_SOURCE_INVALID)
      {
         XMC_USIC_CH_SetInputSource(uart->channel, uart->inputSourceDX1.input, uart->inputSourceDX1.source);
      }

      // Check if dx2 is used
      if (uart->inputSourceDX2.source != INPUT_SOURCE_INVALID)
      {
         XMC_USIC_CH_SetInputSource(uart->channel, uart->inputSourceDX2.input, uart->inputSourceDX2.source);
      }

      // Check if dx3 is used
      if (uart->inputSourceDX3.source != INPUT_SOURCE_INVALID)
      {
         XMC_USIC_CH_SetInputSource(uart->channel, uart->inputSourceDX3.input, uart->inputSourceDX3.source);
      }

      // Configure transmit FIFO
      XMC_USIC_CH_TXFIFO_Configure(uart->channel,
                                   16U,
                                   XMC_USIC_CH_FIFO_SIZE_16WORDS,
                                   1U);
      // Configure receive FIFO
      XMC_USIC_CH_RXFIFO_Configure(uart->channel,
                                   0U,
                                   XMC_USIC_CH_FIFO_SIZE_16WORDS,
                                   0U);

      // Start UART Peripheral
      XMC_UART_CH_Start(uart->channel);

      // Initialize UART TX pin
      XMC_GPIO_Init(uart->txPin.port, uart->txPin.pin, &(uart->txConfig));

      // Set service request for TX FIFO transmit interrupt
      XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(uart->channel, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, uart->txIRQServiceRequest);

      // Set service request for RX FIFO receive interrupt
      XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(uart->channel, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, uart->rxIRQServiceRequest);
      XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(uart->channel, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, uart->rxIRQServiceRequest);

      // Set priority and enable NVIC node for TX interrupt
#if UC_FAMILY == XMC1
      NVIC_SetPriority(uart->txIrqNum, uart->txIRQPriority);
#else
      NVIC_SetPriority(uart->txIrqNum, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), uart->txIRQPriority, 0U));
#endif

      // Set the TX interrupt control -- only for XMC1400
#if UC_SERIES == XMC14
      XMC_SCU_SetInterruptControl(uart->txIrqNum, (XMC_SCU_IRQCTRL_t)(uart->txInterruptControlSource));
#endif

      // Enable the TX IRQ
      NVIC_EnableIRQ(uart->txIrqNum);

      // Set priority and enable NVIC node for RX interrupt
#if UC_FAMILY == XMC1
      NVIC_SetPriority(uart->rxIrqNum, uart->rxIRQPriority);
#else
      NVIC_SetPriority(uart->rxIrqNum, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), uart->rxIRQPriority, 0U));
#endif

      // Set the RX interrupt control -- only for XMC1400
#if UC_SERIES == XMC14
      XMC_SCU_SetInterruptControl(uart->rxIrqNum, (XMC_SCU_IRQCTRL_t)(uart->rxInterruptControlSource));
#endif

      // Enable the RX IRQ
      NVIC_EnableIRQ(uart->rxIrqNum);

      // Enabld the RX Event
      XMC_USIC_CH_RXFIFO_EnableEvent(uart->channel, (uint32_t)((uint32_t)XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | (uint32_t)XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE));

      // Wait for the first byte
      XMC_USIC_CH_TXFIFO_EnableEvent(uart->channel, (uint32_t)XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
   }
}

// Write data to the given UART
void UART_Drv_Write(const UART_Drv_Channel_t channel, uint8_t *const data, const uint8_t length)
{
   // Store PWM as number to prevent debugger confusion
   uint8_t channelIndex = (uint8_t)channel;

   // Verify the given channel
   if (channelIndex < UART_DRV_CHANNEL_Count)
   {
      // Validate the given buffer
      if (data != 0)
      {
         // Store the channel configuration for easier access
         const UART_Drv_ConfigItem_t *uart = &uartConfigTable[channelIndex];

         // Loop through the given data and add to the circular buffer
         for (uint8_t i = 0U; i < length; i++)
         {
            // Put given byte in TX buffer
            Lunar_CircularBufferLib_Enqueue(&(status.portBuffers[channelIndex].txCircularBuffer), *(data + i));
         }

         // See if there is data send and we are idle
         if ((status.portBuffers[channelIndex].txCircularBuffer.size > 0) && (XMC_USIC_CH_TXFIFO_IsEmpty(uart->channel)))
         {
            // Just trigger the TX Callback so all sending goes through the same mechanism
            // First enable the TX event
            XMC_USIC_CH_TXFIFO_EnableEvent(uart->channel, (uint32_t)XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
            // Then trigger the transmit buffer interrupt
            XMC_USIC_CH_TriggerServiceRequest(uart->channel, (uint32_t)0x01U);

				// Variable where new byte is stored
            uint8_t tmpByte = 0;
				
            // Trigger the first write
            if (Lunar_CircularBufferLib_Dequeue(&(status.portBuffers[channelIndex].txCircularBuffer), &tmpByte))
            {

               // Byte successfully dequeued from the TX buffer, send it
               // XMC_UART_CH_Transmit(UART_configInfo.channel, tmpByte);
               // TODO Check is full?
               XMC_USIC_CH_TXFIFO_PutData(uart->channel, tmpByte);
            }
         }
      }
   }
}

// Read a byte from the RX circular buffer
bool UART_Drv_ReadByte(const UART_Drv_Channel_t channel, uint8_t *byteRead)
{
   // Verify the given channel and buffer are valid
   if ((channel < UART_DRV_CHANNEL_Count) && (byteRead != 0))
   {
      // Get the next byte from the buffer, if available
      return(Lunar_CircularBufferLib_Dequeue(&(status.portBuffers[channel].rxCircularBuffer), byteRead));
   }
   else
   {
      // Buffer was not valid -- return no data
      return(false);
   }
}


/*******************************************************************************
// Interrupt Handlers
*******************************************************************************/

// TX Callback called when the Tx buffer is empty
// This is mapped above to the XMC IRQ handler name in the config file
void UART0_CONFIG_TX_HANDLER(void)
{
   // Not an easy way to map handlers -- pass the channel index from the config file
   // This is done manually to avoid the overhead of matching this IRQ with
   // the corresponding driver configuration.
   HandleChannelTXInterrupt(UART0_CONFIG_TX_CHANNEL_INDEX);
}

// This callback is called by the IRQHandler when the given number of bytes are received
// This is mapped above to the XMC IRQ handler name in the config file
void UART0_CONFIG_RX_HANDLER(void)
{
   // Not an easy way to map handlers --  pass the channel index from the config file
   // This is done manually to avoid the overhead of matching this IRQ with
   // the corresponding driver configuration.
   HandleChannelRXInterrupt(UART0_CONFIG_TX_CHANNEL_INDEX);
}

// For devices that do not have 2 UART channels implemented, 
// leave UART1_CONFIG_TX_CHANNEL_INDEX undefined
#ifdef UART1_CONFIG_TX_CHANNEL_INDEX

// TX Callback called when the Tx buffer is empty
// This is mapped above to the XMC IRQ handler name in the config file
void UART1_CONFIG_TX_HANDLER(void)
{
   // Not an easy way to map handlers --  pass the channel index from the config file
   // This is done manually to avoid the overhead of matching this IRQ with
   // the corresponding driver configuration.
   HandleChannelTXInterrupt(UART1_CONFIG_TX_CHANNEL_INDEX);
}

// This callback is called by the HAL_UART_IRQHandler when the given number of bytes are received
void UART1_CONFIG_RX_HANDLER(void)
{
   // Not an easy way to map handlers --  pass the channel index from the config file
   // This is done manually to avoid the overhead of matching this IRQ with
   // the corresponding driver configuration.
   HandleChannelRXInterrupt(UART1_CONFIG_TX_CHANNEL_INDEX);
}
#endif

