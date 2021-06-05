/*******************************************************************************
// main.c
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "main.h"
// Platform Includes
#include "Lunar_Main.h"
// Other Includes
#include <stdio.h>
#include <stdlib.h>


/*******************************************************************************
// Private Constants
*******************************************************************************/


/*******************************************************************************
// Private Types
*******************************************************************************/


/*******************************************************************************
// Private Variables
*******************************************************************************/

// Variable used in the Hard Fault handler to allow a convenient place for
// placing a breakpoint
static volatile unsigned int _Continue = 0U;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/

/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// C entry point
int main(void)
{
   // The Core Main moudle privide a single entry point for starting up the system
   // It will initialize all configured modules and start the system scheduler
   // *** NOTE THIS WILL NOT RETURN ***
   Lunar_Main_Execute();

   // Standard C exit code
   return(EXIT_SUCCESS);
}


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Interrupt Handlers
*******************************************************************************/

// IRQ handler for Hard Fault
void HardFault_Handler(void)
{
   while (_Continue == 0U)
   {
      // Loop forever
   }
}
