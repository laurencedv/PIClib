/*!
 @file		datatype_megaxone.h
 @brief		Data Type macro for C18

 @version	0.5
 @note		This file check for global define identifying each compiler, and load the correct
			datatype in each typedef

 @date		September 12th 2011
 @author	Laurence DV
*/

#ifndef _DATATYPE_MEGAXONE_H
#define _DATATYPE_MEGAXONE_H 1

#include <limits.h>

// ################ Microchip C18 ############### //
#if defined (__18CXX)
// Signed Integer
typedef char				S8;
typedef int				S16;
typedef short long			S24;
typedef long				S32;

// Unsigned Integer
typedef unsigned char			U8;
typedef unsigned int			U16;
typedef unsigned short long		U24;
typedef unsigned long			U32;

// Float
typedef float				F32;

//FSM Standard State
typedef enum
{
	unknown = 0,
	init,
	idle,
	busy,
	fetch,
	transfer,
	error
}tFSMState;

//Limits
#define S8_MIN				(SCHAR_MIN)
#define S8_MAX				(SCHAR_MAX)
#define S8_BIT				(8)
#define S16_MIN				(INT_MIN)
#define S16_MAX				(INT_MAX)
#define S16_BIT				(16)
#define S24_MIN				(SHRT_MIN)
#define S24_MAX				(SHRT_MAX)
#define S24_BIT				(24)
#define S32_MIN				(LONG_MIN)
#define S32_MAX				(LONG_MAX)
#define S32_BIT				(32)

#define U8_MIN				(0)
#define U8_MAX				(255)
#define U8_BIT				(8)
#define U16_MIN				(0)
#define U16_MAX				(65535)
#define U16_BIT				(16)
#define U24_MIN				(0)
#define U24_MAX				(16777215)
#define U24_BIT				(24)
#define U32_MIN				(0)
#define U32_MAX				(4294967295)
#define U32_BIT				(32)
// ############################################## //


// ################ Microchip C32 ############### //
#elif defined (__PIC32MX)

// Signed Integer
typedef char				S8;
typedef short				S16;
typedef int				S32;
typedef long long			S64;

// Unsigned Integer
typedef unsigned char			U8;
typedef unsigned short			U16;
typedef unsigned int			U32;
typedef unsigned long long		U64;

// Float
typedef double				F32;
typedef long double			F64;

//FSM Standard State
typedef enum
{
	unknown = 0,
	init,
	idle,
	busy,
	fetch,
	transfer,
	error
}tFSMState;

//Limits
#define S8_MIN				(SCHAR_MIN)
#define S8_MAX				(SCHAR_MAX)
#define S8_BIT				(8)
#define S16_MIN				(SHRT_MIN)
#define S16_MAX				(SHRT_MAX)
#define S16_BIT				(16)
#define S32_MIN				(INT_MIN)
#define S32_MAX				(INT_MAX)
#define S32_BIT				(32)
#define S64_MIN				(-9223372036854775808)
#define S64_MAX				(9223372036854775807)
#define S64_BIT				(64)

#define U8_MIN				(0)
#define U8_MAX				(255)
#define U8_BIT				(8)
#define U16_MIN				(0)
#define U16_MAX				(65535)
#define U16_BIT				(16)
#define U32_MIN				(0)
#define U32_MAX				(4294967295)
#define U32_BIT				(32)
#define U64_MIN				(0)
#define U64_MAX				(18446744073709551616)
#define U64_BIT				(64)
// ############################################## //


// ################ Microchip XC8 ############### //
#elif defined (__XC8)
// Signed Integer
typedef signed char			S8;
typedef signed int			S16;
typedef signed short long		S24;
typedef signed long			S32;

// Unsigned Integer
typedef unsigned char			U8;
typedef unsigned int			U16;
typedef unsigned short long		U24;
typedef unsigned long			U32;

// Float
typedef float				F32;

//FSM Standard State
typedef enum
{
	unknown = 0,
	init,
	idle,
	busy,
	fetch,
	transfer,
	error
}tFSMState;

//Limits
#define S8_MIN				(SCHAR_MIN)
#define S8_MAX				(SCHAR_MAX)
#define S8_BIT				(8)
#define S16_MIN				(INT_MIN)
#define S16_MAX				(INT_MAX)
#define S16_BIT				(16)
#define S24_MIN				(SHRT_MIN)
#define S24_MAX				(SHRT_MAX)
#define S24_BIT				(24)
#define S32_MIN				(LONG_MIN)
#define S32_MAX				(LONG_MAX)
#define S32_BIT				(32)

#define U8_MIN				(0)
#define U8_MAX				(255)
#define U8_BIT				(8)
#define U16_MIN				(0)
#define U16_MAX				(65535)
#define U16_BIT				(16)
#define U24_MIN				(0)
#define U24_MAX				(16777215)
#define U24_BIT				(24)
#define U32_MIN				(0)
#define U32_MAX				(4294967295)
#define U32_BIT				(32)
// ############################################## //

// ############### Microchip XC32 ############### //
#elif defined (__XC32)

// Signed Integer
typedef char				S8;
typedef short				S16;
typedef int				S32;
typedef long long			S64;

// Unsigned Integer
typedef unsigned char			U8;
typedef unsigned short			U16;
typedef unsigned int			U32;
typedef unsigned long long		U64;

// Float
typedef double				F32;
typedef long double			F64;

//FSM Standard State
typedef enum
{
	unknown = 0,
	init,
	idle,
	busy,
	fetch,
	transfer,
	error
}tFSMState;

//Limits
#define S8_MIN				(SCHAR_MIN)
#define S8_MAX				(SCHAR_MAX)
#define S8_BIT				(8)
#define S16_MIN				(SHRT_MIN)
#define S16_MAX				(SHRT_MAX)
#define S16_BIT				(16)
#define S32_MIN				(INT_MIN)
#define S32_MAX				(INT_MAX)
#define S32_BIT				(32)
#define S64_MIN				(-9223372036854775808)
#define S64_MAX				(9223372036854775807)
#define S64_BIT				(64)

#define U8_MIN				(0)
#define U8_MAX				(255)
#define U8_BIT				(8)
#define U16_MIN				(0)
#define U16_MAX				(65535)
#define U16_BIT				(16)
#define U32_MIN				(0)
#define U32_MAX				(4294967295)
#define U32_BIT				(32)
#define U64_MIN				(0)
#define U64_MAX				(18446744073709551616)
#define U64_BIT				(64)
// ############################################## //

#else
#error This Compiler is not yet compatible!
#endif

#endif
