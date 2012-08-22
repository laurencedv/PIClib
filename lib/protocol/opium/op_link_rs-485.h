/*!
 @file		op_link_rs-485.h
 @brief		

 @version	0.1
 @note
 @todo

 @date		August 14th 2012
 @author	Laurence DV
*/

#ifndef _OP_LINK_RS_485_H
#define _OP_LINK_RS_485_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################# Data Type ################## //
// Control
typedef union
{
	U32 all[4];
	struct
	{
		tOpRS485LinkState linkState;
		tOpRS485LinkSubState linkSubState;
		U8 timerID;
		U8 uartID;
		U8 utilityCnt;
		U8 slotNb;
		tOpRS485Slot * slotControl;
		U8 currentFrame;
		U8 currentSlot;
	};
}tOpRS485Control;

// Slot control
typedef union
{
	U16 all;
	struct
	{
		U8 frame;
		U8 slot;
	};
}tOpRS485Slot;

// Link State
typedef enum
{
	detect = 0,
	election,
	master,
	slave
}tOpRS485LinkState;

// Link SubState
typedef enum
{
	init = 0,
	active,
	wait
}tOpRS485LinkSubState;
// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //

// =========================== //
// ############################################## //

#endif
