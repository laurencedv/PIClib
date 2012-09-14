/*!
 @file		pic32_realtime.c
 @brief		Real Time keeping for pic32 based on hardware Timer 1

 @version	0.1.2
 @note		The sysTick Rate is also the upTime Update Rate, keep it below a second to use the realTime system
			accurately.
 *			The software Counters will count AT LEAST the number of systick specified, but can count a bit more (not much)
 @todo		- Make the RTCC System functionnal again

 @date		March 2th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_realtime.h"
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################## Variables ################# //
// == SysTick system == //
U32 sysTick = 0;										//Number of sysTick passed
U32 sysTickValue = 0;									//Value of a sysTick (in us)
// ==================== //

// == General realTime == //
const U8 dayPerMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};	//Number of day per month
tFSMState rtccEngineState = unknown;
U8 rtccEngineSoftCntID = 0;								//ID of the Soft Counter for the rtcc Engine
U8 rtccEngineFlag = 0;									//Rtcc Engine Flag
U8 rtccEngineUpdatePtr = RT_ENGINE_UPDATE_RTCC;			//Tell the engine which time to update
extern U32 globalDump;
// ====================== //

// == UpTime system == //
U32 upTimeLast = 0;										//Last update of the upTime (in systick)
tRealTime upTime;										//Up Time in real time
U16 upTimeRemaininguS = 0;								//Remaining µS after an update (to be added at the next update)
// =================== //

// == RTCC system == //
tRealTime rtccTime;										//Real time

#if RTCC_SYSTEM	== RTCC_SOFTWARE
U32 rtccTimeLast = 0;									//Last update of the rtccTime (in sytick)
U16 rtccRemaininguS = 0;								//Remaining µS after an update (to be added at the next update)
#endif
// ================= //

// == Software Counter == //
#if USE_RT_SOFT_COUNTER == ENABLE
U32 softCnt[RT_SOFT_COUNTER_NB];						//Actual value of the software counter
U32 softCntReloadVal[RT_SOFT_COUNTER_NB];				//Value to reload after the underrun of the counter
U8 * softCntTargetPtr[RT_SOFT_COUNTER_NB];				//Target to modify at the underrun
U32 softCntTargetVal[RT_SOFT_COUNTER_NB];				//Value to input in the target
tSoftCounterControl softCntControl[RT_SOFT_COUNTER_NB];	//Control register of the software counter
U8 softCntEnabled = 0;									//Number of counter enabled
U16 softCntEnable = 0;									//Bitwise soft counter enable control
U16 softCntRun = 0;										//Bitwise soft counter running control
#endif
// ====================== //
// ############################################## //


// ############# Real-time Functions ############ //
// === Interrupt Handler ===== //
/**
* \fn		void realTimeISR(void)
* @brief	Interrupt Service Routine for the realTime system
* @note		Place it in the Timer 1 vector
* @arg		nothing
* @return	nothing
*/
/*	Vector Example
 void __ISR(RT_TIMER_INT_VECTOR, IPL7SOFT)realTimeISR(void)
{
	rtISR();

	intFastClearFlag(RT_TIMER_INT_VECTOR);
}*/

void rtISR(void)
{
	sysTick++;

	// -- Soft Counter -- //
	#if USE_RT_SOFT_COUNTER == ENABLE
	U8 wu0 = 0;

	while ((softCntRun) & (U16_MAX << wu0))
	{
		if (softCnt[wu0])
			softCnt[wu0]--;							//Counter is still decrementing
		else
			softCntControl[wu0].underRun = 1;		//Counter as reach bottom, flag it
		wu0++;
	}
	#endif
	// ------------------ //
}
// =========================== //


// === Control Functions ===== //
/**
* \fn		U8 realTimeInit(U32 tickPeriod)
* @brief	Initialised the Real-Time system to a specified sysTick period
* @note		Intensive Computation, used only at init time.
*			Keep the value between 100 and 64 535 000 to be accurate and useful
*			Use the Hardware Timer 1
*			Use a dividable number by 1000 to assure precision of the software rtcc
* @arg		U32 tickPeriod		Desired period of a sysTick (in µs)
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 realTimeInit(U32 tickPeriod)
{
	// -- Init and set the Timer 1 -- //
	timerInit(RT_TIMER_ID,TMR_CS_PBCLK|TMR_FRZ_STOP);	//Timer 1 is based off PBCLK (and freezed in debug mode)
	timerSetOverflow(RT_TIMER_ID,tickPeriod);			//Set the overflow time to the desired sysTick period
	intFastSetPriority(RT_TIMER_INT_ID,7);				//Set the Timer 1 interrupt to max priority
	intFastSetSubPriority(RT_TIMER_INT_ID,3);
	intFastEnable(RT_TIMER_INT_ID);						//Enable Timer 1 interrupt
	// ------------------------------ //

	sysTickValue = tickPeriod;							//Save the tick period in µs

	// -- Init the soft rtcc if needed -- //
	#if RTCC_SYSTEM == RTCC_SOFTWARE
	rtTimeClear(&rtccTime);
	#endif
	// ---------------------------------- //

	// -- Reset the up-time -- //
	rtTimeClear(&upTime);
	// ----------------------- //
	
	//Start the timer
	timerStart(RT_TIMER_ID);

	return STD_EC_SUCCESS;
}

/**
* \fn		void rtTimeClear(tRealTime * timeToClear)
* @brief	Reset to 0 every part of a tRealTime variable
* @note
* @arg		tRealTime * timeToClear		Pointer to the variable to clear
* @return	nothing
*/
void rtTimeClear(tRealTime * timeToClear)
{
	U8 wu0 = 0;
	U32 * workPtr = (U32*)timeToClear;

	if (timeToClear != NULL)
	{
		for (; wu0 < (sizeof(tRealTime)/4) ; wu0++)
			workPtr[wu0] = 0;
	}
}

/**
* \fn		void rtTimeEngine(void)
* @brief	Engine to keep the rtccTime and upTime accurate and updated
* @note		This function must be in the infinite loop
* @arg		nothing
* @return	nothing
*/
void rtTimeEngine(void)
{
	switch (rtccEngineState)
	{
		// == Initial ======== //
		case unknown:
		{
			rtccEngineFlag = 0;

			// If there was already a softCnt assign to this engine
			if (rtccEngineSoftCntID)
				softCntRelease(rtccEngineSoftCntID);			//Release it

			// Init a soft Counter
			rtccEngineSoftCntID = softCntInit(RTCC_UPDATE_RATE, &rtccEngineFlag, 0xFF, SOFT_CNT_RELOAD_EN+SOFT_CNT_TARGET_EN);
			rtccEngineState = init;
			break;
		}
		// == Start counter == //
		case init:
		{
			softCntStart(rtccEngineSoftCntID);					//Start the counter
			rtccEngineState = busy;
			break;
		}
		// == Action ========= //
		case busy:
		{
			// -- Time to do stuff -- //
			if (rtccEngineFlag)
			{
				switch (rtccEngineUpdatePtr)
				{
					case RT_ENGINE_UPDATE_RTCC:		rtccUpdate();	break;
					case RT_ENGINE_UPDATE_UPTIME:	upTimeUpdate();	break;
				}

				rtccEngineUpdatePtr++;
				if (rtccEngineUpdatePtr > 1)
					rtccEngineUpdatePtr = 0;

				rtccEngineFlag = 0;								//Clear the flag
			}
			// ---------------------- //
			break;
		}
		default: rtccEngineState = unknown;
		// =================== //
	}
}
// =========================== //


// ====== Software Counter ====== //
/**
* \fn		U8 softCntInit(U32 cntPeriod, U32 * targetPtr, U32 targetValue, U8 option)
* @brief	Initialise a software counter
* @note
* @arg		U32 cntPeriod		Number of sysTick of 1 count
* @arg		U32 * targetPtr		Target to be modified at underRun
* @arg		U32 targetValue		Value to bitwise OR the target after a underRun
* @arg		U8 option			Options of the counter (Use the "Init Option" defines)
* @return	U8 softCntID		ID of the initialised counter
*/
U8 softCntInit(U32 cntPeriod, U8 * targetPtr, U32 targetValue, U8 option)
{
	U8 softCntID;

	// -- Check if a counter is available -- //
	if (softCntEnabled < RT_SOFT_COUNTER_NB)
	// ------------------------------------- //
	{
		// -- Set the ID -- //
		softCntID = softCntEnabled;
		softCntEnabled++;
		// ---------------- //

		// -- Set the target -- //
		softCntControl[softCntID].targetEn = (option & SOFT_CNT_TARGET_EN) >> 1;	//Set the target action option
		softCntTargetVal[softCntID] = targetValue;						//Save the value
		if (targetPtr == NULL)											//Handle NULL pointer
			targetPtr = &globalDump;
		softCntTargetPtr[softCntID] = targetPtr;						//Save the target
		// -------------------- //

		// -- Init the counter -- //
		softCntStop(softCntID);											//Ensure that the counter is stopped
		softCntEnable |= (BIT0 << softCntID);							//Enable the counter
		softCntControl[softCntID].reload = option & SOFT_CNT_RELOAD_EN;	//Set the auto reload option
		softCntReloadVal[softCntID] = cntPeriod;						//Save the period
		softCntControl[softCntID].underRun = 0;							//Ensure the underRun flag is cleared
		// ---------------------- //

	}
	else
		softCntID = STD_EC_OVERFLOW;

	return softCntID;
}

/**
* \fn		void softCntRelease(U8 softCntID)
* @brief	Disable a counter
* @note		Will stop the counter now, and will be release when it is possible
* @arg		U8 softCntID		ID of the counter to be released
* @return	nothing
*/
void softCntRelease(U8 softCntID)
{
	// -- Only process if the counter was enabled -- //
	if ((softCntEnable) & (BIT0 << softCntID))
	{
		softCntStop(softCntID);					//Stop the counter
		softCntEnable &= ~(BIT0 << softCntID);	//Disable the counter

		// -- Release the soft counter -- //
		while (!((softCntEnable) & (BIT0 << (softCntEnabled - 1))))		//Scan the bitwise Enable control starting by the last enabled softCnt
			softCntEnabled--;					//Subtract 1 to the Software Counter Enabled counter
		// ------------------------------ //
	}
	// --------------------------------------------- //
}

/**
* \fn		void softCntEngine(void)
* @brief	UnderRun reaction function for software counter
* @note		This function must be in the infinite loop of the main to ensure that the software counter will react correctly
*			The target will only be modify with a "|" (ex: target = 0x10 , value = 0x01 , result = 0x11)
* @arg		nothing
* @return	nothing
*/
void softCntEngine(void)
{
	U8 softCntIDtemp = 0;

	while ((softCntEnable) & (U16_MAX << softCntIDtemp))
	{
		// -- UnderRun condition -- //
		if (softCntControl[softCntIDtemp].underRun)
		{
			// -- Auto reload -- //
			if (softCntControl[softCntIDtemp].reload)
				softCnt[softCntIDtemp] = softCntReloadVal[softCntIDtemp];	//Reload the value
			else
				softCntStop(softCntIDtemp);							//Stop the counter
			// ----------------- //

			// -- Target Action -- //
			if (softCntControl[softCntIDtemp].targetEn)
				*softCntTargetPtr[softCntIDtemp] |= (softCntTargetVal[softCntIDtemp]);
			// ------------------- //

			softCntControl[softCntIDtemp].underRun = 0;				//Clear the underRun flag
		}
		// ------------------------ //

		softCntIDtemp++;											//Check the next counter
	}
}

/**
* \fn		void softCntStart(U8 softCntID)
* @brief	Start a Software Counter
* @note
* @arg		U8 softCntID		ID of the Software Counter
* @return	nothing
*/
void softCntStart(U8 softCntID)
{
	softCnt[softCntID] = softCntReloadVal[softCntID];	//Reload the counter
	softCntRun |= (BIT0 << softCntID);					//Start the counter
}

/**
* \fn		void softCntStop(U8 softCntID)
* @brief	Stop a Software Counter
* @note
* @arg		U8 softCntID		ID of the Software Counter
* @return	nothing
*/
void softCntStop(U8 softCntID)
{
	softCntRun &= ~(BIT0 << softCntID);
}

/**
* \fn		void softCntChangePeriod(U8 softCntID, U32 newPeriod)
* @brief	Update the period value of a Software Counter
* @note		Change will affect the counter on the next period reload (manual of auto)
* @arg		U8 softCntID		ID of the Software Counter
* @arg		U32 netPeriod		New period for the counter (in sysTick)
* @return	nothing
*/
void softCntUpdatePeriod(U8 softCntID, U32 newPeriod)
{
	softCntReloadVal[softCntID] = newPeriod;
}
// ============================== //


// ====== Up-Time Function ====== //
/**
* \fn		void upTimeUpdate(void)
* @brief	Update the system Up Time
* @note		Need to be called in the realTimeISR
* @arg		nothing
* @return	nothing
*/
void upTimeUpdate(void)
{
	U32 uStemp = upTimeRemaininguS + ((sysTick - upTimeLast) * sysTickValue);	//Exact time between update (in µs)

	upTimeLast = sysTick;								//Save the time of the last update

	// -- Compute the millis between update ------ //
	while (uStemp > 999)
	{
		uStemp -= 1000;
		upTime.millis++;
	}
	rtccRemaininguS = uStemp;
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
* \fn		void upTimeSet(tRealTime * newTime)
* @brief	Set the Up-Time to the specified value
* @note		
* @arg		tRealTime * newTime		Pointer to load the time
* @return	nothing
*/
void upTimeSet(tRealTime * newTime)
{
	U8 wu0;
	U32 * workPtr = (U32*)&upTime;

	if (newTime != NULL)
	{
		for (wu0 = 0; wu0 < (sizeof(tRealTime)/4) ; wu0++)
			workPtr[wu0] = ((U32*)newTime)[wu0];
	}
}

/**
* \fn		tRealTime* upTimeGet(void)
* @brief	Return the actual Up Time value
* @note
* @arg		nothing
* @return	tRealTime* pUpTime	Pointer to the upTime global variable
*/
tRealTime* upTimeGet(void)
{
	return &upTime;
}
// ============================== //


// ======== RTCC Function ======= //
/**
* \fn		void rtccUpdate(void)
* @brief	Update the rtcc with the actual time
* @note		This function should be call regularly for the rtccTime to be accurate
* @arg		nothing
* @return	nothing
*/
void rtccUpdate(void)
{
	#if RTCC_SYSTEM	== RTCC_SOFTWARE
	U32 uStemp = rtccRemaininguS + ((sysTick - rtccTimeLast) * sysTickValue);	//Exact time between update (in µs)

	rtccTimeLast = sysTick;							//Save the time of the last update

	// -- Compute the millis between update ------ //
	while (uStemp > 999)
	{
		uStemp -= 1000;
		rtccTime.millis++;
	}
	rtccRemaininguS = uStemp;
	// -- Resplit the millis value in real time -- //
	while (rtccTime.millis > 999)
	{
		rtccTime.millis -= 1000;
		rtccTime.sec++;
		
		//Update the Minutes
		while (rtccTime.sec > 59)
		{
			rtccTime.sec -= 60;
			rtccTime.min++;

			//Update the Hours
			while (rtccTime.min > 59)
			{
				rtccTime.min -= 60;
				rtccTime.hour++;

				//Update the Days
				while (rtccTime.hour > 23)
				{
					rtccTime.hour -= 24;
					rtccTime.day++;

					//Update the Months
					while (rtccTime.day >= dayPerMonth[rtccTime.month])
					{
						rtccTime.day -= dayPerMonth[rtccTime.month];
						rtccTime.month++;

						//Update the Years
						while (rtccTime.month > 11)
						{
							rtccTime.month -= 12;
							rtccTime.year++;
						}
					}
				}
			}
		}
	}
	// ------------------------------------------- //
	#elif RTCC_SYSTEM == RTCC_HARDWARE

	#elif RTCC_SYSTEM == RTCC_EXTERNAL

	#endif
}
// =========================== //
// ############################################## //
