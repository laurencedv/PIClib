/*!
 @file		pic32_onewire.c
 @brief		Dallas OneWire serial bus lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		February 16th 2012
 @author	Laurence DV
*/

// ################# Includes ################### //
#include <pic32_onewire.h>
// ############################################## //


// ################# Variables ################## //
// Control
tOWBusCtl * owBus[];				//Pointer to the OneWire Buses control register
U8 owBusNb = 0;					//Number of bus actually crated

// Timing
const U32 owTimeValue[2][10]={{OW_TMR_NORM_1_LOW,OW_TMR_NORM_1_HIGH,OW_TMR_NORM_0_LOW,OW_TMR_NORM_0_HIGH,OW_TMR_NORM_READ_SAMPLE,OW_TMR_NORM_READ_WAIT,OW_TMR_NORM_RESET_INIT,OW_TMR_NORM_RESET_LOW,OW_TMR_NORM_RESET_SAMPLE,OW_TMR_NORM_RESET_WAIT},{OW_TMR_OVDR_1_LOW,OW_TMR_OVDR_1_HIGH,OW_TMR_OVDR_0_LOW,OW_TMR_OVDR_0_HIGH,OW_TMR_OVDR_READ_SAMPLE,OW_TMR_OVDR_READ_WAIT,OW_TMR_OVDR_RESET_INIT,OW_TMR_OVDR_RESET_LOW,OW_TMR_OVDR_RESET_SAMPLE,OW_TMR_OVDR_RESET_WAIT}};
U16 owTimerValue[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};	//Value for the timer depending on the speed and value outputed

U8 ow_buffer_in[OW_BUF_IN_MAX];			//Buffer for consecutive reception
U8 ow_buffer_out[OW_BUF_OUT_MAX];		//Buffer for consecutive transmission
// ############################################## //


// ############ Primary functions ############### //
// === ISR ================= //
// ========================= //

// === Control Functions === //
U8 owCreateBus(volatile U32 * portPtr, U16 pinPosition);
{
	U8 wu0,wu1;
	
	// -- Init and configure the timer -- //
	timerInit(OW_TIMER_ID,TMR_DIV_1|TMR_CS_PBCLK|TMR_16BIT);
	
	// Compute the correct PR value for each timing point
	for (wu1 = 0; wu1 < 2; wu1++)
	{
		for (wu0 = 0; wu0 < 10; wu0++)		//These formula are simplified by the TMR_DIV_1 and the fixed Timer size
			owTimerValue[wu1][wu0] = owTimeValue[wu1][wu0]/((1000000000/clockGetPBCLK()));	// divided by (clockGetPBCLK()*Prescaler) if using a prescaler
	}
	// ---------------------------------- //
	
	// -- Create the bus control reg -- //
	if (owBusNb == 0)
	{
		owBus[0] = (tOWBusCtl*)malloc(sizeof(tOWBusCtl));
	}
	else
	{
		owBus[owBusNb] = (tOWBusCtl*)realloc(owBus,sizeof(tOWBusCtl * owBusNb));
	}
	owBusNb++;		//Count the newly created bus
	// -------------------------------- //
	
	// -- Set the bus in a known state -- //
	
	// ---------------------------------- //
	
	// -- 
}

U8 owDestroyBus(U8 busID)
{

}
// ========================= //

// === Data Functions ====== //
U8 owSendByte(void);

U8 owSendMessage(void);
// ========================= //





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

