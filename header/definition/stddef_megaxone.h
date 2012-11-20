/*!
 @file		stddef_megaxone.h
 @brief		Standard syntax definition and macro

 @version	0.3.4
 @note

 @date		September 15th 2012
 @author	Laurence DV
*/

#ifndef _STDDEF_MEGAXONE_H
#define _STDDEF_MEGAXONE_H 1

// ################## Includes ################## //
#include <stddef.h>
// ############################################## //


// ############ Developpement Macro ############# //
#define CONCAT(a,b)		XCONCAT(a,b)
#define	CONCAT3(a,b,c)		(CONCAT(CONCAT(a,b),c))
#define XCONCAT(a,b)		a ## b
// ############################################## //


// ############### Register Macro ############### //
#if defined (__PIC32MX)
	#define REG_OFFSET_CLR_32	0x1
	#define REG_OFFSET_SET_32	0x2
	#define REG_OFFSET_INV_32	0x3
	#define REG_OFFSET_NEXT_32	0x4

	#define REG_OFFSET_CLR_16	0x2
	#define REG_OFFSET_SET_16	0x4
	#define REG_OFFSET_INV_16	0x6
	#define REG_OFFSET_NEXT_16	0x8

	#define REG_OFFSET_CLR_8	0x4
	#define REG_OFFSET_SET_8	0x8
	#define REG_OFFSET_INV_8	0xC
	#define REG_OFFSET_NEXT_8	0x10
#endif
// ############################################## //


// ################# Data Value ################# //
#ifndef HIGH
	#define	HIGH		1
#endif
#ifndef LOW
	#define	LOW		0
#endif
	
#define RISING			1
#define FALLING			0

#ifndef TRUE
	#define TRUE		1
#endif
#ifndef FALSE
	#define FALSE		0
#endif

#define ENABLE			1
#define DISABLE			0

#define LEFT			0
#define RIGHT			1

#define	OUTPUT			0
#define	INPUT			1

#define RECEIVE			0
#define SEND			1

// ############################################## //


// ############## Processor Family ############## //
#define	PIC18Fx7Jx3		1	//PIC 18(L)F 27J13 | 27J53 | 47J13 | 47J53
#define PIC18FxxK80		2	//PIC 18(L)F 25K80 | 26K80 | 45K80 | 46K80 | 65K80 | 66K80
#define PIC18FxxK22		3	//PIC 18(L)F 23K22 | 24K22 | 25K22 | 26K22 | 43K22 | 44K22 | 45K22 | 46K22
#define PIC32MX1xx		4	//PIC 32MX 110F016[B,C,D] | 120F032[B,C,D] | 130F064[B,C,D] | 150F128[B,C,D]
#define PIC32MX2xx		5	//PIC 32MX 210F016[B,C,D] | 220F032[B,C,D] | 220F064[B,C,D] | 220F128[B,C,D]
#define PIC32MX3xx		6
#define PIC32MX4xx		7
#define PIC32MX5xxH		8	//PIC 32MX 534F064H | 564F064H | 564F128H | 575F256H | 575F512H
#define PIC32MX5xxL		9	//PIC 32MX 534F064L | 564F064L | 564F128L | 575F256L | 575F512L
#define PIC32MX6xx		10
#define PIC32MX7xx		11	//PIC 32MX 764F128[H,L] | 775F256[H,L] | 775F512[H,L] | 795F512[H,L]
// ############################################## //


// ############### STD Error Code ############### //
#define	STD_EC_SUCCESS		0
#define STD_EC_FAIL		1
#define	STD_EC_NOTFOUND		2
#define STD_EC_COLLISION	3
#define STD_EC_BUSY		4
#define STD_EC_INVALID		5
#define STD_EC_SHUTDOWN		6
#define STD_EC_TIMEOUT		7
#define	STD_EC_EMPTY		8
#define STD_EC_MEMORY		9
#define	STD_EC_TOOLARGE		20
#define STD_EC_TOOSMALL		21
#define STD_EC_UNDERRUN		254
#define STD_EC_OVERFLOW		255
// ############################################## //


// ################ Bit position ################ //
#define	BIT0			(1 << 0)
#define	BIT1			(1 << 1)
#define	BIT2			(1 << 2)
#define	BIT3			(1 << 3)
#define	BIT4			(1 << 4)
#define	BIT5			(1 << 5)
#define	BIT6			(1 << 6)
#define	BIT7			(1 << 7)
#define	BIT8			(1 << 8)
#define	BIT9			(1 << 9)
#define	BIT10			(1 << 10)
#define	BIT11			(1 << 11)
#define	BIT12			(1 << 12)
#define	BIT13			(1 << 13)
#define	BIT14			(1 << 14)
#define	BIT15			(1 << 15)
#define	BIT16			(1 << 16)
#define	BIT17			(1 << 17)
#define	BIT18			(1 << 18)
#define	BIT19			(1 << 19)
#define	BIT20			(1 << 20)
#define	BIT21			(1 << 21)
#define	BIT22			(1 << 22)
#define	BIT23			(1 << 23)
#define	BIT24			(1 << 24)
#define	BIT25			(1 << 25)
#define	BIT26			(1 << 26)
#define	BIT27			(1 << 27)
#define	BIT28			(1 << 28)
#define	BIT29			(1 << 29)
#define	BIT30			(1 << 30)
#define	BIT31			(1 << 31)
#define	BIT32			(1 << 32)
#define	BIT33			(1 << 33)
#define	BIT34			(1 << 34)
#define	BIT35			(1 << 35)
#define	BIT36			(1 << 36)
#define	BIT37			(1 << 37)
#define	BIT38			(1 << 38)
#define	BIT39			(1 << 39)
#define	BIT40			(1 << 40)
#define	BIT41			(1 << 41)
#define	BIT42			(1 << 42)
#define	BIT43			(1 << 43)
#define	BIT44			(1 << 44)
#define	BIT45			(1 << 45)
#define	BIT46			(1 << 46)
#define	BIT47			(1 << 47)
#define	BIT48			(1 << 48)
#define	BIT49			(1 << 49)
#define	BIT50			(1 << 50)
#define	BIT51			(1 << 51)
#define	BIT52			(1 << 52)
#define	BIT53			(1 << 53)
#define	BIT54			(1 << 54)
#define	BIT55			(1 << 55)
#define	BIT56			(1 << 56)
#define	BIT57			(1 << 57)
#define	BIT58			(1 << 58)
#define	BIT59			(1 << 59)
#define	BIT60			(1 << 60)
#define	BIT61			(1 << 61)
#define	BIT62			(1 << 62)
#define	BIT63			(1 << 63)
// ############################################## //

#endif
