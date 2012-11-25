/*!
 @file		pic18_ccp.h
 @brief		CCP lib for C18 and XC8

 @version	0.1
 @note		PWM duty as only a precision of 8bit (datasheet not clear on the 10bit period operation)
 @todo

 @date		November 11th 2012
 @author	Laurence DV
*/

#ifndef _PIC18_CCP_H
#define _PIC18_CCP_H

// ################# Includes ################### //
// Hardware
#include <hardware.h>

// Librairies
#include <peripheral/pic18_timer.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################# Defines #################### //
//CCP Mode Selection
#define CCP_MODE_DISABLE		0	
#define CCP_MODE_COMPARE_TOGGLE		0x2	
#define CCP_MODE_CAPTURE_FALLING	0x4	
#define CCP_MODE_CAPTURE_RISING		0x5	
#define CCP_MODE_CAPTURE_FALLING_4	0x6	
#define CCP_MODE_CAPTURE_FALLING_16	0x7	
#define CCP_MODE_COMPARE_SET_HIGH	0x8	
#define CCP_MODE_COMPARE_SET_LOW	0x9	
#define CCP_MODE_COMPARE_INT		0xA	
#define CCP_MODE_COMPARE_SPECIAL_EVENT	0xB	
#define CCP_MODE_PWM			0xC	

//CCP Timer Selection	(check Datasheet for the correct config of the correct CCP module)
#define CCP_TMR_1_2			0
#define CCP_TMR_5_4			1
#define CCP_TMR_5_6			2
#define CCP_TMR_5_8			3
#define CCP_TMR_5_2			1
#define CCP_TMR_5_4			1
#define CCP_TMR_3_4			1
#define CCP_TMR_3_6			2
#define CCP_TMR_1_4			1
#define CCP_TMR_1_6			2

// == CCP Port == //
#define CCP_1				0
#define CCP_2				1
#define CCP_3				2
#define CCP_4				3
#define CCP_5				4
#define CCP_6				5
#define CCP_7				6
#define CCP_8				7
#define CCP_9				8
#define CCP_10				9
// ============== //
// ############################################## //


// ################# Data Type ################## //
// CCPxCON Reg
typedef union
{
	U8 all;
	struct
	{
		U8 CCPM:4;	//CCP module Mode
		U8 DCxB:2;	//PWM Duty LSb
		U8 :2;		
	};
}tCCPxCON;

// CCPTMRS1 Reg
typedef union
{
	U8 all;
	struct
	{
		U8 C4TSEL:2;	//CCP4 Timer selection
		U8 C5TSEL:1;	//CCP5 Timer selection
		U8 :1;
		U8 C6TSEL:1;	//CCP6 Timer selection
		U8 :1;
		U8 C7TSEL:2;	//CCP7 Timer selection
	};
}tCCPTMRS1;

// CCPTMRS2 Reg
typedef union
{
	U8 all;
	struct
	{
		U8 C8TSEL:2;	//CCP8 Timer selection
		U8 C9TSEL:1;	//CCP9 Timer selection
		U8 :1;
		U8 C10TSEL0:1;	//CCP10 Timer selection
		U8 :3;
	};
}tCCPTMRS2;

// CCPRxL Reg
typedef union
{
	U8 all;
	U8 CCPRxL;		//Period Low Register
}tCCPRxL;

// CCPRxH Reg
typedef union
{
	U8 all;
	U8 CCPRxH;		//Period High Register
}tCCPRxH;

// Registers structure
typedef struct
{
	volatile tCCPRxH CCPRxH;
	volatile tCCPRxL CCPRxL;
	volatile tCCPxCON CCPxCON;
}tCCPReg;

// Control structure
typedef union
{
	U8 all[1];
	struct
	{
		U8 timerID:3;			//Selected timer for the actual mode
		U8 :1;
		tFSMState state:4;		//State of the CCP
	};
}tCCPCtl;
// ############################################## //


// ############ Primary functions ############### //
/**
* \fn		void ccpISR(U8 ccpID)
* @brief	Interrupt Handler for any CCP port
* @note		Place this in the correct ISR in the main
* @arg		U8 ccpID			Hardware CCP ID
* @return	nothing
*/
void ccpISR(U8 ccpID);

/**
* \fn		void ccpEngine(U8 ccpID)
* @brief	Engine function for advance CCP fonction
* @note		Place this in the infinite loop of the main
* @arg		U8 ccpID			Hardware CCP ID
* @return	nothing
*/
void ccpEngine(U8 ccpID);

/**
* \fn		U8 ccpInit(U8 ccpID, U8 timerSetting, U8 mode)
* @brief	Initialize a CCP to a stand-by state
* @note		Start the timer to start the CCP function
*		This function will not initialise the timer (must be done externally)
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U8 timerSetting			Timer to attach to the CCP
* @arg		U8 mode				Mode to put the CCP into
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
void ccpInit(U8 ccpID, U8 timerSetting, U8 mode);

/**
* \fn		void ccpSetPeriod(U8 ccpID, U16 newPeriod)
* @brief	Set the Period Register
* @note
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U16 newPeriod			Period value to set (in ns)
* @return	nothing
*/
void ccpSetPeriod(U8 ccpID, U16 newPeriod);

/**
* \fn		U16 ccpGetPeriod(U8 ccpID)
* @brief	Return the actual Period value in ns
* @note
* @arg		U8 ccpID			Hardware CCP ID
* @return	U16 newPeriod			Period value (in ns)
*/
U16 ccpGetPeriod(U8 ccpID);
// ############################################## //


// ############ Compare functions ############### //
void ccpSetCompare(U8 ccpID, U16 newCompare);

U16 ccpGetCompare(U8 ccpID);
// ############################################## //


// ############ Capture functions ############### //
U16 ccpGetCapture(U8 ccpID);
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
void ccpSetDuty(U8 ccpID, U8 numerator, U8 denominator);

/**
* \fn		void ccpFastSetDuty(U8 ccpID, U16 newDuty)
* @brief	Direct set of the duty register
* @note		This function will not check if the duty is compatible with the actual PWM period (use ccpSetDuty for that)
* @arg		U8 ccpID			Hardware CCP ID
* @arg		U16 newDuty			New value for the duty (maximum 10bit)
* @return	nothing
*/
void ccpFastSetDuty(U8 ccpID, U16 newDuty);

/**
* \fn		U16 ccpGetDuty(U8 ccpID)
* @brief	Return the duty of the specified CCP
* @note		The duty as a max width of 10bit
* @arg		U8 ccpID			Hardware CCP ID
* @return	U16 duty			Duty value
*/
U16 ccpGetDuty(U8 ccpID);
// ############################################## //

#endif
