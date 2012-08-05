/*!
 @file		pic32_dma.h
 @brief		DMA Control lib for pic32

 @version	0.1
 @note		DMA control use the physical Address not the virtual one!
 @todo		

 @date		February 16th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_DMA_H
#define _PIC32_DMA_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <toolbitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// ############################################## //


// ################# Data Type ################## //
//DMA channel control structure
typedef union
{

	struct
	{
		U32 * sourcePtr;
		U32 * destinationPtr;
		U16 cellSize;
		U16 blockSize;
		U32 startEvent;
		U32 abortEvent;
		struct
		{
			
		}option;
	};
};

//DCHxCON	DMA channel control
typedef union
{
	U32 all;
	struct
	{
		U32 CHPRI:2;				//Channel priority Selection
		U32 CHEDET:1;				//Channel event detection Flag
		U32 :1;
		U32 CHAEN:1;				//Channel automatic enabling Enable
		U32 CHCHN:1;				//Channel chain Enable
		U32 CHAED:1;				//Channel allow event when disabled Enable
		U32 CHEN:1;					//Channel Enable
		U32 CHCHNS:1;				//Channel chain Selection
		U32 :6;
		U32 CHBUSY:1;				//Channel busy/active Flag
		U32 :16;
	};
}tDCHxCON;

//DCHxECON	DMA channel event control
typedef union
{
	U32 all;
	struct
	{
		U32 :3;
		U32 AIRQEN:1;				//Channel abort IRQ Enable
		U32 SIRQEN:1;				//Channel start IRQ Enable
		U32 PATEN:1;				//Channel pattern match abort Enable
		U32 CABORT:1;				//Channel force abort Enable
		U32 CFORCE:1;				//Channel force start Enable
		U32 CHSIRQ:8;				//Start IRQ Selection
		U32 CHAIRQ:8;				//Abort IRQ Selection
		U32 :8;
	};
}tDCHxECON;

//DCHxINT	DMA channel interrupt control and status
typedef union
{
	U32 all;
	struct
	{
		U32 CHERIF:1;				//Channel address error int Flag
		U32 CHTAIF:1;				//Channel transfer abort int Flag
		U32 CHCCIF:1;				//Channel cell transfer completed int Flag
		U32 CHBCIF:1;				//Channel block transfer completed int Flag
		U32 CHDHIF:1;				//Channel destination half-full int Flag
		U32 CHDDIF:1;				//Channel destination full int Flag
		U32 CHSHIF:1;				//Channel source half-empty int Flag
		U32 CHSDIF:1;				//Channel source empty int Flag
		U32 :8;
		U32 CHERIE:1;				//Channel address error int Enable
		U32 CHTAIE:1;				//Channel transfer abort int Enable
		U32 CHCCIE:1;				//Channel cell transfer completed int Enable
		U32 CHBCIE:1;				//Channel block transfer completed int Enable
		U32 CHDHIE:1;				//Channel destination half-full int Enable
		U32 CHDDIE:1;				//Channel destination full int Enable
		U32 CHSHIE:1;				//Channel source half-empty int Enable
		U32 CHSDIE:1;				//Channel source empty int Enable
		U32 :8;
	};
}tDCHxINT;

//DCHxSSIZ	DMA channel source size
typedef union
{
	U32 all;
	struct
	{
		U32 CHSSIZ:16;				//Channel source size selection
		U32 :16;
	};
}tDCHxSSIZ;

//DCHxDSIZ	DMA channel destination size
typedef union
{
	U32 all;
	struct
	{
		U32 CHDSIZ:16;				//Channel destination size selection
		U32 :16;
	};
}tDCHxDSIZ;

//DCHxSPTR	DMA channel source pointer
typedef union
{
	U32 all;
	struct
	{
		U32 CHSPTR:16;				//Channel source pointer
		U32 :16;
	};
}tDCHxSPTR;

//DCHxDPTR	DMA channel destination pointer
typedef union
{
	U32 all;
	struct
	{
		U32 CHDPTR:16;				//Channel destination pointer
		U32 :16;
	};
}tDCHxDPTR;

//DCHxCSIZ	DMA channel cell size
typedef union
{
	U32 all;
	struct
	{
		U32 CHCSIZ:16;				//Channel cell size Selection
		U32 :16;
	};
}tDCHxCSIZ;

//DCHxCPTR	DMA channel cell pointer
typedef union
{
	U32 all;
	struct
	{
		U32 CHCPTR:16;				//Channel cell status
		U32 :16;
	};
}tDCHxCPTR;

//DCHxDAT	DMA channel pattern data
typedef union
{
	U32 all;
	struct
	{
		U32 CHPDAT:8;				//Channel pattern match data
		U32 :24;
	};
}tDCHxDAT;
// ############################################## //


// ################# Prototypes ################# //
// === Control Functions ===== //

// ========================== //
// ############################################## //

#endif
