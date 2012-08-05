/*!
 @file		pic18_cli.c
 @brief		Command Line Interface for pic18

 @version	0.1
 @note

 @date		November 22th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic18_cli.h"
// ############################################## //


// ################## Defines ################### //
// ############################################## //


// ################## Variables ################# //
extern U32 sysTick;

// Serial Port
U8 cliSerialPortID = 0;


// ############################################## //


// ################ ADC Functions ############### //
// === Interrupt Handler ===== //

// =========================== //


// === Control Functions ===== //
/**
* \fn		void cliInit(U8 usartID)
* @brief	Initialised the Command Line Interface on the specified USART
* @note		
* @arg		U8 usartID			USART Hardware ID to use for the CLI
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 cliInit(U8 usartID)
{
	cliSerialPortID = usartID;
	
	//Init the USART
	usartInit(cliSerialPortID,CLI_SERIAL_BAUD_RATE,USART_MODE_8N1);


}
// =========================== //


// === Run-time Functions ==== //

void cliEngine(void)
{
	
}
// =========================== //
// ############################################## //
