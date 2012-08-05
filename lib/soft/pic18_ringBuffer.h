/*!
 @file		pic18_eusart.c
 @brief		EUSART functions for C18

 @version	0.2
 @note		Define the pin for each EUSART
			Depend on $globalFOSC (in hardware.h) for Baud Rate Computation

 @date		October 26th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_EUSART_H
#define _PIC18_EUSART_H
// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_C18.h>
// ############################################## //


// ################## Defines ################### //
// Application Variable //
#define RBUF_SIZE		128						//Size of the buffer
// -------------------- //


// -- Init Options -- //

// ------------------ //
// ############################################## //


// ################# Data Type ################## //
// Control Flag //
typedef union
{
	U8 all;
	struct
	{
		U8 full:1;
		U8 empty:1;
		U8 :6;
	};
}tRBufCtl;
// ----------- //

// Buffer struct //
typedef struct
{
	U8 byte[RBUF_SIZE];
	U8 in;
	U8 out;
	U8 size;
	tRBufCtl control;
}tRBuf;
// ------------- //

// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //
// =========================== //


// === Transfert Functions === //à
// =========================== //
// ############################################## //


#endif
