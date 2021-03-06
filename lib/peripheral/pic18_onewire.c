
/*!
 @file pic18_onewire.c
 @brief		Dallas OneWire serial bus communication functions for C18
 *          TODO: ROM command, correct the delay lib and readjust the timing
 *			LONG_TODO: switch to a non-obstrusive way of communiation (timer interrupt?) 
 @version 0.1
 @date May 9th 2011
 @author Laurence DV
*/
// ################# Includes ################### //
#include <pic18_onewire.h>
// ############################################## //


// ################# Variables ################## //
U8 ow_buffer_in[OW_BUF_IN_MAX];			//Buffer for consecutive reception
U8 ow_buffer_out[OW_BUF_OUT_MAX];		//Buffer for consecutive transmission
// ############################################## //


// ############ Primary functions ############### //
// Initialisation of the OneWire Bus
void ow_init(void)
{
	ow_write_high();
    ow_reset();
}

// Send a reset pulse and wait for a device presence pulse return high if device detected
U8 ow_reset(void)
{
	U8 temp_result;
	
	int_global_dis();
	ow_write_high();
	ow_write_low();			//Drive the bus low for 500us
	delay_nUS(200);
	delay_nUS(220);
	ow_write_high();		//Release it and wait 70us
	delay_nUS(50);
	temp_result = !OW_PIN;	//Sample the bus (if low there is a device)
	delay_nUS(200);			//Wait 430us
	delay_nUS(150);
	int_global_en();
	
	return temp_result;
}

// Write a byte on the bus
void ow_write_byte(U8 to_send)
{
	U8 l1;
	
	int_global_dis();
	for (l1 = 0; l1 < 8; l1++)
	{
		//Check the LSb and send it
		if (to_send&0x01)		//Send a HIGH bit
		{
			ow_write_low();		//Drive the bus low for 5us
			delay_nUS(1);
			ow_write_high();	//Release it and wait the rest of the 60-120us time slot			
			delay_nUS(46);		//Wait 60us
		}
		else					//Send a LOW bit
		{
			ow_write_low();		//Drive the bus low for 60us
			delay_nUS(46);
			ow_write_high();	//Release it and wait 5us
			delay_nUS(1);
		}
		//Shift the data to the right for the next bit
		to_send >>= 1;
	}
	int_global_en();
}

// Read a byte on the bus
U8 ow_read_byte(void)
{
	U8 temp_result = 0;
	U8 local1;
	
	int_global_dis();
	for (local1 = 0; local1 < 8; local1++)
	{
		ow_write_low();				//Drive the bus low for 6us
		delay_nUS(5);
		ow_write_high();			//Release the bus and wait 4us
		delay_nUS(1);
		temp_result |= (OW_PIN<<local1);//Sample the bus and shift the result
		delay_nUS(42);				//Wait 50us for the rest of the time slot
	}
	int_global_en();
	
	return temp_result;
}
// ############################################## //

// ================= ROM Commands Functions ================= //

// Search ROM : to identify multiple Device on the bus

// Read ROM : to identify one Device on the bus

// Match ROM : to talk to the specified Device

// Alarm Search : to talk to Devices that expericed an alarm condition

