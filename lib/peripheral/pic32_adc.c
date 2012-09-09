/*!
 @file		pic32_adc.c
 @brief		ADC Control lib for pic32

 @version	0.1
 @note		This lib use auto-sample time function, there is no facilities to hangle the manual sample mode
 @todo		

 @date		February 16th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_adc.h"
// ############################################## //


// ################## Variables ################# //
tADxCON1 * pADxCON1 = NULL;
tADxCON2 * pADxCON2 = NULL;
tADxCON3 * pADxCON3 = NULL;
tADxCHS * pADxCHS = NULL;
tADxCSSL * pADxCSSL = NULL;

S16 adcOffsetValue[ADC_MAX_PORT];							//Calibration offset value result
tADCState adcState[ADC_MAX_PORT];							//ADC module actual state
U32 * adcResultPtr[ADC_MAX_PORT];							//Pointer to store the conversion result
U32 adcConversionID[ADC_MAX_PORT];							//ID of the last completed conversion
// ############################################## //


// ################ ADC Functions ############### //
// === Control Functions ==== //
/**
* \fn		U8 adcSelectPort(U8 adcPort)
* @brief	Correctly point all reg pointers for a designated ADC port
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 adcPort					Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSelectPort(U8 adcPort)
{
	switch (adcPort)
	{
		case ADC_1:	pADxCON1 = (tADxCON1*)&AD1CON1; pADxCON2 = (tADxCON2*)&AD1CON2; pADxCON3 = (tADxCON3*)&AD1CON3; pADxCHS = (tADxCHS*)&AD1CHS; pADxCSSL = (tADxCSSL*)&AD1CSSL;	break;
		default :	return STD_EC_NOTFOUND;
	}
	return STD_EC_SUCCESS;
}

/**
* \fn		U8 adcInit(U8 adcPort)
* @brief
* @note
* @arg		U8 adcPort					Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcInit(U8 adcPort)
{
	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		

	}

	return errorCode;
}

/**
* \fn		U8 adcSetSampleRate(U8 adcPort, U32 desiredSampleRate)
* @brief
* @note		This function will return error if the timing constraint are not met
*			Check the family reference section 17 for details on equations
*			Return STD_EC_TOOLARGE if the desired sample rate is too large for the PBCLK
*			Return STD_EC_INVALID if the ADC is using FRC as the clock source
* @arg		U8 adcPort					Hardware ADC ID
* @arg		U32 SampleRate				Sample Rate to configure
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSetSampleRate(U8 adcPort, U32 sampleRate)
{
	U8 adcs = 0;
	U8 samc = 0;
	U8 adcState;
	U32 tempPBclock = clockGetPBCLK();

	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Hadle boundary -- //
		if (sampleRate > 1000000)
			return STD_EC_TOOLARGE;
		// -------------------- //

		// -- Stop the ADC -- //
		adcState = pADxCON1->ON;
		pADxCON1->ON = 0;
		// ------------------ //

		// -- Check if using PBCLK -- //
		if (pADxCON3->ADRC == ADC_CLK_PBCLK)
		{
			// -- Determine the minimum adcs -- //
			for (; (tempPBclock / ((adcs+1) << 1 )) > ADC_TAD_FREQ_MAX; adcs++);
			// -------------------------------- //

			// -- Check if it is possible -- //
			if ((tempPBclock / (((adcs+1) << 1 ) * (ADC_CONV_TIME+1))) >= sampleRate)	//Minimum value of SAMC is 1
			{

					// -- Test for a valid samc with the lowest adcs possible -- //
					for (; adcs <= 255; adcs++)
					{
						samc = (tempPBclock / (sampleRate * ((adcs+1) << 1 ))) - ADC_CONV_TIME;

						if ((samc < ADC_SAMC_MAX) && ((tempPBclock / ((adcs+1) << 1 )) < ADC_TAD_FREQ_MAX))		//If it is suitable exit loop
							break;
					}
					// --------------------------------------------------------- //

					// -- Save the values -- //
					pADxCON3->ADCS = adcs;
					pADxCON3->SAMC = samc;
					// --------------------- //

					// -- Verify the actual sample rate -- //
					for (; adcGetSampleRate(adcPort) < ADC_SAMP_RATE_MAX; (pADxCON3->SAMC)++);
					// ----------------------------------- //
			}
			else
				errorCode = STD_EC_TOOLARGE;									//Sample rate too large for the PBCLK
			// ----------------------------- //
		}
		else
			errorCode = STD_EC_INVALID;											//Using FRC
		// -------------------------- //
	}

	// -- Restore the ADC State -- //
	pADxCON1->ON = adcState;
	// --------------------------- //

	return errorCode;
}
/**
* \fn		U32 adcGetSampleRate(U8 adcPort)
* @brief	Return the actual Sample Rate of the selected ADC
* @note		
* @arg		U8 adcPort					Hardware ADC ID
* @return	U32 SampleRate				Actual Sample Rate
*/
U32 adcGetSampleRate(U8 adcPort)
{
	U32 sampleRate = 0;

	if (adcSelectPort(adcPort) == STD_EC_SUCCESS)
	{
		// -- Compute the actual Sample Rate -- //
		sampleRate = clockGetPBCLK()/((((pADxCON3->ADCS)+1) <<1) * (ADC_CONV_TIME + pADxCON3->SAMC));
		// ------------------------------------ //
	}

	return sampleRate;
}

/**
* \fn		U8 adcCalibrate(U8 adcPort)
* @brief	Calibrate and offset the selected ADC
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 adcPort					Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcCalibrate(U8 adcPort)
{
	if (adcSelectPort(adcPort) == STD_EC_SUCCESS)
	{
		// -- Init for calibration -- //
		pADxCON2->OFFCAL = 1;				//Set for calib
				//Start the conversion
		// -------------------------- //

		// -- Wait and save the result -- //
		while (!(pADxCON1->DONE));			//Wait the result
		adcOffsetValue[adcPort] = 0;		//Save the result
		// ------------------------------ //

		return STD_EC_SUCCESS;
	}
}

/**
* \fn		U8 adcCalibrate(U8 adcPort)
* @brief
* @note
* @arg		U8 adcPort					Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSetConfig(U8 adcPort, U32 adcConfig)
{

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 adcSetScan(U8 adcPort, U32 scanInput)
* @brief	Set the enabled input matrix for the scan mode of the selected ADC port
* @note		Use tADCScanInput for the correct input selection
* @arg		U8 adcPort					Hardware ADC ID
* @arg		tADCScanInput scanInput		Input Matrix to enable
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSetScan(U8 adcPort, tADCScanInput scanInput)
{
	U8 inputNb = 0;

	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		pADxCSSL->CSSL = scanInput;			//Set the selected input

		// -- Count the number of input -- //
		while (scanInput)
		{
			if (scanInput && BIT0)			//Check if the LSchannel is enabled
				inputNb++;					//Count the channel
			scanInput >>= 1;				//Position for the next channel
		}
		// ------------------------------- //

		pADxCON2->SMPI = (inputNb-1);
	}
	return errorCode;
}

/**
* \fn		U32 adcGetScan(U8 adcPort)
* @brief	Return the enabled input matrix for the scan mode of the selected ADC port
* @note		Use tADCScanInput for the correct return value
* @arg		U8 adcPort					Hardware ADC ID
* @return	tADCScanInput inputMatrix	Input Matrix enabled
*/
tADCScanInput adcGetScan(U8 adcPort)
{
	if (adcSelectPort(adcPort) == STD_EC_SUCCESS)
		return pADxCSSL->CSSL;
}
// ========================== //

// === Conversion Functions ==== //
/**
* \fn		U32 adcGetScan(U8 adcPort)
* @brief	
* @note		
* @arg		U8 adcPort					Hardware ADC ID
* @arg		tADCInput adcInput			Analog input to convert
* @arg		U8 conversionNb				Number of conversion to do and round up (maximum 16)
* @arg		U32 * resultPtr				Pointer to store the result
* @return	U32 adcConversionID			ID of this conversion (used to check if the conversion is done)
*/
U32 adcConvert(U8 adcPort, tADCInput adcInput, U8 conversionNb, U32 * resultPtr)
{
	// -- Handle boundary -- //
	if (conversionNb > 16)
		conversionNb = 16;
	// --------------------- //


}

U8 adcStartConvert(U8 adcPort)
{

}

U8 adcStopConvert(U8 adcPort)
{

}
// ============================= //
// ############################################## //

