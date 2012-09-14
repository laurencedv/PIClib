/*!
 @file		pic32_adc.c
 @brief		ADC Control lib for pic32

 @version	0.1
 @note		This lib use auto-sample time function, there is no facilities to hangle the manual sample mode
 @todo		- Transform this for the rtos
 *		- Change adcResultPtr to be able to point different size of element

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
U32 * pADxBUF = NULL;

S16 adcOffsetValue[ADC_MAX_PORT];							//Calibration offset value result
tADCState adcState[ADC_MAX_PORT];							//ADC module actual state
U16 * adcResultPtr[ADC_MAX_PORT];							//Pointer to store the conversion result
U32 adcConversionID[ADC_MAX_PORT];							//ID of the last completed conversion
// ############################################## //


// ################ ADC Functions ############### //
// ========== ISR =========== //
void adcISR(U8 adcPort)
{
	U8 wu0;
	
	switch (adcState[adcPort])
	{
		case ADCidle:
		{
			//Not really supposed to be here...
			break;
		}
		case ADCconfig:
		{
			//Not really supposed to be here...
			break;
		}
		case ADCbusy:
		{
			// -- Save the result in the destination -- //
			for (wu0 = 0; wu0 < pADxCON2->SMPI; wu0++)
				adcResultPtr[adcPort][wu0] = (pADxBUF[wu0 << 2]) + adcOffsetValue[adcPort];
			// ---------------------------------------- //

			adcState[adcPort] = ADCidle;
			break;
		}
		case ADCcalibration:
		{
			// -- Save the calibration result -- //
			adcOffsetValue[adcPort] = *pADxBUF;
			// --------------------------------- //

			adcState[adcPort] = ADCidle;
			
			break;
		}
		default: adcState[adcPort] = ADCerror;
	}
}
// ========================== //


// === Control Functions ==== //
/**
* \fn		U8 adcSelectPort(U8 adcPort)
* @brief	Correctly point all reg pointers for a designated ADC port
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 adcPort				Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSelectPort(U8 adcPort)
{
	switch (adcPort)
	{
		case ADC_1:	pADxCON1 = (tADxCON1*)&AD1CON1; pADxCON2 = (tADxCON2*)&AD1CON2; pADxCON3 = (tADxCON3*)&AD1CON3; pADxCHS = (tADxCHS*)&AD1CHS; pADxCSSL = (tADxCSSL*)&AD1CSSL; pADxBUF = &ADC1BUF0;	break;
		default :	return STD_EC_NOTFOUND;
	}
	return STD_EC_SUCCESS;
}

/**
* \fn		U8 adcInit(U8 adcPort)
* @brief	Initialize the ADC module for single mode
* @note
* @arg		U8 adcPort				Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcInit(U8 adcPort)
{
	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Stop the ADC -- //
		pADxCON1->ON = 0;
		// ------------------ //

		pADxCON1->SSRC = ADC_TRIG_AUTO;		//Internal counter for trigger
		pADxCON2->BUFM = 0;			//16bit buffer
		pADxCON1->CLRASAM = 1;			//Will stop the conversion after the interrupt
		pADxCON2->VCFG = 0;			//Set the references to AVdd and AVss
		pADxCON2->ALTS = 0;			//Use only MUX A
		pADxCON3->ADRC = ADC_CLK_PBCLK;		//Use PBClock as the clock source
		pADxCON1->FORM = ADC_FORMAT_U16;	//Format the result as a unsigned 16bit

		// -- Init control -- //
		adcOffsetValue[adcPort] = 0;
		adcState[adcPort] = ADCconfig;
		// ------------------ //

		// -- Start the ADC -- //
		pADxCON1->ON = 1;
		Nop();					//TODO wait ADC_BOOT_TIME
		// ------------------- //
	}

	return errorCode;
}

/**
* \fn		U8 adcSetSampleRate(U8 adcPort, U32 desiredSampleRate)
* @brief
* @note		This function will return error if the timing constraint are not met
*		Check the family reference section 17 for details on equations
*		Return STD_EC_TOOLARGE if the desired sample rate is too large for the PBCLK
*		Return STD_EC_INVALID if the ADC is using FRC as the clock source
* @arg		U8 adcPort				Hardware ADC ID
* @arg		U32 SampleRate				Sample Rate to configure (in sample per second)
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSetSampleRate(U8 adcPort, U32 sampleRate)
{
	U8 adcs = 0;
	U8 samc = 0;
	U8 adcON;
	U32 tempPBclock = clockGetPBCLK();

	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Hadle boundary -- //
		if (sampleRate > 1000000)
			return STD_EC_TOOLARGE;
		// -------------------- //

		// -- Stop the ADC -- //
		adcState[adcPort] = ADCconfig;
		adcON = pADxCON1->ON;
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
					while (adcGetSampleRate(adcPort) > ADC_SAMP_RATE_MAX)
						pADxCON3->SAMC++;
					// ----------------------------------- //
			}
			else
				errorCode = STD_EC_TOOLARGE;						//Sample rate too large for the PBCLK
			// ----------------------------- //
		}
		else
			errorCode = STD_EC_INVALID;							//Using FRC
		// -------------------------- //
	
		// -- Restore the ADC State -- //
		adcState[adcPort] = ADCidle;
		pADxCON1->ON = adcON;
		if (adcON)
		{
			Nop();				//TODO wait ADC_BOOT_TIME
		}
		// --------------------------- //
	}
	
	return errorCode;
}

/**
* \fn		U32 adcGetSampleRate(U8 adcPort)
* @brief	Return the actual Sample Rate of the selected ADC
* @note		
* @arg		U8 adcPort				Hardware ADC ID
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
* @arg		U8 adcPort				Hardware ADC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcCalibrate(U8 adcPort)
{
	U8 errorCode = adcSelectPort(adcPort);

	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Init for calibration -- //
		pADxCON2->OFFCAL = 1;			//Set for calib
		pADxCON2->SMPI = 0;			//Do 1 conversion
		pADxCON1->SAMP = 1;			//Start the conversion

		adcState[adcPort] = ADCcalibration;
		// -------------------------- //
	}
	return errorCode;
}


/**
* \fn		U8 adcSetScan(U8 adcPort, U32 scanInput)
* @brief	Set the enabled input matrix for the scan mode of the selected ADC port
* @note		Use tADCScanInput for the correct input selection
* @arg		U8 adcPort				Hardware ADC ID
* @arg		tADCScanInput scanInput			Input Matrix to enable
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 adcSetScan(U8 adcPort, tADCScanInput scanInput)
{
	U8 inputNb = 0;

	U8 errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		pADxCSSL->CSSL = scanInput;		//Set the selected input

		// -- Count the number of input -- //
		while (scanInput)
		{
			if (scanInput && BIT0)		//Check if the LSchannel is enabled
				inputNb++;		//Count the channel
			scanInput >>= 1;		//Position for the next channel
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
* @arg		U8 adcPort				Hardware ADC ID
* @return	tADCScanInput inputMatrix		Input Matrix enabled
*/
tADCScanInput adcGetScan(U8 adcPort)
{
	if (adcSelectPort(adcPort) == STD_EC_SUCCESS)
		return pADxCSSL->CSSL;
}
// ========================== //

// == Conversion Functions == //
/**
* \fn		U32 adcGetScan(U8 adcPort)
* @brief	Wait for the ADC to be idle, and then initiated conversionNb of conversion on the selected channel
* @note		This function can jam everything...
*		Super ugly and crappy function, waiting for RTOS to be better
* @arg		U8 adcPort				Hardware ADC ID
* @arg		tADCInput adcInput			Analog input to convert
* @arg		U8 conversionNb				Number of conversion to do and round up (maximum 16)
* @arg		U32 * resultPtr				Pointer to store the result
* @return	U32 adcConversionID			ID of this conversion (used to check if the conversion is done)
*/
U32 adcConvert(U8 adcPort, tADCInput adcInput, U8 conversionNb, U16 * resultPtr)
{
	U8 errorCode;

	errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Handle boundary -- //
		if (conversionNb > 16)
			conversionNb = 16;
		// --------------------- //

		// Wait for the ADC to be idle
		while (!(pADxCON1->DONE));

		// -- Select the correct channel -- //
		pADxCHS->CH0NA = 0;			//Select VrefL as the negative input
		pADxCHS->CH0SA = adcInput;
		// -------------------------------- //

		adcResultPtr[adcPort] = resultPtr;	//Save the result pointer
		adcState[adcPort] = ADCbusy;

		pADxCON1->ASAM = 1;			//Auto mode
		pADxCON1->CLRASAM = 1;			//Stop after SMPI nb of conversion (clear ASAM automaticaly)
		pADxCON2->SMPI = conversionNb-1;	//Set the number of conversion to do

		pADxCON1->SAMP = 1;			//Start the sampling/conversion
	}

	return errorCode;
}

U8 adcStartScan(U8 adcPort)
{
	return STD_EC_NOTFOUND;
}

U8 adcStopScan(U8 adcPort)
{
	return STD_EC_NOTFOUND;
}
// ============================= //
// ############################################## //

