/*!
 @file		pic18_clock.c
 @brief		Real Time keeping for pic18 based on hardware TMR0

 @version	0.1
 @note		Need the initialOSC and initialPeriod global variable normally found in the hardware.c file of the project
 @todo		- Config reg read function, to check the OSC/PLL configuration and compute the correct running Frequency
			- REFOCLK control function

 @date		November 22th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_clock.h"
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################## Variables ################# //
U32 globalCLK = OSC;
U32 globalMIPS = OSC/4;
// ############################################## //


// ################ ADC Functions ############### //
// === Interrupt Handler ===== //

// =========================== //


// === Control Functions ===== //


void refoclkSet(U32 desiredCLK)
{

}
// =========================== //


// === Convert Functions ===== //

// =========================== //
// ############################################## //
