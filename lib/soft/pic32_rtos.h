/*!
 @file		pic32_rtos.h
 @brief		Real Time Operating System for pic32

 @version	0.1
 @note		
 @todo		

 @date		August 17th 2012
 @author	Laurence DV
*/



#ifndef _PIC32_RTOS_H
#define _PIC32_RTOS_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic32_timer.h>
#include <peripheral/pic32_interrupt.h>
#include <soft/pic32_realtime.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define RTOS_INITIAL_SIZE			10						//Initial Number of avaialble space in the pool
#define RTOS_INCREMENT_SIZE			10						//Number of available space to add when poolResize
#define RTOS_RESIZE_TIME			1000					//Systick Interval between poolResize
// ############################################## //


// ################# Data Type ################## //

// Process Data Container
typedef union
{
	U32 all[];
	struct
	{
		union
		{
			U8 all;
			struct
			{
				U8 enable:1;			//This process is enabled
				U8 :7;
			};
		}control;
		U8 ID;							//Process unique ID
		U8 defaultPriority;				//Process's default priority
		U8 priority;					//Process's actual priority
		U32 executionTime;				//Estismated total execution time
		void (*functionPtr)(void);		//Function to execute in this process
	};
}tRTOSProcessContainer;

// Process Message header
typedef union
{
	U32 all;
	struct
	{
		U8 source;						//Process ID sending the message
		U8 destination;					//Process ID receiving the message
		U8 messageLenght;				//Number of 32bit word of the message
	};
}tRTOSProcessMessage;
// ############################################## //


// ################# Prototypes ################# //

// ############################################## //

#endif
