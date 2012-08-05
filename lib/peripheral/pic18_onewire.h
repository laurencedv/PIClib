
/*!
 @file pic18_onewire.h
 @brief		Dallas OneWire serial bus communication functions for C18
 *              
 @version 0.1
 @date May 9th 2011
 @author Laurence DV
*/

#ifndef _PIC18_ONEWIRE_H
#define _PIC18_ONEWIRE_H
// ################# Includes ################### //
#include <hardware.h>

// Lib
#include <soft/pic18_delay.h>
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megxaone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################# Defines #################### //
#define OW_PIN			PORTBbits.RB3		//Data pin for the OneWire bus
#define OW_TRIS			TRISBbits.TRISB3
#define OW_BUF_IN_MAX	9					//Maximum number of consecutive byte the buffer in can store
#define OW_BUF_OUT_MAX	8					//Maximum number of consecutive byte the buffer out can store (min: 8*8 = 64bit for ROM)

// Macro for simulated Open drain control
#define ow_write_high()	OW_TRIS=INPUT
#define ow_write_low()	OW_PIN=0;OW_TRIS=OUTPUT
// ############################################## //


// ################# Prototype ################## //
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
