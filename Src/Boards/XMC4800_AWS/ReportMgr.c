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
#include "Lunar_ErrorMgr.h"
#include "Lunar_SoftTimerLib.h"
// Other Includes
#include "Control.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h> // Used for memcpy


/*******************************************************************************
// Private Constant Definitions
*******************************************************************************/

// This value represent the fault state returned by the BB_BatteryStatusRSP
// This is currently used to test cacading faults over CAN but is expected to 
// replaced as the control is implemented.
#define BATTERY_FAULT_VALUE (99U)


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
   //static uint8_t data[] = {1,2,3}; //,4,5,6,7,8};
   //CAN_Drv_TransmitPacket(&data[0], sizeof(data));
		
}


/*******************************************************************************
// Message Router Function Implementations
*******************************************************************************/


//INV_L2NStatusRSP_CANID
void ReportMgr_MessageRouter_INV_L2NStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_L2NStatusRSP_t Response_t;

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
      response->L1NVoltageVrms = INV_L2NStatusRSP_L1NVoltageVrms_toS(300.01);
      //      Bits=16.  [ 0     , 655.35 ]  Unit:''      Factor= 0.01
      response->L2NVoltageVrms = INV_L2NStatusRSP_L2NVoltageVrms_toS(300.02);
      //      Bits=16.  [ 0     , 327.675 ]  Unit:''      Factor= 0.005
      response->L1NCurrentArms = INV_L2NStatusRSP_L1NCurrentArms_toS(100.005);
      //      Bits=16.  [ 0     , 327.675 ]  Unit:''      Factor= 0.005
      response->L2NCurrentArms = INV_L2NStatusRSP_L2NCurrentArms_toS(100.010);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}


//INV_L2LStatusRSP_CANID
void ReportMgr_MessageRouter_INV_L2LStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_L2LStatusRSP_t Response_t;

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
      response->L1L2VoltageVrms = INV_L2LStatusRSP_L1L2VoltageVrms_toS(300.01);
      //      Bits=16.  [ 0     , 327.675 ]  Unit:''      Factor= 0.005
      response->L1L2CurrentArms = INV_L2LStatusRSP_L1L2CurrentArms_toS(100.005);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }
}

//INV_DCBusStatusRSP_CANID
void ReportMgr_MessageRouter_INV_DCBusStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_DCBusStatusRSP_t Response_t;

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
		response->PosBusVoltage = INV_DCBusStatusRSP_PosBusVoltage_toS(300.01);
		//      Bits=16.  [ 0     , 655.35 ]  Unit:''      Factor= 0.01
		response->NegBusVoltage = INV_DCBusStatusRSP_NegBusVoltage_toS(300.02);
		//      Bits=16.  [ 0     , 327.675 ]  Unit:''      Factor= 0.005
		response->PosBusCurrent = INV_DCBusStatusRSP_PosBusCurrent_toS(100.05);
		//      Bits=16.  [ 0     , 327.675 ]  Unit:''      Factor= 0.005
		response->NegBusCurrent = INV_DCBusStatusRSP_NegBusCurrent_toS(100.10);

      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }	
}

//INV_TemperaturesRSP_CANID
void ReportMgr_MessageRouter_INV_TemperaturesRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_TemperaturesRSP_t Response_t;

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

		//      Bits=08.  [ -60   , 195    ]  Unit:''      Offset= -60
		response->InvHeatsinkC = INV_TemperaturesRSP_InvHeatsinkC_toS(100);
		//      Bits=08.  [ -60   , 195    ]  Unit:''      Offset= -60
		response->InvPowerStage1C = INV_TemperaturesRSP_InvPowerStage1C_toS(101);
		//      Bits=08.  [ -60   , 195    ]  Unit:''      Offset= -60
		response->InvPowerStage2C = INV_TemperaturesRSP_InvPowerStage2C_toS(102);
		//     Bits=08.  [ -60   , 195    ]  Unit:''      Offset= -60
		response->InvPowerStage3C = INV_TemperaturesRSP_InvPowerStage3C_toS(103);
		//      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1
		response->InvFanDutyCycle1 = INV_TemperaturesRSP_InvFanDutyCycle1_toS(90.1);
		//      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1
		response->InvFanDutyCycle2 = INV_TemperaturesRSP_InvFanDutyCycle1_toS(90.2);
		
      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }	
}

//INV_StateRSP_CANID
void ReportMgr_MessageRouter_INV_StateRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef INV_StateRSP_t Response_t;

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

		//      Bits=08.  [ 0     , 255    ]  Unit:''
		response->InvState = (uint8_t)Control_GetState();;
		//      Bits=08.  [ 0     , 255    ]  Unit:''
		response->InvACRelay = 255U;
		//      Bits=08.  [ 0     , 255    ]  Unit:''
		response->INV_L1L2InPhase = 255U;
		
      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, sizeof(Response_t));
   }		
}

//BB_BatteryStatusRSP
void ReportMgr_MessageRouter_BB_BatteryStatusRSP(Lunar_MessageRouter_Message_t *const message)
{
   // Command/Response Params defined in ReportMgr_CAN.h file
   typedef BB_BatteryStatusRSP_t Command_t;

   //-----------------------------------------------
   // Message Processing
   //-----------------------------------------------

   // Verify the length of the command parameters and make sure we have room for the response
   //	Note that the error response will be set, if necessary
   if (Lunar_MessageRouter_VerifyParameterSizes(message, sizeof(Command_t), 0))
   {
      // Cast the response buffer as the response type
      Command_t *command = (Command_t *)message->commandParams.data;

      //-----------------------------------------------
      // Execute Command
      //-----------------------------------------------
		
		// Just set the error state if the battery is in fault mode
		Lunar_ErrorMgr_SetErrorState(LUNAR_ERRORMGR_ERROR_BATTERY_FAULT, command->BatteryBlockState == BATTERY_FAULT_VALUE);
		
      // Set the response length
      Lunar_MessageRouter_SetResponseSize(message, 0);
   }		
}



