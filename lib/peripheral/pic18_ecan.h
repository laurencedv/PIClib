/*!
 @file		pic18_ecan.h
 @brief		ECAN functions for C18

 @version	0.1
 @note		All Function are Non-Blocking
			Depends on some global parameters in hardware.h

 @date		November 2th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_ECAN_H
#define _PIC18_ECAN_H
// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>

// Lib
#include <peripheral/pic18_interrupt.h>
// ############################################## //


// ################## Defines ################### //
// Application Variable //
#define	CAN_BUF_SIZE				116				//Number of byte for the FIFO RX/TX buffer for each EUSART (max 116)
// -------------------- //

// Module Mode //
#define CAN_MODE_CONF				4               //Configuration of the module is only possible in this mode
#define CAN_MODE_LISTEN				3               //
#define CAN_MODE_LOOPBACK			2				//
#define CAN_MODE_SLEEP				1				//
#define CAN_MODE_NORMAL				0               //Normal Mode
// ----------- //

// Interrupt Code //
/*
#define CAN_ICODE_ERROR				0x2
#define CAN_ICODE_TXB2				0x4
#define CAN_ICODE_TXB1				0x6
#define CAN_ICODE_TXB0				0x8
#define CAN_ICODE_RXB1				0xA
#define CAN_ICODE_RXB0				0xC

#define CAN_ICODE_MODE0_WAKE		0x2
#define CAN_ICODE_WAKE				0xE
#define CAN_ICODE_RX				0x10
#define	CAN_ICODE_TX
*/
// -------------- //

// Hardware Constant //

// ----------------- //

// -- Init Options -- //

// ------------------ //
// ############################################## //


// ################# Data Type ################## //

// ############################################## //


// ################# Prototypes ################# //
// ==== Interrupt Handler ==== //

// =========================== //


// ==== Control Functions ==== //

// =========================== //


// === Transfert Functions === //

// =========================== //
// ############################################## //


#endif
