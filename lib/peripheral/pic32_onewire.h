/*!
 @file		pic32_onewire.h
 @brief		Dallas OneWire serial bus lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		February 16th 2012
 @author	Laurence DV
*/

/*	Example Usage

	







*/

#ifndef _PIC32_ONEWIRE_H
#define _PIC32_ONEWIRE_H

// ################# Includes ################### //
#include <hardware.h>

// Lib
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megxaone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################# Defines #################### //
#define	OW_BITBUF_SIZE		9				//Size of the RX/TX Shift Register
#define OW_MESSAGE_BUF_SIZE	10				//Size of the message in/out message buffer

// Timing
#define OW_TIMER_ID		TIMER_5

#define OW_NORM_SPD		0
#define OW_OVDR_SPD		1

// Timer value ID
#define OW_TIME_1_LOW		0
#define OW_TIME_1_HIGH		1
#define OW_TIME_0_LOW		2
#define OW_TIME_0_HIGH		3
#define OW_TIME_READ_LOW	0
#define OW_TIME_READ_SAMPLE	4
#define OW_TIME_READ_WAIT	5
#define OW_TIME_RESET_INIT	6
#define OW_TIME_RESET_LOW	7
#define OW_TIME_RESET_SAMPLE	8
#define OW_TIME_RESET_WAIT	9

// Timer value (in ns)
#define OW_TMR_NORM_1_LOW		6000
#define OW_TMR_NORM_1_HIGH		64000
#define OW_TMR_NORM_0_LOW		60000
#define OW_TMR_NORM_0_HIGH		10000
#define OW_TMR_NORM_READ_SAMPLE		9000
#define OW_TMR_NORM_READ_WAIT		55000
#define OW_TMR_NORM_RESET_INIT		0
#define OW_TMR_NORM_RESET_LOW		480000
#define OW_TMR_NORM_RESET_SAMPLE	70000
#define OW_TMR_NORM_RESET_WAIT		410000

#define OW_TMR_OVDR_1_LOW		1000
#define OW_TMR_OVDR_1_HIGH		7500
#define OW_TMR_OVDR_0_LOW		7500
#define OW_TMR_OVDR_0_HIGH		2500
#define OW_TMR_OVDR_READ_SAMPLE		1000
#define OW_TMR_OVDR_READ_WAIT		7000
#define OW_TMR_OVDR_RESET_INIT		2500
#define OW_TMR_OVDR_RESET_LOW		70000
#define OW_TMR_OVDR_RESET_SAMPLE	8500
#define OW_TMR_OVDR_RESET_WAIT		40000
// ############################################## //


// ################# Data Type ################## //
typedef struct
{
	volatile U32 * portPtr;		//Pointer to the Port Register
	U16 pinPosition;		//Position of the bus on the port
	tFSMState state;		//State of the OneWire Bus
}tOWBusCtl;
// ############################################## //


// ################# Prototype ################## //
// === ISR ================= //
// ========================= //

// === Control Functions === //
U8 owCreateBus(void);
// ========================= //

// === Data Functions ====== //
U8 owSendByte(void);

U8 owSendMessage(void);
// ========================= //

// ----------------- Primary Functions ----------------- //

// Initialisation of the OneWire Bus
void ow_init(void);

// Send a reset pulse and wait for a device presence pulse return high if device detected
U8 ow_reset(void);

// Write a byte on the bus
void ow_write_byte(U8 to_send);

// Read a byte on the bus
U8 ow_read_byte(void);

// ----------------- ROM Commands Functions ----------------- //

// Skip ROM : to talk to all Devices on the bus
#define ow_skip_rom()	ow_write_byte(0xCC)

// Search ROM : to identify multiple Device on the bus

// Read ROM : to identify one Device on the bus

// Match ROM : to talk to the specified Device

// Alarm Search : to talk to Devices that expericed an alarm condition
// ############################################## //

#endif
