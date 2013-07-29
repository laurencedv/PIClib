/*!
 @file		pic18_realtime.c
 @brief		Real Time keeping for pic18 based on hardware TMR0

 @version	0.1
 @note

 @date		November 22th 2011
 @author	Laurence DV
*/
//TODO: Add result format to the control part of the tADCResult


#ifndef _PIC18_REALTIME_H
#define _PIC18_REALTIME_H

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <time.h>
#include <peripheral/pic18_clock.h>
#include <peripheral/pic18_timer.h>
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define	RT_CENTURY_PREFIX		20

// -- RTCC System Compile Time option -- //
#define RTCC_SYSTEM				RTCC_SOFTWARE

#define RTCC_SOFTWARE			1
#define RTCC_HARDWARE			2
#define RTCC_EXTERNAL			3
// ------------------------------------- //
// ############################################## //


// ################# Data Type ################## //
typedef union
{
	U16 all[9];
	tm time;
}tRTime;

typedef union
{
	U8 all[9];
	struct
	{
		U8 year;
		U8 month;
		U16 day;
		U8 hour;
		U8 min;
		U8 sec;
		U16 millis;
	};
}tRealTime;
// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void realTimeISR(void)
* @brief	Interrupt Service Routine for the realTime system
* @note		Place it in the TMR0 vector
* @arg		nothing
* @return	nothing
*/
void realTimeISR(void);
// =========================== //


// ==== Timing Functions ===== //
/**
* \fn		U8 realTimeInit(U16 tickPeriod)
* @brief	Initialised the Real-Time system to a specified tick period
* @note		Intensive Computation, used only at init time.
* @arg		U16 tickPeriod		Period of a tick (in µs)
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 realTimeInit(U32 tickPeriod);

/**
* \fn		void upTimeUpdate(void)
* @brief	Update the system Up Time
* @note		Need to be called in the realTimeISR
* @arg		nothing
* @return	nothing
*/
void upTimeUpdate(void);

/**
* \fn		void realTimeISR(void)
* @brief	Interrupt Service Routine for the realTime system
* @note		Place it in the TMR0 vector
* @arg		nothing
* @return	tRealTime* pUpTime	Pointer to the upTime global variable
*/
tRealTime* upTimeGet(void);
// =========================== //


// ===== RTCC Functions ====== //
void rtccInit(void);
void rtccUpdate(void);
// =========================== //
// ############################################## //

#endif
		