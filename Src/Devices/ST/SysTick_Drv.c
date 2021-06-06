/*******************************************************************************
// SysTick Driver
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "SysTick_Drv.h"
#include "SysTick_Drv_Config.h"
// Platform Includes
#include "Timebase_Drv_Config.h"
// Other Includes
#include <stdint.h>
#include "stm32l1xx_hal.h"


/*******************************************************************************
// Private Constants
*******************************************************************************/


/*******************************************************************************
// Private Types
*******************************************************************************/


/*******************************************************************************
// Private Variables
*******************************************************************************/

// 32-bit system tick counter used for software timers
// In the current implementation, this value is read by the Timebase module
// to prevent the overhead of calling a function.
volatile uint32_t SysTick_Drv_sysTickCount = 0U;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/** Description:
  *    This function initializes the RCC Oscillators before configuring the SysTick
  * History: 
  *    * 05/1/2021 : Function created (EJH)
  *                                                              
*/
void SystemClock_Config(void);

/*******************************************************************************
// Private Function Implementations
*******************************************************************************/

// Initialize clock source
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
   RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

   // Configure the main internal regulator output voltage
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
   // Initializes the RCC Oscillators according to the specified parameters
   // in the RCC_OscInitTypeDef structure.
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
   RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   // Initializes the CPU, AHB and APB buses clocks
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Initialize module
void SysTick_Drv_Init(void)
{
	// System Clock Configuration
	SystemClock_Config();
	
   // Set the SysTick Period to timebase setting of 1ms
   HAL_SetTickFreq(TIMEBASE_DRV_NUM_TICKS_PER_SECOND);

   // Initialize the global systick variable
   SysTick_Drv_sysTickCount = 0U;
}


// Get the current SysTick value
uint32_t SysTick_Drv_GetCurrentTickCount(void)
{
   // Simply return the current SysTick value
   return(SysTick_Drv_sysTickCount);
}


/*******************************************************************************
// Interrupt Handler
*******************************************************************************/

// SysTick Interrupt
void SYSTICK_DRV_IRQ_HANDLER(void)
{
   // Simply increment the SysTick counter by 1
   SysTick_Drv_sysTickCount++;
}
