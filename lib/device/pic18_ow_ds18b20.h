
/*!
 @file pic18_ow_ds18b20.h
 @brief		Functions for operating the DS18B20 on a OneWire bus for C18
 *              
 @version 0.1
 @date May 10th 2011
 @author Laurence DV
*/

#ifndef _PIC18_OW_DS18B20_H
#define _PIC18_OW_DS18B20_H

// ================= Includes ================= //
#include <datatype_C18.h>
#include <splitvar_C18.h>
#include <stddef_C18.h>
#include <p18cxxx.h>

#include <pic18_onewire.h>

// ================= Defines ================= //


// ================= Prototype ================= //

// ----------------- DS18B20 Command ----------------- //
// Convert T : Start a Temperature conversion
#define ds18b20_convert_t()			ow_write_byte(0x44)

// Copy Scratchpad : Copy the content of Th,Tl and config-reg to the EEPROM (10ms write time)
#define ds18b20_copy_scratchpad()	ow_write_byte(0x48)

// Write Scratchpad : to write into the internal registers of the DS18B20
void ds18b20_write_scratchpad(U8 temp_high, U8 temp_low, U8 control_byte);

// Read Scratchpad : to read the internal registers of the DS18B20
void ds18b20_read_scratchpad(U8 number_of_byte);

// Recall EE : Recall the content of the EEPROM into the internal registers
void ds18b20_recall_ee(void);

// Read Power Supply : Check for parasite powered DS18B20
// Return 1 if there are parasite power Devices
U8 ds18b20_read_power_supply(void);


// ----------------- DS18B20 Complete Functions ----------------- //


#endif
