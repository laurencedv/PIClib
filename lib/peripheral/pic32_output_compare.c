/*!
 @file		pic32_output_compare.c
 @brief		Output Compare control lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		March 21th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_output_compare.h"
// ############################################## //


// ################## Variables ################# //
//Config
tOCConfig ocConfig[OC_MAX_PORT];
U32 pwmPeriodTime[OC_MAX_PORT];
F32 pwmCntPeriodTime[OC_MAX_PORT];

//Reg pointers
tOCxCON * pOCxCON = NULL;				//Control reg pointer
U32 * pOCxR = NULL;					//Compare reg pointer
U32 * pOCxRS = NULL;					//Duty reg pointer
// ############################################## //


// ############# Internal Functions ############# //
/**
* \fn		U8 ocSelectPort(U8 ocPort)
* @brief	Correctly point all reg pointers for a designated OC port
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 ocPort				Hardware OC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ocSelectPort(U8 ocPort)
{
	switch (ocPort)
	{
		case OC_1:	pOCxCON = (tOCxCON*)&OC1CON;	pOCxR = (U32*)&OC1R;	pOCxRS = (U32*)&OC1RS;	break;
		case OC_2:	pOCxCON = (tOCxCON*)&OC2CON;	pOCxR = (U32*)&OC2R;	pOCxRS = (U32*)&OC2RS;	break;
		case OC_3:	pOCxCON = (tOCxCON*)&OC3CON;	pOCxR = (U32*)&OC3R;	pOCxRS = (U32*)&OC3RS;	break;
		case OC_4:	pOCxCON = (tOCxCON*)&OC4CON;	pOCxR = (U32*)&OC4R;	pOCxRS = (U32*)&OC4RS;	break;
		case OC_5:	pOCxCON = (tOCxCON*)&OC5CON;	pOCxR = (U32*)&OC5R;	pOCxRS = (U32*)&OC5RS;	break;
		default: return STD_EC_NOTFOUND;
	}
	return STD_EC_SUCCESS;
}
// ############################################## //


// ########### Output Compare Functions ######### //
// === Interrupt Handler ===== //
/**
* \fn		void FSCMISR(void)
* @brief	ISR for the Fail-Safe Clock Monitor
* @note
* @arg		nothing
* @return	nothing
*/
/*

*/
// =========================== //


// ==== Control Functions ==== //
/**
* \fn		void ocSetConfig(U8 ocPort, U32 localOcConfig)
* @brief	Save the configuration to the config struct of the specified OC port
* @note		The config can also be directly access via the config struct
*		For manual configuration, option must be | or + (ex: ocSetConfig(OC_1, OC_MODE_PWM|OC_TIMER_2))
* @arg		U8 ocPort				Hardware OC ID
* @arg		U32 ocLocalConfig			Setting to configure for the OC
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ocSetConfig(U8 ocPort, U32 localOcConfig)
{
	// -- Detect error -- //
	//Check for coherent data width
	if (localOcConfig & OC_32BIT_ON)
	{
		if (timerGetSize((((tOCConfig)localOcConfig).timer)+1) != 32)
			return STD_EC_FAIL;
	}
	// ------------------ //

	ocConfig[ocPort].all = localOcConfig;

	return STD_EC_SUCCESS;
}

/**
* \fn		tOCConfig ocGetConfig(U8 ocPort)
* @brief	Return the configuration of the specified OC port
* @note		The config can also be directly access via the config struct
* @arg		U8 ocPort				Hardware OC ID
* @return	tOCConfig ocLocalConfig			Setting to configure for the OC
*/
tOCConfig ocGetConfig(U8 ocPort)
{
	return ocConfig[ocPort];
}

/**
* \fn		U8 ocStart(U8 ocPort)
* @brief	Configure and start a specified OC port
* @note		If any error occur during the starting, will shutdown the specified OC port and return the error code
*		Execute lengthy computation to save human-readable time information
* @arg		U8 ocPort				Hardware OC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ocStart(U8 ocPort)
{
	U8 errorCode;
	
	// -- Select the port -- //
	errorCode = ocSelectPort(ocPort);
	if (errorCode == STD_EC_SUCCESS)
	// --------------------- //
	{
		// -- Configure the OC -- //
		*pOCxCON = (tOCxCON)ocConfig[ocPort].registers.OCxCON;
		*pOCxR = 0;
		*pOCxRS = 0;
		// ---------------------- //

		// -- Start the timer -- //
		timerStart((ocConfig[ocPort].timer)+1);
		// --------------------- //

		// Start the Output Compare module
		pOCxCON->ON = 1;
	}
	return errorCode;
}
// =========================== //


// ====== PWM Functions ====== //
/**
* \fn		void pwmSetPeriod(U8 ocPort, U32 newPeriod)
* @brief	Set the correct timer's period for the specified OC port
* @note		Will not check if any other OC port is using the same timer
*		Will update the pwmPeriodTime and pwmCntPeriodTime value
* @arg		U8 ocPort				Hardware OC ID
* @arg		U32 newPeriod				New period (in µs)
* @return	nothing
*/
void pwmSetPeriod(U8 ocPort, U32 newPeriod)
{
	// Set the new period value
	timerSetOverflow((ocConfig[ocPort].timer)+1, newPeriod);

	// -- Update the Time values -- //
	pwmPeriodTime[ocPort] = newPeriod;
	pwmCntPeriodTime[ocPort] = timerGetCntPeriod((ocConfig[ocPort].timer)+1);
	// ---------------------------- //
}

/**
* \fn		U32 pwmGetPeriod(U8 ocPort)
* @brief	Return the correct timer's period for the specified OC port
* @note		CPU intensive, use the pwmPeriodTime array for faster access
* @arg		U8 ocPort				Hardware OC ID
* @return	U32 newPeriod				New period (in µs)
*/
U32 pwmGetPeriod(U8 ocPort)
{
	return timerGetOverflow((ocConfig[ocPort].timer)+1);
}

/**
* \fn		U8 pwmSetTon(U8 ocPort, U32 timeON)
* @brief	Set the ON time for a specified OC port
* @note		Will not update if the ON time is less than the period time (and will return STD_EC_TOOLARGE)
*		ATTENTION! This function use floating point math for better precision
* @arg		U8 ocPort				Hardware OC ID
* @arg		U32 timeON				ON time desired (in µs)
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 pwmSetTon(U8 ocPort, U32 timeON)
{
	U8 errorCode;

	// -- Select the port -- //
	errorCode = ocSelectPort(ocPort);
	if (errorCode == STD_EC_SUCCESS)
	// --------------------- //
	{
		// -- Only change Ton if the period is large enough -- //
		if (timeON <= pwmPeriodTime[ocPort])
			*pOCxRS = (U32)(((F32)timeON) / pwmCntPeriodTime[ocPort]);
		// --------------------------------------------------- //
		else
			errorCode = STD_EC_TOOLARGE;
	}

	return errorCode;
}

/**
* \fn		void pwmSetDuty(U8 ocPort, U32 numerator, U32 denominator)
* @brief	Set the duty of the specified OC port
* @note		Use the numerator and the denominator as a fraction representing the duty (ex: pwmSetDuty(OC_1,1,2) will set to 50%)
*		This function use integer, for more precision use the OCxRS register directly
* @arg		U8 ocPort				Hardware OC ID
* @arg		U32 numerator				Numerator of the duty
* @arg		U32 denominator				Denominator of the duty
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
void pwmSetDuty(U8 ocPort, U32 numerator, U32 denominator)
{
	U32 tempPR = timerGetPR((ocConfig[ocPort].timer)+1);

	if (ocSelectPort(ocPort) == STD_EC_SUCCESS)
	{
		// Handle division by 0
		if (denominator != 0)
		{
			//Handle error from integer inaccuracy
			if (denominator <= tempPR)
			{
				// Compute and set the new value
				*pOCxRS = (tempPR/denominator)*numerator;
			}
		}
	}
}
// =========================== //
// ############################################## //
