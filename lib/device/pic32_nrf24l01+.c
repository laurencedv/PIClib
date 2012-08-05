/*!
 @file		pic32_nrf24l01+.c
 @brief		nRF24L01+ lib for the OpIUM network

 @version	0.1
 @note		The pin "CSN" in the datasheet is used in the lib as "SS".
 			The lib assume the SPI module is correctly initialised

 @date		March 29th 2012
 @author	Laurence DV
*/

// ################## Include ################### //
#include "pic32_nrf24l01+.h"
// ############################################## //


// ################# Variables ################## //
// -- Control -- //
// SPI Comm //
tSPISlaveControl * nrfSPIControl;				//pointer to the nRF SPI Control structure

tSPITransaction * nrfSPICommand;				//pointer to the Command Transaction

//tSPIStatusFlag nrfSPIStatus;					//SPI Status byte
U8 nrfCommandBuf[6];							//Buffer to format commands with parameters
U8 nrfResponseBuf[6];							//Buffer for the command response (maximum 5 byte + status Byte)

// nRF Reg //
tNRFStatusReg  nrfStatusReg;
// ------------- //

// -- Data -- //
U8 nrfBufIn[NRF_PACKET_SIZE_MAX+1];				//Buffer to store incoming payload from the nRF
U8 nrfBufOut[NRF_PACKET_SIZE_MAX+1];			//Buffer to store the outgoing payload for the nRF
// ---------- //
// ############################################## //


// ############## Control Function ############## //
/**
* \fn		U8 nrfInit(void)
* @brief	Initialize the various variable for the nRF
* @note
* @arg		U8 spiPort						SPI port the nRF is connected to
* @arg		U32 * SSpinPortPtr				Pointer to the SSpin port register
* @arg		U32 SSpinPortMask				Mask to apply to the SSpin port register
* @return	U8 errorCode					STD Error Code
*/
U8 nrfInit(U8 spiPort, volatile U32 * SSpinPortPtr, U16 SSpinMask)
{
	U8 wu0;

	// -- Init the Buffers and variables -- //
	for (wu0 = 0; wu0 < sizeof(nrfBufIn); wu0++)
	{
		nrfBufIn[wu0] = 0;
		nrfBufOut[wu0] = 0;
	}

	for (wu0 = 0; wu0 < sizeof(nrfCommandBuf); wu0++)
	{
		nrfCommandBuf[wu0] = 0;
		nrfResponseBuf[wu0] = 0;
	}
	// ------------------------------------ //

	// -- Init the SPI control -- //
	nrfSPIControl = spiAddSlave(spiPort, SSpinPortPtr, SSpinMask);
	if (nrfSPIControl == NULL)
		return STD_EC_FAIL;
	// -------------------------- //
	
	return STD_EC_SUCCESS;
}

/**
* \fn		U8 nrfSendCommand(U8 command, U8 * parametersPtr, U8 paramNb)
* @brief	Send a command to the local NRF with parameters and save the response in the $nrfResponseBuf
* @note		You can specify 0 to paramNb to send only the command byte and save only the Status REG in response
* @arg		U8	command						Command to send (see nRF Commands section in header)
* @arg		U8 * parametersPtr				Pointer to load the command's parameters to send to the local nRF
* @arg		U8 paramNb						Number of byte of parameters
* @return	U8 errorCode					STD Error Code
*/


// ############################################## //

// ############## Transfer Function ############# //


// ############################################## //

