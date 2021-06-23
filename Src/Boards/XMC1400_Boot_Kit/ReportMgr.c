/*******************************************************************************
// Report Manager
*******************************************************************************/

/*******************************************************************************
// Includes
*******************************************************************************/

// Module Includes
#include "ReportMgr.h"
#include "ReportMgr_CAN.h"
// Platform Includes
#include "CAN_Drv.h"
#include "Platform.h"
#include "Lunar_SoftTimerLib.h"
// Other Includes
#include "Control.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h> // Used for memcpy


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/


/*******************************************************************************
// Private Type Declarations
*******************************************************************************/

// This structure defines the internal variables used by the module
typedef struct
{
	// Last update timestamp
	uint32_t lastUpdateTimestamp;	
} ReportMgr_Status_t;


/*******************************************************************************
// Private Variable Definitions
*******************************************************************************/

// The variable used for holding all internal data for this module.
static ReportMgr_Status_t status;


/*******************************************************************************
// Private Function Declarations
*******************************************************************************/


/*******************************************************************************
// Private Function Implementations
*******************************************************************************/


/*******************************************************************************
// Public Function Implementations
*******************************************************************************/

// Module initialization
void ReportMgr_Init(void)
{
	// Clear the last update timestamp
	status.lastUpdateTimestamp = 0;
}

// Periodic update function
void ReportMgr_Update(void)
{
   // TODO
}


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/

// BB_BatteryStatusRSP
void ReportMgr_MessageRouter_BB_BatteryStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_BatteryStatusRSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      // Cast the response buffer as the response type
      Response_t *response = (Response_t *)message->responseParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
     
      //      Bits=16.  [ 0     , 255    ]  Unit:''     
      response->BatteryBlockState = (uint8_t)Control_GetState();
      //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
      response->BatteryVoltageV = BB_BatteryStatusRSP_BatteryVoltageV_toS(3);
      //      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1   
      response->BatteryCurrentA = BB_BatteryStatusRSP_BatteryCurrentA_toS(100); 
      //      Bits=16.  [ 0     , 255    ]  Unit:''     
      response->BatterySOC = BB_BatteryStatusRSP_BatterySOC_toS(100);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

// BB_DCDCStatusRSP
void ReportMgr_MessageRouter_BB_DCDCStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_DCDCStatusRSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=16.  [ 0     , 655.35 ]  Unit:''      Factor= 0.01  
         response->DCBusVoltageV = BB_DCDCStatusRSP_DCBusVoltageV_toS(300);
         //  [-] Bits=16.  [ -163.84, 163.835 ]  Unit:''      Factor= 0.005        
         response->DCBusCurrentA = BB_DCDCStatusRSP_DCBusCurrentA_toS(100);                   

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}

// BB_CellVoltage1to4RSP
void ReportMgr_MessageRouter_BB_CellVoltage1to4RSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_CellVoltage1to4RSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell1V = BB_CellVoltage1to4RSP_Cell1V_toS(3.01);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell2V = BB_CellVoltage1to4RSP_Cell2V_toS(3.02);                        
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell3V = BB_CellVoltage1to4RSP_Cell3V_toS(3.03);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell4V = BB_CellVoltage1to4RSP_Cell4V_toS(3.04);

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}

// BB_CellVoltage5to8RSP
void ReportMgr_MessageRouter_BB_CellVoltage5to8RSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_CellVoltage5to8RSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell5V = BB_CellVoltage5to8RSP_Cell5V_toS(3.05);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell6V = BB_CellVoltage5to8RSP_Cell6V_toS(3.06);                        
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell7V = BB_CellVoltage5to8RSP_Cell7V_toS(3.07);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell8V = BB_CellVoltage5to8RSP_Cell8V_toS(3.08);

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}

// BB_CellVoltage9to12RSP
void ReportMgr_MessageRouter_BB_CellVoltage9to12RSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_CellVoltage9to12RSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell9V = BB_CellVoltage9to12RSP_Cell9V_toS(3.09);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell10V = BB_CellVoltage9to12RSP_Cell10V_toS(3.10);                        
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell11V = BB_CellVoltage9to12RSP_Cell11V_toS(3.11);
         //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001 
         response->Cell12V = BB_CellVoltage9to12RSP_Cell12V_toS(3.12);

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}

// BB_CellTempRSP
void ReportMgr_MessageRouter_BB_CellTempRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_CellTempRSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule1C = BB_CellTempRSP_Submodule1C_toS(21.5);
         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule2C = BB_CellTempRSP_Submodule2C_toS(22.5);
         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule3C = BB_CellTempRSP_Submodule3C_toS(23.5);
         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule4C = BB_CellTempRSP_Submodule4C_toS(24.5);
         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule5C = BB_CellTempRSP_Submodule5C_toS(25.5);
         //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5   
         response->Submodule6C = BB_CellTempRSP_Submodule6C_toS(26.5);

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}

// BB_DCDCTemperaturesRSP
void ReportMgr_MessageRouter_BB_DCDCTemperaturesRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_DCDCTemperaturesRSP_t Response_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
   {
      //-----------------------------------------------
      // Message Processing
      //-----------------------------------------------

      // Verify the length of the command parameters and make sure we have room for the response
      //	Note that the error response will be set, if necessary
      if (Lunar_MessageRouter_VerifyParameterSizes(message, 0, sizeof(Response_t)))
      {
         // Cast the response buffer as the response type
         Response_t *response = (Response_t *)message->responseParams.data;

         //-----------------------------------------------
         // Execute Command
         //-----------------------------------------------

         //      Bits=16.  [ -60   , 195    ]  Unit:''      Offset= -60      
         response->BBHeatsinkC = BB_DCDCTemperaturesRSP_BBHeatsinkC_toS(20);
         //      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1   
         response->FanDutyCycle = BB_DCDCTemperaturesRSP_FanDutyCycle_toS(90.5);

         // Set the response length
         Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
      }
   }
}


