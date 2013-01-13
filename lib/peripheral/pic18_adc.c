/*!
 @file		pic18_adc.c
 @brief		ADC functions for PIC 18

 @version	0.2.1
 @note

 @date		November 9th 2011
 @author	Laurence DV
*/
//TODO: ADC functions in Sleep mode (probably need an RC clock function to be added)
//TODO: Check the "ADCON1bits.TRIGSEL" in the ADCSetTrigger for the K80 family
//TODO: ADCConvertDiff
//TODO: Add a better timeout mechanism to ensure the internal reference is start-up correctly (check ADCConfInternalRef())

// ################## Includes ################## //
#include "pic18_adc.h"
// ############################################## //


// ################## Defines ################### //
// -- Clock selection -- //
#define ADC_DIV_64	6
#define ADC_DIV_32	2
#define ADC_DIV_16	5
#define ADC_DIV_8	1
#define ADC_DIV_4	4
#define ADC_DIV_2	0
#define ADC_RC		3

#define ADC_TAD_20	7
#define ADC_TAD_16	6
#define ADC_TAD_12	5
#define ADC_TAD_8	4
#define ADC_TAD_6	3
#define ADC_TAD_4	2
#define ADC_TAD_2	1
#define ADC_TAD_0	0
// --------------------- //

// -- Resolution setting -- //
#define ADC_12BIT	0
#define ADC_10BIT	1
// ------------------------ //
// ############################################## //


// ################## Variables ################# //
tADCResult * pADCResult;					//Pointer to the deposit space for the ongoing conversion
extern U32 globalCLK;
U8 ADCScanModeLow = ANA_AN0;
U8 ADCScanModeHigh = ANA_AN0;
// ############################################## //


// ################ ADC Functions ############### //
// === Interrupt Handler ===== //
/**
* \fn		void ADCISR(void)
* @brief	Handle the ADC conversion Interrupt.
* @note		Put the result in the pADCResult, flag the new result
			and check for correct mode of operation.
* @arg		nothing
* @return	nothing
*/
void ADCISR(void)
{
	// -- Save the result -- //
	pADCResult->data = (U16)((((U16)ADRESH)<<8) + ADRESL);
	pADCResult->control.done = 1;							//Flag the new result
	// --------------------- //


	// -- Handle the correct mode -- //
	switch (pADCResult->control.mode)
	{
		case ADC_MODE_CONTINOUS:
		{
			pADCResult->control.conversionCnt++;

			if (!(pADCResult->control.stop))
				ADCON0bits.GO_NOT_DONE = 1;				//Start the new conversion
			break;
		}
		case ADC_MODE_COUNTDOWN:
		{
			pADCResult->control.conversionCnt--;

			if (pADCResult->control.conversionCnt)
				ADCON0bits.GO_NOT_DONE = 1;				//Start the new conversion

			break;
		}
		case ADC_MODE_SCAN:
		{
			//Select the new channel
			if (ADCON0bits.CHS == ADCScanModeHigh)
				ADCON0bits.CHS = ADCScanModeLow;
			else
				ADCON0bits.CHS++;

			//Start the new conversion if we were not tell to stop
			if (!(pADCResult->control.stop))
				ADCON0bits.GO_NOT_DONE = 1;
			break;
		}
	}
	// ----------------------------- //

	int_adc_clr();											//Clear the Interrupt Flag
}

// =========================== //


// === Control Functions ===== //
/**
* \fn		void ADCInit(void)
* @brief	Configure the ADC Module with default setting and adjust the ADC Clock
* @note		Power up the ADC module too.
* @arg		nothing
* @return	nothing
*/
void ADCInit(void)
{
	// -- Init the default setting -- //
	ADCSetRef(ADC_REF_P_AVDD|ADC_REF_N_AVSS);
	ADCJustify(RIGHT);
	ADCSetClock();
	// ------------------------------ //

	//Enable the ADC
	ADCON0bits.ADON = 1;

	// -- Init the Interrupt -- //
	int_adc_init();
	// ------------------------ //
}

/**
* \fn		void ADCSetClock(void)
* @brief	Reconfigure the ADC Clock setting for the current Main OSC
* @note		need access to the globalOSC variable
* @arg		nothing
* @return	nothing
*/
void ADCSetClock(void)
{
	#if CPU_FAMILY == PIC18FxxJ53
	U8 * tempPtr = 0x300005;
		// -- Set the correct Acquisition Time -- //
		if ((*tempPtr & 0x02) == ADC_12BIT)
			ADCON1bits.ACQT = ADC_TAD_16;				//Minimum 13 TAD for 12b resolution
		else
			ADCON1bits.ACQT = ADC_TAD_12;				//Minimum 11 TAD for 10b resolution
		// -------------------------------------- //


		// -- Set the correct Clock -- //				//Those are volontarely set for worst case (12bit conversion time)
		if (globalCLK <= 2500000)
			ADCON1bits.ADCS = ADC_DIV_2;
		else if (globalCLK <= 5000000)
			ADCON1bits.ADCS = ADC_DIV_4;
		else if (globalCLK <= 10000000)
			ADCON1bits.ADCS = ADC_DIV_8;
		else if (globalCLK <= 20000000)
			ADCON1bits.ADCS = ADC_DIV_16;
		else if (globalCLK <= 40000000)
			ADCON1bits.ADCS = ADC_DIV_32;
		else
			ADCON1bits.ADCS = ADC_DIV_64;
		// --------------------------- //


	#elif CPU_FAMILY == PIC18FxxK22
		// -- Set the correct Acquisition Time -- //
		ADCON2bits.ACQT = ADC_TAD_12;					//Minimum 11 TAD for 10b resolution
		// -------------------------------------- //


		// -- Set the correct Clock -- //
		if (globalOSC <= 1000000)
			ADCON2bits.ADCS = ADC_DIV_2;
		else if (globalOSC <= 4000000)
			ADCON2bits.ADCS = ADC_DIV_4;
		else if (globalOSC <= 16000000)
			ADCON2bits.ADCS = ADC_DIV_16;
		else
			ADCON2bits.ADCS = ADC_DIV_64;
		// --------------------------- //

	#elif CPU_FAMILY == PIC18FxxK80
		// -- Set the correct Acquisition Time -- //
		ADCON2bits.ACQT = ADC_TAD_16;					//Minimum 14 TAD for 12b resolution
		// -------------------------------------- //
		

		// -- Set the correct Clock -- //
		if (globalCLK <= 2500000)
			ADCON2bits.ADCS = ADC_DIV_2;
		else if (globalCLK <= 5000000)
			ADCON2bits.ADCS = ADC_DIV_4;
		else if (globalCLK <= 10000000)
			ADCON2bits.ADCS = ADC_DIV_8;
		else if (globalCLK <= 20000000)
			ADCON2bits.ADCS = ADC_DIV_16;
		else if (globalCLK <= 40000000)
			ADCON2bits.ADCS = ADC_DIV_32;
		else
			ADCON2bits.ADCS = ADC_DIV_64;
		// --------------------------- //
	#endif
}

/**
* \fn		void ADCSetChannel(U16 channels)
* @brief	Enable the selected Analog functions on the corresponding pin
* @note		You can + or | all the channel you want (ex: ADCSetChannel(ADC_AN0+ADC_AN1|ADC_AN2))
* @arg		U16 channels			Channels to be enabled, use "Channels Initialisation" definition
* @arg		U32 channels			Channels to be enabled, only used on device with more than 16 ch.
* @return	nothing
*/
#if CPU_FAMILY == PIC18FxxK22
	void ADCSetChannel(U32 channels)
#else
	void ADCSetChannel(U16 channels)
#endif
{
	#if CPU_FAMILY == PIC18FxxJ53
		ANCON0 = (U8)((channels & 0x00FF) ^ 0xFF);			//Inverse the value and put it in the first reg
		ANCON1 |= (U8)(((channels & 0x1F00)>>8) ^ 0x1F);	//Inverse the value and don't touch the VBGEN bit
	#elif CPU_FAMILY == PIC18FxxK22
		split32 splittedChannels;

		splittedChannels.all = channels;

		//Port A
		ANSELAbits.ANSA0 = splittedChannels.b0;
		ANSELAbits.ANSA1 = splittedChannels.b1;
		ANSELAbits.ANSA2 = splittedChannels.b2;
		ANSELAbits.ANSA3 = splittedChannels.b3;
		ANSELAbits.ANSA5 = splittedChannels.b4;

		//Port B
		ANSELBbits.ANSB0 = splittedChannels.b5;
		ANSELBbits.ANSB1 = splittedChannels.b6;
		ANSELBbits.ANSB2 = splittedChannels.b7;
		ANSELBbits.ANSB3 = splittedChannels.b8;
		ANSELBbits.ANSB4 = splittedChannels.b9;
		ANSELBbits.ANSB5 = splittedChannels.b10;

		//Port C
		ANSELCbits.ANSC2 = splittedChannels.b11;
		ANSELCbits.ANSC3 = splittedChannels.b12;
		ANSELCbits.ANSC4 = splittedChannels.b13;
		ANSELCbits.ANSC5 = splittedChannels.b14;
		ANSELCbits.ANSC6 = splittedChannels.b15;
		ANSELCbits.ANSC7 = splittedChannels.b16;

		//Port D
		ANSELDbits.ANSD0 = splittedChannels.b17;
		ANSELDbits.ANSD1 = splittedChannels.b18;
		ANSELDbits.ANSD2 = splittedChannels.b19;
		ANSELDbits.ANSD3 = splittedChannels.b20;
		ANSELDbits.ANSD4 = splittedChannels.b21;
		ANSELDbits.ANSD5 = splittedChannels.b22;
		ANSELDbits.ANSD6 = splittedChannels.b23;
		ANSELDbits.ANSD7 = splittedChannels.b24;

		//Port E
		ANSELEbits.ANSE0 = splittedChannels.b24;
		ANSELEbits.ANSE1 = splittedChannels.b25;
		ANSELEbits.ANSE2 = splittedChannels.b26;

	#elif CPU_FAMILY == PIC18FxxK80
		ANCON0 = (U8)(channels & 0x00FF);
		ANCON1 = (U8)((channels & 0xFF00)>>8);
	#endif
}

/**
* \fn		void ADCConfInternalRef(U8 settings)
* @brief	Configure the internal Voltage Reference (VBG or FVR)
* @note		Can be fixed or variable depending of the processor family
* @arg		U8 settings			Different settings to pass to the Voltage Ref
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConfInternalRef(U8 settings)
{
	#if CPU_FAMILY == PIC18FxxJ53
		U16 timeout;
		split8 splittedSettings;

		//En/Dis the Band-Gap Reference
		ANCON1bits.VBGEN = splittedSettings.b0;

		// -- Wait for the ref to be stable (max 10mS (120 000 cycle)) -- //
		timeout = 30000;
		while (timeout)
			timeout--;

		return STD_EC_SUCCESS;
		// -------------------------------------------------------------- //

	#elif CPU_FAMILY == PIC18FxxK22
		U16 timeout;
		split8 splittedSettings;

		//Set the Ref Level
		VREFCON0bits.FVRS0 = splittedSettings.b1;
		VREFCON0bits.FVRS1 = splittedSettings.b2;

		//En/Dis the Fixed Voltage Reference
		VREFCON0bits.FVREN = splittedSettings.b0;

		// -- Wait for the ref to be stable (max 100uS (1600 cycle)) -- //
		timeout = 400;
		while (!VREFCON0bits.FVRST)
		{
			timeout--;
			if (!timeout)
				return STD_EC_FAIL;
		}

		return STD_EC_SUCCESS;
		// ------------------------------------------------------------ //
	#else
		return STD_EC_SUCCESS;
	#endif
}

/**
* \fn		U8 ADCCalibrate(void)
* @brief	Start an auto calibration of the ADC
* @note		Will return STD_EC_FAIL if unsuccesful for whatever reason
* @arg		nothing					Channels to be converted
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
#if CPU_FAMILY == PIC18FxxJ53
U8 ADCCalibrate(void)
{
	U16 timeout = 0;
	tADCResult tempResult;

	ADCON1bits.ADCAL = SET;				//Plan a calibration for the next conversion
	ADCConvert(ANA_VCORE,&tempResult);	//Start a conversion of the VCORE

	while(!tempResult.control.done)		//Wait for the completion of the conversion
	{
		timeout++;
		if (timeout == 0xFFFF)
		{
			ADCON1bits.ADCAL = CLEAR;	//Put the ADC in normal mode
			return STD_EC_FAIL;
		}
	}

	ADCON1bits.ADCAL = CLEAR;			//Put the ADC in normal mode
	return STD_EC_SUCCESS;
}
#endif
// =========================== //


// === Convert Functions ===== //

/**
* \fn		void ADCConvert(U8 channel, tADCResult * pResult)
* @brief	Start a conversion on the selected channel and put the result in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
* @arg		U8 channel				Channels to be converted
* @arg		tADCResult * pResult	Result container for this channel
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConvert(U8 channel, tADCResult * pResult)
{
	if (!ADCON0bits.GO_NOT_DONE)
	{
		// -- Select Channels -- //
		ADCON0bits.CHS = channel;
		// --------------------- //


		// -- Init the container -- //
		pResult->control.done = 0;
		pResult->control.stop = 0;
		pResult->control.conversionCnt = 0;
		pADCResult = pResult;
		// ------------------------ //


		// -- Start the conversion -- //
		ADCON0bits.GO_NOT_DONE = 1;
		// -------------------------- //
	}
	else
		return STD_EC_BUSY;
	return STD_EC_SUCCESS;
}

/**
* \fn		void ADCConvert(U8 channel, tADCResult * pResult)
* @brief	Start a conversion on each channel between the two boundary and put the result in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
			Each channel will be converted one time in one cycle
			The ADC will cycle endlessly until the control.stop bit is set
* @arg		U8 channelLow			Low Boundary of the scan (inclusive)
* @arg		U8 channelHigh			High Boundary of the scan (inclusive)
* @arg		tADCResult * pResult	Result container for this channel
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConverScan(U8 channelLow, U8 channelHigh, tADCResult * pResult)
{
	if (!ADCON0bits.GO_NOT_DONE)
	{
		// -- Save Scan boundaries -- //
		ADCScanModeLow = channelLow;
		ADCScanModeHigh = channelHigh;
		// -------------------------- //

		// -- Start the conversion -- //
		ADCConvert(channelLow, pResult);
		// -------------------------- //
	}
	else
		return STD_EC_BUSY;
	return STD_EC_SUCCESS;
}


#if CPU_FAMILY == PIC18FxxK80
U8 ADCConvertDiff(U8 channel, U8 negativeChannel, tADCResult * pResult)
{
	if (!ADCON0bits.GO_NOT_DONE)
	{
		// -- Select Channels -- //
		// --------------------- //


		// -- Save the destination -- //
		// -------------------------- //


		// -- Set Refs -- //
		// -------------- //


		// -- Start the conversion -- //
		// -------------------------- //
	}
	else
		return STD_EC_BUSY;
	return STD_EC_SUCCESS;
}
#endif
// =========================== //
// ############################################## //
