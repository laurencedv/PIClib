/*!
 @file 		pic18_timer.c
 @brief		Timer functions for C18 and XC8
 
 @version 	0.2
 @note		This lib does not use gated feature of some timer
 
 @date 		November 19th 2012
 @author 	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_timer.h"
// ############################################## //


// ################## Defines ################### //
#define TMR0_CKPS_NB	8
#define TMR8_CKPS_NB	3
#define TMR16_CKPS_NB	4
// ############################################## //


// ################## Variables ################# //
extern U32 globalCLK;

// Reg Pointer //
#if CPU_FAMILY == PIC18Fx7Jx3
const void * volatile const timerRegAddress[8] = {&T0CON,&T1CON,&T2CON,&T3CON,&T4CON,&T5CON,&T6CON,&T8CON};
#elif CPU_FAMILY == PIC18FxxK22
const void * volatile const timerRegAddress[7] = {&T0CON,&T1CON,&T2CON,&T3CON,&T4CON,&T5CON,&T6CON};
#else
const void * volatile const timerRegAddress[5] = {&T0CON,&T1CON,&T2CON,&T3CON,&T4CON};
#endif
// ----------- //

// Prescaler divider //
const U16 tmr0PrescalerValue[TMR0_CKPS_NB] = {2,4,8,16,32,64,128,256};		//TO REVISE!!!!
const U8 tmr8PrescalerValue[TMR8_CKPS_NB] = {0,2,4};				//TO REVISE!!!!
const U8 tmr16PrescalerValue[TMR16_CKPS_NB] = {0,1,2,3};
// ----------------- //
// ############################################## //


// ############### Timer Functions ############## //
// ==== Control ==== //
/**
* \fn		U8 timerInit(U8 timerID, U8 option)
* @brief	Initialise the selected timer with the specified options.
* @note		Use the Setting Map in the header for correct setting
*		WARNING: not all setting are valid for any timer!
*		No sanity check of the settings, will return STD_EC_NOTFOUND if invalid timer ID is inputed.
*		Option must be | or + (ex: timerInit(TIMER_0, TMR_DIV_1|TMR_CS_INST|TMR_RISING|TMR_16BIT))
* @arg		U8 timerID		Hardware Timer ID
* @arg		U8 option		Setting to configure for the timer
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerInit(U8 timerID, U8 option)
{
	timerStop(timerID);

	// -- Timer 0 ------ //
	if (timerID == TIMER_0)
	{
		volatile tTimer0Reg * workPtr = timerRegAddress[0];

		// -- Set the prescaler -- //
		if (option & 0x80)
		{
			workPtr->TxCON.PSA = DISABLE;			//Enable the prescaler
			workPtr->TxCON.TxPS = option & 0x7;		//Set the choosen prescaler
		}
		else
			workPtr->TxCON.PSA = ENABLE;			//Disable the prescaler
		// ----------------------- //

		// -- Set the clock -- //
		workPtr->TxCON.TxCS = (option & 0x20) >> 5;		//Set the Clock Source
		workPtr->TxCON.TxSE = (option & 0x08) >> 3;		//Set the Clock Edge
		// ------------------- //

		// -- Set the options -- //
		workPtr->TxCON.Tx8BIT = (option & 0x40) >> 6;		//Set the Timer Size
		// --------------------- //
	}
	// -- Timer 16bit -- //
	else if (timerID & 0x1)
	{
		volatile tTimer16Reg * workPtr = timerRegAddress[timerID];

		// -- Set the prescaler -- //
		if (option & 0x80)					//Check the prescaler flag
			workPtr->TxCON.TxCKPS = (option & 0x3)+1;	//Set the prescaler
		else
			workPtr->TxCON.TxCKPS = 0;			//Set to DIV by 1
		// ----------------------- //

		// -- Set the clock -- //
		workPtr->TxCON.TMRxCS = (option & 0x30) >> 4;
		// ------------------- //

		// -- Set the option -- //
		workPtr->TxCON.TxOSCEN = (option & 0x4) >> 2;		//En/Dis the OSC driver
		workPtr->TxCON.TxSYNC = (option & 0x8) >> 3;		//Set the External Sync
		workPtr->TxCON.RD16 = (option & 0x40) >> 6;		//Set the Timer Read/Write size
		// -------------------- //
	}
	// -- Timer 8bit --- //
	else
	{
		volatile tTimer8Reg * workPtr = timerRegAddress[timerID];

		// -- Set the prescaler -- //
		if (option & 0x80)					//Check the prescaler flag
			workPtr->TxCON.TxCKPS = option & 0x3;
		else
			workPtr->TxCON.TxCKPS = 0;			//Set to DIV by 1
		// ----------------------- //

		// -- Set the postscaler -- //
		workPtr->TxCON.TxOUTPS = (option & 0x78) >> 3;
		// ------------------------ //
	}
	// ----------------- //

	//Reset the timer
	timerClear(timerID);

	return STD_EC_SUCCESS;
}

/**
* \fn		void timerSetOverflow(U8 timerID, U16 ovfPeriod)
* @brief	Set the timer overflow interrupt at the desired period
* @note		This function will round up to the possible freq depending on the actual globalCLK.
*		Only work with internal clock source.
*		Use the timerInit with the correct parameters for external clock source.
* @arg		U8 timerID			Hardware Timer ID
* @arg		U16 ovfPeriod			Total Period between overflow (in us)
* @return	nothing
*/
void timerSetOverflow(U8 timerID, U16 ovfPeriod)
{
	if (timerID <= TIMER_MAX_ID)
	{
		// -- Determine the type of the timer -- //
		if (timerID == 0)
		{
			volatile tTimer0Reg * workTimer = timerRegAddress[0];		//Timer 0
		}
		else if (timerID & 0x01)
		{
			volatile tTimer16Reg * workTimer = timerRegAddress[timerID];	//16bit Timers
		}
		else
		{
			volatile tTimer8Reg * workTimer = timerRegAddress[timerID];	//8bit Timers
		}
		// ------------------------------------- //

		
	}
}

/**
* \fn		U16 timerGetOverflow(U8 timerID)
* @brief	Return the overflow period of the specified timer
* @note		Will return 0 if either the timerID is invalid or the clock source
*		is external
* @arg		U8 timerID			Hardware Timer ID
* @return	U16 ovfPeriod			Overflow period (in µs)
*/
U16 timerGetOverflow(U8 timerID)
{
	U16 timerOvf = 0;
	
	if (timerID <= TIMER_MAX_ID)
	{
		// -- Determine the type of the timer -- //
		if (timerID == 0)
		{
			volatile tTimer0Reg * workTimer = timerRegAddress[0];		//Timer 0
		}
		else if (timerID & 0x01)
		{
			volatile tTimer16Reg * workTimer = timerRegAddress[timerID];	//16bit Timers
		}
		else
		{
			volatile tTimer8Reg * workTimer = timerRegAddress[timerID];	//8bit Timers
		}
		// ------------------------------------- //
		
		
	}
	
	return timerOvf;
}

/**
* \fn		U16 timerGetTickPeriod(U8 timerID)
* @brief	Return the tick period of the specified timer
* @note		Will return 0 if either the timerID is invalid or the clock source
*		is external
* @arg		U8 timerID			Hardware Timer ID
* @return	U16 tickPeriod			Timer's tick period (in ns)
*/
U16 timerGetTickPeriod(U8 timerID)
{
	U16 tickPeriod = 0;

	if (timerID < TIMER_MAX_ID)
	{
		// -- Timer 0 ------ //
		if (timerID == 0)
		{
			volatile tTimer0Reg * workTimer = timerRegAddress[0];
			
			if (!workTimer->TxCON.TxCS)
			{
				if (workTimer->TxCON.PSA)
					tickPeriod = (U16)((U32)250000000 / (globalCLK >> (workTimer->TxCON.TxPS+1)));
				else
					tickPeriod = (U16)((U32)250000000 / globalCLK);
			}
		}
		// -- Timer 16bit -- //
		else if (timerID & 0x01)
		{
			volatile tTimer16Reg * workTimer = timerRegAddress[timerID];	

			if (workTimer->TxCON.TMRxCS == 1)				//Clock source is FOSC
				tickPeriod = (U16)((U32)1000000000 / (globalCLK >> workTimer->TxCON.TxCKPS));
			else if (workTimer->TxCON.TMRxCS == 0)				//Clock source is FOSC/4
				tickPeriod = (U16)((U32)250000000 / (globalCLK >> workTimer->TxCON.TxCKPS));
		}
		// -- Timer 8bit --- //
		else
		{
			volatile tTimer8Reg * workTimer = timerRegAddress[timerID];

			tickPeriod = (U16)((U32)1000000000 /(globalCLK >> (workTimer->TxCON.TxCKPS << 1)));
		}
		// ----------------- //
	}

	return tickPeriod;
}

/**
* \fn		void timerStart(U8 TimerID)
* @brief	Start the selected Timer
* @note		nothing
* @arg		U8 timerID			Hardware Timer ID
* @return	nothing
*/
void timerStart(U8 timerID)
{	
	switch (timerID)
	{
		case TIMER_0:	timerFastStart(TIMER_0);	break;
		case TIMER_1:	timerFastStart(TIMER_1);	break;
		case TIMER_2:	timerFastStart(TIMER_2);	break;
		case TIMER_3:	timerFastStart(TIMER_3);	break;
		case TIMER_4:	timerFastStart(TIMER_4);	break;
		#if (CPU_FAMILY == PIC18Fx7Jx3) || (CPU_FAMILY == PIC18FxxK22)
		case TIMER_5:	timerFastStart(TIMER_5);	break;
		case TIMER_6:	timerFastStart(TIMER_6);	break;
		#endif
		#if CPU_FAMILY == PIC18Fx7Jx3
		case TIMER_8:	timerFastStart(TIMER_8);	break;
		#endif
	}
}

/**
* \fn		void timerStop(U8 TimerID)
* @brief	Stop the selected Timer
* @note		nothing
* @arg		U8 timerID			Hardware Timer ID
* @return	nothing
*/
void timerStop(U8 timerID)
{
	switch (timerID)
	{
		case TIMER_0:	timerFastStop(TIMER_0);		break;
		case TIMER_1:	timerFastStop(TIMER_1);		break;
		case TIMER_2:	timerFastStop(TIMER_2);		break;
		case TIMER_3:	timerFastStop(TIMER_3);		break;
		case TIMER_4:	timerFastStop(TIMER_4);		break;
		#if (CPU_FAMILY == PIC18Fx7Jx3) || (CPU_FAMILY == PIC18FxxK22)
		case TIMER_5:	timerFastStop(TIMER_5);		break;
		case TIMER_6:	timerFastStop(TIMER_6);		break;
		#endif
		#if CPU_FAMILY == PIC18Fx7Jx3
		case TIMER_8:	timerFastStop(TIMER_8);		break;
		#endif
	}
}
// ================= //


// ==== Data Handling ==== //
/**
* \fn		void timerSet(U8 timerID, U8/U16 data)
* @brief	Set the selected Timer to the specified value
* @note		Depend on wich timer is selected, the data width will vary between 8bit and 16bit
			Timer 0 is assume as a 16bit, if 8bit mode is selected use TMR0L directly
* @arg		U8 timerID			Hardware Timer ID
* @arg		U8/U16 data			Data to input in the timer
* @return	nothing
*/
void timerSet(U8 timerID, U16 data)
{
		switch(timerID)
		{
			case TIMER_0:	timerFastSet(TIMER_0,data);	break;
			case TIMER_1:	timerFastSet(TIMER_1,data);	break;
			case TIMER_2:	timerFastSet(TIMER_2,data);	break;
			case TIMER_3:	timerFastSet(TIMER_3,data);	break;
			case TIMER_4:	timerFastSet(TIMER_4,data);	break;
			#if (CPU_FAMILY == PIC18Fx7Jx3) || (CPU_FAMILY == PIC18FxxK22)
			case TIMER_5:	timerFastSet(TIMER_5,data);	break;
			case TIMER_6:	timerFastSet(TIMER_6,data);	break;
			#endif
			#if CPU_FAMILY == PIC18Fx7Jx3
			case TIMER_8:	timerFastSet(TIMER_8,data);	break;
			#endif
		}
}

/**
* \fn		U8/U16 timerGet(U8 TimerID)
* @brief	Return the value of the selected Timer
* @note		Depend on wich timer is selected, the return size will be 8bit or 16bit
			Timer 0 is assume as a 16bit, if 8bit mode is selected use TMR0L directly
* @arg		U8 timerID			Hardware Timer ID
* @return	U8/U16				Actual value of the timer
*/
U16 timerGet(U8 timerID)
{
	switch(timerID)
	{
		case TIMER_0:	return timerFastGet(TIMER_0);
		case TIMER_1:	return timerFastGet(TIMER_1);
		case TIMER_2:	return timerFastGet(TIMER_2);
		case TIMER_3:	return timerFastGet(TIMER_3);
		case TIMER_4:	return timerFastGet(TIMER_4);
		#if (CPU_FAMILY == PIC18Fx7Jx3) || (CPU_FAMILY == PIC18FxxK22)
		case TIMER_5:	return timerFastGet(TIMER_5);
		case TIMER_6:	return timerFastGet(TIMER_6);
		#endif
		#if CPU_FAMILY == PIC18Fx7Jx3
		case TIMER_8:	return timerFastGet(TIMER_8);
		#endif
		default:	return 0xFFFF;
	}
}

/**
* \fn		void timerSetPR(U8 timerID, U8 data)
* @brief	Set the period register of the selected Timer
* @note		Will only work with timer that have a PRx Register (timer2,4,6 and 8)
* @arg		U8 timerID			Hardware Timer ID
* @arg		U8 data				Data to set the PRx reg
* @return	nothing
*/
void timerSetPR(U8 timerID, U8 data)
{
	if (timerID < TIMER_MAX_ID)
	{
		if (timerID && ((timerID & 0x1) == 0))				//Only 8bit wide Timers have a PR reg
			((tTimer8Reg *)timerRegAddress[timerID])->PRx = data;
	}
}

/**
* \fn		U8 timerGetPR(U8 timerID)
* @brief	Return the period register of the selected Timer
* @note		Will only work with timer that have a PRx Register (timer2,4,6 and 8)
* @arg		U8 timerID			Hardware Timer ID
* @return	U8 data				Value of the PRx reg
*/
U8 timerGetPR(U8 timerID)
{
	if (timerID < TIMER_MAX_ID)
	{
		if (timerID && ((timerID & 0x1) == 0))				//Only 8bit wide Timers have a PR reg
		{
			return ((tTimer8Reg *)timerRegAddress[timerID])->PRx;
		}
	}
	return 0;
}
// ======================= //
// ############################################## //
