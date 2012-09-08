/*!
 @file		pic32_adc.c
 @brief		ADC Control lib for pic32

 @version	0.1
 @note		
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

U8 adcInit(U8 adcPort)
{
	U8 errorCode;

	errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
	{
		//Do stuff
	}

	return errorCode;
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
	U8 errorCode;

	errorCode = adcSelectPort(adcPort);
	if (errorCode == STD_EC_SUCCESS)
		pADxCSSL->CSSL = scanInput;

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
* @brief	Return the enabled input matrix for the scan mode of the selected ADC port
* @note		Use tADCScanInput for the correct return value
* @arg		U8 adcPort					Hardware ADC ID
* @return	tADCScanInput inputMatrix	Input Matrix enabled
*/
U32 adcConvert(U8 adcPort, tADCInput adcInput, U8 conversionNb, U32 * resultPtr)
{


}

U8 adcStartConvert(U8 adcPort)
{

}

U8 adcStopConvert(U8 adcPort)
{

}
// ============================= //
// ############################################## //

