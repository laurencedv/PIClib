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

// == Timer control == //
#define RT_TIMER_ID				TIMER_1
#define RT_TIMER_INT_ID			INT_TIMER_1
#define RT_TIMER_VECTOR			INT_VEC_TIMER_1
// =================== //

// == Software Counter == //
#define USE_RT_SOFT_COUNTER		ENABLE				//Enable the software counter functions by setting to "ENABLE"
#define RT_SOFT_COUNTER_NB		10					//Number of software counter to create (max 10)

// Init options
#define SOFT_CNT_RELOAD_EN		0x1					//Enable the auto reload of the counter (timer mode)
#define SOFT_CNT_RELOAD_DIS		0x0					//Disable the auto reload of the counter
#define SOFT_CNT_TARGET_EN		0x2					//Enable the target action at underRun
#define SOFT_CNT_TARGET_DIS		0x0					//Disable the target action at underRun
// ====================== //

// == RTCC System Compile Time option == //
#define RTCC_UPDATE_RATE		1000				//Update rate of the software RTCC (in sysTick)
#define RTCC_SYSTEM				RTCC_HARDWARE

#define RTCC_SOFTWARE			1
#define RTCC_HARDWARE			2
#define RTCC_EXTERNAL			3
// ===================================== //
// ############################################## //


// ################# Data Type ################## //
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

typedef union
{
	U32 all;
	struct
	{
		U32 enable:1;				//The counter is currently enabled
		U32 underRun:1;				//The counter as underRun
		U32 targetEn:1;				//The counter will modify the softCntTargetPtr with the softCntTargetVal
		U32 reload:1;				//The counter will auto reload it-self at underRun (timer mode)
		U32 :28;
	};
}tSoftCounterControl;
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
void rtISR(void);
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


// ====== Software Counter ====== //
/**
* \fn		U8 softCntInit(U32 cntPeriod, U32 * targetPtr, U32 targetValue, U8 option)
* @brief	Initialise a software counter
* @note
* @arg		U32 cntPeriod		Number of sysTick of 1 count
* @arg		U32 * targetPtr		Target to be modified at underRun
* @arg		U32 targetValue		Value to set the target after a underRun
* @arg		U8 option			Options of the counter
* @return	U8 softCntID		ID of the initialised counter
*/
U8 softCntInit(U32 cntPeriod, U32 * targetPtr, U32 targetValue, U8 option);

/**
* \fn		void softCntEngine(void)
* @brief	UnderRun reaction function for software counter
* @note		This function must be in the infinite loop of the main to ensure that the software counter will react correctly
* @arg		nothing
* @return	nothing
*/
void softCntEngine(void);

/**
* \fn		void softCntStart(U8 softCntID)
* @brief	Start a Software Counter
* @note
* @arg		U8 softCntID		ID of the Software Counter
* @return	nothing
*/
void softCntStart(U8 softCntID);

/**
* \fn		void softCntStop(U8 softCntID)
* @brief	Stop a Software Counter
* @note
* @arg		U8 softCntID		ID of the Software Counter
* @return	nothing
*/
void softCntStop(U8 softCntID);

/**
* \fn		void softCntChangePeriod(U8 softCntID, U32 newPeriod)
* @brief	Update the period value of a Software Counter
* @note		Change will affect the counter on the next period reload (manual of auto)
* @arg		U8 softCntID		ID of the Software Counter
* @arg		U32 netPeriod		New period for the counter (in sysTick)
* @return	nothing
*/
void softCntUpdatePeriod(U8 softCntID, U32 newPeriod);
// ============================== //


// ===== RTCC Functions ====== //
void rtccInit(void);
void rtccUpdate(void);
// =========================== //
// ############################################## //

#endif
		