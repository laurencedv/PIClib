/*!
 @file		pic18_ccp.c
 @brief		CCP lib for C18 and XC8

 @version	0.1
 @note		PWM duty as only a precision of 8bit (datasheet not clear on the 10bit period operation)
 @todo		

 @date		November 11th 2012
 @author	Laurence DV
*/

// ################# Includes ################### //
#include <pic18_ccp.h>
// ############################################## //


// ################## Variables ################# //
#if CPU_FAMILY == PIC18Fx7Jx3
volatile tCCPReg * const ccpRegAddress[10] = {&CCPR1H,&CCPR2H,&CCPR3H,&CCPR4H,&CCPR5H,&CCPR6H,&CCPR7H,&CCPR8H,&CCPR9H,&CCPR10H};

const U8 ccpTimerMatrix[10][4][2] = {	{{TIMER_1,TIMER_3,TIMER_3,TIMER_3},{TIMER_2,TIMER_4,TIMER_6,TIMER_8}},
					{{TIMER_1,TIMER_3,TIMER_3,TIMER_3},{TIMER_2,TIMER_4,TIMER_6,TIMER_8}},
					{{TIMER_1,TIMER_3,TIMER_3,TIMER_3},{TIMER_2,TIMER_4,TIMER_6,TIMER_8}},
					{{TIMER_1,TIMER_3,TIMER_3,0xFF},{TIMER_2,TIMER_4,TIMER_6,0xFF}},
					{{TIMER_1,TIMER_5,0xFF,0xFF},{TIMER_2,TIMER_4,0xFF,0xFF}},
					{{TIMER_1,TIMER_5,0xFF,0xFF},{TIMER_2,TIMER_2,0xFF,0xFF}},
					{{TIMER_1,TIMER_5,TIMER_5,TIMER_5},{TIMER_2,TIMER_4,TIMER_6,TIMER_8}},
					{{TIMER_1,TIMER_1,TIMER_1,0xFF},{TIMER_2,TIMER_4,TIMER_6}},
					{{TIMER_1,TIMER_1,0xFF,0xFF},{TIMER_2,TIMER_4,0xFF,0xFF}},
					{{TIMER_1,0xFF,0xFF,0xFF},{TIMER_2,0xFF,0xFF,0xFF}}};
#endif

// Control
tCCPCtl CCPControl[CCP_MAX_ID];
// ############################################## //


// ############ Primary functions ############### //
/**
* \fn		void ccpISR(U8 ccpID)
* @brief	Interrupt Handler for any CCP port
* @note		Place this in the correct ISR in the main
* @arg		U8 ccpID			Hardware CCP ID
* @return	nothing
*/
void ccpISR(U8 ccpID)
{
	
}

/**
* \fn		void ccpEngine(U8 ccpID)
* @brief	Engine function for advance CCP fonction
* @note		Place this in the infinite loop of the main
* @arg		U8 ccpID			Hardware CCP ID
* @return	nothing
*/
void ccpEngine(U8 ccpID)
{

}

/**
* \fn		U8 ccpInit(U8 ccpID, U8 timerSetting, U8 mode)
* @brief	Initialize a CCP to a stand-by state
* @note		Start the timer to start the CCP function
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U8 timerSetting			Timer to attach to the CCP
* @arg		U8 mode				Mode to put the CCP into
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ccpInit(U8 ccpID, U8 timerSetting, U8 mode)
{
	if (ccpID < CCP_MAX_ID)
	{
		U8 wu0 = 0;

		// -- Init the work Pointer -- //
		tCCPReg * workPtr = ccpRegAddress[ccpID];
		// --------------------------- //

		// -- Sanity Check -- //
		CCPControl[ccpID].all[0] = 0;					//Clear all previous setting

		if (timerSetting > 0x3)						//Filter impossible timer id
			return STD_EC_INVALID;					//return the error

		if (mode > CCP_MODE_COMPARE_SPECIAL_EVENT)			//Check if PWM or Compare/Capture
			wu0 = 1;
		wu0 = ccpTimerMatrix[ccpID][timerSetting][wu0];			//Find the correct timer associated

		if (wu0 < 8)							//Check if a valid timer has been found
			CCPControl[ccpID].timerID = wu0;			//Save the timerID
		else
			return STD_EC_INVALID;					//return the error
		// ------------------ //

		// -- Associate the timer -- //		
		switch (ccpID)
		{
			case CCP_1:	CCPTMRS0bits.C1TSEL = timerSetting;	break;
			case CCP_2:	CCPTMRS0bits.C2TSEL = timerSetting;	break;
			case CCP_3:	CCPTMRS0bits.C3TSEL = timerSetting;	break;
			case CCP_4:	CCPTMRS1bits.C4TSEL = timerSetting;	break;
			case CCP_5:	CCPTMRS1bits.C5TSEL0 = timerSetting;	break;
			case CCP_6:	CCPTMRS1bits.C6TSEL0 = timerSetting;	break;
			case CCP_7:	CCPTMRS1bits.C7TSEL = timerSetting;	break;
			case CCP_8:	CCPTMRS2bits.C8TSEL = timerSetting;	break;
			case CCP_9:	CCPTMRS2bits.C9TSEL0 = timerSetting;	break;
			case CCP_10:	CCPTMRS2bits.C10TSEL0 = timerSetting;	break;
		}
		// ------------------------- //
		
		// -- Set the mode -- //
		workPtr->CCPxCON.CCPM = mode;
		// ------------------ //
		
		// -- Init the module -- //
		CCPControl[ccpID].state = init;
		// --------------------- //
		
		return STD_EC_SUCCESS;
	}
	return STD_EC_NOTFOUND;
}


void ccpSetPeriod(U8 ccpID, U16 newPeriod)
{
	if (ccpID < CCP_MAX_ID)
	{
		timerSetPR(CCPControl[ccpID].timerID) = (newPeriod/timerGetTickPeriod(CCPControl[ccpID].timerID)) -1;
	}
}


U16 ccpGetPeriod(U8 ccpID)
{
	if (ccpID < CCP_MAX_ID)
	{
		return (timerGetPR(CCPControl[ccpID].timerID)+1)*timerGetTickPeriod(CCPControl[ccpID].timerID);
	}
}
// ############################################## //


// ############ Compare functions ############### //
void ccpSetCompare(U8 ccpID, U16 newCompare)
{

}

U16 ccpGetCompare(U8 ccpID)
{

}
// ############################################## //


// ############ Capture functions ############### //
U16 ccpGetCapture(U8 ccpID)
{

}
// ############################################## //


// ############## PWM functions ################# //
/**
* \fn		void ccpSetDuty(U8 ccpID, U8 numerator, U8 denominator)
* @brief	This function set the PWM duty to a specified ratio
* @note		This function will check if the duty is compatible with the actual PWM period (use ccpFastSetDuty for direct access)
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U8 numerator			Numerator of the duty ratio
* @arg		U8 denominator			Denominator of the duty ratio
* @return	nothing
*/
void ccpSetDuty(U8 ccpID, U8 numerator, U8 denominator)
{
	if (ccpID < CCP_MAX_ID)
	{
		if (denominator)
		{
			U8 tempPR = timerGetPR(CCPControl[ccpID].timerID);
			if (denominator <= tempPR)
			{
				U16 newDuty;

				// -- Compute the new duty -- //
				newDuty = ((U16)tempPR * (U16)numerator) / denominator;
				// -------------------------- //

				ccpFastSetDuty(ccpID,newDuty);		//Set the duty
			}
		}
	}
}

/**
* \fn		void ccpFastSetDuty(U8 ccpID, U16 newDuty)
* @brief	Direct set of the duty register
* @note		This function will not check if the duty is compatible with the actual PWM period (use ccpSetDuty for that)
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U16 newDuty			New value for the duty (maximum 10bit)
* @return	nothing
*/
void ccpFastSetDuty(U8 ccpID, U16 newDuty)
{
	if (ccpID < CCP_MAX_ID)
	{
		volatile tCCPReg * workPtr = ccpRegAddress[ccpID];

		workPtr->CCPRxL = newDuty >> 2;				//Set the MS part
		workPtr->CCPxCON.DCxB = newDuty & 0x3;			//Set the LS part
	}
}

U16 ccpGetDuty(U8 ccpID)
{

}
// ############################################## //
