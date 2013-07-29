/*!
 @file		pic18_realtime.c
 @brief		Real Time keeping for pic18 based on hardware TMR0

 @version	0.1
 @note		The sysTick Rate is also the upTime Update Rate, keep it below a second to use the realTime system
			accuretly.

 @date		November 22th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_realtime.h"
// ############################################## //


// ################## Defines ################### //
#define	TMR0_PRESCALER_NB	9
// ############################################## //


// ################## Variables ################# //
extern U32 globalCLK;
extern U16 globalCLKps;

// Timer control
//const U8 rtTimerPrescaler[TMR0_PRESCALER_NB] = {1,2,4,8,16,32,64,128,256};
//U16 rtTimerInitValue = 0;
S8 tickOffset = -24;							//Offset of 1 sysTick (in µs)

U32 sysTick = 0;

U16 sysTickValue;							//Value of a sysTick (in ms)
tRTime upTime;								//Up Time of the system
tRTime localTime;							//This is the real actual local date and time

#if (RTCC_SYSTEM == RTCC_SOFTWARE) || (RTCC_SYSTEM == RTCC_EXTERNAL)
	U16 rtccTickValue;						//number of sysTick for 1 sec (call rtccUpdate)
	U32 rtccTickNext;						//Next update of the rtcc (in sysTick)
	U8 rtccCurrentMonthDayNb = 30;					//Number of day in the Current Month
#endif
// ############################################## //


// ################ ADC Functions ############### //
// === Interrupt Handler ===== //
/**
* \fn		void realTimeISR(void)
* @brief	Interrupt Service Routine for the realTime system
* @note		Place it in the TMR0 vector
* @arg		nothing
* @return	nothing
*/
void realTimeISR(void)
{
	timerISR(TIMER_0);
	sysTick++;

	//upTimeUpdate();

	#if RTCC_SYSTEM	== RTCC_SOFTWARE
	if (sysTick == rtccTickNext)
	{
		rtccUpdate();
		rtccTickNext = sysTick+rtccTickValue;
	}
	#endif

	int_tmr0_clr();
}

// =========================== //


// === Timing Functions ===== //
/**
* \fn		U8 realTimeInit(U32 tickPeriod)
* @brief	Initialised the Real-Time system to a specified tick period
* @note		Intensive Computation, used only at init time.
*		Keep the value between 100 and 64 535 000 to be accurate and useful
*		Use the Timer 0
*		Use a dividable number by 1000 to assure precision of the software rtcc
* @arg		U32 tickPeriod		Period of a tick (in µs)
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 realTimeInit(U32 tickPeriod)
{
	U8 wu0;

	// -- Save the value of a sysTick in ms -- //
	sysTickValue = tickPeriod/1000;					//Should not be more than 64 535
	// --------------------------------------- //

	// -- Init the soft rtcc if needed -- //
	#if RTCC_SYSTEM == RTCC_SOFTWARE
	rtccInit();
	rtccTickValue = 1000/sysTickValue;
	#endif
	// ---------------------------------- //

	// -- Init the timer -- //
	timerInit(TIMER_0,0);
	timerSetOverflow(TIMER_0,tickPeriod/1000)
	timerStart(TIMER_0);
	// -------------------- //


	// -- Reset the Up Time -- //
	for (wu0 = 0; wu0 < 9 ; wu0++)
		upTime.all[wu0] = 0;
	// ----------------------- //

	// -- Set the timer -- //
	
	// ------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		void upTimeUpdate(void)
* @brief	Update the system Up Time
* @note		Need to be called in the realTimeISR
* @arg		nothing
* @return	nothing
*/
void upTimeUpdate(void)
{
	//Add a Tick (in ms) to the global millis count
	upTime.millis += sysTickValue;

	// -- Resplit the millis value in real time -- //
	while (upTime.millis > 999)
	{
		upTime.millis -= 1000;
		upTime.sec++;

		//Update the Minutes
		while (upTime.sec > 59)
		{
			upTime.sec -= 60;
			upTime.min++;

			//Update the Hours
			while (upTime.min > 59)
			{
				upTime.min -= 60;
				upTime.hour++;

				//Update the Days
				while (upTime.hour > 23)
				{
					upTime.hour -= 24;
					upTime.day++;
				}
			}
		}
	}
	// ------------------------------------------- //
}

/**
* \fn		void realTimeISR(void)
* @brief	Interrupt Service Routine for the realTime system
* @note		Place it in the TMR0 vector
* @arg		nothing
* @return	tRealTime* pUpTime	Pointer to the upTime global variable
*/
tRealTime* upTimeGet(void)
{
	return &upTime;
}
// =========================== //

// ====== RTCC Function ====== //
/**
* \fn		void rtccInit(void)
* @brief	Init the software rtcc for software and external mode
* @note		realTimeInit will define the correct sysTick value for 1 sec
*			and rtccUpdate will be called in the realTimeISR
* @arg		nothing
* @return	nothing
*/
void rtccInit(void)
{
	U8 wu0;

	// -- Reset the realTime variable -- //
	for (wu0 = 0; wu0<9 ;wu0++)
		realTime.all[wu0] = 0;
	// --------------------------------- //
}

void rtccUpdate(void)
{
	#if RTCC_SYSTEM	== RTCC_SOFTWARE
	//Count the new second
	upTime.sec++;

	//Update the Minutes
	if (upTime.sec > 59)
	{
		upTime.sec -= 60;
		upTime.min++;

		//Update the Hours
		if (upTime.min > 59)
		{
			upTime.min -= 60;
			upTime.hour++;

			//Update the Days
			if (upTime.hour > 23)
			{
				upTime.hour -= 24;
				upTime.day++;

				//Update the month
				if (upTime.day > (rtccCurrentMonthDayNb-1))
				{
					upTime.day -= rtccCurrentMonthDayNb;

					//Update the current month day number

					upTime.month++;

					if (upTime.month > 11)
					{
						upTime.month -= 12;
						upTime.year++;
					}
				}
			}
		}
	}
	#elif RTCC_SYSTEM == RTCC_HARDWARE

	#elif RTCC_SYSTEM == RTCC_EXTERNAL

	#endif
}
// =========================== //
// ############################################## //
