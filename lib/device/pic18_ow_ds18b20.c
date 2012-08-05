
/*!
 @file pic18_ow_ds18b20.c
 @brief		Functions for operating the DS18B20 on a OneWire bus for C18
 *			TODO: Complete function with "match ROM" command and ow_dev_id_table
 @version 0.1
 @date May 10th 2011
 @author Laurence DV
*/

// ================= Includes ================= //
#include <device/pic18_ow_ds18b20.h>



// ================= Variables ================= //
extern U8 ow_buffer_in[];			//Common OneWire Reception buffer



// ================= DS18B20 Command ================= //

// Write Scratchpad : to write into the internal registers of the DS18B20
void ds18b20_write_scratchpad(U8 temp_high, U8 temp_low, U8 config_byte)
{
	ow_write_byte(0x4E);			//Send the command
	ow_write_byte(temp_high);		//Send the Temp high alarm value
	ow_write_byte(temp_low);		//Send the Temp low alarm value
	ow_write_byte(config_byte);		//Send the config byte
}

// Read Scratchpad : to read the internal registers of the DS18B20
void ds18b20_read_scratchpad(U8 number_of_byte)
{
	U8 l1;
	
	ow_write_byte(0xBE);						//Send Command
	
	for (l1 = 0; l1 < number_of_byte; l1++)
	{
		ow_buffer_in[l1] = ow_read_byte();		//Return 1 byte and store it into the ow_buffer_in in reception order
	}
	ow_reset();									//After reading the correct number of byte, terminate
}

// Recall EE : Recall the content of the EEPROM into the internal registers
void ds18b20_recall_ee(void)
{
	ow_write_byte(0xB8);		//Send the command
	
	while (!OW_PIN);			//Wait until the bus is release by the device (finish)
}
	
// Read Power Supply : Check for parasite powered DS18B20
// Return 1 if there are parasite power Devices
U8 ds18b20_read_power_supply(void)
{
	U8 temp_result = 0;
	
	ow_write_byte(0xB4);		//Send the command
	//Read a time slot
	ow_write_low();				//Drive the bus low for 6us
	delay_nUS(3);
	ow_write_high();			//Release the bus and wait 4us
	delay_nUS(1);
	temp_result = OW_PIN;		//Sample the bus
	delay_nUS(43);				//Wait 50us for the rest of the time slot
	
	return temp_result;
}

// ================= DS18B20 Complete Functions ================= //

