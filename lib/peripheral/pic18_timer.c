/*!
 @file pic18_timer.c
 @brief		Timer functions for C18
 *              
 @version 0.1
 @date April 2th 2011
 @author Laurence DV
*/

// ################## Includes ################## //
#include "pic18_timer.h"
// ############################################## //


// ################## Defines ################### //
// ############################################## //


// ################## Variables ################# //
// Reg Pointer //
tToddCON * pToddCON;
tTevenCON * pTevenCON;
tTGCON * pTGCON;
// ----------- //
// ############################################## //


// ############### Timer Functions ############## //
// ==== Control ==== //
/**
* \fn		U8 timerInit(U8 timerID, U8 option)
* @brief	Initialise the selected timer with the specified options.
* @note		Use the Setting Map in the header for correct setting to send to this function
*			No sanity check of the settings, will return STD_EC_NOTFOUND if invalid timer ID is inputed.
*			Option must be | or + (ex: timerInit(0, TMR_DIV_1|TMR_CS_INST|TMR_RISING|TMR_16BIT))
* @arg		U8 timerID			Hardware Timer ID
* @arg		U8 option			Setting to configure for the timer
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerInit(U8 timerID, U8 option)
{
	// -- Select the correct Timer -- //
	switch (timerID)
	{
		case 0:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
			{
				T0CONbits.PSA = CLEAR;				//Enable the Prescaler
				T0CONbits.T0PS = option & 0x7;
			}
			else
				T0CONbits.PSA = SET;				//Disable the Prescaler (DIV by 1)
			// ----------------------- //

			// -- Set the clock -- //
			T0CONbits.T0CS = option & 0x20;			//Set the Clock Source
			T0CONbits.T0SE = option & 0x08;			//Set the Clock Edge
			// ------------------- //

			// -- Set the options -- //
			T0CONbits.T08BIT = option & 0x40;		//Set the Timer Size
			// --------------------- //

			break;
		}
		case 1:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T1CONbits.T1CKPS = (option & 0x3)+1;
			else
				T1CONbits.T1CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the clock -- //
			T1CONbits.TMR1CS = option & 0x30;
			// ------------------- //

			// -- Set the option -- //
			#if CPU_FAMILY == PIC18FxxJ53
				T1CONbits.T1OSCEN = option & 0x4;	//En/Dis the OSC driver
			#elif CPU_FAMILY == PIC18FxxK22
				T1CONbits.T1SOSCEN = option & 0x4;
			#elif CPU_FAMILY == PIC18FxxK80
				T1CONbits.SOSCEN = option & 0x4;
			#endif
			T1CONbits.NOT_T1SYNC = option & 0x8;	//Set the External Sync
			T1CONbits.RD16 = option & 0x40;			//Set the Timer Read/Write size
			// -------------------- //

			break;
		}
		case 2:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T2CONbits.T2CKPS = option & 0x3;
			else
				T2CONbits.T2CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the postscaler -- //
			T2CONbits.T2OUTPS = option &0x78;
			// ------------------------ //
			break;
		}
		case 3:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T3CONbits.T3CKPS = (option & 0x3)+1;
			else
				T3CONbits.T3CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the clock -- //
			T3CONbits.TMR3CS = option & 0x30;
			// ------------------- //

			// -- Set the option -- //
			#if CPU_FAMILY == PIC18FxxJ53
				T3CONbits.T3OSCEN = option & 0x4;	//En/Dis the OSC driver
			#elif CPU_FAMILY == PIC18FxxK22
				T3CONbits.T3SOSCEN = option & 0x4;
			#elif CPU_FAMILY == PIC18FxxK80
				T3CONbits.SOSCEN = option & 0x4;
			#endif
			T3CONbits.NOT_T3SYNC = option & 0x8;	//Set the External Sync
			T3CONbits.RD16 = option & 0x40;			//Set the Timer Read/Write size
			// -------------------- //
			break;
		}
		case 4:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T4CONbits.T4CKPS = option & 0x3;
			else
				T4CONbits.T4CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the postscaler -- //
			T4CONbits.T4OUTPS = option &0x78;
			// ------------------------ //
			break;
		}
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T5CONbits.T5CKPS = (option & 0x3)+1;
			else
				T5CONbits.T5CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the clock -- //
			T5CONbits.TMR5CS = option & 0x30;
			// ------------------- //

			// -- Set the option -- //
			#if CPU_FAMILY == PIC18FxxJ53
				T5CONbits.T5OSCEN = option & 0x4;	//En/Dis the OSC driver
			#elif CPU_FAMILY == PIC18FxxK22
				T5CONbits.T5SOSCEN = option & 0x4;
			#endif
			T5CONbits.NOT_T5SYNC = option & 0x8;	//Set the External Sync
			T5CONbits.RD16 = option & 0x40;			//Set the Timer Read/Write size
			// -------------------- //
			break;
		}
		case 6:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T6CONbits.T6CKPS = option & 0x3;
			else
				T6CONbits.T6CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the postscaler -- //
			T6CONbits.T6OUTPS = option &0x78;
			// ------------------------ //
			break;
		}
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:
		{
			// -- Set the prescaler -- //
			if (option & 0x80)						//Check the prescaler flag
				T8CONbits.T8CKPS = option & 0x3;
			else
				T8CONbits.T8CKPS = 0;				//Set to DIV by 1
			// ----------------------- //

			// -- Set the postscaler -- //
			T8CONbits.T8OUTPS = option &0x78;
			// ------------------------ //
			break;
		}
		#endif
		default:	return STD_EC_NOTFOUND;		//Invalid Timer id return error
	}
	// ------------------------------ //

	//Reset the timer
	timerClear(timerID);

	return STD_EC_SUCCESS;
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
		case 0:	timerFastStart(0);	break;
		case 1:	timerFastStart(1);	break;
		case 2:	timerFastStart(2);	break;
		case 3:	timerFastStart(3);	break;
		case 4:	timerFastStart(4);	break;
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:	timerFastStart(5);	break;
		case 6:	timerFastStart(6);	break;
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:	timerFastStart(8);	break;
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
		case 0:	timerFastStop(0);	break;
		case 1:	timerFastStop(1);	break;
		case 2:	timerFastStop(2);	break;
		case 3:	timerFastStop(3);	break;
		case 4:	timerFastStop(4);	break;
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:	timerFastStop(5);	break;
		case 6:	timerFastStop(6);	break;
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:	timerFastStop(8);	break;
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
		case 0: timerFastSet(0,data);	break;
		case 1:	timerFastSet(1,data);	break;
		case 2: timerFastSet(2,data);	break;
		case 3:	timerFastSet(3,data);	break;
		case 4:	timerFastSet(4,data);	break;
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:	timerFastSet(5,data);	break;
		case 6:	timerFastSet(6,data);	break;
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:	timerFastSet(8,data);	break;
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
		case 0: return timerFastGet(0);
		case 1:	return timerFastGet(1);
		case 2: return timerFastGet(2);
		case 3:	return timerFastGet(3);
		case 4:	return timerFastGet(4);
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:	return timerFastGet(5);
		case 6:	return timerFastGet(6);
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:	return timerFastGet(8);
		#endif
	}
}

/**
* \fn		void timerClear(U8 TimerID)
* @brief	Reset to 0 the selected Timer
* @note		nothing
* @arg		U8 timerID			Hardware Timer ID
* @return	nothing
*/
void timerClear(U8 timerID)
{
	switch (timerID)
	{
		case 0:	timerFastClear(0);	break;
		case 1:	timerFastClear(1);	break;
		case 2:	timerFastClear(2);	break;
		case 3:	timerFastClear(3);	break;
		case 4:	timerFastClear(4);	break;
		#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
		case 5:	timerFastClear(5);	break;
		case 6:	timerFastClear(6);	break;
		#endif
		#if CPU_FAMILY == PIC18FxxJ53
		case 8:	timerFastClear(8);	break;
		#endif
	}
}
// ======================= //
// ############################################## //
