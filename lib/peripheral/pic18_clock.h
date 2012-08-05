/*!
 @file		pic18_realtime.c
 @brief		Real Time keeping for pic18 based on hardware TMR0

 @version	0.1
 @note

 @date		November 22th 2011
 @author	Laurence DV
*/
//TODO: Add result format to the control part of the tADCResult


#ifndef _PIC18_CLOCK_H
#define _PIC18_CLOCK_H

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>

// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################# Data Type ################## //
// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //

// =========================== //


// === Control Functions ===== //
/**
* \fn		void clockUpdateBase(void)
* @brief	Update the base time value with assumed OSC frequency
* @note
* @arg		nothing
* @return	nothing
*/
void clockUpdateBase(void);
// =========================== //


// === Convert Functions ===== //

// =========================== //
// ############################################## //

#endif
