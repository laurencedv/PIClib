/*!
 @file		pic18_spi_nrf24l01+.c
 @brief		nRF24L01+ lib for C18 using SPI interrupt lib

 @version	0.1
 @note

 @date		October 12th 2011
 @author	Laurence DV
*/

// ================== Include =================== //
#include <device/pic18_spi_nrf24l01+.h>
// ============================================== //


// ================= Variables ================== //
// -- Control -- //
// SPI Comm //
U8 nrfSlaveID;									//Slave ID of the nRF in the SPI Slave Table
tSPIStatusFlag nrfSPIStatus;					//SPI Status byte
U8 nrfCommandBuf[6];							//Buffer to format commands with parameters
U8 nrfResponseBuf[6];							//Buffer for the command response (maximum 5 byte + status Byte)

// nRF Reg //
tNRFStatusReg  nrfStatusReg;

// ------------- //

// -- Data -- //
U8 nrfBufIn[NRF_PACKET_SIZE_MAX+1];				//Buffer to store incoming payload from the nRF
U8 nrfBufOut[NRF_PACKET_SIZE_MAX+1];			//Buffer to store the outgoing payload for the nRF
// ---------- //
// ============================================== //


// ============ Control Sub-routine ============= //
/**
* \fn		U8 nrfInit(void)
* @brief	Initialize the SPI port and various variable for the nRF
* @note		Save the SPISlaveID of the nRF in the $nrfSlaveID global variable
			as well as the SPI Status in $nrfSPIStatus.
			USE Define for the port and pin of the SS of the nRF
* @return	U8 errorCode		STD Error Code
*/
U8 nrfInit(void)
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

	nrfSPIStatus.all = 0;
	// ------------------------------------ //


	// -- Start the MSSP -- //
	spiInit(NRF_MSSP_PORT, SPI_CLK_DIV64, SPI_CKP_LOW+SPI_SMP_MIDDLE+SPI_CKE_IDLE_TO_ACV+SPI_OPEN_DRAIN_DIS);
	// -------------------- //


	// -- Add the nRF to the SPI Slave Table -- //
	nrfSlaveID = spiAddSlave(NRF_MSSP_PORT, &NRF_PIN_SS_PORT, NRF_PIN_SS_PIN, &nrfSPIStatus);
	// ---------------------------------------- //
}

/**
* \fn		U8 nrfSendCommand(U8 command, U8 * parametersPtr, U8 paramNb)
* @brief	Send a command to the local NRF with parameters and save the response in the $nrfResponseBuf
* @note		You can specify 0 to paramNb to send only the command byte and save only the Status REG in response
* @arg		U8	command			Command to send (see nRF Commands section in header)
* @arg		U8 * parametersPtr	Pointer to load the command's parameters to send to the local nRF
* @arg		U8 paramNb			Number of byte of parameters
* @return	U8 errorCode		STD Error Code
*/
U8 nrfSendCommand(U8 command, U8 * parametersPtr, U8 paramNb)
{
	U8 wu0;

	// -- Send only if not already in comm -- //
	if (spiStatus(NRF_MSSP_PORT) == ready)
	{
		// -- Load in the command buf -- //
		for (wu0 = 0; wu0 < paramNb; wu0++)
			nrfCommandBuf[wu0+1] = parametersPtr[wu0];		//Save the parameters

		nrfCommandBuf[0] = command;							//Save the command
		// ----------------------------- //

		return spiSend(nrfResponseBuf, nrfCommandBuf, (paramNb+1), nrfSlaveID);
	}
	// -------------------------------------- //

	return STD_EC_BUSY;
}

U8 nrfReceiveCommand(U8 command, U8 * responsePtr, U8 responseNb)
{
	// -- Send only if not already in comm -- //
	if (spiStatus(NRF_MSSP_PORT) == ready)
		return spiSend(responsePtr,&command,(responseNb+1),nrfSlaveID);
	// -------------------------------------- //
	else
		return STD_EC_BUSY;
}

// ============================================== //


// ============ Transfer Sub-routine ============ //


// ============================================== //

