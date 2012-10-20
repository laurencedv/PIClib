/*!
 @file		pic32_uart.c
 @brief		Uart Control lib for C32

 @version	0.1.1
 @note
 @todo		- Transfert function 9 bit mode
 		- Remove the temp buffering in the TX ISR
 		- Complete the selectPort function and "genericalise" every function for it
 		- Do a generic ISR function

 @date		February 15th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_uart.h"
// ############################################## //


// ################## Variables ################# //
extern U32 globalDump;
tUARTConfig uartConfig[UART_MAX_PORT];

//Data Buffers
tRBufCtl * uartRxBuf[UART_MAX_PORT];
tRBufCtl * uartTxBuf[UART_MAX_PORT];

//Reg pointers
tUxMODE * pUxMODE = NULL;
tUxSTA * pUxSTA = NULL;
volatile U32 * pUxRXREG = NULL;
volatile U32 * pUxTXREG = NULL;
volatile U32 * pUxBRG = NULL;

// Interrupt Mapping
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
const tIntIRQ UART_INT[6] = {IRQ_UART_1,IRQ_UART_2,IRQ_UART_3,IRQ_UART_4,IRQ_UART_5,IRQ_UART_6};
#else
const tIntIRQ UART_INT[2] = {IRQ_UART_1,IRQ_UART_2};
#endif
// ############################################## //


// ############# Internal Functions ############# //
/**
* \fn		U8 uartSelectPort(U8 uartPort)
* @brief	Correctly point all reg pointers for a designated UART port
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 uartSelectPort(U8 uartPort)
{
	// -- Select the correct UART -- //
	switch (uartPort)
	{
		case UART_1: pUxMODE = (tUxMODE*)&U1MODE; pUxSTA = (tUxSTA*)&U1STA; pUxBRG = &U1BRG; pUxRXREG = &U1RXREG; pUxTXREG = &U1TXREG; break;
		case UART_2: pUxMODE = (tUxMODE*)&U2MODE; pUxSTA = (tUxSTA*)&U2STA; pUxBRG = &U2BRG; pUxRXREG = &U2RXREG; pUxTXREG = &U2TXREG; break;
	#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case UART_3: pUxMODE = (tUxMODE*)&U3MODE; pUxSTA = (tUxSTA*)&U3STA; pUxBRG = &U3BRG; pUxRXREG = &U3RXREG; pUxTXREG = &U3TXREG; break;
		case UART_4: pUxMODE = (tUxMODE*)&U4MODE; pUxSTA = (tUxSTA*)&U4STA; pUxBRG = &U4BRG; pUxRXREG = &U4RXREG; pUxTXREG = &U4TXREG; break;
		case UART_5: pUxMODE = (tUxMODE*)&U5MODE; pUxSTA = (tUxSTA*)&U5STA; pUxBRG = &U5BRG; pUxRXREG = &U5RXREG; pUxTXREG = &U5TXREG; break;
		case UART_6: pUxMODE = (tUxMODE*)&U6MODE; pUxSTA = (tUxSTA*)&U6STA; pUxBRG = &U6BRG; pUxRXREG = &U6RXREG; pUxTXREG = &U6TXREG; break;
	#endif
		default : return STD_EC_NOTFOUND;		//Invalid Uart port ID
	}

	return STD_EC_SUCCESS;
	// ----------------------------- //
}
// ############################################## //


// ############### UART Functions ############## //
// === Interrupt Handler ===== //
/**
* \fn		void uartISR(U8 uartID)
* @brief	Interrupt handler for any uart
* @note		
* @arg		U8 uartID			Hardware UART ID
* @return	nothing
*/
void uartISR(U8 uartID)
{
	U32 interruptCheck = intGetFlag(UART_INT[uartID]);	//Fetch all the flags for UART_1
	U16 byteNb = 0;
	static U8 tempBuf[8];

	if (uartSelectPort(uartID) == STD_EC_SUCCESS)
	{
		// === RX Interrupt ==== //
		if (interruptCheck & INT_MASK_UART_RX)
		{
			// -- Empty the buffer -- //
			while (pUxSTA->URXDA)
			{
				// Discard if error detected
				if (pUxSTA->all & (UART_MASK_PERR|UART_MASK_FERR))
					globalDump = *pUxRXREG;

				//Save if the data is valid
				else
					tempBuf[byteNb] = *pUxRXREG;

				byteNb++;
			}
			// ---------------------- //

			// -- Save to the buffer -- //
			rBufPushU8(uartRxBuf[uartID], tempBuf, byteNb, RBUF_FREERUN_PTR);	//Can loose data if the buffer is full
			// ------------------------ //
		}
		// ===================== //

		// === TX Interrupt ==== //
		if (interruptCheck & INT_MASK_UART_TX)
		{
			//Check for pending data
			byteNb = rBufGetUsedSpace(uartTxBuf[uartID]);

			// -- No more byte to send --- //
			if (byteNb == 0)
				(pUxSTA + REG_OFFSET_CLR_32)->all = UTXEN_MASK;		//Stop the transmitter
			// -- Send the pending data -- //
			else
			{
				if (byteNb > UART_FIFO_LVL)			//If there is more byte that the HW buffer can
					byteNb = UART_FIFO_LVL;			//contain, load the maximum

				//Pull the correct number of byte from the soft buffer into the HW buffer
				rBufPullU8(uartTxBuf[uartID], pUxTXREG, byteNb, RBUF_FIXED_PTR);
			}
			// --------------------------- //
		}
		// ===================== //

		// === ERR Interrupt === //
		if (interruptCheck & INT_MASK_UART_ERR)
		{
			// -- Overrun Error -- //
			if (pUxSTA->OERR)
			{
				//Read all the HW buffer
				while (pUxSTA->URXDA)
				{
					globalDump = *pUxRXREG;
				}
				//Clear the error flag
				pUxSTA->OERR = 0;
			}
			// -- Framing Error -- //
			else if (pUxSTA->FERR)
			{
				//Clear the error flag
				pUxSTA->FERR = 0;
			}
			else
			// -- Parity Error --- //
			{
				//Clear the error flag
			}
			// ------------------- //
		}
		// ===================== //
	}
}
// =========================== //


// === Control Functions ===== //
/**
* \fn		void uartSetConfig(U8 uartPort, tUARTConfig uartLocalConfig)
* @brief	Save the configuration to the config struct of the specified UART port
* @note		No sanity check for any settings and it does not apply those config
*		The config can also be directly access via the config struct
*		For manual configuration, option must be | or + (ex: uartInit(UART_1, UART_MODE_8N1| UART_RX_INT_DATA_READY))
* @arg		U8 uartPort			Hardware UART ID
* @arg		tUARTConfig uartLocalConfig	Setting to configure for the UART
* @return	nothing
*/
void uartSetConfig(U8 uartPort, U64 uartLocalConfig)
{
	uartConfig[uartPort].all = uartLocalConfig;
}

/**
* \fn		tUARTConfig uartGetConfig(U8 uartPort)
* @brief	Return the configuration of the specified UART port
* @note		The config can also be directly access via the config struct
* @arg		U8 uartPort			Hardware UART ID
* @return	tUARTConfig uartLocalConfig	Setting to configure for the UART
*/
tUARTConfig uartGetConfig(U8 uartPort)
{
	return uartConfig[uartPort];
}

/**
* \fn		U8 uartInit(U8 uartPort, U32 option)
* @brief	Initialise and start the selected UART with the specified options.
* @note		No sanity check of the settings
*		Return STD_EC_NOTFOUND if invalid UART HW ID is given.
*		Option must be | or + (ex: uartInit(0, UART_IDLE_RUN|UART_MODE_8N1|UART_RX_INT_BUF_ALMOST_FULL, 9600))
* @arg		U8 uartPort			Hardware UART ID
* @arg		U32 option			Setting to configure for the UART
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartInit(U8 uartPort, U32 option)
{
	split32 splittedOption;
	splittedOption.all = option;
	U8 errorCode;

	// -- Select the correct UART -- //
	errorCode = uartSelectPort(uartPort);
	if (errorCode == STD_EC_SUCCESS)
	// ----------------------------- //
	{
		// -- Init the buffer -- //
		uartRxBuf[uartPort] = rBufCreate(UART_BUF_SIZE,sizeof(U8));
		if (uartRxBuf[uartPort] == NULL)
			errorCode = STD_EC_FAIL;

		uartTxBuf[uartPort] = rBufCreate(UART_BUF_SIZE,sizeof(U8));
		// --------------------- //

		// -- Set the option -- //
		pUxMODE->STSEL = splittedOption.b0;			//Stop bit
		pUxMODE->PDSEL = (option & 0x6)>>1;			//Data mode
		pUxMODE->RXINV = splittedOption.b4;			//RX Polarity
		pUxMODE->LPBACK = splittedOption.b6;			//Loopback mode
		pUxMODE->WAKE = splittedOption.b7;			//Wake on char
		pUxMODE->RTSMD = splittedOption.b11;			//RTS Mode
		pUxMODE->IREN = splittedOption.b12;			//IrDA Encode/Decode
		pUxMODE->SIDL = splittedOption.b13;			//Idle mode

		pUxSTA->ADDEN = splittedOption.b3;			//Address Char detect
		pUxSTA->ADM_EN = splittedOption.b3;			//Auto Address Detect mode
		pUxSTA->UTXISEL = (option & 0xC000)>>14;		//TX Interrupt mode
		pUxSTA->URXISEL = (option & 0x3000)>>12;		//RX Interrupt mode

		if (pUxMODE->IREN)
			pUxSTA->UTXINV = !splittedOption.b5;		//In IrDA mode the idle state is inversed
		else
			pUxSTA->UTXINV = splittedOption.b5;		//TX polarity
		// -------------------- //

		// -- Start the uart -- //
		pUxSTA->URXEN = 1;					//Start the receiver
		pUxMODE->ON = 1;
		// -------------------- //
	}

	return errorCode;
}

/**
* \fn		U8 uartSetBaudRate(U8 uartPort, U32 baudRate)
* @brief	Compute the correct BRG value for the desired baudrate
* @note		Will round up to the nearest possible, use uartGetBaudRate() to have the exact one.
*		Will return STD_EC_NOTFOUND if invalid timer ID is inputed.
 *		Use the actual PBCLK freq for it's computation
* @arg		U8 uartPort			Hardware UART ID
* @arg		U32 baudRate			Desired baudrate (in bps)
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSetBaudRate(U8 uartPort, U32 baudRate)
{
	U8 divider = 4;							//Default: BRGH=0 div by 16
	U8 uartSaveState;
	U8 errorCode;

	// -- Select the correct UART -- //
	errorCode = uartSelectPort(uartPort);
	// ----------------------------- //
	if (errorCode == STD_EC_SUCCESS)
	{
		// -- Handle exception -- //
		if (baudRate == 0)
			return STD_EC_INVALID;
		// ---------------------- //

		// Stop the UART
		uartSaveState = pUxMODE->ON;
		pUxMODE->ON = 0;

		// -- Set the divider -- //
		if (baudRate > 38400)
		{
			divider = 2;						//Div by 4
			pUxMODE->BRGH = 1;					//Set the formula for High speed
		}
		// --------------------- //

		// -- Compute the BDGEN value -- //
		*pUxBRG = ((U32)( ((F32)clockGetPBCLK())/(F32)baudRate ) >> divider )-1 ;
		// ----------------------------- //

		//Restore the UART State
		pUxSTA->URXEN = uartSaveState;
		pUxMODE->ON = uartSaveState;
	}

	return errorCode;
}

/**
* \fn		U32 uartGetBaudRate(U8 uartPort)
* @brief	Return the actual baudrate of the selected UART
* @note		Use the actual PBCLK freq for it's computation
* @arg		U8 uartPort			Hardware UART ID
* @return	U32 baudRate			Actual Baudrate (in bps)
*/
U32 uartGetBaudRate(U8 uartPort)
{
	U8 divider = 16;						//Default: BRGH=0 div by 16
	U32 baudRate = 0;
	U32 * pUxBRG;

	// -- Select the correct UART -- //
	switch (uartPort)
	{
		case UART_1: pUxMODE = (tUxMODE*)&U1MODE;	pUxBRG = (U32*)&U1BRG; break;
		case UART_2: pUxMODE = (tUxMODE*)&U2MODE;	pUxBRG = (U32*)&U2BRG; break;
	#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case UART_3: pUxMODE = (tUxMODE*)&U3MODE;	pUxBRG = (U32*)&U3BRG; break;
		case UART_4: pUxMODE = (tUxMODE*)&U4MODE;	pUxBRG = (U32*)&U4BRG; break;
		case UART_5: pUxMODE = (tUxMODE*)&U5MODE;	pUxBRG = (U32*)&U5BRG; break;
		case UART_6: pUxMODE = (tUxMODE*)&U6MODE;	pUxBRG = (U32*)&U6BRG; break;
	#endif
		default : return 0;					//Invalid Uart port ID
	}
	// ----------------------------- //

	// -- Compute the Baud rate value -- //
	if (pUxMODE->BRGH)
		divider = 4;						//Div by 4
	baudRate = (U32)( ((F32)clockGetPBCLK())/((F32)( divider * (*pUxBRG +1) )) );
	// --------------------------------- //

	return baudRate;
}

/**
* \fn		U8 uartSetAddressMask(U8 uartPort, U8 addressMask)
* @brief	Set the Address Mask for the automatic address detection mode
* @note
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 addressMask			Value of the mask
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSetAddressMask(U8 uartPort, U8 addressMask)
{
	// -- Select the correct UART -- //
	switch (uartPort)
	{
		case UART_1: U1STAbits.ADDR = addressMask;	break;
		case UART_2: U2STAbits.ADDR = addressMask;	break;
	#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case UART_3: U3STAbits.ADDR = addressMask;	break;
		case UART_4: U4STAbits.ADDR = addressMask;	break;
		case UART_5: U5STAbits.ADDR = addressMask;	break;
		case UART_6: U6STAbits.ADDR = addressMask;	break;
	#endif
		default : return STD_EC_NOTFOUND;			//Invalid Uart port ID
	}
	// ----------------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 uartGetAddressMask(U8 uartPort)
* @brief	Return the Address Mask for the automatic address detection mode
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 addressMask			Value of the mask
*/
U8 uartGetAddressMask(U8 uartPort)
{
	// -- Select the correct UART -- //
	switch (uartPort)
	{
		case UART_1: return U1STAbits.ADDR;
		case UART_2: return U2STAbits.ADDR;
	#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case UART_3: return U3STAbits.ADDR;
		case UART_4: return U4STAbits.ADDR;
		case UART_5: return U5STAbits.ADDR;
		case UART_6: return U6STAbits.ADDR;
	#endif
		default : return 0;					//Invalid Uart port ID
	}
	// ----------------------------- //
}

/**
* \fn		U16 uartGetRxSize(U8 uartPort)
* @brief	Return the number of byte waiting in the RX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 rxBufSize			Number fo byte in the RX buffer
*/
U16 uartGetRxSize(U8 uartPort)
{
	return rBufGetUsedSpace(uartRxBuf[uartPort]);
}

/**
* \fn		U16 uartGetRxSpace(U8 uartPort)
* @brief	Return the number of available space is in the RX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 rxBufSpace			Space available (in byte)
*/
U16 uartGetRxSpace(U8 uartPort)
{
	return rBufGetFreeSpace(uartRxBuf[uartPort]);
}

/**
* \fn		U16 uartGetTxSize(U8 uartPort)
* @brief	Return the number of byte waiting in the TX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 txBufSize			Number fo byte in the TX buffer
*/
U16 uartGetTxSize(U8 uartPort)
{
	return rBufGetUsedSpace(uartTxBuf[uartPort]);
}

/**
* \fn		U16 uartGetTxSpace(U8 uartPort)
* @brief	Return the number of available space is in the TX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 txBufSpace			Space available (in byte)
*/
U16 uartGetTxSpace(U8 uartPort)
{
	return rBufGetFreeSpace(uartTxBuf[uartPort]);
}
// =========================== //


// === Status Function ====== //
/**
* \fn		U8 uartSetTxStatus(U8 uartPort, U8 state)
* @brief	Set the state of the Transmitter of the selected UART
* @note		The state should be passed as ENABLE or DISABLE
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 state			New state for the transmitter
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSetTxStatus(U8 uartPort, U8 state)
{
	U8 errorCode = uartSelectPort(uartPort);

	if (errorCode == STD_EC_SUCCESS)
		pUxSTA->UTXEN = state;

	return errorCode;
}

/**
* \fn		U8 uartGetTxStatus(U8 uartPort)
* @brief	Return the status of the Transmitter of the selected UART
* @note		The returned value is ENABLE or DISABLE
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 UTXENState			State of the Transmitter
*/
U8 uartGetTxStatus(U8 uartPort)
{
	if (uartSelectPort(uartPort) == STD_EC_SUCCESS)
		return pUxSTA->UTXEN;
}

/**
* \fn		U8 uartSetRxStatus(U8 uartPort, U8 state)
* @brief	Set the state of the Receiver of the selected UART
* @note		The state should be passed as ENABLE or DISABLE
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 state			New state for the receiver
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSetRxStatus(U8 uartPort, U8 state)
{
	U8 errorCode = uartSelectPort(uartPort);

	if (errorCode == STD_EC_SUCCESS)
		pUxSTA->URXEN = state;

	return errorCode;
}

/**
* \fn		U8 uartGetRxStatus(U8 uartPort)
* @brief	Return the status of the Receiver of the selected UART
* @note		The returned value is ENABLE or DISABLE
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 URXENState			State of the Receiver
*/
U8 uartGetRxStatus(U8 uartPort)
{
	if (uartSelectPort(uartPort) == STD_EC_SUCCESS)
		return pUxSTA->URXEN;
}
// ========================== //


// === Transfer Functions ==== //
/**
* \fn		U8 uartSendByte(U8 uartPort, U8 byteToSend)
* @brief	Send a single bit on the designated UART
* @note		The byte is pushed into the Tx FIFO Ring Buffer of the UART prior to sending.
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 byteToSend			Byte to be sent
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSendByte(U8 uartPort,U8 byteToSend)
{
	U8 errorCode;
	
	// -- Push the byte in the correct buffer -- //
	errorCode = rBufPushU8(uartTxBuf[uartPort], &byteToSend, 1, RBUF_FREERUN_PTR);
	// ----------------------------------------- //

	// -- Enable Transmission -- //
	if (errorCode == STD_EC_SUCCESS)
	{
		errorCode = uartSelectPort(uartPort);

		if (errorCode == STD_EC_SUCCESS)
			pUxSTA->UTXEN = 1;
	}
	// ------------------------- //

	return errorCode;
}

/**
* \fn		U8 uartRcvByte(U8 uartPort)
* @brief	Receive 1 byte from the designated UART
* @note		The byte is pulled from the Rx FIFO Ring Buffer of the UART.
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 receivedByte			Byte received
*/
U8 uartRcvByte(U8 uartPort)
{
	static U8 receivedByte;

	rBufPullU8(uartRxBuf[uartPort], &receivedByte, 1, RBUF_FREERUN_PTR);
	
	return receivedByte;
}

/**
* \fn		U16 uartSendArray(U8 uartPort, void * sourcePtr, U16 byteNb)
* @brief	Append $byteNb number of byte from the $sourcePtr to the transmit buffer
* @note		Return the actual number of byte placed in the buffer
*		If the passed $byteNb is greater than the number of available byte in the buffer,
*		will return 0 and transfer nothing
* @arg		U8 uartPort			Hardware UART ID
* @arg		void * sourcePtr		Pointer to load the byte from
* @arg		U16 byteNb			Number of byte to load
* @return	U16 receivedByte		Number of byte really loaded
*/
U16 uartSendArray(U8 uartPort, U8 * sourcePtr, U16 byteNb)
{
	U8 errorCode;

	// -- Push the array in the correct buffer -- //
	errorCode = rBufPushU8(uartTxBuf[uartPort], sourcePtr, byteNb, RBUF_FREERUN_PTR);
	// ------------------------------------------ //

	// -- Enable Transmission -- //
	if (errorCode == STD_EC_SUCCESS)
	{
		errorCode = uartSelectPort(uartPort);

		if (errorCode == STD_EC_SUCCESS)
			pUxSTA->UTXEN = 1;
	}
	else
		return 0;
	// ------------------------- //
	
	return byteNb;
}

/**
* \fn		U16 uartRcvArray(U8 uartPort, void * destinationPtr, U16 byteNb)
* @brief	Extract $byteNb number of byte from the receive buffer and place it in $destinationPtr
* @note		Return the actual number of byte placed in the destination
*		If the passed $byteNb is greater than the number of valid byte in the buffer, will only
*		extract the number of byte present in the buffer
* @arg		U8 uartPort			Hardware UART ID
* @arg		void * destinationPtr		Pointer to save the byte to
* @arg		U16 byteNb			Number of byte to extract
* @return	U16 receivedByte		Number of byte really extracted
*/
U16 uartRcvArray(U8 uartPort, U8 * destinationPtr, U16 byteNb)
{
	U16 actualByteInBuffer = rBufGetUsedSpace(uartRxBuf[uartPort]);

	// -- Handle boundary -- //
	if (byteNb > actualByteInBuffer)
		byteNb = actualByteInBuffer;
	// --------------------- //

	// -- Pull the array from the correct buffer -- //
	rBufPullU8(uartRxBuf[uartPort], destinationPtr, byteNb, RBUF_FREERUN_PTR);
	// -------------------------------------------- //

	return byteNb;
}


U16 uartSendFrame(U8 uartPort, U8 * framePtr, U8 delimiter)
{
	// -- Push the frame in the correct buffer -- //
	// ------------------------------------------ //
	return 0;
}

U16 uartRcvFrame(U8 uartPort, U8 * framePtr, U8 delimiter)
{
	// -- Pull the frame from the correct buffer -- //
	// -------------------------------------------- //
	return 0;
}
// =========================== //
// ############################################## //
