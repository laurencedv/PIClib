/*!
 @file		pic32_cpu.c
 @brief		CPU Control lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		September 20th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_cpu.h"
// ############################################## //


// ################## Variables ################# //
extern U32 globalDump;
// ############################################## //


// ################ CPU Functions ############### //
/**
* \fn		void cpuSoftReset(void)
* @brief	Call a software Reset
* @note		This function will never exit, the reset is done inside
* @arg		nothing
* @return	nothing
*/
void cpuSoftReset(void)
{
	sysUnlock();			//Unlock the register
	RSWRSTSET = SWRST_MASK;		//Enable the Soft Reset Event
	globalDump = RSWRST;		//Read the Register
	for(;;);			//Wait for the reset
}
// ############################################## //

