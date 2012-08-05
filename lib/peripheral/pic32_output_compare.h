/*!
 @file		pic32_output_compare.h
 @brief		Output Compare control lib for pic32

 @version	0.1
 @note
 @todo

 @date		March 21th 2012
 @author	Laurence DV
*/


#ifndef _PIC32_OUTPUT_COMPARE_H
#define _PIC32_OUTPUT_COMPARE_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic32_interrupt.h>
#include <peripheral/pic32_timer.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// == Application dependant == //
// =========================== //

// == Config option == //
// Mode
#define OC_MODE_DISABLED			0
#define OC_MODE_COMPARE_HIGH		(BIT0)
#define OC_MODE_COMPARE_LOW			(BIT1)
#define OC_MODE_COMPARE_TOGGLE		(BIT1|BIT0)
#define OC_MODE_SINGLE_PULSE		(BIT2)
#define OC_MODE_CONTINUOUS_PULSE	(BIT2|BIT0)
#define OC_MODE_PWM					(BIT2|BIT1)
#define OC_MODE_PWM_FAULT			(BIT2|BIT1|BIT0)

// Timer selection
#define OC_TIMER_2					0
#define OC_TIMER_3					(BIT3)

// Compare width
#define OC_32BIT_OFF				0
#define OC_32BIT_ON					(BIT5)

// Idle mode status
#define OC_IDLE_RUN					0
#define OC_IDLE_STOP				(BIT13)
// =================== //

// == OC HW ID == //
#define OC_1		0
#define OC_2		1
#define	OC_3		2
#define OC_4		3
#define OC_5		4
// ============== //
// ############################################## //


// ################# Data Type ################## //
// Config
typedef union
{
	U32 all;
	struct
	{
		U32 mode:3;
		U32 timer:1;
		U32 :1;
		U32 width:1;
		U32 :7;
		U32 idle:1;
		U32 :18;
	};

	struct
	{
		U32 OCxCON;
	}registers;
}tOCConfig;

// OCxCON Register
typedef union
{
	U32 all;
	struct
	{
		U32 OCM:3;							//Output Compare Mode selection
		U32 OCTSEL:1;						//Timer selection
		U32 OCFLT:1;						//Fault flag
		U32 OC32:1;							//32bit mode Enable
		U32 :7;
		U32 SIDL:1;							//Idle mode
		U32 :1;
		U32 ON:1;							//Output Compare Enable
		U32 :16;
	};
}tOCxCON;
// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void FSCMISR(void)
* @brief	ISR for the Fail-Safe Clock Monitor
* @note
* @arg		nothing
* @return	nothing
*/

// =========================== //


// ==== Control Functions ==== //
/**
* \fn		void ocSetConfig(U8 ocPort, U32 localOcConfig)
* @brief	Save the configuration to the config struct of the specified OC port
* @note		The config can also be directly access via the config struct
*			For manual configuration, option must be | or + (ex: ocSetConfig(OC_1, OC_MODE_PWM|OC_TIMER_2))
* @arg		U8 ocPort					Hardware OC ID
* @arg		U32 ocLocalConfig			Setting to configure for the OC
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ocSetConfig(U8 ocPort, U32 localOcConfig);

/**
* \fn		tOCConfig ocGetConfig(U8 ocPort)
* @brief	Return the configuration of the specified OC port
* @note		The config can also be directly access via the config struct
* @arg		U8 ocPort					Hardware OC ID
* @return	tOCConfig ocLocalConfig		Setting to configure for the OC
*/
tOCConfig ocGetConfig(U8 ocPort);

/**
* \fn		U8 ocStart(U8 ocPort)
* @brief	Configure and start a specified OC port
* @note		If any error occur during the starting, will shutdown the specified OC port and return the error code
 *			Execute lengthy computation to save human-readable time information
* @arg		U8 ocPort					Hardware OC ID
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 ocStart(U8 ocPort);
// =========================== //

// ====== PWM Functions ====== //
/**
* \fn		void pwmSetPeriod(U8 ocPort, U32 newPeriod)
* @brief	Set the correct timer's period for the specified OC port
* @note		Will not check if any other OC port is using the same timer
*			Will update the pwmPeriodTime and pwmCntPeriodTime value
* @arg		U8 ocPort					Hardware OC ID
* @arg		U32 newPeriod				New period (in µs)
* @return	nothing
*/
void pwmSetPeriod(U8 ocPort, U32 newPeriod);

/**
* \fn		U32 pwmGetPeriod(U8 ocPort)
* @brief	Return the correct timer's period for the specified OC port
* @note		CPU intensive, use the pwmPeriodTime array for faster access
* @arg		U8 ocPort					Hardware OC ID
* @return	U32 newPeriod				New period (in µs)
*/
U32 pwmGetPeriod(U8 ocPort);

/**
* \fn		U8 pwmSetTon(U8 ocPort, U32 timeON)
* @brief	Set the ON time for a specified OC port
* @note		Will not update if the ON time is less than the period time (and will return STD_EC_TOOLARGE)
*			ATTENTION! This function use floating point math for better precision
* @arg		U8 ocPort					Hardware OC ID
* @arg		U32 timeON					ON time desired (in µs)
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 pwmSetTon(U8 ocPort, U32 timeON);

/**
* \fn		void pwmSetDuty(U8 ocPort, U32 numerator, U32 denominator)
* @brief	Set the duty of the specified OC port
* @note		Use the numerator and the denominator as a fraction representing the duty (ex: pwmSetDuty(OC_1,1,2) will set to 50%)
*			This function use integer, for more precision use the OCxRS register directly
* @arg		U8 ocPort					Hardware OC ID
* @arg		U32 numerator				Numerator of the duty
* @arg		U32 denominator				Denominator of the duty
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
void pwmSetDuty(U8 ocPort, U32 numerator, U32 denominator);
// ========================== //
// ############################################## //

#endif
