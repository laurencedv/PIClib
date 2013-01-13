/*!
 @file		splitvar_megaxone.h
 @brief		Splitted variable structure definition for various compiler

 @version	0.3.2
 @note		Work and tested for C18,C32

 @date		February 15th 2011
 @author	Laurence DV
*/

#ifndef _SPLITVAR_MEGAXONE_H
#define _SPLITVAR_MEGAXONE_H

// ################## Includes ################## //
#include <definition/datatype_megaxone.h>
// ############################################## //


// ################# Definition ################# //
//1 Byte splitted into 8bits, 4 pairs and 2 nibbles
typedef union
{
	U8 all;

	struct
	{
		U8 b0:1;
		U8 b1:1;
		U8 b2:1;
		U8 b3:1;
		U8 b4:1;
		U8 b5:1;
		U8 b6:1;
		U8 b7:1;
	};

	struct
	{
		U8 pair0:2;
		U8 pair1:2;
		U8 pair2:2;
		U8 pair3:2;
	};

	struct
	{
		U8 nib0:4;
		U8 nib1:4;
	};
}split8;

//2 Byte splitted into 16bits, 8 pairs, 4 nibbles, and 2 bytes
typedef union
{
	U16 all;

	struct
	{
		U8 b0:1;
		U8 b1:1;
		U8 b2:1;
		U8 b3:1;
		U8 b4:1;
		U8 b5:1;
		U8 b6:1;
		U8 b7:1;
		U8 b8:1;
		U8 b9:1;
		U8 b10:1;
		U8 b11:1;
		U8 b12:1;
		U8 b13:1;
		U8 b14:1;
		U8 b15:1;
	};

	struct
	{
		U8 pair0:2;
		U8 pair1:2;
		U8 pair2:2;
		U8 pair3:2;
		U8 pair4:2;
		U8 pair5:2;
		U8 pair6:2;
		U8 pair7:2;
	};

	struct
	{
		U8 nib0:4;
		U8 nib1:4;
		U8 nib2:4;
		U8 nib3:4;
	};

	struct
	{
		U8 byte1;
		U8 byte0;
	};
}split16;

//4 Bytes splitted into 32bits, 16 pairs, 8 nibbles, 4 bytes and 2 halfs
typedef union
{
	U32 all;

	struct
	{
		U8 b0:1;
		U8 b1:1;
		U8 b2:1;
		U8 b3:1;
		U8 b4:1;
		U8 b5:1;
		U8 b6:1;
		U8 b7:1;
		U8 b8:1;
		U8 b9:1;
		U8 b10:1;
		U8 b11:1;
		U8 b12:1;
		U8 b13:1;
		U8 b14:1;
		U8 b15:1;
		U8 b16:1;
		U8 b17:1;
		U8 b18:1;
		U8 b19:1;
		U8 b20:1;
		U8 b21:1;
		U8 b22:1;
		U8 b23:1;
		U8 b24:1;
		U8 b25:1;
		U8 b26:1;
		U8 b27:1;
		U8 b28:1;
		U8 b29:1;
		U8 b30:1;
		U8 b31:1;
	};

	struct
	{
		U8 pair0:2;
		U8 pair1:2;
		U8 pair2:2;
		U8 pair3:2;
		U8 pair4:2;
		U8 pair5:2;
		U8 pair6:2;
		U8 pair7:2;
		U8 pair8:2;
		U8 pair9:2;
		U8 pair10:2;
		U8 pair11:2;
		U8 pair12:2;
		U8 pair13:2;
		U8 pair14:2;
		U8 pair15:2;
	};

	struct
	{
		U8 nib0:4;
		U8 nib1:4;
		U8 nib2:4;
		U8 nib3:4;
		U8 nib4:4;
		U8 nib5:4;
		U8 nib6:4;
		U8 nib7:4;
	};

	struct
	{
		U8 byte3;
		U8 byte2;
		U8 byte1;
		U8 byte0;
	};

	struct
	{
		U16 half1;
		U16 half0;
	};

    
}split32;

#if defined (__PIC32MX)

//8 Bytes splitted into 64bits, 32 pairs, 16 nibbles, 8 bytes, 4 quarters and 2 halfs
typedef union
{
	U64 all;

	struct
	{
		U8 b0:1;
		U8 b1:1;
		U8 b2:1;
		U8 b3:1;
		U8 b4:1;
		U8 b5:1;
		U8 b6:1;
		U8 b7:1;
		U8 b8:1;
		U8 b9:1;
		U8 b10:1;
		U8 b11:1;
		U8 b12:1;
		U8 b13:1;
		U8 b14:1;
		U8 b15:1;
		U8 b16:1;
		U8 b17:1;
		U8 b18:1;
		U8 b19:1;
		U8 b20:1;
		U8 b21:1;
		U8 b22:1;
		U8 b23:1;
		U8 b24:1;
		U8 b25:1;
		U8 b26:1;
		U8 b27:1;
		U8 b28:1;
		U8 b29:1;
		U8 b30:1;
		U8 b31:1;
		U8 b32:1;
		U8 b33:1;
		U8 b34:1;
		U8 b35:1;
		U8 b36:1;
		U8 b37:1;
		U8 b38:1;
		U8 b39:1;
		U8 b40:1;
		U8 b41:1;
		U8 b42:1;
		U8 b43:1;
		U8 b44:1;
		U8 b45:1;
		U8 b46:1;
		U8 b47:1;
		U8 b48:1;
		U8 b49:1;
		U8 b50:1;
		U8 b51:1;
		U8 b52:1;
		U8 b53:1;
		U8 b54:1;
		U8 b55:1;
		U8 b56:1;
		U8 b57:1;
		U8 b58:1;
		U8 b59:1;
		U8 b60:1;
		U8 b61:1;
		U8 b62:1;
		U8 b63:1;
	};

	struct
	{
		U8 pair0:2;
		U8 pair1:2;
		U8 pair2:2;
		U8 pair3:2;
		U8 pair4:2;
		U8 pair5:2;
		U8 pair6:2;
		U8 pair7:2;
		U8 pair8:2;
		U8 pair9:2;
		U8 pair10:2;
		U8 pair11:2;
		U8 pair12:2;
		U8 pair13:2;
		U8 pair14:2;
		U8 pair15:2;
		U8 pair16:2;
		U8 pair17:2;
		U8 pair18:2;
		U8 pair19:2;
		U8 pair20:2;
		U8 pair21:2;
		U8 pair22:2;
		U8 pair23:2;
		U8 pair24:2;
		U8 pair25:2;
		U8 pair26:2;
		U8 pair27:2;
		U8 pair28:2;
		U8 pair29:2;
		U8 pair30:2;
		U8 pair31:2;
	};

	struct
	{
		U8 nib0:4;
		U8 nib1:4;
		U8 nib2:4;
		U8 nib3:4;
		U8 nib4:4;
		U8 nib5:4;
		U8 nib6:4;
		U8 nib7:4;
		U8 nib8:4;
		U8 nib9:4;
		U8 nib10:4;
		U8 nib11:4;
		U8 nib12:4;
		U8 nib13:4;
		U8 nib14:4;
		U8 nib15:4;
	};

	struct
	{
		U8 byte7;
		U8 byte6;
		U8 byte5;
		U8 byte4;
		U8 byte3;
		U8 byte2;
		U8 byte1;
		U8 byte0;
	};

	struct
	{
		U16 quarter3;
		U16 quarter2;
		U16 quarter1;
		U16 quarter0;
	};

	struct
	{
		U32 half1;
		U32 half0;
	};
}split64;

#endif

// ############################################## //

#endif
