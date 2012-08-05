/*!
 @file pic18_lin.h
 @brief		LIN functions for C18
 *			Define the $LIN_SLAVE_ID if the device is a slave
 @depency pic18_eusart
 @version 0.1
 @date September 12th 2011
 @author Laurence DV
*/

#ifndef _PIC18_LIN_H
#define _PIC18_LIN_H
// ################## Include ################### //
#include <hardware.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_C18.h>

//Lib
#include <peripheral/pic18_eusart.h>
#include <peripheral/pic18_interrupt.h>
#include <device/pic18_lcd_hd44780.h>
// ############################################## //


// ################### Define ################### //
// Application ID command
#define LIN_COMMAND_TABLE_SIZE	10					//Maximum number of command for this slave (max 254)
#define	LIN_COMMAND_DIR_IN		1
#define LIN_COMMAND_DIR_OUT		0

#define LIN_RSP_DIR_RECEIVE		1
#define	LIN_RSP_DIR_TRANSMIT	2

// LIN Constant
#define LIN_BREAK_CHAR			0x00				//Value of the Break Character
#define LIN_SYNC_FIELD			0x55				//Value of the Sync Field
#define LIN_MAX_DATA			8					//Maximum number of byte per packet
// ############################################## //


// ################# Variables ################## //
enum tLINState
{
	idle,
	sync,
	ID,
	dataRx,
	dataTx,
	checkChecksum,
	sendChecksum,
	packetComplete
};

typedef struct
{
	U8 command;
	U8 byteNb;
	U8 direction;
	U8 *packetPtr;
	U8 *flagPtr;
}tLinCommandTable;

typedef union
{
	U8 byte[12];
	struct
	{
		U8 linBreak;
		U8 sync;
		U8 ID;
		U8 data[8];
		U8 checksum;
	};
}tLINPacket;
// ############################################## //


// ################ Prototype ################### //
/**
* \fn		U8 linAddCommand(U8 *packetPtr,U8 command, U8 byteNb)
* @brief	Append a command with it's information into the linCommandTable
* @note
* @arg		U8 *packetPtr		Pointer to save the received packet
* @arg		U8 command			Value of the command
* @arg		U8 byteNb			Number of byte to received for the command
* @return	U8 commandID		The ID of the command in the linCommandTable
*/
U8 linAddCommand(U8 *dataPtr,U8 command, U8 byteNb, U8 direction, U8 *flagPtr);

/**
* \fn		U8 linReceivePacket(U8 receivedByte)
* @brief	Receive and decode a packet
* @note
* @arg		U8 receivedData		Byte received on the RxD pin of the EUSART
* @return	U8 commandID		LIN Command ID received for the last packet
*/
U8 linParsePacket(U8 receivedByte);

/**
* \fn		U8 linCheckStatus(void)
* @brief	Return the state of the LIN Finite State Machine
* @note		Can be used to check for new packet
* @arg		nothing
* @return	U8 linStatus		Status of the LIN Finite State Machine
*/
U8 linCheckStatus(void);

/**
* \fn		U8 linCheckCommand(void)
* @brief	Return the ID of the last completed command and clear the newPacket flag if so.
* @note		Must be used after being alterted of a new packet
* @arg		nothing
* @return	U8 newCommandID		Command ID of the last completed packet (return 0xFF is no new packet)
*/
U8 linCheckCommand(void);


// MASTER Functions

// Interrupt handler to send a packet as MASTER
//void linPacketAssembler(void);

// Interrupt data handling
U8 linDataReceiver(U8 dataReceived);
U8 linMasterEngine(void);

// Function to send a packet as MASTER
void linSendCommand(U8 *dataPtr, U8 linCommandID, U8 byteNb, U8 commandDir, U8 *flagPtr);

//Timeout functions
void linTimeout(void);
U16 linTimeoutCheck(void);

// ############################################## //
#endif

