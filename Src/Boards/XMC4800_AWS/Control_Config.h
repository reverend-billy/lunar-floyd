/*******************************************************************************
// Control Module Configuration
*******************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
// Public Constant Declarations
*******************************************************************************/

// Defines the "Running" state for fast blinking
#define CONTROL_ACTIVE_STATE  (CONTROL_STATE_MPPT)


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Defines the enumerated list of control states for the system
typedef enum
{
   //Default state at boot up
   CONTROL_STATE_UNKNOWN,
   // Device is turned off
   CONTROL_STATE_OFF,
   // Device is in sleep state
   CONTROL_STATE_SLEEPING,
   // Device is in the process of starting up
   CONTROL_STATE_STARTING,
   // Device is running
   CONTROL_STATE_MPPT,
   // Running with foldbacks
   CONTROL_STATE_THROTTLED,
   // Device is in the process of shutting down
   CONTROL_STATE_SHUTTING_DOWN,
   // Device is in idle mode
   CONTROL_STATE_STANDBY,
   // This defines the total number of states -- excluding fault
   CONTROL_STATE_Count,
   // Device is in a fault state This state is not set directly. 
   // It is triggered automatically based on Error Manager faults.
   CONTROL_STATE_FAULT = 99U
} Control_State_t;


#ifdef __cplusplus
extern "C"
}
#endif


