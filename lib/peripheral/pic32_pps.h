/*!
 @file		pic32_pps.h
 @brief		Peripheral Pin Select lib for pic32

 @version	0.1
 @note		Always respect the group or you could attach an unknow peripheral to a pins.
 			Ex: Peripherals in "PPS in Group 1" can only be attached to the pins in "PPS in Group 1"
 			Use only the significant part of the names (ex: PPS_IN_INT4 would be INT4)
 @todo		

 @date		February 13th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_PPS_H
#define _PIC32_PPS_H 1
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// ------ Input PPS ------ //
// -- PPS In Group 1 -- //
//Peripherals
#define PPS_IN_INT4			INT4R
#define PPS_IN_T2CK			T2CKR
#define PPS_IN_IC4			IC4R
#define PPS_IN_SS1			SS1R
#define PPS_IN_REFCLKI		REFCLKIR

//Pins
#define PPS_PIN_RPA0		0
#define PPS_PIN_RPB3		1
#define PPS_PIN_RPB4		2
#define PPS_PIN_RPB7		4
#define PPS_PIN_RPB15		3
#define PPS_PIN_RPC0		6
#define PPS_PIN_RPC5		7
#define PPS_PIN_RPC7		5

// -- PPS In Group 2 -- //
//Peripherals
#define PPS_IN_INT3			INT3R
#define PPS_IN_T3CK			T3CKR
#define PPS_IN_IC3			IC3R
#define PPS_IN_U1CTS		U1CTSR
#define PPS_IN_U2RX			U2RXR
#define PPS_IN_SDI1			SDI1R

//Pins
#define PPS_PIN_RPA1		0
#define PPS_PIN_RPA8		5
#define PPS_PIN_RPA9		7
#define PPS_PIN_RPB1		2
#define PPS_PIN_RPB5		1
#define PPS_PIN_RPB8		4
#define PPS_PIN_RPB11		3
#define PPS_PIN_RPC8		6

// -- PPS In Group 3 -- //
//Peripherals
#define PPS_IN_INT2			INT2R
#define PPS_IN_T4CK			T4CKR
#define PPS_IN_IC1			IC1R
#define PPS_IN_IC5			IC5R
#define PPS_IN_U1RX			U1RXR
#define PPS_IN_U2CTS		U2CTSR
#define PPS_IN_SDI2			SDI2R
#define PPS_IN_OCFB			OCFBR

//Pins
#define PPS_PIN_RPA2		0
#define PPS_PIN_RPA4		2
#define PPS_PIN_RPB2		4
#define PPS_PIN_RPB6		1
#define PPS_PIN_RPB13		3
#define PPS_PIN_RPC1		6
#define PPS_PIN_RPC3		7
#define PPS_PIN_RPC6		5

// -- PPS In Group 4 -- //
//Peripherals
#define PPS_IN_INT1			INT1R
#define PPS_IN_T5CK			T5CKR
#define PPS_IN_IC2			IC2R
#define PPS_IN_SS2			SS2R
#define PPS_IN_OCFA			OCFAR

//Pins
#define PPS_PIN_RPA3		0
#define PPS_PIN_RPB0		2
#define PPS_PIN_RPB9		4
#define PPS_PIN_RPB10		3
#define PPS_PIN_RPB14		1
#define PPS_PIN_RPC2		6
#define PPS_PIN_RPC4		7
#define PPS_PIN_RPC9		5
// -------------------- //
// ------------------------ //

// ------ Output PPS ------ //
// -- PPS Out Group 1 -- //
//Pins
#define PPS_POUT_RPA0		RPA0R
#define PPS_POUT_RPB3		RPB3R
#define PPS_POUT_RPB4		RPB4R
#define PPS_POUT_RPB7		RPB7R
#define PPS_POUT_RPB15		RPB15R
#define PPS_POUT_RPC0		RPC0R
#define PPS_POUT_RPC5		RPC5R
#define PPS_POUT_RPC7		RPC7R

//Peripherals
#define PPS_OUT_NULL		0
#define PPS_OUT_U1TX		1
#define PPS_OUT_U2RTS		2
#define PPS_OUT_SS1			3
#define PPS_OUT_OC1			5
#define PPS_OUT_C2OUT		7

// -- PPS Out Group 2 -- //
//Pins
#define PPS_POUT_RPA1		RPA1R
#define PPS_POUT_RPA8		RPA8R
#define PPS_POUT_RPA9		RPA9R
#define PPS_POUT_RPB1		RPB1R
#define PPS_POUT_RPB5		RPB5R
#define PPS_POUT_RPB8		RPB8R
#define PPS_POUT_RPB11		RPB11R
#define PPS_POUT_RPC8		RPC8R

//Peripherals
#define PPS_OUT_NULL		0
#define PPS_OUT_SDO1		3
#define PPS_OUT_SDO2		4
#define PPS_OUT_OC2			5

// -- PPS Out Group 3 -- //
//Pins
#define PPS_POUT_RPA2		RPA2R
#define PPS_POUT_RPA4		RPA4R
#define PPS_POUT_RPB2		RPB2R
#define PPS_POUT_RPB6		RPB6R
#define PPS_POUT_RPB13		RPB13R
#define PPS_POUT_RPC1		RPC1R
#define PPS_POUT_RPC3		RPC3R
#define PPS_POUT_RPC6		RPC6R

//Peripherals
#define PPS_OUT_NULL		0
#define PPS_OUT_SDO1		3
#define PPS_OUT_SDO2		4
#define PPS_OUT_OC4			5
#define PPS_OUT_OC5			6
#define PPS_OUT_REFCLKO		7

// -- PPS Out Group 4 -- //
//Pins
#define PPS_POUT_RPA3		RPA3R
#define PPS_POUT_RPB0		RPB0R
#define PPS_POUT_RPB9		RPB9R
#define PPS_POUT_RPB10		RPB10R
#define PPS_POUT_RPB14		RPB14R
#define PPS_POUT_RPC2		RPC2R
#define PPS_POUT_RPC4		RPC4R
#define PPS_POUT_RPC9		RPC9R

//Peripherals
#define PPS_OUT_NULL		0
#define PPS_OUT_U1RTS		1
#define PPS_OUT_U2TX		2
#define PPS_OUT_SS2			4
#define PPS_OUT_OC3			5
#define PPS_OUT_C1OUT		7
// --------------------- //
// ------------------------ //
// ############################################## //


// ################# Prototypes ################# //
// === Control Functions ===== //
//IOLock control
/**
* \fn		void ppsUnlock(void)
* @brief	Unlock the PPS control reg
* @note		Will force lock before unlocking
* @arg		nothing
* @return	nothing
*/
#define ppsUnlock()						(SYSKEY = 0); (SYSKEY = 0xAA996655); (SYSKEY = 0x556699AA)

/**
* \fn		void ppsLock(void)
* @brief	Lock the PPS control reg
* @note		
* @arg		nothing
* @return	nothing
*/
#define ppsLock()						(SYSKEY = 0)

/**
* \fn		void ppsAttachIn(peripheral,pin)
* @brief	Attach an input pin to a peripheral
* @note		Arguments have no type as this is a macro.
*			Use the defined pins and peripherals in the "Input PPS" Define section
*			IO control must be unlock prior to calling this macro
*			Use only the significant part of the define (ex: ppsAttachIn(INT4,RPA0))
* @arg		peripheral			Peripheral to attach to the pin
* @arg		pin					Pin to be attached to the Peripheral
* @return	nothing
*/
#define ppsAttachIn(peripheral,pin)		((PPS_IN_##peripheral) = (PPS_PIN_##pin))

/**
* \fn		void ppsAttachOut(peripheral,pin)
* @brief	Attach a peripheral to an output pin
* @note		Arguments have no type as this is a macro.
*			Use the defined pins and peripherals in the "Output PPS" Define section
*			IO control must be unlock prior to calling this macro
*			Use only the significant part of the define (ex: ppsAttachOut(U1TX,RPA0))
* @arg		peripheral			Peripheral to attach to the pin
* @arg		pin					Pin to be attached to the Peripheral
* @return	nothing
*/
#define ppsAttachOut(peripheral,pin)	((PPS_POUT_##pin) = (PPS_OUT_##peripheral))
// ========================== //
// ############################################## //
#endif

#endif
