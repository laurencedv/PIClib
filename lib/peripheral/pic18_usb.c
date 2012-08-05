/*!
 @file		pic18_usb.c
 @brief		USB lib for use with C18

 @version	0.1
 @note		Really Basic at the moment contain only function for the Device CDC class
			Not working still

 @date		October 24th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_usb.h"
// ############################################## //


// ################## Defines ################### //
#define USB_BUF_OWNER_CPU			0				//Buffer is currently owned by the µC core
#define USB_BUF_OWNER_SIE			1				//Buffer is currently owned by the SIE
// ############################################## //


// ################# Variables ################## //
tUSBBufferDescriptor *pUSBWorkingBD;
// ############################################## //


// ############ Interrupt Functions ############# //
/**
* \fn		void USBISR(void)
* @brief	Handler the different Interrupt for normal USB operation
* @note
* @arg		nothing
* @return	nothing
*/
void USBISR(void)
{

	//* -- USB Module Error ---------- *//
	if (INT_USB_ERROR_FLAG)
	{
		USBErrorHandler();
		UIRbits.UERRIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* -- Stall Condition detected -- *//
	if (INT_USB_STALL_FLAG)
	{
		USBStallHandler();
		UIRbits.STALLIF = CLEAR;		//Clear the USB Interrupt specific flag
	}
	//* -- Transaction Complete ------ *//
	if (INT_USB_TRANSACTION_FLAG)
	{
		USBTransactionHandler();
		UIRbits.TRNIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* -- Start Of Frame detected --- *//
	if (INT_USB_SOF_FLAG)
	{
		USBSOFHandler();
		UIRbits.SOFIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* -- BUS Idle state detected --- *//
	if (INT_USB_IDLE_FLAG)
	{
		USBIdleHandler();
		UIRbits.IDLEIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* -- BUS Activity detected ----- *//
	if (INT_USB_ACTIVITY_FLAG)
	{
		USBResumeHandler();
		UIRbits.ACTVIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* -- Reset command detected ---- *//
	if (INT_USB_RESET_FLAG)
	{
		USBResetHandler();
		UIRbits.URSTIF = CLEAR;			//Clear the USB Interrupt specific flag
	}
	//* ------------------------------ *//

	int_usb_clr();						//Clear the USB Interrupt general flag
}
// ############################################## //


// ######## Packet and Condition Handler ######## //
/**
* \fn		void USBErrorHandler(void)
* @brief	Handle all the possible error condition detected by USB Module
* @note		Can be used in ISR or by Polling method
* @arg		nothing
* @return	nothing
*/
void USBErrorHandler(void)
{
	
}

/**
* \fn		void USBStallHandler(void)
* @brief	Handle a Stall condition on an Endpoint
* @note
* @arg		nothing
* @return	nothing
*/
void USBStallHandler(void)
{

}

/**
* \fn		void USBTransactionHandler(void)
* @brief	Handle a any type of data transaction (IN,OUT,SETUP)
* @note
* @arg		nothing
* @return	nothing
*/
void USBTransactionHandler(void)
{
	// -- Select the correct endpoint -- //
	switch (USTATbits.ENDP)
	{

	}
	// --------------------------------- //

	//parse the received packet
}

/**
* \fn		void USBSOFHandler(void)
* @brief	Handle a Start Of Frame condition and correctly set the USB module
* @note
* @arg		nothing
* @return	nothing
*/
void USBSOFHandler(void)
{

}

/**
* \fn		void USBIdleHandler(void)
* @brief	Put the Module in sleep mode when an BUS idle condition has been detected
* @note
* @arg		nothing
* @return	nothing
*/
void USBIdleHandler(void)
{

}

/**
* \fn		void USBResumeHandler(void)
* @brief	Resume normal operation when BUS Activity is detected
* @note
* @arg		nothing
* @return	nothing
*/
void USBResumeHandler(void)
{

}

/**
* \fn		void USBResetHandler(void)
* @brief	Reset the USB module after receiving the Reset command
* @note
* @arg		nothing
* @return	nothing
*/
void USBResetHandler(void)
{

}
// ############################################## //


// ############# Control Functions ############## //
/**
* \fn		U8 USBDeviceInit(U8 deviceClass)
* @brief	Initialise the USB
* @note		
* @arg		U8 deviceClass		Device Class needed to be init
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 USBDeviceInit(U8 option, U8 deviceClass)
{
	split8 splittedOption;

	UCFGbits.UTRDIS = SET;				//Power down the internal Transceiver for configuration

	//Reset the address

	//Reset the Buffers

	//Clear all USB Interrupts

	// -- Configure the Internal Transceiver -- //
	if (splittedOption.b0)					//If the internal transceiver was selected
	{
		UCFGbits.UPUEN = SET;				//Enable internal Pull-up Resistor
		UCFGbits.FSEN = splittedOption.b1;	//Set the desired speed
		UCFGbits.UTRDIS = CLEAR;			//Power up the internal Transceiver
	}
	// ---------------------------------------- //

	//Init the Buffer Descriptor Table

	//Init the Buffers

	//Init the Endpoint

}

/**
* \fn		U8 USBAttach(void)
* @brief	Attach the Device to the USB Bus
* @note		Will wait a maximum of USB_ATTACH_DELAY_MAX cycle for the PLL to be locked
* @arg		nothing
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 USBAttach(void)
{
	U16 pllDelay = 0;

	//Wait for the PLL to be locked

	//Enable the USB Module

	//Check the Bus for a single ended zero condition

	//Init the interrupts
}

/**
* \fn		void USBDetach(void)
* @brief	Detach the Device from the USB Bus
* @note		Will Abort all transfert
* @arg		nothing
* @return	nothing
*/
void USBDetach(void)
{
	UCONbits.SUSPND = CLEAR;		//Wake up the SIE to be sure to detach correctly
	UCONbits.USBEN = CLEAR;			//Disable the USB Module completly
}

U8 USBInitEndpoint(U8 option, )
// ############################################## //

