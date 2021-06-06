/*******************************************************************************
// Reset Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "Reset_Drv.h"
// Platform Includes
// Other Includes
#include "stm32l1xx_hal.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/


/*******************************************************************************
// Private Types
*******************************************************************************/

// Enumeration to define the reset resasons given by the STM32 HAL
typedef enum 
{
   // Low-Power reset
   RESET_REASON_LPWRRST,                 
   // Window watchdog reset
   RESET_REASON_WWDGRST,                 
   // Independent Watchdog reset
   RESET_REASON_IWDGRST,                 
   // Software Rese       
   RESET_REASON_SFTRST,                  
   // POR/PDR reset            
   RESET_REASON_PORRST,                  
   // PIN reset             
   RESET_REASON_PINRST,                  
   // Options bytes loading reset
   RESET_REASON_OBLRST,
   // Number of reset reasons
   RESET_REASON_Count
} Reset_Reason_t;

// Structure used to pair the STM32 reset flags with the defined reset reasons
typedef struct 
{
   // The reset flag identifying a reset reason
   uint8_t halRccFlag;
   // The list of reset reasons specific to the STM32 HAL
   Reset_Reason_t reason;
} Reset_Item_t;

// This structure defines the internal variables used by the module
typedef struct
{
   // Store the cause of the last reset
   uint32_t deviceResetReason;
} Reset_Status_t;


/*******************************************************************************
// Private Variables
*******************************************************************************/

// Table to pair reset flags with the reason reason enumeration
static const Reset_Item_t resetReasons[RESET_REASON_Count] = 
{
   {RCC_FLAG_LPWRRST,  RESET_REASON_LPWRRST },  
   {RCC_FLAG_WWDGRST,  RESET_REASON_WWDGRST },  
   {RCC_FLAG_IWDGRST,  RESET_REASON_IWDGRST },  
   {RCC_FLAG_SFTRST,   RESET_REASON_SFTRST  },  
   {RCC_FLAG_PORRST,   RESET_REASON_PORRST  },
   {RCC_FLAG_PINRST,   RESET_REASON_PINRST  },  
   {RCC_FLAG_OBLRST,   RESET_REASON_OBLRST  }   
};

// The variable used for holding all internal data for this module.
static Reset_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initializes the module
void Reset_Drv_Init(void)
{
   // Used for building bit mask
   uint32_t bitMask = 1;

   // Test the reset flags in order because the pin reset is always set
   status.deviceResetReason = 0;

   // Get the cause of reset
   for (int i = 0; i < RESET_REASON_Count; i++)
   {		
      if (__HAL_RCC_GET_FLAG(resetReasons[i].halRccFlag))
      {
         // OR the bit at postition i
         status.deviceResetReason |= bitMask;
      }

      // Shift the mask for next comparison
      bitMask <<= 1U;
   }

   // Clear reset reason
   __HAL_RCC_CLEAR_RESET_FLAGS(); 
}


// Returns a value representing the reason for the previous reset
uint32_t Reset_Drv_GetResetReason(void)
{
   // Just return the reset reason
   return(status.deviceResetReason);
}


// Reset the device immediately
void Reset_Drv_Execute(void)
{
   // STM32 resets via NVIC call
   HAL_NVIC_SystemReset();
}


