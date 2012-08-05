/*!
 @file		pic32_realtime.h
 @brief		Real Time keeping for pic32 based on hardware Timer 1

 @version	0.1.2
 @note		The sysTick Rate is also the upTime Update Rate, keep it below a second to use the realTime system
			accurately.

 @date		March 2th 2012
 @author	Laurence DV
*/



#ifndef _PIC32_REALTIME_H
#define _PIC32_REALTIME_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic32_timer.h>
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define	RT_CENTURY_PREFIX		20

// -- Timer control -- //
#define RT_TIMER_ID				TIMER_1
#define RT_TIMER_INT_ID			INT_TIMER_1
#define RT_TIMER_VECTOR			INT_VEC_TIMER_1
// ------------------- //

// -- RTCC System Compile Time option -- //
#define RTCC_UPDATE_RATE		1000				//Update rate of the software RTCC (in sysTick)
#define RTCC_SYSTEM				RTCC_HARDWARE

#define RTCC_SOFTWARE			1
#define RTCC_HARDWARE			2
#define RTCC_EXTERNAL			3
// ------------------------------------- //
// ############################################## //


// ################# Data Type ################## //
typedef union __attribute__ ((packed))
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
* @note		Place it in the Timer 1 vector
* @arg		nothing
* @return	nothing
*/
//void realTimeISR(void);
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
		