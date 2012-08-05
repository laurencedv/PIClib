/*!
 @file		pic32_clock.h
 @brief		Clock control lib for pic32

 @version	0.1
 @note		Need the initialOSC and initialPeriod global variable normally found in the hardware.c file of the project
 @todo		- Config reg read function, to check the OSC/PLL configuration and compute the correct running Frequency
			- REFOCLK control function

 @date		February 2th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_CLOCK_H
#define _PIC32_CLOCK_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// System PLL values //
// ----------------- //

// USB PLL values //
// -------------- //
// ############################################## //


// ################# Data Type ################## //
// Clock Source
typedef enum
{
	temporarynameforcompile = 0
}tClockSource;

// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void FSCMISR(void)
* @brief	ISR for the Fail-Safe Clock Monitor
* @note
* @arg		nothing
* @return	nothing
*/
void FSCMISR(void);
// =========================== //


// === Primary Functions ===== //
//Fast RC Oscillator
/**
* \fn		void clockSetFRC(U32 desiredFreq)
* @brief	Set the Fast RC Oscillator to the desired freq
* @note		Rounded to the nearest possible setting
* @arg		U32 desiredFreq			Human-readable freq to set (in Hz)
* @return	nothing
*/
void clockSetFRC(U32 desiredFreq);

/**
* \fn		U32 clockGetFRC(void)
* @brief	Return the actual running freq of the Fast RC Oscillator
* @note		output 0 if the FRC is not running
* @arg		nothing
* @return	U32 runningFreq			Human-readable running freq (in Hz)
*/
U32 clockGetFRC(void);

// Clock Source Selector
/**
* \fn		void clockSetSource(tClockSource clockSource)
* @brief	Set the System clock to the desired clock source
* @note
* @arg		tClockSource desiredclkSource	Clock source to select (use tClockSource for a list)
* @return	nothing
*/
void clockSetSource(tClockSource desiredClkSource);

/**
* \fn		tClockSource clockGetSource(void)
* @brief	Return the actual Clock source feeding the System Clock
* @note
* @arg		nothing
* @return	tClockSource actualClkSource	Actual clock source (use tClockSource for a list)
*/
tClockSource clockGetSource(void);

// Peripheral Bus Clock divider
/**
* \fn		void clockSetPBCLK(tPBCLKDivider clockDivider)
* @brief	Set the Peripheral Bus clock to the desired clock speed
* @note		Will round up to possible value
*			Use clockGetPBCLK() to verify the actual freq
* @arg		U32 desiredClock			Clock freq to set (in Hz)
* @return	nothing
*/
void clockSetPBCLK(U32 desiredClock);

/**
* \fn		tPBCLKDivider clockGetPBCLK(void)
* @brief	Return the actual freq of the Peripheral Bus clock
* @note
* @arg		nothing
* @return	U32 actualPBCLK				Actual PBCLK freq (in Hz)
*/
U32 clockGetPBCLK(void);

// System PLL control
void clockSetPLL(U8 ratio);

U8 clockGetPLL(void);

// USB PLL control
void clockSetUSBPLL(U8 option);

U8 clockGetUSBPLL(void);

// Reference Clock output control
void clockSetREFCLKO(U32 desiredCLK);

U32 clockGetREFCLKO(void);
// ========================== //


// === Secondary Function === //
/**
* \fn		U32 clockSetSYSCLK(U32 desiredFreq)
* @brief	Set the System clock to the desired freq
* @note		Rounded to neareast possible value
*			This is a blocking function as it will wait for the PLL Lock to return
*			Use clockGetSYSCLK() to get the actual  freq
* @arg		U32 desiredFreq				Human-readable desired freq (in Hz)
* @return	nothing
*/
void clockSetSYSCLK(U32 desiredFreq);

/**
* \fn		U32 clockGetSYSCLK(void)
* @brief	Return the actual System clock freq
* @note		Will also update the globalCLK variable
* @arg		nothing
* @return	U32 runningFreq				Human-readable running freq (in Hz)
*/
U32 clockGetSYSCLK(void);
// ========================== //
// ############################################## //

#endif
