/*!
 @file		bitmanip_megaxone.h
 @brief		Bit manipulation macro

 @version	0.1
 @note		This file check for global define identifying each compiler, and load the correct macro in each typedef
 		Pin manipulation require 2 definition ex: LED_B is on RB0	#define LED_B		BIT0
 										#define LED_B_PORT	B

 @date		February 10th 2011
 @author	Laurence DV
*/

#ifndef _BITMANIP_MEGAXONE_H
#define _BITMANIP_MEGAXONE_H 1
// ################## Includes ################## //
#include <definition/stddef_megaxone.h>
// ############################################## //


// ################ Microchip C18 ############### //
#if defined (__18CXX)

// ############################################## //


// ################ Microchip C32 ############### //
#elif defined (__PIC32MX)

//Reg manip
#define setREG(reg)			(CONCAT(reg,SET) = 0xFFFFFFFF)
#define clearREG(reg)			(CONCAT(reg,CLR) = 0xFFFFFFFF)
#define toggleREG(reg)			(CONCAT(reg,INV) = 0xFFFFFFFF)

//Bit manip
#define setBIT(reg,position)		(CONCAT(reg,SET) = (U32)position)
#define clearBIT(reg,position)		(CONCAT(reg,CLR) = (U32)position)
#define toggleBIT(reg,position)		(CONCAT(reg,INV) = (U32)position)
//#define stateBIT(reg,state,position)	(_ins(state,reg,position,1))
#define checkBIT(reg,position)		(reg & ((U32)position))

//Pin manip
#define setPIN(_pin)			(setBIT(CONCAT(LAT,_pin ## _PORT),_pin))
#define clearPIN(_pin)			(clearBIT(CONCAT(LAT,_pin ## _PORT),_pin))
#define togglePIN(_pin)			(toggleBIT(CONCAT(LAT,_pin ## _PORT),_pin))
//#define statePIN(_pin)		(stateBIT(CONCAT(LAT,_pin ## _PORT),_pin))
#define checkPIN(_pin)			(checkBIT(CONCAT(PORT,_pin ## _PORT),_pin))
#define inputPIN(_pin)			(setBIT(CONCAT(TRIS,_pin ## _PORT),_pin))
#define outputPIN(_pin)			(clearBIT(CONCAT(TRIS,_pin ## _PORT),_pin))

//Pin Register check
#define getSetRegPin(_pin)		(CONCAT(CONCAT(LAT,_pin ## _PORT)),SET)
#define getClrRegPin(_pin)		(CONCAT(CONCAT(LAT,_pin ## _PORT)),CLR)
#define getInvRegPin(_pin)		(CONCAT3(LAT,_pin ## _PORT,INV))

// ############################################## //


// ################ Microchip XC8 ############### //
#if defined (__XC8)

// ############################################## //


// ############# Undefined Compiler ############# //
#else
#error This Compiler is not yet compatible!
#endif
// ############################################## //

#endif
