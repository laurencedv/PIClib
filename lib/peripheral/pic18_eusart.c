/*!
 @file		pic18_eusart.c
 @brief		EUSART functions for C18

 @version	0.2.1
 @note		All Function are Non-Blocking
			Depends on some global parameters in hardware.h

 @date		October 29th 2011
 @author	Laurence DV
*/
//TODO : Could use a localPtr to the correct buffer for faster loop in sendArray functions (only one index to fetch)
//TODO : switch to Macro for get parameter functions
//TODO : Extract all the ringBuffer stuff to a Ring Buffer lib and use it here

// ################## Includes ################## //
#include "pic18_eusart.h"
// ############################################## //


// ################# Variables ################## //
// Reg Pointer //
tTXSTA * usartTXSTA;
tRCSTA * usartRCSTA;
tBAUDCON * usartBAUDCON;
U8 * usartSPBRG;
U8 * usartSPBRGH;
U8 * usartRCREG;
U8 * usartTXREG;
// ----------- //

// FIFO Buffer //
#pragma udata arrayRx								//Take a full Ram Bank for the Rx Buf
volatile U8 usartBufRx[USART_PORT_NB][USART_BUF_SIZE];		//FIFO Buffer for Reception
volatile U8 usartBufRxIDin[USART_PORT_NB];					//Rx Buf index for write
volatile U8 usartBufRxIDout[USART_PORT_NB];					//Rx Buf index for read
volatile U8 usartBufRxSize[USART_PORT_NB];					//Rx Buf actual size
#pragma udata arrayTx								//Take a full Ram Bank for the Tx Buf
volatile U8 usartBufTx[USART_PORT_NB][USART_BUF_SIZE];		//FIFO Buffer for Transmission
volatile U8 usartBufTxIDin[USART_PORT_NB];					//Tx Buf index for write
volatile U8 usartBufTxIDout[USART_PORT_NB];					//Tx Buf index for read
volatile U8 usartBufTxSize[USART_PORT_NB];					//Tx Buf actual size
#pragma udata
// ----------- //

// Global Parameters //
extern U32 globalCLK;
extern U8 globalDump;
U32 usartBaudRate[USART_PORT_NB];					//Actual Baud Rate for each EUSART
tUsartCtl usartCtl[USART_PORT_NB];
// ----------------- //
// ############################################## //


// ############## EUSART Functions ############## //
// ==== Interrupt Handler ==== //
/**
* \fn		void usartRxHandler(U8 portID)
* @brief	Handle the interrupt reception, append byte in the $usartRxBuf
* @note		If the soft buf is full, will discard the byte
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartRxISR(U8 portID)
{
	U8 tempID;

	switch(portID)
	{
		//* -- EUSART 1 -- *//
		case 0:
		{
			usartRCREG = (U8*)&RCREG1;
			usartRCSTA = (tRCSTA*)&RCSTA1;
			break;
		}
		//* -- EUSART 2 -- *//
		case 1:
		{
			usartRCREG = (U8*)&RCREG2;
			usartRCSTA = (tRCSTA*)&RCSTA2;
			break;
		}
		//* -- Invalid --- *//
		default:
			return;
		//* -------------- *//
	}

	//* -- Save the byte --- *//
	if (usartBufRxSize[portID] < USART_BUF_SIZE)
	{
		tempID = usartBufRxIDin[portID];				//Local Save for faster execution

		// -- Put the Data in the buffer -- //
		usartBufRx[portID][tempID] = *usartRCREG;
		// -------------------------------- //

		// -- Move the pointers -- //
		tempID++;
		usartBufRxSize[portID]++;

		if (tempID == USART_BUF_SIZE)
			usartBufRxIDin[portID] = 0;					//Wrap around the buffer
		else
			usartBufRxIDin[portID] = tempID;			//Save the pointer
		// ----------------------- //

		// -- Handle Overrun Error -- //
		if (usartRCSTA->OERR)
		{
			usartRCSTA->CREN = 0;
			usartRCSTA->CREN = 1;
			Nop();										//Wait 2 cycle as the Silicon Errata
			Nop();										//for 18Fx7j53 suggest
		}
		// -------------------------- //
	}
	//* -- Buffer is full -- *//
	else
	{
		globalDump = *usartRCREG;						//Discard the byte
		usartCtl[portID].rxFull = 1;					//Flag the overflow
	}
	//* -------------------- *//
}

/**
* \fn		void usartTxHandler(U8 portID)
* @brief	Handle the interrupt transmission, send byte from the $usartTxBuf
* @note		If the soft buf is empty, will disable Tx interrupt
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartTxISR(U8 portID)
{
	U8 tempID;

	switch(portID)
	{
		//* -- EUSART 1 -- *//
		case 0:
		{
			usartTXREG = (U8*)&TXREG1;
			usartTXSTA = (tTXSTA*)&TXSTA1;
			break;
		}
		//* -- EUSART 2 -- *//
		case 1:
		{
			usartTXREG = (U8*)&TXREG2;
			usartTXSTA = (tTXSTA*)&TXSTA2;
			break;
		}
		//* -- Invalid --- *//
		default:
			return;
		//* -------------- *//
	}

	//* -- Send the Next byte -- *//
	if (usartBufTxSize[portID])
	{
		tempID = usartBufTxIDout[portID];				//Local Save for faster execution

		// -- Send the byte -- //
		*usartTXREG = usartBufTx[portID][tempID];
		// ------------------- //

		// -- Move the pointer -- //
		tempID++;
		usartBufTxSize[portID]--;

		if (tempID == USART_BUF_SIZE)
			usartBufTxIDout[portID] = 0;				//Wrap around the buffer
		else
			usartBufTxIDout[portID] = tempID;			//Save the pointer
		// ---------------------- //

		if (usartBufTxSize[portID] < USART_BUF_SIZE)
			usartCtl[portID].txFull = 0;				//Clear the "Buffer Full" flag

	}
	//* -- Buffer is empty ----- *//
	else
	{
		// -- Disable the interrupt -- //
		switch (portID)
		{
			case 0:	int_eusart1tx_dis();	break;
			case 1:	int_eusart2tx_dis();	break;
		}
		// --------------------------- //
	}
	//* ------------------------ *//
}

// =========================== //


// ==== Control Functions ==== //
/**
* \fn		U8 usartInit(U8 portID, U32 desiredBaudRate, U8 options)
* @brief	Initialize the designated ($portID) EUSART into the designated mode ($options)
			at the designated speed ($desiredBaudRate)
* @note		Modifiable at run-time.
* @arg		U8 portID			Hardware EUSART ID
* @arg		U32 desiredBaudRate	Desired Baud Rate (in baud)
* @arg		U8 options			Options for the EUSART
* @return	U8 errorCode		STD Error Code
*/
U8 usartInit(U8 portID, U32 desiredBaudRate, U8 options)
{
	split8 splitedOption = options;
	U8 wu0;
	
	// -- Set the reg ptr and Specifics -- //
	switch (portID)
	{
		//* -- EUSART 1 -- *//
		case 0:
		{
			usartTXSTA = (tTXSTA*)&TXSTA1;
			usartRCSTA = (tRCSTA*)&RCSTA1;
			usartBAUDCON = (tBAUDCON*)&BAUDCON1;
			TXREG1 = 0;								//Clear the Tx Reg
			
			// Init interrupt
			int_eusart1rx_init();

			// Set the IO
			USART1_RX = ~(usartBAUDCON->RXDTP);		//Idle state for RX
			if (splitedOption.b0)
				USART1_TX = usartBAUDCON->TXCKP;		//Idle state for TX
			else
				USART1_TX = ~(usartBAUDCON->TXCKP);		//Idle state for TX (inverted in async mode)
			
			USART1_TX_DIR = OUTPUT;
			USART1_RX_DIR = INPUT;

			break;
		}
		//* -- EUSART 2 -- *//
		case 1:
		{
			usartTXSTA = (tTXSTA*)&TXSTA2;
			usartRCSTA = (tRCSTA*)&RCSTA2;
			usartBAUDCON = (tBAUDCON*)&BAUDCON2;
			TXREG2 = 0;								//Clear the Tx Reg
			
			// Init interrupt
			int_eusart2rx_init();

			// Set the IO
			USART2_RX = ~(usartBAUDCON->RXDTP);		//Idle state for RX
			if (splitedOption.b0)
				USART2_TX = usartBAUDCON->TXCKP;		//Idle state for TX
			else
				USART2_TX = ~(usartBAUDCON->TXCKP);		//Idle state for TX (inverted in async mode)

			USART2_TX_DIR = OUTPUT;
			USART2_RX_DIR = INPUT;

			break;
		}
		default:
		{
			return STD_EC_NOTFOUND;					//Return immediately
		}
		//* -------------- *//
	}
	// ----------------------------------- //


	// -- Reset all Register -- //
    usartTXSTA->all = 0;
    usartRCSTA->all = 0;
    usartBAUDCON->all = 0;
	usartCtl[portID].all = 0;
	// ------------------------ //


	// -- Init Buffers -- //
	usartResetBuffer(portID);
	// ------------------ //


	// -- Set the Options -- //
	usartTXSTA->SYNC = splitedOption.b0;	//Sync/Async
	usartTXSTA->TX9 = splitedOption.b6;		//Set the bit number per frame
	usartRCSTA->RX9 = splitedOption.b6;
	usartBAUDCON->RXDTP = splitedOption.b5;	//Set the Rx polarity
	usartBAUDCON->TXCKP = splitedOption.b4;	//Set the Tx idle state
	// --------------------- //

	// Set the baudrate
	usartBaudRate[portID] = usartSetBaudRate(portID, desiredBaudRate);

	// -- Start the EUSART -- //
	usartTXSTA->TXEN = 1;					//Start the Transmitter
	Nop();									//Wait 2 cycle as the Silicon Errata
	Nop();									//for 18Fx7j53 suggest
	usartRCSTA->CREN = 1;					//Start the continuous reception
	Nop();									//Wait 2 cycle as the Silicon Errata
	Nop();									//for 18Fx7j53 suggest
	usartRCSTA->SPEN = 1;					//Enable the Serial Port
	Nop();									//Wait 2 cycle as the Silicon Errata
	Nop();									//for 18Fx7j53 suggest
	// ---------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 usartSetBaudRate(U8 portID, U32 desiredBaudRate)
* @brief	Set the designated EUSART ($portID) at the designated speed ($desiredBaudRate)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @arg		U32 BaudRate		Desired Baud Rate (in baud)
* @return	U32 realBaudRate	Real Baud Rate set (in baud)
*/
U32 usartSetBaudRate(U8 portID, U32 BaudRate)
{
	split16 resultTemp;

	// -- Set the reg ptr and Specifics -- //
	switch (portID)
	{
		//* -- EUSART 1 -- *//
		case 0:
		{
			usartTXSTA = (tTXSTA*)&TXSTA1;
			usartBAUDCON = (tBAUDCON*)&BAUDCON1;
			usartRCSTA = (tRCSTA*)&RCSTA1;
			usartSPBRG = (U8*)&SPBRG1;
			usartSPBRGH = (U8*)&SPBRGH1;
			break;
		}
		//* -- EUSART 2 -- *//
		case 1:
		{
			usartTXSTA = (tTXSTA*)&TXSTA2;
			usartBAUDCON = (tBAUDCON*)&BAUDCON2;
			usartRCSTA = (tRCSTA*)&RCSTA2;
			usartSPBRG = (U8*)&SPBRG2;
			usartSPBRGH = (U8*)&SPBRGH2;
			break;
		}
		//* -- Invalid --- *//
		default:
			return 0;
		//* -------------- *//
	}
	// ----------------------------------- //

	// -- Shutdown the EUSART -- //
	usartRCSTA->SPEN = 0;
	// ------------------------- //
	
	// -- Set Parameters -- //
	usartBAUDCON->BRG16 = 1;				//Error rate are significantly less with those settings
	usartTXSTA->BRGH = 1;					//At any clock speed and any Baud Rate, so just keep them
	// -------------------- //


	// -- Set the BRG value -- //
	resultTemp.all = (U16)((globalCLK/BaudRate)/4)-1;
	*usartSPBRG = resultTemp.lvl1;			//LSB
	*usartSPBRGH = resultTemp.lvl2;			//MSB
	// ----------------------- //


	// -- Compute the real Baud Rate -- //
	resultTemp.lvl1 = *usartSPBRG;
	resultTemp.lvl2 = *usartSPBRGH;
	usartBaudRate[portID] = globalCLK/(4*(resultTemp.all + 1));
	// -------------------------------- //

	
	// -- Start the EUSART -- //
	usartRCSTA->SPEN = 1;
	Nop();										//Wait 2 cycle as the Silicon Errata
	Nop();										//for 18Fx7j53 suggest
	// ---------------------- //

	return usartBaudRate[portID];			//Return the real Baud Rate
}

/**
* \fn		U32 usartGetBaudRate(U8 portID)
* @brief	Return the actual Baudrate of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U32 desiredBaudRate	Actual Baudrate (in baud)
*/
U32 usartGetBaudRate(U8 portID)
{
	return usartBaudRate[portID];
}

/**
* \fn		U8 usartGetRxSize(U8 portID)
* @brief	Return the size of the usartBufRx of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 bufSize			Number of byte currently in the buffer
*/
U8 usartGetRxSize(U8 portID)
{
	return usartBufRxSize[portID];
}

/**
* \fn		U8 usartGetTxSize(U8 portID)
* @brief	Return the size of the usartBufTx of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 bufSize			Number of byte currently in the buffer
*/
U8 usartGetTxSize(U8 portID)
{
	return usartBufTxSize[portID];
}

/**
* \fn		void usartResetBuffer(U8 portID)
* @brief	Reset the usartBufTx of the designated EUSART ($portID)
* @note		Fill the buffers with 0
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartResetBuffer(U8 portID)
{
	U8 wu0;

	//Reset the buffers ctl var
	usartBufRxIDin[portID] = 0;
	usartBufRxIDout[portID] = 0;
	usartBufRxSize[portID] = 0;
	usartBufTxIDin[portID] = 0;
	usartBufTxIDout[portID] = 0;
	usartBufTxSize[portID] = 0;

	//Clear the buffers
	for (wu0 = 0; wu0 < USART_BUF_SIZE; wu0++)
	{
		usartBufRx[portID][wu0] = 0;
		usartBufTx[portID][wu0] = 0;
	}
}

// =========================== //


// === Transfert Functions === //
/**
* \fn		U8 usartPushByte(U8 portID, U8 byteToSend)
* @brief	Send 1 byte trought the software buffer
* @note		Return STD_EC_OVERFLOW if the buffer is full
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @arg		U8 byteToSend		The byte to send
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushByte(U8 portID, U8 byteToSend)
{
	U8 tempID;

	if (usartBufTxSize[portID] < USART_BUF_SIZE)
	{
		tempID = usartBufTxIDin[portID];					//Local save for faster execution

		// -- Put the Data in the buffer -- //
		usartBufTx[portID][tempID] = byteToSend;
		// -------------------------------- //

		// -- Move the pointers -- //
		tempID++;
		usartBufTxSize[portID]++;

		if (tempID == USART_BUF_SIZE)
			usartBufTxIDin[portID] = 0;						//Wrap around the buffer
		else
			usartBufTxIDin[portID] = tempID;				//Save the pointer
		// ----------------------- //

		// -- Start the transmission -- //
		switch (portID)
		{
			case 0: int_eusart1tx_en();	break;
			case 1: int_eusart2tx_en();	break;
		}
		// ---------------------------- //

		return STD_EC_SUCCESS;
	}
	else
	{
		usartCtl[portID].txFull = 1;						//Flag the "Buffer Full" condition
		return STD_EC_OVERFLOW;
	}
}

/**
* \fn		U8 usartPushFromRam(U8 portID, U8 * arrayPtr, U8 byteNb)
* @brief	Send an array of $byteNb number of byte from the Ram starting at the $arrayPtr
* @note		Return STD_EC_OVERFLOW if there is not enought space
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @arg		U8 * arrayPtr		Pointer to the first element of the array
* @arg		U8 byteNb			Number of byte to send (max USART_BUF_SIZE)
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushFromRam(U8 portID, U8 * arrayPtr, U8 byteNb)
{
	U8 wu0;
	U8 tempID;
	
	if ((usartBufTxSize[portID]+byteNb) < USART_BUF_SIZE)
	{
		// -- Put the Data in the buffer -- //
		tempID = usartBufTxIDin[portID];					//Save the pointer locally (faster loop)
		for (wu0 = 0; wu0 < byteNb; wu0++)
		{
			usartBufTx[portID][tempID] = arrayPtr[wu0];
			tempID++;

			if (tempID == USART_BUF_SIZE)
				tempID = 0;									//Wrap around the buffer
		}
		usartBufTxIDin[portID] = tempID;					//Retreive the correct pointer
		// -------------------------------- //

		//Adjust the buffer size
		usartBufTxSize[portID] += byteNb;

		// -- Start the transmission -- //
		switch (portID)
		{
			case 0: int_eusart1tx_en();	break;
			case 1: int_eusart2tx_en();	break;
		}
		// ---------------------------- //

		return STD_EC_SUCCESS;
	}
	else
	{
		return STD_EC_OVERFLOW;
	}
}

/**
* \fn		U8 usartPushFromRom(U8 portID, rom const U8 * arrayPtr, U8 byteNb)
* @brief	Send an array of $byteNb number of byte from the Rom starting at the $arrayPtr
* @note		Return STD_EC_OVERFLOW if there is not enought space
			No detection for invalid portID
* @arg		U8 portID				Hardware EUSART ID
* @arg		rom const U8 * arrayPtr	Pointer to the first element of the array
* @arg		U8 byteNb				Number of byte to send
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushFromRom(U8 portID, rom const U8 * arrayPtr, U8 byteNb)
{
	U8 wu0;
	U8 tempID;

	if ((usartBufTxSize[portID]+byteNb) < USART_BUF_SIZE)
	{
		// -- Put the remaining Data in the buffer -- //
		tempID = usartBufTxIDin[portID];					//Save the pointer locally (faster loop)
		for (wu0 = 0; wu0 < byteNb; wu0++)
		{
			if (tempID == USART_BUF_SIZE)
				tempID = 0;									//Wrap around the buffer

			usartBufTx[portID][tempID] = arrayPtr[wu0];
			tempID++;
		}
		usartBufTxIDin[portID] = tempID;					//Retreive the correct pointer
		// ------------------------------------------ //

		//Adjust the buffer size
		usartBufTxSize[portID] += byteNb;

		// -- Start the transmission -- //
		switch (portID)
		{
			case 0: int_eusart1tx_en();	break;
			case 1: int_eusart2tx_en();	break;
		}
		// ---------------------------- //

		return STD_EC_SUCCESS;
	}
	else
	{
		return STD_EC_OVERFLOW;
	}
}

/**
* \fn		U8 usartPushString(U8 portID, rom const U8 * arrayPtr, U8 delimiter)
* @brief	Send a string from the Rom starting at $arrayPtr
* @note		Return STD_EC_OVERFLOW if there is not enought space
			No detection for invalid portID
* @arg		U8 portID				Hardware EUSART ID
* @arg		rom const U8 * arrayPtr	Pointer to the first element of the array
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushString(U8 portID, rom const U8 * arrayPtr)
{
	U8 wu0;
	U8 tempID;
	U8 byteNb = 0;

	// -- Measure the string -- //
	while (arrayPtr[byteNb] != '\0')
	{
		byteNb++;											//Count a char
		if (byteNb == USART_BUF_SIZE)						//Break if larger than the buffer
			break;
	}
	// ------------------------ //


	// -- Space available in the buffer ---- //
	if ((usartBufTxSize[portID]+byteNb) < USART_BUF_SIZE)
	{
		// -- Put the remaining Data in the buffer -- //
		tempID = usartBufTxIDin[portID];					//Save the pointer locally (faster loop)
		for (wu0 = 0; wu0 < byteNb; wu0++)
		{
			usartBufTx[portID][tempID] = arrayPtr[wu0];
			tempID++;

			if (tempID == USART_BUF_SIZE)
				tempID = 0;									//Wrap around the buffer
		}
		usartBufTxIDin[portID] = tempID;					//Retreive the correct pointer
		// ------------------------------------------ //

		//Adjust the buffer size
		usartBufTxSize[portID] += byteNb;

		// -- Start the transmission -- //
		switch (portID)
		{
			case 0: int_eusart1tx_en();	break;
			case 1: int_eusart2tx_en();	break;
		}
		// ---------------------------- //

		return STD_EC_SUCCESS;
	}
	// -- not enought space in the buffer -- //
	else
	{
		return STD_EC_OVERFLOW;
	}
	// ------------------------------------- //
}

/**
* \fn		U8 usartPullByte(U8 portID)
* @brief	Extract 1 byte from the usartBufRx in a FIFO manner
* @note		Must check the buffer size before calling this, or else could return false data
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 byteReceived		FIFO byte return
*/
U8 usartPullByte(U8 portID)
{
	U8 byteReceived;
	U8 tempID;
	
	tempID = usartBufRxIDout[portID];						//Local save for faster execution

	// -- Extract the Data from the buffer -- //
	byteReceived = usartBufRx[portID][tempID];
	// -------------------------------------- //


	// -- Move the pointers -- //
	tempID++;
	usartBufRxSize[portID]--;

	if (tempID == USART_BUF_SIZE)
		usartBufRxIDout[portID] = 0;
	else
		usartBufRxIDout[portID] = tempID;
	// ----------------------- //

	usartCtl[portID].rxFull = 0;							//Clear the "Buffer Full" condition

	return byteReceived;
}

/**
* \fn		U8 usartPullArray(U8 portID, U8 * destinationPtr, U8 byteNb)
* @brief	Extract an array of $byteNb size from the usartBufRx in a FIFO manner
* @note		This function check for the bufSize >= byteNb, if not return STD_EC_EMPTY
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @arg		U8 * destinationPtr	Pointer to save the resulting array
* @arg		U8 byteNb			Number of byte to extract
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPullArray(U8 portID, U8 * destinationPtr, U8 byteNb)
{
	U8 wu0;
	U8 tempID;

	// -- Enough Byte in the buffer -- //
	if (usartBufRxSize[portID] >= byteNb)
	{
		// -- Extract the data from the buffer -- //
		tempID = usartBufRxIDout[portID];					//Save the pointer locally (faster loop)
		for (wu0 = 0; wu0 < byteNb; wu0++)
		{
			destinationPtr[wu0] = usartBufRx[portID][tempID];
			tempID++;

			if (tempID == USART_BUF_SIZE)
				tempID = 0;									//Wrap around the buffer
		}
		usartBufRxIDout[portID] = tempID;					//Save the pointer
		// -------------------------------------- //

		//Adjust the buffer size
		usartBufRxSize[portID] -= byteNb;

		return STD_EC_SUCCESS;
	}
	// -- Buffer too empty ----------- /
	else
	{
		return STD_EC_EMPTY;
	}
	// ------------------------------- //
}

/**
* \fn		U8 usartPullFrame(U8 portID, U8 * destinationPtr, U8 delimiter)
* @brief	Extract an array from the usartBufRx until a $delimiter is found in a FIFO manner
* @note		You must ensure that enought space in the destination is available for the frame (max USART_BUF_SIZE)
			This function will first scan for a delimiter, if none found will return 0
			Also check for coherency with the buffer size (ex: frame found of 20B but buffer contain 10 Byte) will return 0
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @arg		U8 * destinationPtr	Pointer to save the resulting array
* @arg		U8 delimiter		Value Representing the end of the frame
* @return	U8 nbByte			Number of byte in the frame
*/
U8 usartPullFrame(U8 portID, U8 * destinationPtr, U8 delimiter)
{
	U8 wu0;
	U8 tempID;
	U8 byteNb = 0;											//Number of byte in the frame

	PORTBbits.RB1 = 1;

	// -- Scan for delimiter -- //
	tempID = usartBufRxIDout[portID];						//Save the pointer locally for faster execution
	while (usartBufRx[portID][tempID] != delimiter)
	{
		byteNb++;											//Count a byte
		tempID++;											//Move the pointer

		if (byteNb == USART_BUF_SIZE)						//No delimiter found
			return 0;										//Return 0 byte transferred
		else if (tempID == USART_BUF_SIZE)					//Wrap around the buffer
			tempID = 0;
	}
	// ------------------------ //

	
	// -- Delete the delimiter -- //
	usartBufRx[portID][tempID] = 0x00;
	// -------------------------- //


	// -- Check for coherency ----------------- //
	if (usartBufRxSize[portID] >= byteNb)
	{
		// -- Delimiter found, extract the frame -- //
		tempID = usartBufRxIDout[portID];					//reset the tempID at the beginning of the frame
		for (wu0 = 0; wu0 < byteNb; wu0++)
		{
			destinationPtr[wu0] = usartBufRx[portID][tempID];
			tempID++;

			if (tempID == USART_BUF_SIZE)
				tempID = 0;									//Wrap around the buffer
		}
		
		// -- Skip the delimiter -- //
		tempID++;
		if (tempID == USART_BUF_SIZE)
			tempID = 0;										//Wrap around the buffer
		// ------------------------ //

		usartBufRxIDout[portID] = tempID;					//Save the pointer

		//Adjust the buffer size
		usartBufRxSize[portID] -= (byteNb+1);				//Discard the delimiter too

		return byteNb;										//Return the number of byte transferred
		// ---------------------------------------- //
	}
	// -- Old data return 0 byte transferred -- //
	else
		return 0;
	// ------------------------------------------------- //
}
// =========================== //
// ############################################## //
