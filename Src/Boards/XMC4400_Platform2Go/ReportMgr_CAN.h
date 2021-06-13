/*******************************************************************************
// Report Manager CAN Commands (Autogenerated)
*******************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

// NOTE: The macros and structures below are autogenerated

// BB_CommandCMD Message Length
#define BB_CommandCMD_DLC (2U)
// BB_CommandCMD CAN Message (2817)
#define BB_CommandCMD_CANID (0xB01U)
// BB_CommandCMD CAN Message (2817)
typedef struct
{
   //      Bits=08.  [ 0     , 255    ]  Unit:''
   uint8_t BB_StateReq;
   //      Bits=08.  [ 0     , 255    ]  Unit:''
   uint8_t BB_ResetReq;
} BB_CommandCMD_t;

// BB_BatteryStatusRSP Message Length
#define BB_BatteryStatusRSP_DLC (8U)
// BB_BatteryStatusRSP CAN Message (68097)
#define BB_BatteryStatusRSP_CANID (0x10A01U)
// signal: @BatteryVoltageV
#define BB_BatteryStatusRSP_BatteryVoltageV_CovFactor (0.001)
// conversion value to CAN signal
#define BB_BatteryStatusRSP_BatteryVoltageV_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_BatteryStatusRSP_BatteryVoltageV_fromS(x) ((x) * 0.001)

// signal: @BatteryCurrentA
#define BB_BatteryStatusRSP_BatteryCurrentA_CovFactor (0.005)
// conversion value to CAN signal
#define BB_BatteryStatusRSP_BatteryCurrentA_toS(x) ((int16_t)((x) / 0.005))
// conversion value from CAN signal
#define BB_BatteryStatusRSP_BatteryCurrentA_fromS(x) ((x) * 0.005)

// signal: @BatterySOC
#define BB_BatteryStatusRSP_BatterySOC_CovFactor (0.1)
// conversion value to CAN signal
#define BB_BatteryStatusRSP_BatterySOC_toS(x) ((uint16_t)((x) / 0.1))
// conversion value from CAN signal
#define BB_BatteryStatusRSP_BatterySOC_fromS(x) ((x) * 0.1)

// BB_BatteryStatusRSP CAN Message (68097)
typedef struct
{
   //      Bits=16.  [ 0     , 255    ]  Unit:''
   uint16_t BatteryBlockState;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t BatteryVoltageV;
   //  [-] Bits=16.  [ -163.84, 163.835 ]  Unit:''      Factor= 0.005
   int16_t BatteryCurrentA;
   //      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1
   uint16_t BatterySOC;
} BB_BatteryStatusRSP_t;

// BB_DCDCStatusRSP Message Length
#define BB_DCDCStatusRSP_DLC (4U)
// BB_DCDCStatusRSP CAN Message (68098)
#define BB_DCDCStatusRSP_CANID (0x10A02U)
// signal: @DCBusVoltageV
#define BB_DCDCStatusRSP_DCBusVoltageV_CovFactor (0.01)
// conversion value to CAN signal
#define BB_DCDCStatusRSP_DCBusVoltageV_toS(x) ((uint16_t)((x) / 0.01))
// conversion value from CAN signal
#define BB_DCDCStatusRSP_DCBusVoltageV_fromS(x) ((x) * 0.01)

// signal: @DCBusCurrentA
#define BB_DCDCStatusRSP_DCBusCurrentA_CovFactor (0.005)
// conversion value to CAN signal
#define BB_DCDCStatusRSP_DCBusCurrentA_toS(x) ((int16_t)((x) / 0.005))
// conversion value from CAN signal
#define BB_DCDCStatusRSP_DCBusCurrentA_fromS(x) ((x) * 0.005)

// BB_DCDCStatusRSP CAN Message (68098)
typedef struct
{
   //      Bits=16.  [ 0     , 655.35 ]  Unit:''      Factor= 0.01
   uint16_t DCBusVoltageV;
   //  [-] Bits=16.  [ -163.84, 163.835 ]  Unit:''      Factor= 0.005
   int16_t DCBusCurrentA;
} BB_DCDCStatusRSP_t;

// BB_CellVoltage1to4RSP Message Length
#define BB_CellVoltage1to4RSP_DLC (8U)
// BB_CellVoltage1to4RSP CAN Message (68099)
#define BB_CellVoltage1to4RSP_CANID (0x10A03U)
// signal: @Cell1V
#define BB_CellVoltage1to4RSP_Cell1V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage1to4RSP_Cell1V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage1to4RSP_Cell1V_fromS(x) ((x) * 0.001)

// signal: @Cell2V
#define BB_CellVoltage1to4RSP_Cell2V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage1to4RSP_Cell2V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage1to4RSP_Cell2V_fromS(x) ((x) * 0.001)

// signal: @Cell3V
#define BB_CellVoltage1to4RSP_Cell3V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage1to4RSP_Cell3V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage1to4RSP_Cell3V_fromS(x) ((x) * 0.001)

// signal: @Cell4V
#define BB_CellVoltage1to4RSP_Cell4V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage1to4RSP_Cell4V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage1to4RSP_Cell4V_fromS(x) ((x) * 0.001)

// BB_CellVoltage1to4RSP CAN Message (68099)
typedef struct
{
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell1V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell2V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell3V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell4V;
} BB_CellVoltage1to4RSP_t;



// BB_CellVoltage5to8RSP Message Length
#define BB_CellVoltage5to8RSP_DLC (8U)
// BB_CellVoltage5to8RSP CAN Message (68100)
#define BB_CellVoltage5to8RSP_CANID (0x10A04U)
// signal: @Cell5V
#define BB_CellVoltage5to8RSP_Cell5V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage5to8RSP_Cell5V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage5to8RSP_Cell5V_fromS(x) ((x) * 0.001)

// signal: @Cell6V
#define BB_CellVoltage5to8RSP_Cell6V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage5to8RSP_Cell6V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage5to8RSP_Cell6V_fromS(x) ((x) * 0.001)

// signal: @Cell7V
#define BB_CellVoltage5to8RSP_Cell7V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage5to8RSP_Cell7V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage5to8RSP_Cell7V_fromS(x) ((x) * 0.001)

// signal: @Cell8V
#define BB_CellVoltage5to8RSP_Cell8V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage5to8RSP_Cell8V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage5to8RSP_Cell8V_fromS(x) ((x) * 0.001)

// BB_CellVoltage5to8RSP CAN Message (68100)
typedef struct
{
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell5V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell6V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell7V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell8V;
} BB_CellVoltage5to8RSP_t;



// BB_CellVoltage9to12RSP Message Length
#define BB_CellVoltage9to12RSP_DLC (8U)
// BB_CellVoltage9to12RSP CAN Message (68101)
#define BB_CellVoltage9to12RSP_CANID (0x10A05U)
// signal: @Cell9V
#define BB_CellVoltage9to12RSP_Cell9V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage9to12RSP_Cell9V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage9to12RSP_Cell9V_fromS(x) ((x) * 0.001)

// signal: @Cell10V
#define BB_CellVoltage9to12RSP_Cell10V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage9to12RSP_Cell10V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage9to12RSP_Cell10V_fromS(x) ((x) * 0.001)

// signal: @Cell11V
#define BB_CellVoltage9to12RSP_Cell11V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage9to12RSP_Cell11V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage9to12RSP_Cell11V_fromS(x) ((x) * 0.001)

// signal: @Cell12V
#define BB_CellVoltage9to12RSP_Cell12V_CovFactor (0.001)
// conversion value to CAN signal
#define BB_CellVoltage9to12RSP_Cell12V_toS(x) ((uint16_t)((x) / 0.001))
// conversion value from CAN signal
#define BB_CellVoltage9to12RSP_Cell12V_fromS(x) ((x) * 0.001)

// BB_CellVoltage9to12RSP CAN Message (68101)
typedef struct
{
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell9V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell10V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell11V;
   //      Bits=16.  [ 0     , 65.535 ]  Unit:''      Factor= 0.001
   uint16_t Cell12V;
} BB_CellVoltage9to12RSP_t;

// BB_CellTempRSP Message Length
#define BB_CellTempRSP_DLC (6U)
// BB_CellTempRSP CAN Message (68102)
#define BB_CellTempRSP_CANID (0x10A06U)
// signal: @Submodule1C
#define BB_CellTempRSP_Submodule1C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule1C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule1C_fromS(x) ((x) * 0.5)

// signal: @Submodule2C
#define BB_CellTempRSP_Submodule2C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule2C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule2C_fromS(x) ((x) * 0.5)

// signal: @Submodule3C
#define BB_CellTempRSP_Submodule3C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule3C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule3C_fromS(x) ((x) * 0.5)

// signal: @Submodule4C
#define BB_CellTempRSP_Submodule4C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule4C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule4C_fromS(x) ((x) * 0.5)

// signal: @Submodule5C
#define BB_CellTempRSP_Submodule5C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule5C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule5C_fromS(x) ((x) * 0.5)

// signal: @Submodule6C
#define BB_CellTempRSP_Submodule6C_CovFactor (0.5)
// conversion value to CAN signal
#define BB_CellTempRSP_Submodule6C_toS(x) ((int16_t)((x) / 0.5 + 80))
// conversion value from CAN signal
#define BB_CellTempRSP_Submodule6C_fromS(x) ((x) * 0.5)

// BB_CellTempRSP CAN Message (68102)
typedef struct
{

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule1C;

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule2C;

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule3C;

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule4C;

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule5C;

   // EJH: Corrected autogenerated code to 8-bit
   //      Bits=08.  [ -40   , 87.5   ]  Unit:''      Offset= -40       Factor= 0.5
   uint8_t Submodule6C;
} BB_CellTempRSP_t;

// BB_DCDCTemperaturesRSP Message Length
#define BB_DCDCTemperaturesRSP_DLC (4U)
// BB_DCDCTemperaturesRSP CAN Message (68103)
#define BB_DCDCTemperaturesRSP_CANID (0x10A07U)
// signal: @BBHeatsinkC
#define BB_DCDCTemperaturesRSP_BBHeatsinkC_CovFactor (1)
// conversion value to CAN signal
#define BB_DCDCTemperaturesRSP_BBHeatsinkC_toS(x) ((int32_t)((x) + 60))
// conversion value from CAN signal
#define BB_DCDCTemperaturesRSP_BBHeatsinkC_fromS(x) ((x))

// signal: @FanDutyCycle
#define BB_DCDCTemperaturesRSP_FanDutyCycle_CovFactor (0.1)
// conversion value to CAN signal
#define BB_DCDCTemperaturesRSP_FanDutyCycle_toS(x) ((uint16_t)((x) / 0.1))
// conversion value from CAN signal
#define BB_DCDCTemperaturesRSP_FanDutyCycle_fromS(x) ((x) * 0.1)

// BB_DCDCTemperaturesRSP CAN Message (68103)
typedef struct
{

   // EJH: Corrected autogenerated code to 16-bit
   //      Bits=16.  [ -60   , 195    ]  Unit:''      Offset= -60
   int16_t BBHeatsinkC;
   //      Bits=16.  [ 0     , 102.3  ]  Unit:''      Factor= 0.1
   uint16_t FanDutyCycle;
} BB_DCDCTemperaturesRSP_t;


#ifdef __cplusplus
}
#endif

