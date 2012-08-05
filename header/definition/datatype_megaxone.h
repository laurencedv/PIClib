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


// ################ Microchip C18 ############### //
#if defined (__18CXX)
// Signed Integer
typedef char				S8;
typedef int					S16;
typedef short long			S24;
typedef long				S32;

// Unsigned Integer
typedef unsigned char		U8;
typedef unsigned int		U16;
typedef unsigned short long	U24;
typedef unsigned long		U32;

// Float
typedef float				F32;
// ############################################## //


// ################ Microchip C32 ############### //
#elif defined (__PIC32MX)

// Signed Integer
typedef char				S8;
typedef short				S16;
typedef int					S32;
typedef long long			S64;

// Unsigned Integer
typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;
typedef unsigned long long	U64;

// Float
typedef double				F32;
typedef long double			F64;
// ############################################## //
#else
#error This Compiler is not yet compatible!
#endif

#endif
