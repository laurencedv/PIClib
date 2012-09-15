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
tUARTConfig uartConfig[UART_MAX_PORT];


//Data Buffers
tRBufCtl * uartRxBuf[UART_MAX_PORT];
tRBufCtl * uartTxBuf[UART_MAX_PORT];

//Reg pointers
tUxMODE * pUxMODE = NULL;
tUxSTA * pUxSTA = NULL;
U32 * pUxBRG = NULL;
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
		case UART_1: pUxMODE = (tUxMODE*)&U1MODE;	pUxSTA = (tUxSTA*)&U1STA;	pUxBRG = (U32*)&U1BRG;	break;
		case UART_2: pUxMODE = (tUxMODE*)&U2MODE;	pUxSTA = (tUxSTA*)&U2STA;	pUxBRG = (U32*)&U2BRG;	break;
	#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case UART_3: pUxMODE = (tUxMODE*)&U3MODE;	pUxSTA = (tUxSTA*)&U3STA;	pUxBRG = (U32*)&U3BRG;	break;
		case UART_4: pUxMODE = (tUxMODE*)&U4MODE;	pUxSTA = (tUxSTA*)&U4STA;	pUxBRG = (U32*)&U4BRG;	break;
		case UART_5: pUxMODE = (tUxMODE*)&U5MODE;	pUxSTA = (tUxSTA*)&U5STA;	pUxBRG = (U32*)&U5BRG;	break;
		case UART_6: pUxMODE = (tUxMODE*)&U6MODE;	pUxSTA = (tUxSTA*)&U6STA;	pUxBRG = (U32*)&U6BRG;	break;
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
* \fn		void timer1ISR(void)
* @brief	Example of an Interrupt Service Routine for the uart 1
* @note		Place it at the UART 1 vector in the main
*		One vector for all interrupt source
*		Must used UART_TX_INT_BUF_EMPTY and UART_RX_INT_DATA_READY init option
*		for this ISR to behave correctly.
* @arg		nothing
* @return	nothing
*/
/*
void __ISR(INT_VEC_UART_1, IPL5SOFT) uart1ISR(void)
{
	extern tRBufCtl * uartRxBuf[];
	extern tRBufCtl * uartTxBuf[];

	U32 interruptCheck = intFastCheckFlag(INT_UART_1);			//Fetch the all the flags for UART_1
	U16 byteNb;
	U8 tempBuf[8];

	// === RX Interrupt ==== //
	if (interruptCheck & INT_MASK_UART_RX)
	{
		// -- Empty the buffer -- //
		while (U1STAbits.URXDA)
		{
			// Discard if error detected
			if (U1STA & UART_MASK_PERR|UART_MASK_FERR)
				U1RXREG;
			//Save if the data is valid
			else
				tempBuf[byteNb] = U1RXREG;

			byteNb++;
		}
		// ---------------------- //

		// -- Save to the buffer -- //
		rBufPushU8(uartRxBuf[UART_1], tempBuf, byteNb, RBUF_FREERUN_PTR);	//Can loose data if the buffer is full
		// ------------------------ //

		intFastClearFlag(INT_UART_1_RX);
	}
	// ===================== //

	// === TX Interrupt ==== //
	if (interruptCheck & INT_MASK_UART_TX)
	{
		//Check for pending data
		byteNb = rBufGetUsedSpace(uartTxBuf[UART_1]);

		// -- No more byte to send --- //
		if (!byteNb)
			clearBIT(U1STA,UTXEN_MASK);		//Stop the transmitter
		// -- Send the pending data -- //
		else
		{
			if (byteNb > UART_FIFO_LVL)			//If there is more byte that the HW buffer can
				byteNb = UART_FIFO_LVL;			//contain, load the maximum

			//Pull the correct number of byte from the soft buffer into the HW buffer
			rBufPullU8(uartTxBuf[UART_1], &U1TXREG, byteNb, RBUF_FIXED_PTR);
		}
		// --------------------------- //

		intFastClearFlag(INT_UART_1_TX);
	}
	// ===================== //

	// === ERR Interrupt === //
	if (interruptCheck & INT_MASK_UART_ERR)
	{
		// -- Overrun Error -- //
		if (U1STAbits.OERR)
		{
			//Read all the HW buffer

			//Clear the error flag
		}
		// -- Framing Error -- //
		else if (U1STAbits.FERR)
		{
			//Clear the error flag
		}
		else
		// -- Parity Error --- //
		{
			//Clear the error flag
		}
		// ------------------- //

		intFastClearFlag(INT_UART_1_ERR);
	}
	// ===================== //
}
*/
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
*		This function enable the corresponding interrupt, but priority must be set in the main
*		and multi-vectored interrupt mode enabled
* @arg		U8 uartPort			Hardware UART ID
* @arg		U32 option			Setting to configure for the UART
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartInit(U8 uartPort, U32 option)
{
	split32 splittedOption;
	splittedOption.all = option;
	U16 intState;
	U8 errorCode;

	// -- Select the correct UART -- //
	errorCode = uartSelectPort(uartPort);
	if (errorCode == STD_EC_SUCCESS)
	// ----------------------------- //
	{
		// Disable Global Interrupt
		intState = intFastDisableGlobal();

		// Enable the correct interrupt //
		switch (uartPort)
		{
			case UART_1: intFastInit(INT_UART_1);	break;
			case UART_2: intFastInit(INT_UART_2);	break;
		#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
			case UART_3: intFastInit(INT_UART_3);	break;
			case UART_4: intFastInit(INT_UART_4);	break;
			case UART_5: intFastInit(INT_UART_5);	break;
			case UART_6: intFastInit(INT_UART_6);	break;
		#endif
		}
		// ---------------------------- //

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

		// Restore Interrupt state
		intFastRestoreGlobal(intState);
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
		default : return STD_EC_NOTFOUND;			//Invalid Uart port ID
	}
	// ----------------------------- //

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
	pUxMODE->ON = uartSaveState;

	return STD_EC_SUCCESS;
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
// =========================== //


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
		switch (uartPort)
		{
			case UART_1:	setBIT(U1STA,UTXEN_MASK);	break;
			case UART_2:	setBIT(U2STA,UTXEN_MASK);	break;
		#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
			case UART_3:	setBIT(U3STA,UTXEN_MASK);	break;
			case UART_4:	setBIT(U4STA,UTXEN_MASK);	break;
			case UART_5:	setBIT(U5STA,UTXEN_MASK);	break;
			case UART_6:	setBIT(U6STA,UTXEN_MASK);	break;
		#endif
		}
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


U16 uartSendArray(U8 uartPort, void * arrayPtr, U16 byteNb)
{
	// -- Push the array in the correct buffer -- //
	// ------------------------------------------ //
}


U16 uartRcvArray(U8 uartPort, void * destinationPtr, U16 byteNb)
{
	// -- Pull the array from the correct buffer -- //
	// -------------------------------------------- //
}


U16 uartSendFrame(U8 uartPort, void * framePtr, U8 delimiter)
{
	// -- Push the frame in the correct buffer -- //
	// ------------------------------------------ //
}


U16 uartRcvFrame(U8 uartPort, void * framePtr, U8 delimiter)
{
	// -- Pull the frame from the correct buffer -- //
	// -------------------------------------------- //
}
// =========================== //
// ############################################## //
