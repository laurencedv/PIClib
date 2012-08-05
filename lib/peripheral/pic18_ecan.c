/*!
 @file		pic18_ecan.c
 @brief		ECAN functions for C18

 @version	0.1
 @note		All Function are Non-Blocking
			Depends on some global parameters in hardware.h

 @date		November 2th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_ecan.h"
// ############################################## //


// ################# Variables ################## //
// FIFO Buffer //
#pragma udata canBuf								//Take a full Ram Bank for the Buffers
// ----------- //

// Global Parameters //
extern U32 globalOSC;
extern U8 globalDump;
// ----------------- //
// ############################################## //


// ############## EUSART Functions ############## //
// ==== Interrupt Handler ==== //
void canISR(void)
{

}


// =========================== //


// ==== Control Functions ==== //

/**
* \fn		U8 canInit(U8 baudRate)
* @brief	Init the CAN module
* @note
* @arg		U8 baudRate			Speed to initalise the pipes
* @return	U8 errorCode		STD Error Code
*/
U8 canInit(U8 baudRate)
{
	// -- Init IO Direction -- //
	// ----------------------- //

	//Force in Config mode
	CANCONbits.REQOP = CAN_MODE_CONF;

	// -- Set the mode -- //
	// ------------------ //

	//Set the baudRate
	canSetBaudRate(baudRate);

	// -- Set Filter and Mask -- //
	// ------------------------- //

	// -- Start the CAN Module -- //
	// -------------------------- //
}

/**
* \fn		U8 canSetBaudRate(U8 baudRate)
* @brief	Set the baud rate of the CAN module
* @note
* @arg		U8 baudRate			Speed to initalise the pipes
* @return	U8 errorCode		STD Error Code
*/
U8 canSetBaudRate(U8 baudRate)
{

}

// =========================== //


// === Transfert Functions === //

// =========================== //
// ############################################## //
