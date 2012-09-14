/*!
 @file		pic32_timer.c
 @brief		Hardware Timer control functions for C32

 @version	0.2.3
 @note		timerSetOverflow and timerGetOverflow are really cpu intensive use wisely

 @todo		Shrink the ROM footprint
 			Make a timerSelectPort function to reduce the redundant code

 @date		March 2th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_timer.h"
// ############################################## //


// ################## Defines ################### //
#define TMR_CKPS_NB		8
#define	TMR1_CKPS_NB	4
// ############################################## //


// ################## Variables ################# //
// Reg Pointer //
tTCON * pTxCON = NULL;
U32 * pPRx = NULL;
U32 * pPR32x = NULL;
// ----------- //

// Clock configuration //
const U16 tmrPrescalerValue[TMR_CKPS_NB] = {1,2,4,8,16,32,64,256};				//Possible prescaler values for other timers
const U16 tmr1PrescalerValue[TMR1_CKPS_NB] = {1,8,64,256};						//Possible prescaler values for timer 1
const U32 tmrMax[2] = {0xFFFF,0xFFFFFFFF};										//Maximum value of timers (16bit and 32 bits)
// ------------------- //
// ############################################## //


// ############# Internal Functions ############# //
/**
* \fn		U8 timerSelectPort(U8 timerPort)
* @brief	Correctly point all reg pointers for a designated Timer
* @note		Will return STD_EC_NOTFOUND if an invalid timer is given
* @arg		U8 timerPort				HW Timer ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 timerSelectPort(U8 timerPort)
{
	switch (timerPort)
	{
		case TIMER_1:	pTxCON = (tTCON*)&T1CON;	pPRx = (U32*)&PR1;				break;
		case TIMER_2:	pTxCON = (tTCON*)&T2CON;	pPRx = (U32*)&PR2;	pPR32x = (U32*)&PR3;	break;
		case TIMER_3:	pTxCON = (tTCON*)&T3CON;	pPRx = (U32*)&PR3;				break;
		case TIMER_4:	pTxCON = (tTCON*)&T4CON;	pPRx = (U32*)&PR4;	pPR32x = (U32*)&PR5;	break;
		case TIMER_5:	pTxCON = (tTCON*)&T5CON;	pPRx = (U32*)&PR5;				break;
		default:	return STD_EC_NOTFOUND;
	}
	return STD_EC_SUCCESS;
}
// ############################################## //

// ############### Timer Functions ############## //
// ==== ISR ======== //
/**
* \fn		void timer1ISR(void)
* @brief	Example of an Interrupt Service Routine for Timer1
* @note		Place it at the Timer 1 vector in the main
* @arg		nothing
* @return	nothing
*/
/*
void __ISR(_TIMER_1_VECTOR, IPL7SOFT) timer1ISR(void)
{

	INTClearFlag(INT_T1);
}
*/
// ================= //


// ==== Control ==== //
/**
* \fn		U8 timerInit(U8 timerPort, U32 option)
* @brief	Initialise the selected timer with the specified options.
* @note		Use the Setting Map in the header for correct setting to send to this function
		No sanity check of the settings, will return STD_EC_NOTFOUND if invalid timer ID is inputed.
		Option must be | or + (ex: timerInit(0, TMR_DIV_1|TMR_CS_PBCLK|TMR_16BIT))
		For 32bit timer must be called for each timer in the pair.
* @arg		U8 timerPort		Hardware Timer ID
* @arg		U32 option			Setting to configure for the timer
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerInit(U8 timerPort, U32 option)
{
	split32 splittedOption;
	U8 errorCode;

	splittedOption.all = option;

	// -- Select the correct Timer -- //
	errorCode = timerSelectPort(timerPort);
	if (errorCode == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		// -- Set the options -- //

		// -- Timer 1 is a special case
		if (timerPort == TIMER_1)
		{
			//Set the prescaler
			splittedOption.b1 = splittedOption.b2;		//check only the msb and lsb of the prescaler def
			pTxCON->TCKPS = splittedOption.all & 0x3;	//for the timer 1 (ex: 0b101 = 0b11)

			//Set the clock sync
			pTxCON->TSYNC = splittedOption.b5;
			pTxCON->TWDIS = splittedOption.b6;
		}
		// -- All other Timers
		else
		{
			//Set the prescaler
			pTxCON->TCKPS = splittedOption.all & 0x7;

			//Set the Timer size
			pTxCON->T32 = splittedOption.b4;
		}

		//Set the clock source
		pTxCON->TCS = splittedOption.b3;

		//Set the gate
		pTxCON->TGATE = splittedOption.b8;

		//Set the Idle mode
		pTxCON->SIDL = splittedOption.b7;

		//Set the debug mode
		pTxCON->FRZ = splittedOption.b9;
		// --------------------- //

		//Reset the timer to 0
		timerClear(timerPort);
	}

	return errorCode;
}

/**
* \fn		void timerStart(U8 timerPort)
* @brief	Start the selected Timer
* @note		nothing
* @arg		U8 timerPort		Hardware Timer ID
* @return	nothing
*/
void timerStart(U8 timerPort)
{
	switch (timerPort)
	{
		case TIMER_1:	timerFastStart(0);	break;
		case TIMER_2:	timerFastStart(1);	break;
		case TIMER_3:	timerFastStart(2);	break;
		case TIMER_4:	timerFastStart(3);	break;
		case TIMER_5:	timerFastStart(4);	break;
	}
}

/**
* \fn		void timerStop(U8 timerPort)
* @brief	Stop the selected Timer
* @note		nothing
* @arg		U8 timerPort		Hardware Timer ID
* @return	nothing
*/
void timerStop(U8 timerPort)
{
	switch (timerPort)
	{
		case TIMER_1:	timerFastStop(0);	break;
		case TIMER_2:	timerFastStop(1);	break;
		case TIMER_3:	timerFastStop(2);	break;
		case TIMER_4:	timerFastStop(3);	break;
		case TIMER_5:	timerFastStop(4);	break;
	}
}

/**
* \fn		U8 timerGetSize(U8 timerPort)
* @brief	Return the data width of the designated timer
* @note		nothing
* @arg		U8 timerPort		Hardware Timer ID
* @return	U8 dataSize			Timer data width in bits
*/
U8 timerGetSize(U8 timerPort)
{
	switch (timerPort)
	{
		case TIMER_1:	return 16;
		case TIMER_2:	return ((T2CONbits.T32 << 4)+16);
		case TIMER_3:	return ((T2CONbits.T32 << 4)+16);
		case TIMER_4:	return ((T4CONbits.T32 << 4)+16);
		case TIMER_5:	return ((T4CONbits.T32 << 4)+16);
		default:	return 0;
	}
}


/**
* \fn		U32 timerGetClock(U8 timerPort)
* @brief	Return the actual Timer clock source freq for the designated timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
*			is external
* @arg		U8 timerPort		Hardware Timer ID
* @return	U32 clockFreq		Timer clock source freq (in Hz)
*/
U32 timerGetClock(U8 timerPort)
{
	U8 tempDivID = 0;
	U16 * pDivTable = (U16*)tmrPrescalerValue;

	// -- Select the correct Timer -- //
	if (timerSelectPort(timerPort) == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		// Timer 1 is a special case
		if (timerPort == TIMER_1)
			pDivTable = (U16*)tmr1PrescalerValue;

		// -- Compute if Clock source is the PBCLK -- //
		if (pTxCON->TCS == TMR_CS_PBCLK)
		{
			tempDivID = pTxCON->TCKPS;			//check the actual prescaler setting

			//Return the actual Timer clock source freq
			return (clockGetPBCLK()/(pDivTable[tempDivID]));
		}
		else
			return 0;					//Return 0 if the clock is external (0 Hz)
		// ------------------------------------------ //
	}
}

/**
* \fn		U8 timerSetOverflow(U8 timerPort, U32 ovfPeriod)
* @brief	Set the timer overflow interrupt at the desired period
* @note		This function will round up to the possible freq depending on the actual globalCLK.
		Only work if using the PBCLK as the clock source.
		Use the timerInit with the correct parameters for external clock source.
* @arg		U8 timerPort		Hardware Timer ID
* @arg		U32 ovfPeriod		Total Period between overflow (in us)
* @return	U8 errorCode		STD Error Code
*/
U8 timerSetOverflow(U8 timerPort, F32 ovfPeriod)
{
	U8 timerDivID;
	U8 timerDivIDmax = TMR_CKPS_NB;
	U16 * pDivTable = (U16*)tmrPrescalerValue;
	F32 cntPeriod = 0;
	U32 tempPBCLK = clockGetPBCLK();
	U8 okFlag = 0;
	U8 errorCode;

	// -- Select the correct Timer -- //
	errorCode = timerSelectPort(timerPort);
	if (errorCode == STD_EC_SUCCESS)
	// ------------------------------ //
	{

		//Timer 1 is a special case
		if (timerPort == TIMER_1)
		{
			timerDivIDmax = TMR1_CKPS_NB;
			pDivTable = (U16*)tmr1PrescalerValue;
		}

		// -- Set the needed freq -- //
		//Find the maximum frequency possible
		for (timerDivID = 0; timerDivID < timerDivIDmax ;timerDivID++)
		{
			cntPeriod = (1000000/((F32)(tempPBCLK/(pDivTable[timerDivID]))));

			if (ovfPeriod <= (cntPeriod * (tmrMax[pTxCON->T32] +1)))
			{
				pTxCON->TCKPS = timerDivID;		//Set the prescaler
				okFlag = 1;
				break;
			}
		}
		if (!okFlag)
			return STD_EC_TOOLARGE;				//We were not able to find a prescaler to fit this overflow period
		// ------------------------- //

		// -- Set the Overflow Match value -- //
		*pPRx = (U32)(ovfPeriod / cntPeriod);
		// ---------------------------------- //
	}

	return errorCode;
}

/**
* \fn		U32 timerGetOverflow(U8 timerPort)
* @brief	Return the overflow period of the specified timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
*			is external
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32 ovfPeriod		Overflow period (in µs)
*/
U32 timerGetOverflow(U8 timerPort)
{
	F32 cntPeriod = 0;

	// -- Select the correct Timer -- //
	if (timerSelectPort(timerPort) == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		// get the time for 1 count
		cntPeriod = timerGetCntPeriod(timerPort);

		// Compute the overflow period
		return ((U32)(*pPRx * cntPeriod));
	}
	return 0;
}

/**
* \fn		F32 timerGetCntPeriod(U8 timerPort)
* @brief	Return the count period of the specified timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
		is external
* @arg		U8 timerPort			Hardware Timer ID
* @return	F32 cntPeriod			Count period (in µs)
*/
F32 timerGetCntPeriod(U8 timerPort)
{
	U16 * pDivTable = (U16*)tmrPrescalerValue;

	// -- Select the correct Timer -- //
	if (timerSelectPort(timerPort) == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		if (timerPort == TIMER_1)
			pDivTable = (U16*)tmr1PrescalerValue;

		// compute the time for 1 count
		return (1000000/((F32)(clockGetPBCLK()/(pDivTable[pTxCON->TCKPS]))));
	}

	return 0;
}

/**
* \fn		U8 timerSetPR(U8 timerPort, U32 PRvalue)
* @brief	Set the PR reg of the designated Timer to the specified value
* @note		Data Width can vary between 16 and 32 bits but it is automaticly handled
* @todo		Must check if T32 is set, but trying to access the odd timer
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 PRvalue			Value to set PR reg
* @return	errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerSetPR(U8 timerPort, U32 PRvalue)
{
	U8 errorCode;

	// -- Select the correct Timer -- //
	errorCode = timerSelectPort(timerPort);
	if (errorCode == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		// -- Set the correct data size -- //
		if (pTxCON->T32)
			*pPR32x = (PRvalue & 0xFF00)>>16;		//Save the MSpart to the odd timer

		*pPRx = PRvalue & 0xFF;					//Save the LSpart to the even number
		// ------------------------------- //
	}

	return errorCode;
}

/**
* \fn		U32 timerGetPR(U8 timerPort)
* @brief	Return the value of the PR reg of the designated timer
* @note		Data Width can vary between 16 and 32 bits but it is automaticly handled
		Always return an Unsigned variable
* @todo		Must check if T32 is set, but trying to access the odd timer
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32 PRvalue			Value of the PR reg
*/
U32 timerGetPR(U8 timerPort)
{
	U32 PRvalue = 0;

	// -- Select the correct Timer -- //
	if (timerSelectPort(timerPort) == STD_EC_SUCCESS)
	// ------------------------------ //
	{
		// -- Retreive the correct data size -- //
		if (pTxCON->T32)
			PRvalue = *pPR32x<<16;				//Save the MSpart from the odd timer

		PRvalue += *pPRx;					//Save the LSpart from the even number
		// ------------------------------------ //
	}
	return PRvalue;
}
// ================= //


// ==== Data Handling ==== //
/**
* \fn		void timerSet(U8 timerPort, U32 data)
* @brief	Set the selected Timer to the specified value
* @note		Depend on wich timer is selected, the data width will vary between 16bit and 32bit
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 data			Data to input in the timer
* @return	nothing
*/
void timerSet(U8 timerPort, U32 data)
{
	switch(timerPort)
	{
		case TIMER_1:	timerFastSet(0,data);	break;
		case TIMER_2:	timerFastSet(1,data);	break;
		case TIMER_3:	timerFastSet(2,data);	break;
		case TIMER_4:	timerFastSet(3,data);	break;
		case TIMER_5:	timerFastSet(4,data);	break;
	}
}

/**
* \fn		U32 timerGet(U8 timerPort)
* @brief	Return the value of the selected Timer
* @note		The return size will always be 32bit but the valid width must be known
		Use timerGetSize() to check the data width
		Will return 0 if invalid timerPort is provided
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32					Actual value of the timer
*/
U32 timerGet(U8 timerPort)
{
	switch(timerPort)
	{
		case TIMER_1:	return timerFastGet(0);
		case TIMER_2:	return timerFastGet(1);
		case TIMER_3:	return timerFastGet(2);
		case TIMER_4:	return timerFastGet(3);
		case TIMER_5:	return timerFastGet(4);
		default:		return 0;
	}
}

/**
* \fn		void timerClear(U8 timerPort)
* @brief	Reset to 0 the selected Timer
* @note		nothing
* @arg		U8 timerPort			Hardware Timer ID
* @return	nothing
*/
void timerClear(U8 timerPort)
{
	switch (timerPort)
	{
		case TIMER_1:	timerFastClear(0);	break;
		case TIMER_2:	timerFastClear(1);	break;
		case TIMER_3:	timerFastClear(2);	break;
		case TIMER_4:	timerFastClear(3);	break;
		case TIMER_5:	timerFastClear(4);	break;
	}
}
// ======================= //
// ############################################## //
