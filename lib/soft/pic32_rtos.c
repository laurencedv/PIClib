/*!
 @file		pic32_rtos.c
 @brief		Real Time Operating System for pic32

 @version	0.1
 @note		
 @todo		

 @date		August 17th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_rtos.h"
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;

tRTOSProcessContainer * runningProcess;					//Process that is currently running
tRTOSProcessContainer * processPool;					//List of all process
tRTOSProcessContainer * availableProcess;				//List of all enabled process
// ############################################## //


// ################ RTOS Functions ############## //
/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSInitPool(void)
{
	// -- Allocate the initial pool -- //
	// ------------------------------- //

	// -- Crate the resizePool process -- //
	// ---------------------------------- //

	
}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSResizePool(void)
{
	
}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSSpawnProcess(void)
{
	// -- Allocate the Data Container -- //
	// --------------------------------- //

	// -- Initialized the Container -- //
	// ------------------------------- //

	// -- Add it to the pool -- //
	// ------------------------ //
}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSKillProcess(void)
{

}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSEnableProcess(void)
{

}

/**
* \fn		U8 nrfInit(void)
* @brief
* @note
* @arg		U8
* @return	U8 errorCode					STD Error Code
*/
void RTOSDisableProcess(void)
{

}
// ############################################## //
