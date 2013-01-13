/*!
 @file		pic18_adc.h
 @brief		ADC functions for PIC 18
            
 @version	0.2
 @note

 @date		November 9th 2011
 @author	Laurence DV
*/
//TODO: Add result format to the control part of the tADCResult


#ifndef _PIC18_ADC_H
#define _PIC18_ADC_H

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic18_clock.h>
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //

// -- Conversion Mode -- //
#define	ADC_MODE_NORMAL		0
#define	ADC_MODE_CONTINOUS	1
#define ADC_MODE_COUNTDOWN	2
#define ADC_MODE_SCAN		3
// --------------------- //

// -- Channels Initialisation -- //
#if CPU_FAMILY == PIC18Fx7Jx3
	#define ADC_NONE	0
	#define ADC_AN0		0x0001
	#define ADC_AN1		0x0002
	#define ADC_AN2		0x0004
	#define ADC_AN3		0x0008
	#define ADC_AN4		0x0010
	#define ADC_AN5		0x0020
	#define ADC_AN6		0x0040
	#define ADC_AN7		0x0080
	#define ADC_AN8		0x0100
	#define ADC_AN9		0x0200
	#define ADC_AN10	0x0400
	#define ADC_AN11	0x0800
	#define ADC_AN12	0x1000

#elif CPU_FAMILY == PIC18FxxK22
	#define ADC_NONE	0
	#define ADC_AN0		0x00000001
	#define ADC_AN1		0x00000002
	#define ADC_AN2		0x00000004
	#define ADC_AN3		0x00000008
	#define ADC_AN4		0x00000010
	#define ADC_AN5		0x02000000
	#define ADC_AN6		0x04000000
	#define ADC_AN7		0x08000000
	#define ADC_AN8		0x00000080
	#define ADC_AN9		0x00000100
	#define ADC_AN10	0x00000040
	#define ADC_AN11	0x00000200
	#define ADC_AN12	0x00000020
	#define ADC_AN13	0x00000400
	#define ADC_AN14	0x00000800
	#define ADC_AN15	0x00001000
	#define ADC_AN16	0x00002000
	#define ADC_AN17	0x00004000
	#define ADC_AN18	0x00008000
	#define ADC_AN19	0x00010000
	#define ADC_AN20	0x00020000
	#define ADC_AN21	0x00040000
	#define ADC_AN22	0x00080000
	#define ADC_AN23	0x00100000
	#define ADC_AN24	0x00200000
	#define ADC_AN25	0x00400000
	#define ADC_AN26	0x00800000
	#define ADC_AN27	0x01000000

#elif CPU_FAMILY == PIC18FxxK80
	#define ADC_NONE	0
	#define ADC_AN0		0x0001
	#define ADC_AN1		0x0002
	#define ADC_AN2		0x0004
	#define ADC_AN3		0x0008
	#define ADC_AN4		0x0010
	#define ADC_AN5		0x0020
	#define ADC_AN6		0x0040
	#define ADC_AN7		0x0080
	#define ADC_AN8		0x0100
	#define ADC_AN9		0x0200
	#define ADC_AN10	0x0400
	#define ADC_AN11	0x0800
	#define ADC_AN12	0x1000
	#define ADC_AN13	0x2000
	#define ADC_AN14	0x4000

#endif
// ----------------------------- //

// -- Channels Selection -- //
#if CPU_FAMILY == PIC18Fx7Jx3
	#define ANA_AN0			0
	#define ANA_AN1			1
	#define ANA_AN2			2
	#define ANA_AN3			3
	#define ANA_AN4			4
	#define ANA_AN5			5
	#define ANA_AN6			6
	#define ANA_AN7			7
	#define ANA_AN8			8
	#define ANA_AN9			9
	#define ANA_AN10		10
	#define ANA_AN11		11
	#define ANA_AN12		12
	#define ANA_VBG			14
	#define ANA_VCORE		15

#elif CPU_FAMILY == PIC18FxxK22
	#define ANA_AN0			0
	#define ANA_AN1			1
	#define ANA_AN2			2
	#define ANA_AN3			3
	#define ANA_AN4			4
	#define ANA_AN5			5
	#define ANA_AN6			6
	#define ANA_AN7			7
	#define ANA_AN8			8
	#define ANA_AN9			9
	#define ANA_AN10		10
	#define ANA_AN11		11
	#define ANA_AN12		12
	#define ANA_AN13		13
	#define ANA_AN14		14
	#define ANA_AN15		15
	#define ANA_AN16		16
	#define ANA_AN17		17
	#define ANA_AN18		18
	#define ANA_AN19		19
	#define ANA_AN20		20
	#define ANA_AN21		21
	#define ANA_AN22		22
	#define ANA_AN23		23
	#define ANA_AN24		24
	#define ANA_AN25		25
	#define ANA_AN26		26
	#define ANA_AN27		27
	#define ANA_CTMU		29
	#define ANA_DAC			30
	#define	ANA_VBG			31

#elif CPU_FAMILY == PIC18FxxK80
	#define ANA_AN0			0
	#define ANA_AN1			1
	#define ANA_AN2			2
	#define ANA_AN3			3
	#define ANA_AN4			4
	#define ANA_AN5			5
	#define ANA_AN6			6
	#define ANA_AN7			7
	#define ANA_AN8			8
	#define ANA_AN9			9
	#define ANA_AN10		10
	#define ANA_AN11		11
	#define ANA_AN12		12
	#define ANA_AN13		13
	#define ANA_AN14		14
	#define ANA_MUX_DIS		28
	#define ANA_CORE_TEMP		29
	#define ANA_VCORE		30
	#define ANA_VBG			31

#endif
// ------------------------ //

// -- Ref selection -- //
#if CPU_FAMILY == PIC18Fx7Jx3
	#define ADC_REF_P_AVDD		0
	#define ADC_REF_P_AN3		0x40
	#define ADC_REF_N_AVSS		0
	#define ADC_REF_N_AN2		0x80

#elif CPU_FAMILY == PIC18FxxK22
	#define ADC_REF_P_AVDD		0
	#define ADC_REF_P_AN3		0x4
	#define ADC_REF_P_VBG		0x8
	#define ADC_REF_N_AVSS		0
	#define ADC_REF_N_AN2		0x1

#elif CPU_FAMILY == PIC18FxxK80
	#define ADC_REF_P_AVDD		0
	#define ADC_REF_P_AN3		0x10
	#define ADC_REF_P_VBG_2V	0x20
	#define ADC_REF_P_VBG_4V	0x30
	#define ADC_REF_N_AVSS		0
	#define ADC_REF_N_AN2		0x08
#endif
// ------------------- //

// -- Internal Ref Setting -- //
#if CPU_FAMILY == PIC18Fx7Jx3
	#define INT_REF_OFF		0
	#define INT_REF_ON		0x01
#elif CPU_FAMILY == PIC18FxxK22
	#define INT_REF_OFF		0
	#define INT_REF_ON		0x01
	#define INT_REF_1V		0x02
	#define INT_REF_2V		0x04
	#define INT_REF_4V		0x06
#elif CPU_FAMILY == PIC18FxxK80
	#define INT_REF_OFF		0
	#define INT_REF_ON		0x01

#endif
// -------------------------- //

// -- Trigger selection -- //
#if CPU_FAMILY == PIC18Fx7Jx3
	#define	ADC_TRIG_ECCP2		0
	#define	ADC_TRIG_CTMU		1
	#define	ADC_TRIG_TMR1		2
	#define	ADC_TRIG_RTCC		3

#elif CPU_FAMILY == PIC18FxxK22
	#define	ADC_TRIG_CCP5		0
	#define	ADC_TRIG_CTMU		1

#elif CPU_FAMILY == PIC18FxxK80
	#define	ADC_TRIG_ECCP1		0
	#define	ADC_TRIG_CTMU		1
	#define	ADC_TRIG_TMR1		2
	#define	ADC_TRIG_CCP2		3

#endif
// ----------------------- //
// ############################################## //


// ################# Data Type ################## //
// Result Container
typedef union
{
	U8 all[3];
	struct
	{
		U16 data;
		struct
		{
			U8 done:1;			//Conversion Done Flag
			U8 mode:2;			//Conversion Mode for this channel
			U8 stop:1;			//Stop bit for continous and scan mode
			U8 conversionCnt:4;		//Number of conversion done on this channel in continous mode
		}control;
	};
}tADCResult;
// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void ADCISR(void)
* @brief	Handle the ADC conversion Interrupt.
* @note		Put the result in the pADCResult, flag the new result
		and check for correct mode of operation.
* @arg		nothing
* @return	nothing
*/
void ADCISR(void);
// =========================== //


// === Control Functions ===== //
/**
* \fn		void ADCInit(void)
* @brief	Configure the ADC Module with default setting and adjust the ADC Clock
* @note		Power up the ADC module too.
* @arg		nothing
* @return	nothing
*/
void ADCInit(void);


/**
* \fn		void ADCSetClock(void)
* @brief	Reconfigure the ADC Clock setting for the current Main OSC
* @note		need access to the globalOSC variable
* @arg		nothing
* @return	nothing
*/
void ADCSetClock(void);

/**
* \fn		void ADCSetTrigger(U8 trigger)
* @brief	Configure the Special Trigger for the selected event
* @note		This is a macro function, it only set the TRIGSEL bit in the correct register
* @arg		U8 trigger			The trigger wanted (use "Trigger selection" definition)
* @return	nothing
*/
#if CPU_FAMILY == PIC18Fx7Jx3
	#define	ADCSetTrigger(trigger)			(ADCTRIGbits.TRIGSEL = trigger & 0x03)
#elif CPU_FAMILY == PIC18FxxK22
	#define ADCSetTrigger(trigger)			(ADCON1bits.TRIGSEL = trigger & 0x80)
#elif CPU_FAMILY == PIC18FxxK80
	#define	ADCSetTrigger(trigger)			(ADCON1bits.TRIGSEL = trigger & 0x03)
#endif

/**
* \fn		void ADCSetRef(U8 references)
* @brief	Configure the ADC Positive and Negative References
* @note		This is a macro function, it only set VCFG and VNCFG in the correct register
		Be aware of the possibility for the currently selected processor
* @arg		U8 references			References setting (use "Ref Selection" definition)
* @return	nothing
*/
#if CPU_FAMILY == PIC18Fx7Jx3
	#define ADCSetRef(references)			(ADCON0 |= (references & 0xC0))
#elif CPU_FAMILY == PIC18FxxK22
	#define ADCSetRef(references)			(ADCON1 |= (references & 0x0F))
#elif CPU_FAMILY == PIC18FxxK80
	#define ADCSetRef(references)			(ADCON1 |= (references & 0x38))
#endif

/**
* \fn		ADCJustify(U8 side)
* @brief	Configure the justification of the 12bit result in the 16bit result register
* @note		This is a macro function, it only set ADFM in the correct register
* @arg		U8 side					Side to justify on the result (use stddef LEFT/RIGHT)
* @return	nothing
*/
#if CPU_FAMILY == PIC18Fx7Jx3
	#define ADCJustify(side)			ADCON1bits.ADFM = side;
#elif CPU_FAMILY == PIC18FxxK22
	#define ADCJustify(side)			ADCON2bits.ADFM = side;
#elif CPU_FAMILY == PIC18FxxK80
	#define ADCJustify(side)			ADCON2bits.ADFM = side;
#endif

/**
* \fn		void ADCSetChannel(U16 channels)
* @brief	Enable the selected Analog functions on the corresponding pin
* @note		You can + or | all the channel you want (ex: ADCSetChannel(ADC_AN0+ADC_AN1|ADC_AN2))
* @arg		U16 channels			Channels to be enabled, use "Channels Initialisation" definition
* @arg		U32 channels			Channels to be enabled, only used on device with more than 16 ch.
* @return	nothing
*/
#if CPU_FAMILY == PIC18FxxK22
	void ADCSetChannel(U32 channels);
#else
	void ADCSetChannel(U16 channels);
#endif

/**
* \fn		void ADCConfInternalRef(U8 settings)
* @brief	Configure the internal Voltage Reference (VBG or FVR)
* @note		Can be fixed or variable depending of the processor family
* @arg		U8 settings			Different settings to pass to the Voltage Ref
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConfInternalRef(U8 settings);

/**
* \fn		U8 ADCCalibrate(void)
* @brief	Start an auto calibration of the ADC
* @note		Will return STD_EC_FAIL if unsuccesful for whatever reason
* @arg		nothing				Channels to be converted
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
#if CPU_FAMILY == PIC18Fx7Jx3
U8 ADCCalibrate(void);
#endif
// =========================== //


// === Convert Functions ===== //
/**
* \fn		void ADCConvert(U8 channel, tADCResult * pResult)
* @brief	Start a conversion on the selected channel and put the result in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
* @arg		U8 channel			Channels to be converted
* @arg		tADCResult * pResult		Result container for this channel
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConvert(U8 channel, tADCResult * pResult);

/**
* \fn		void ADCConvertContinous(U8 channel, tADCResult * pResult)
* @brief	Start continous conversions on the selected channel and put the results in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
* @arg		U8 channel				Channels to be converted
* @arg		tADCResult * pResult	Result container for this channel
* @return	nothing
*/
#define ADCConvertContinous(channel, pResult)	pResult->control.mode = ADC_MODE_CONTINOUS;\
						ADCConvert(channel, pResult)

/**
* \fn		void ADCConvertContinous(U8 channel, tADCResult * pResult)
* @brief	Start $conversionNb number of conversions on the selected channel and put the results in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
* @arg		U8 channel			Channels to be converted
* @arg		tADCResult * pResult		Result container for this channel
* @arg		U8 conversionNb			Number of conversion to do (max 15)
* @return	nothing
*/
#define ADCConvertCountdown(channel, pResult, conversionNb)	pResult->control.mode = ADC_MODE_COUNTDOWN;\
								pResult->control.conversionCnt = conversionNb;\
								ADCConvert(channel, pResult)

/**
* \fn		void ADCConvert(U8 channel, tADCResult * pResult)
* @brief	Start a conversion on each channel between the two boundary and put the result in the ADC Result container
* @note		Check the control.done flag in the container to know when a new conversion has been completed
		Each channel will be converted one time in one cycle
		The ADC will cycle endlessly until the control.stop bit is set
* @arg		U8 channelLow			Low Boundary of the scan (inclusive)
* @arg		U8 channelHigh			High Boundary of the scan (inclusive)
* @arg		tADCResult * pResult		Result container for this channel
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 ADCConverScan(U8 channelLow, U8 channelHigh, tADCResult * pResult);
// =========================== //
// ############################################## //

#endif
