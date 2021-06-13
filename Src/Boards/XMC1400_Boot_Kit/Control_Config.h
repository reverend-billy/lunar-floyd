/*******************************************************************************
// Control Module Configuration
*******************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
// Public Type Declarations
*******************************************************************************/

// Defines the enumerated list of control states for the system
typedef enum
{
   /*Default state at boot up
   */
   CONTROL_STATE_UNKNOWN,
   // Device is currently disconnected
   CONTROL_STATE_DISCONNECTED,
   // Device is currently initializing
   CONTROL_STATE_INITIALIZING,
   /** Device has successfully connected This is the typical running
     * mode                                                         
   */
   CONTROL_STATE_CONNECTED,
   // Device is in standby 
   // This is the Idle state
   CONTROL_STATE_STANDBY,
   // State of Charge Protection mode
   CONTROL_STATE_SOC_PROTECTION,
   // Device is in the process of suspending operation
   CONTROL_STATE_SUSPENDING,
   // This defines the total number of states -- excluding fault
   CONTROL_STATE_Count,
   /** Device is in a fault state This state is not set directly. It
     * is triggered automatically based on Error Manager faults     
   */
   CONTROL_STATE_FAULT = 99U
} Control_State_t;


#ifdef __cplusplus
extern "C"
}
#endif


