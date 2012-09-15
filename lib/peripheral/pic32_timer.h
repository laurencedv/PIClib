/*!
 @file		pic32_timer.h
 @brief		Timer functions for pic32

 @version	0.2.3
 @note		timerSetOverflow and timerGetOverflow are really cpu intensive use wisely
 		The "fast" Macro can be used with explicit argument only, no variable (ex: timerFastClear(0), not timerFastClear(variable))
		The Timer 1 has only 4 prescaler option refer to the Clock Settings defines to see which one (all other timers have all the option)

 @date		March 2th 2012
 @author	Laurence DV
*/
//TODO: Verify that (U8)data on a U16 just delete the msB

#ifndef _PIC32_TIMER_H
#define _PIC32_TIMER_H 1

// ################## Includes ################## //
//Hardware
#include <hardware.h>

//Lib
#include <peripheral/pic32_clock.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

//Dev macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// ------ Project Dependant ------ //
#define TMR_NB_OF_TMR		5
// ------------------------------- //


// ------ Clock Settings ------ //
//Clock sources		(Timer0 and Timer Odd only)
#define	TMR_CS_CK_PIN		0x8			// Clock input pin
#define	TMR_CS_PBCLK		0			// Internal Peripheral clock

//Prescaler
#define	TMR_DIV_256		0x7			// Available for timer 1
#define	TMR_DIV_64		0x6			// Available for timer 1
#define TMR_DIV_32		0x5
#define	TMR_DIV_16		0x4
#define TMR_DIV_8		0x3			// Available for timer 1
#define	TMR_DIV_4		0x2
#define	TMR_DIV_2		0x1
#define	TMR_DIV_1		0			// Available for timer 1

//External Clock Synchronization
#define	TMR_EXT_SYNC		0x20			// Sync timer
#define	TMR_EXT_ASYNC		0			// Async timer
// ---------------------------- //


// ------ Options ------ //
//Timer Size
#define	TMR_32BIT		0x10			//(TMR2 & TMR3) || (TMR4 & TMR5)
#define	TMR_16BIT		0			//(TMR2) || (TMR3) || (TMR4) || (TMR5)

#define TMR_ASYNC_WRITE_DIS	0x40			//Sync write is delay after the current operation
#define TMR_ASYNC_WRITE_EN	0

#define TMR_IDLE_RUN		0			//Timer enabled in idle mode
#define TMR_IDLE_STOP		0x80			//Timer disabled in idle mode

#define TMR_FRZ_STOP		0x200			//Timer is freeze in debug mode
#define TMR_FRZ_RUN		0			//Timer is free-running in debug mode

#define TMR_GATE_EN		0x100			//Gated time accumulation enable
#define	TMR_GATE_DIS		0			//Gated time accumulation disable
// --------------------- //

// -- Timer HW ID -- //
#define TIMER_1			0
#define TIMER_2			1
#define TIMER_3			2
#define TIMER_4			3
#define TIMER_5			4
// ----------------- //
// ############################################## //


// ################# Data Type ################## //
// Timer control reg
typedef union 
{
	U32 all;
	struct
	{
		U32 :1;
		U32 TCS:1;			//Timer Clock Source
		U32 TSYNC:1;			//Timer Sync/Async control
		U32 T32:1;			//Join Timer to form 32bit timer
		U32 TCKPS:3;			//Timer Prescaler
		U32 TGATE:1;			//Timer Gated time accumulation
		U32 :3;
		U32 TWIP:1;			//Timer Async Write in progress flag
		U32 TWDIS:1;			//Timer Async Write disable
		U32 SIDL:1;			//Timer Disabled in sleep mode
		U32 FRZ:1;			//Timer Freeze in debug mode
		U32 ON:1;			//Timer Enable bit
		U32 :16;
	};
}tTCON;
// ############################################## //


// ################# Prototypes ################# //
// ==== ISR ======== //
/**
* \fn		void timer1ISR(void)
* @brief	Example of an Interrupt Service Routine for Timer1
* @note		Place it at the Timer 1 vector in the main
* @arg		nothing
* @return	nothing
*/
/*
void __ISR(_TIMER_1_VECTOR, IPL7SOFT) timer1ISR(void)
{

	INTClearFlag(INT_T1);
}
*/
// ================= //

// ==== Control ==== //
/**
* \fn		U8 timerInit(U8 timerPort, U32 option)
* @brief	Initialise the selected timer with the specified options.
* @note		Use the Setting Map in the header for correct setting to send to this function
*		No sanity check of the settings, will return STD_EC_NOTFOUND if invalid timer ID is inputed.
*		Option must be | or + (ex: timerInit(0, TMR_DIV_1|TMR_CS_PBCLK|TMR_16BIT))
*		For 32bit timer must be called for each timer in the pair.
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 option			Setting to configure for the timer
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerInit(U8 timerPort, U32 option);

/**
* \fn		void timerStart(U8 timerPort)
* @brief	Start the selected Timer
* @note		nothing
* @arg		U8 timerPort			Hardware Timer ID
* @return	nothing
*/
void timerStart(U8 timerPort);
#define timerFastStart(x)	CONCAT(__TST,x)

/**
* \fn		void timerStop(U8 timerPort)
* @brief	Stop the selected Timer
* @note		nothing
* @arg		U8 timerPort			Hardware Timer ID
* @return	nothing
*/
void timerStop(U8 timerPort);
#define timerFastStop(x)	CONCAT(__TSP,x)

/**
* \fn		U8 timerGetSize(U8 timerPort)
* @brief	Return the data width of the designated timer
* @note		nothing
* @arg		U8 timerPort			Hardware Timer ID
* @return	U8 dataSize			Timer data width in bits
*/
U8 timerGetSize(U8 timerPort);

/**
* \fn		U32 timerGetClock(U8 timerPort)
* @brief	Return the actual Timer clock source freq for the designated timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
*		is external
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32 clockFreq			Timer clock source freq (in Hz)
*/
U32 timerGetClock(U8 timerPort);

/**
* \fn		U8 timerSetOverflow(U8 timerPort, U32 ovfPeriod)
* @brief	Set the timer overflow interrupt at the desired period
* @note		This function will round up to the possible freq depending on the actual globalCLK.
*		Only work if using the PBCLK as the clock source.
*		Use the timerInit with the correct parameters for external clock source.
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 ovfPeriod			Total Period between overflow (in us)
* @return	U8 errorCode			STD Error Code
*/
U8 timerSetOverflow(U8 timerPort, F32 ovfPeriod);

/**
* \fn		U32 timerGetOverflow(U8 timerPort)
* @brief	Return the overflow period of the specified timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
*		is external
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32 ovfPeriod			Overflow period (in µs)
*/
U32 timerGetOverflow(U8 timerPort);

/**
* \fn		F32 timerGetCntPeriod(U8 timerPort)
* @brief	Return the count period of the specified timer
* @note		Will return 0 if either the timerPort is invalid or the clock source
*		is external
* @arg		U8 timerPort			Hardware Timer ID
* @return	F32 cntPeriod			Count period (in µs)
*/
F32 timerGetCntPeriod(U8 timerPort);

/**
* \fn		U8 timerSetPR(U8 timerPort, U32 PRvalue)
* @brief	Set the PR reg of the designated Timer to the specified value
* @note		Data Width can vary between 16 and 32 bits but it is automaticly handled
* @todo		Must check if T32 is set, but trying to access the odd timer
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 PRvalue			Value to set PR reg
* @return	errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 timerSetPR(U8 timerPort, U32 PRvalue);

/**
* \fn		U32 timerGetPR(U8 timerPort)
* @brief	Return the value of the PR reg of the designated timer
* @note		Data Width can vary between 16 and 32 bits but it is automaticly handled
*		Always return an Unsigned variable
* @todo		Must check if T32 is set, but trying to access the odd timer
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32 PRvalue			Value of the PR reg
*/
U32 timerGetPR(U8 timerPort);
// ================= //


// ==== Data Handling ==== //
/**
* \fn		void timerSet(U8 timerPort, U32 data)
* @brief	Set the selected Timer to the specified value
* @note		Depend on wich timer is selected, the data width will vary between 16bit and 32bit
* @arg		U8 timerPort			Hardware Timer ID
* @arg		U32 data			Data to input in the timer
* @return	nothing
*/
void timerSet(U8 timerPort, U32 data);
#define	timerFastSet(x,data)	(CONCAT(__TMR,x)) = data

/**
* \fn		U32 timerGet(U8 timerPort)
* @brief	Return the value of the selected Timer
* @note		The return size will always be 32bit but the valid width must be known
*		Use timerGetSize() to check the data width
*		Will return 0 if invalid timerPort is provided
* @arg		U8 timerPort			Hardware Timer ID
* @return	U32				Actual value of the timer
*/
U32 timerGet(U8 timerPort);
#define timerFastGet(x)		CONCAT(__TMR,x)

/**
* \fn		void timerClear(U8 timerPort)
* @brief	Reset to 0 the selected Timer
* @note		nothing
* @arg		U8 timerPort			Hardware Timer ID
* @return	nothing
*/
void timerClear(U8 timerPort);
#define timerFastClear(x)	CONCAT(__TMR,x) = 0
// ======================= //
// ############################################## //


// ######## Internal macro for fast use ######### //
#define __TMR0			TMR1
#define __TMR1			TMR2
#define __TMR2			TMR3
#define __TMR3			TMR4
#define __TMR4			TMR5

#define __TST0			T1CONSET = BIT15
#define __TST1			T2CONSET = BIT15
#define __TST2			T3CONSET = BIT15
#define __TST3			T4CONSET = BIT15
#define __TST4			T5CONSET = BIT15

#define __TSP0			T1CONCLR = BIT15
#define __TSP1			T2CONCLR = BIT15
#define __TSP2			T3CONCLR = BIT15
#define __TSP3			T4CONCLR = BIT15
#define __TSP4			T5CONCLR = BIT15
// ############################################## //

#endif
