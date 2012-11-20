/*!
 @file		pic18_timer.h
 @brief		Timer functions for C18

 @version	0.2
 @note		This lib does not use gated feature of some timer
		The "fast" Macro can be used with explicit argument only, no variable (ex: timerClear(0), not timerClear(variable))

 @date		November 19th 2012
 @author	Laurence DV
*/
//TODO: Verify that (U8)data on a U16 just delete the msB

#ifndef _PIC18_TIMER_H
#define _PIC18_TIMER_H

// ################## Includes ################## //
//Hardware
#include <hardware.h>

//Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

//Dev macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// ####################################################################### Setting Map ################################################################ //
//		|	Bit 7		|	Bit 6	|	Bit 5	|	Bit 4	|	Bit 3	|	Bit 2	|	Bit 1	|	Bit 0	//
// #################################################################################################################################################### //
// TMR 0	|	Prescaler Flag	|	Size	|	Clock Source		|	Edge	|			Prescaler		//
// TMR odd	|	Prescaler Flag	|	Size	|	Clock Source		|	Sync	|	OSC En	|	Prescaler		//
// TMR even	|	Prescaler Flag	|	Postscaler				|	-	|			Prescaler		//
// #################################################################################################################################################### //
// ------ Clock Settings ------ //
//Clock sources		(Timer0 and Timer Odd only)
#define TMR_CS_INST		0				// FOSC/4				(TMR0 & TMR8)
#define	TMR_CS_FOSC		0x10				// FOSC					(TMR8)
#define	TMR_CS_PIN		0x20				// Input from the TxOSC or TxCKI	(TMR0 & TMR8)

//Prescaler			(All Timer)
#define	TMR_DIV_256		0x87				//TMR0
#define	TMR_DIV_128		0x86				//TMR0
#define	TMR_DIV_64		0x85				//TMR0
#define TMR_DIV_32		0x84				//TMR0
#define	TMR_DIV_16		0x83				//TMR0		TMR16
#define TMR_DIV_8		0x82				//TMR0	TMR8
#define	TMR_DIV_4		0x81				//TMR0	TMR8	TMR16
#define	TMR_DIV_2		0x80				//TMR0	TMR8
#define	TMR_DIV_1		0				//TMR0	TMR8	TMR16
// ---------------------------- //

// ------ Options ------ //
//* Timer 0 only *//
//Edge Selection
#define	TMR_RISING		0
#define	TMR_FALLING		0x08

//Timer Size
#define	TMR_8BIT		0x40
#define	TMR_16BIT		0
//* ------------ *//

//* Timer Odd only *//
//Exterior Clock Synchronization
#define	TMR_EXT_SYNC		0				// Sync for external clock on TxCKI
#define	TMR_EXT_SYNC_OFF	0x08

//Crystal Driver
#define TMR_OSC_ON		0x04				// Oscillator driver on pin TxOSC
#define	TMR_OSC_OFF		0

//Read/Write size
#define TMR_16BIT_READ		0x40				// TMRxH buffered Read/Write
#define TMR_8BIT_READ		0
//* -------------- *//

//* Timer Even only *//
#define TMR_POST_DIV_1		0
#define TMR_POST_DIV_2		0x08
#define TMR_POST_DIV_3		0x10
#define TMR_POST_DIV_4		0x18
#define TMR_POST_DIV_5		0x20
#define TMR_POST_DIV_6		0x28
#define TMR_POST_DIV_7		0x30
#define TMR_POST_DIV_8		0x38
#define TMR_POST_DIV_9		0x40
#define TMR_POST_DIV_10		0x48
#define TMR_POST_DIV_11		0x50
#define TMR_POST_DIV_12		0x58
#define TMR_POST_DIV_13		0x60
#define TMR_POST_DIV_14		0x68
#define TMR_POST_DIV_15		0x70
#define TMR_POST_DIV_16		0x78
//* --------------- *//
// --------------------- //

// == Timer HW ID == //
#define TIMER_0			0
#define TIMER_1			1
#define TIMER_2			2
#define TIMER_3			3
#define TIMER_4			4
#define TIMER_5			5
#define TIMER_6			6
#define TIMER_8			7
// ================= //
// ############################################## //


// ################# Data Type ################## //
// Timer CON
//Timer 0
typedef union
{
	U8 all;
	struct
	{
		U8 TxPS:3;		//Timer Prescaler Selection
		U8 PSA:1;		//Timer Prescaler En
		U8 TxSE:1;		//Timer Clock Edge Selection
		U8 TxCS:1;		//Timer Clock Source
		U8 Tx8BIT:1;		//Timer Data Width
		U8 TMRxON:1;		//Timer En/Dis
	};
}tT0CON;

//8bit Timers
typedef union
{
	U8 all;
	struct
	{
		U8 TxCKPS:2;		//Timer Clock Prescaler
		U8 TMRxON:1;		//Timer En/Dis
		U8 TxOUTPS:4;		//Timer Output Postscaler
		U8 :1;
	};
}tTxCON8;

//16bit Timers
typedef union
{
	U8 all;
	struct
	{
		U8 TMRxON:1;		//Timer En/Dis
		U8 RD16:1;		//Read Size (0: 8bit | 1: 16bit)
		U8 TxSYNC:1;		//External Sync (0: Sync | 1: not Sync)
		U8 TxOSCEN:1;		//Oscillator Driver En/Dis
		U8 TxCKPS:2;		//Timer Clock Prescaler
		U8 TMRxCS:2;		//Timer Clock Source
	};
}tTxCON16;

// Timer Gate CON //
typedef union
{
	U8 all;
	struct
	{
		U8 TGSS:2;		//Timer Gate Source
		U8 TGVAL:1;		//Timer Gate current Status
		U8 TGRDY:1;		//Timer Gate Ready Status (used only with TGSPM set)
		U8 TGSPM:1;		//Timer Gate Single Pulse Mode En/Dis
		U8 TGTM:1;		//Timer Gate Toggle Mode En/Dis
		U8 TGPOL:1;		//Timer Gate Count polarity (0: low | 1: high)
		U8 TMRGE:1;		//Timer Gate En/Dis
	};
}tTxGCON;

// Timer 0 Registers
typedef union
{
	U8 all[3];
	struct
	{
		volatile U8 TMRxH;
		volatile U8 TMRxL;
		volatile tT0CON TxCON;
	};
}tTimer0Reg;

// 8bit Timer Registers
typedef union
{
	U8 all[3];
	struct
	{
		volatile U8 TMRx;
		volatile U8 PRx;
		volatile tTxCON8 TxCON;
	};
}tTimer8Reg;

// 16bit Timer Registers
typedef union
{
	U8 all[3];
	struct
	{
		volatile U8 TMRxH;
		volatile U8 TMRxL;
		volatile tTxCON16 TxCON;
	};
}tTimer16Reg;
// -------------- //
// ############################################## //


// ################# Prototypes ################# //
// ==== Control ==== //
/**
* \fn		U8 timerInit(U8 timerID, U8 option)
* @brief	Initialise the selected timer with the specified options.
* @note		Use the Setting Map in the header for correct setting to send to this function
*		No sanity check of the settings, will return STD_EC_NOTFOUND if invalid timer ID is inputed.
* @arg		U8 timerID		Hardware Timer ID
* @arg		U8 option		Setting to configure for the timer
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerInit(U8 timerID, U8 option);

/**
* \fn		void timerSetOverflow(U8 timerID, U16 ovfPeriod)
* @brief	Set the timer overflow interrupt at the desired period
* @note		This function will round up to the possible freq depending on the actual globalCLK.
*		Only work with internal clock source.
*		Use the timerInit with the correct parameters for external clock source.
* @arg		U8 timerID			Hardware Timer ID
* @arg		U16 ovfPeriod			Total Period between overflow (in us)
* @return	nothing
*/
void timerSetOverflow(U8 timerID, U16 ovfPeriod);

/**
* \fn		U16 timerGetOverflow(U8 timerID)
* @brief	Return the overflow period of the specified timer
* @note		Will return 0 if either the timerID is invalid or the clock source
*		is external
* @arg		U8 timerID			Hardware Timer ID
* @return	U16 ovfPeriod			Overflow period (in µs)
*/
U16 timerGetOverflow(U8 timerID);

/**
* \fn		U16 timerGetTickPeriod(U8 timerID)
* @brief	Return the tick period of the specified timer
* @note		Will return 0 if either the timerID is invalid or the clock source
*		is external
* @arg		U8 timerID			Hardware Timer ID
* @return	U16 tickPeriod			Timer's tick period (in ns)
*/
U16 timerGetTickPeriod(U8 timerID);

/**
* \fn		void timerStart(U8 TimerID)
* @brief	Start the selected Timer
* @note		nothing
* @arg		U8 timerID		Hardware Timer ID
* @return	nothing
*/
void timerStart(U8 timerID);
#define timerFastStart(timerID)		CONCAT(_tmrStart,timerID)

/**
* \fn		void timerStop(U8 TimerID)
* @brief	Stop the selected Timer
* @note		nothing
* @arg		U8 timerID		Hardware Timer ID
* @return	nothing
*/
void timerStop(U8 TimerID);
#define timerFastStop(timerID)		CONCAT(_tmrStop,timerID)
// ================= //


// ==== Data Handling ==== //
/**
* \fn		void timerSet(U8 timerID, U8/U16 data)
* @brief	Set the selected Timer to the specified value
* @note		Depend on wich timer is selected, the data width will vary between 8bit and 16bit
		Timer 0 is set as a 16bit, if 8bit mode is selected use TMR0L directly
* @arg		U8 timerID		Hardware Timer ID
* @arg		U8/U16 data		Data to input in the timer
* @return	nothing
*/
void timerSet(U8 timerID, U16 data);
#define	timerFastSet(timerID,data)	CONCAT(_tmrSet,timerID)(data)

/**
* \fn		U8/U16 timerGet(U8 timerID)
* @brief	Return the value of the selected Timer
* @note		Depend on wich timer is selected, the return size will be 8bit or 16bit
		Timer 0 is returned as a 16bit, if 8bit mode is selected use TMR0L directly
* @arg		U8 timerID		Hardware Timer ID
* @return	U8/U16			Actual value of the timer
*/
U16 timerGet(U8 timerID);
#define timerFastGet(timerID)		CONCAT(_tmrGet,timerID)

/**
* \fn		void timerClear(U8 timerID)
* @brief	Reset to 0 the selected Timer
* @note		nothing
* @arg		U8 timerID		Hardware Timer ID
* @return	nothing
*/
#define timerClear(timerID)		timerSet(timerID,0)
#define timerFastClear(timerID)		CONCAT(_tmrClear,timerID)

/**
* \fn		void timerSetPR(U8 timerID, U8 data)
* @brief	Set the period register of the selected Timer
* @note		Will only work with timer that have a PRx Register (timer2,4,6 and 8)
* @arg		U8 timerID			Hardware Timer ID
* @arg		U8 data				Data to set the PRx reg
* @return	nothing
*/
void timerSetPR(U8 timerID, U8 data);

/**
* \fn		U8 timerGetPR(U8 timerID)
* @brief	Return the period register of the selected Timer
* @note		Will only work with timer that have a PRx Register (timer2,4,6 and 8)
* @arg		U8 timerID			Hardware Timer ID
* @return	U8 data				Value of the PRx reg
*/
U8 timerGetPR(U8 timerID);
// ======================= //
// ############################################## //




// ############### Internal Macro ############### //
// DO NOT USE THOSE MACRO DIRECTLY // 	(yeah, well you can, but... meh)
#define	_tmrStart0		__tmrStart(0)
#define	_tmrStop0		__tmrStop(0)
#define _tmrSet0(data)		__tmrSet16bit(0,data)
#define _tmrGet0		__tmrGet16bit(0)
#define _tmrClear0		__tmrClear16bit(0)

#define	_tmrStart1		__tmrStart(1)
#define	_tmrStop1		__tmrStop(1)
#define _tmrSet1(data)		__tmrSet16bit(1,data)
#define _tmrGet1		__tmrGet16bit(1)
#define _tmrClear1		__tmrClear16bit(1)

#define	_tmrStart2		__tmrStart(2)
#define	_tmrStop2		__tmrStop(2)
#define _tmrSet2(data)		__tmrSet8bit(2,data)
#define _tmrGet2		__tmrGet8bit(2)
#define _tmrClear2		__tmrClear8bit(2)

#define	_tmrStart3		__tmrStart(3)
#define	_tmrStop3		__tmrStop(3)
#define _tmrSet3(data)		__tmrSet16bit(3,data)
#define _tmrGet3		__tmrGet16bit(3)
#define _tmrClear3		__tmrClear16bit(3)

#define	_tmrStart4		__tmrStart(4)
#define	_tmrStop4		__tmrStop(4)
#define _tmrSet4(data)		__tmrSet8bit(4,data)
#define _tmrGet4		__tmrGet8bit(4)
#define _tmrClear4		__tmrClear8bit(4)

#if (CPU_FAMILY == PIC18FxxJ53) || (CPU_FAMILY == PIC18FxxK22)
	#define	_tmrStart5		__tmrStart(5)
	#define	_tmrStop5		__tmrStop(5)
	#define _tmrSet5(data)		__tmrSet16bit(5,data)
	#define _tmrGet5		__tmrGet16bit(5)
	#define _tmrClear5		__tmrClear16bit(5)

	#define	_tmrStart6		__tmrStart(6)
	#define	_tmrStop6		__tmrStop(6)
	#define _tmrSet6(data)		__tmrSet8bit(6,data)
	#define _tmrGet6		__tmrGet8bit(6)
	#define _tmrClear6		__tmrClear8bit(6)
#endif
#if CPU_FAMILY == PIC18FxxJ53
	#define	_tmrStart8		__tmrStart(8)
	#define	_tmrStop8		__tmrStop(8)
	#define _tmrSet8(data)		__tmrSet8bit(8,data)
	#define _tmrGet8		__tmrGet8bit(8)
	#define _tmrClear8		__tmrClear8bit(8)
#endif

#define __tmrStart(x)			CONCAT3(T,x,CONbits).CONCAT3(TMR,x,ON) = 1
#define __tmrStop(x)			CONCAT3(T,x,CONbits).CONCAT3(TMR,x,ON) = 0
#define __tmrClear16bit(x)		CONCAT3(TMR,x,H) = 0; CONCAT3(TMR,x,L) = 0
#define	__tmrClear8bit(x)		CONCAT(TMR,x) = 0
#define __tmrSet16bit(x,data)		CONCAT3(TMR,x,H) = (data>>8); CONCAT3(TMR,x,L) = (U8)data
#define __tmrSet8bit(x,data)		CONCAT(TMR,x) = data
#define	__tmrGet16bit(x)		(((U16)CONCAT3(TMR,x,H))<<8)+CONCAT3(TMR,x,L)
#define __tmrGet8bit(x)			CONCAT(TMR,x)
// ############################################## //

#endif
