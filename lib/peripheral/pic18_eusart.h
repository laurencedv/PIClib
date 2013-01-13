/*!
 @file		pic18_eusart.c
 @brief		EUSART functions for C18

 @version	0.2
 @note		Define the pin for each EUSART
			Depend on $globalFOSC (in hardware.h) for Baud Rate Computation

 @date		October 26th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_EUSART_H
#define _PIC18_EUSART_H
// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic18_clock.h>
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// Application Variable //
#define		USART_BUF_SIZE			116		//Number of byte for the FIFO RX/TX buffer for each EUSART (max 116)
#define		USART_PORT_NB			2		//Number of EUSART Module
// -------------------- //

// Hardware Pin //
#define	    USART1_TX				PORTCbits.RC6	//Tx Pin for EUSART1
#define	    USART1_TX_DIR			TRISCbits.TRISC6
#define     USART1_RX				PORTCbits.RC7	//Rx Pin for EUSART1
#define	    USART1_RX_DIR			TRISCbits.TRISC7

#define	    USART2_TX				PORTCbits.RC6	//Tx Pin for EUSART2
#define	    USART2_TX_DIR			TRISCbits.TRISC6
#define     USART2_RX				PORTCbits.RC7	//Rx Pin for EUSART2
#define	    USART2_RX_DIR			TRISCbits.TRISC7
// ------------ //

// Hardware Constant //
// ----------------- //

// -- Init Options -- //
// STD mode //
#define		USART_MODE_8N1			0		// Standard Mode of 8 data bit, No flowcontrol, 1 stop bit
// -------- //

// Individual Option //
//Bit polarity options
#define		USART_ASYNC_RX_POL_LOW		0b00100000	// Asynchronous Receive polarity is inverted (active-low)
#define		USART_ASYNC_RX_POL_HIGH		0		// Asynchronous Receive polarity is not inverted (active-high)
#define		USART_ASYNC_TX_IDLE_LOW		0b00010000	// Asynchronous Transmit idle state is a low level
#define		USART_ASYNC_TX_IDLE_HIGH	0		// Asynchronous Transmit idle state is a high level

//Bit number options
#define		USART_8BIT			0		// 8 bit transfert with no parity and 1 stop bit
#define		USART_9BIT			0b01000000	// 9 bit transfert with no parity and 1 stop bit

//TXSTAn
#define		USART_ASYNC_MODE		0
#define		USART_SYNC_MODE			0b00000001	// Synchronous mode

//RCSTAn
#define		USART_ADDEN_ON			0b00001000	// Enables address detection
#define		USART_ADDEN_OFF			0		// Disables address detection

//BAUDCONn
#define		USART_DATA_POL_LOW		0b00100000	// Data polarity is inverted (active-low)
#define		USART_DATA_POL_HIGH		0		// Data polarity is not inverted (active-high)
#define		USART_IDLE_CLK_HIGH		0b00010000	// Idle state for clock is a high level
#define		USART_IDLE_CLK_LOW		0		// Idle state for clock is a low level

#define		USART_WAKEUP_ON			0b00000010	// RX pin monitored
#define		USART_WAKEUP_OFF		0		// RX pin not monitored
// ----------------- //
// ------------------ //
// ############################################## //


// ################# Data Type ################## //
// Control Flag //
typedef union
{
	U8 all;
	struct
	{
		U8 rxFull:1;
		U8 txFull:1;
		U8 :6;
	};
}tUsartCtl;


// TXSTA //
typedef union
{
	U8 all;
	struct
	{
		U8 TX9D:1;				//9th bit of Trasmit Data
		U8 TRMT:1;				//Transmit Shift Reg Status (0: full | 1: empty)
		U8 BRGH:1;				//High BaudRate Generator
		U8 SENDB:1;				//Send Break char
		U8 SYNC:1;				//EUSART mode (0: Async | 1: Sync)
		U8 TXEN:1;				//Transmit Enable
		U8 TX9:1;				//9th bit Transmit Enable
		U8 CSRC:1;				//Clock Source Select
	};
}tTXSTA;

// RCSTA //
typedef union
{
	U8 all;
	struct
	{
		U8 RX9D:1;				//9th bit of Receive Data
		U8 OERR:1;				//Overrun Error flag
		U8 FERR:1;				//Framing Error
		U8 ADDEN:1;				//Address Detect Enable
		U8 CREN:1;				//Continuous Receive Enable
		U8 SREN:1;				//Single Receive Enable
		U8 RX9:1;				//9th bit Receive Enable
		U8 SPEN:1;				//Serial Port Enable
	};
}tRCSTA;

// BAUDCON //
typedef union
{
	U8 all;
	struct
	{
		U8 ABDEN:1;				//Auto-Baud Detect Enable
		U8 WUE:1;				//Wake-up Enable
		U8 :1;
		U8 BRG16:1;				//16bit Baud Rate Generator Enable
		U8 TXCKP:1;				//Sync Clock Polarity
		U8 RXDTP:1;				//Data/Receive Polarity
		U8 RCIDL:1;				//Receive Operation Idle Status
		U8 ABDOVF:1;			//Auto-Baud Acquisition Rollover Status
	};
}tBAUDCON;
// ############################################## //


// ################# Prototypes ################# //
// ==== Interrupt Handler ==== //
/**
* \fn		void usartRxHandler(U8 portID)
* @brief	Handle the interrupt reception, append byte in the $usartRxBuf
* @note		If the soft buf is full, will discard the byte
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartRxISR(U8 portID);

/**
* \fn		void usartTxHandler(U8 portID)
* @brief	Handle the interrupt transmission, send byte from the $usartTxBuf
* @note		If the soft buf is empty, will disable Tx interrupt
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartTxISR(U8 portID);
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
U8 usartInit(U8 portID, U32 desiredBaudRate, U8 options);

/**
* \fn		U8 usartSetBaudRate(U8 portID, U32 desiredBaudRate)
* @brief	Set the designated EUSART ($portID) at the designated speed ($desiredBaudRate)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @arg		U32 desiredBaudRate	Desired Baud Rate (in baud)
* @return	U32 actualBaudRate	Real Baud Rate set (in baud)
*/
U32 usartSetBaudRate(U8 portID, U32 desiredBaudRate);

/**
* \fn		U32 usartGetBaudRate(U8 portID)
* @brief	Return the actual Baudrate of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U32 desiredBaudRate	Actual Baudrate (in baud)
*/
U32 usartGetBaudRate(U8 portID);

/**
* \fn		U8 usartGetRxSize(U8 portID)
* @brief	Return the size of the usartBufRx of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 bufSize			Number of byte currntly in the buffer
*/
U8 usartGetRxSize(U8 portID);

/**
* \fn		U8 usartGetTxSize(U8 portID)
* @brief	Return the size of the usartBufTx of the designated EUSART ($portID)
* @note
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 bufSize			Number of byte currntly in the buffer
*/
U8 usartGetTxSize(U8 portID);

/**
* \fn		void usartResetBuffer(U8 portID)
* @brief	Reset the usartBufTx of the designated EUSART ($portID)
* @note		Fill the buffers with 0
* @arg		U8 portID			Hardware EUSART ID
* @return	nothing
*/
void usartResetBuffer(U8 portID);
// =========================== //


// === Transfert Functions === //
/**
* \fn		U8 usartPushByte(U8 portID, U8 byteToSend)
* @brief	Send 1 byte trought the software buffer
* @note		Return STD_EC_OVERFLOW if the buffer is full, or STD_EC_NOTFOUND if invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @arg		U8 byteToSend		The byte to send
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushByte(U8 portID, U8 byteToSend);

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
U8 usartPushFromRam(U8 portID, U8 * arrayPtr, U8 byteNb);

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
U8 usartPushFromRom(U8 portID, const U8 * arrayPtr, U8 byteNb);

/**
* \fn		U8 usartPushString(U8 portID, rom const U8 * arrayPtr, U8 delimiter)
* @brief	Send a string from the Rom starting at $arrayPtr until a '\0' is detected
* @note		Return STD_EC_OVERFLOW if there is not enought space
			Will send USART_BUF_SIZE number of byte if the buffer is empty and no '\0' is detected
			No detection for invalid portID
* @arg		U8 portID				Hardware EUSART ID
* @arg		rom const U8 * arrayPtr	Pointer to the first element of the array
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 usartPushString(U8 portID, const U8 * arrayPtr);

/**
* \fn		U8 usartPullByte(U8 portID)
* @brief	Extract 1 byte from the $usartBufRx in FIFO manner
* @note		Must check the buffer size before calling this, or else could return false data
			No detection for invalid portID
* @arg		U8 portID			Hardware EUSART ID
* @return	U8 byteReceived		FIFO byte return
*/
U8 usartPullByte(U8 portID);

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
U8 usartPullArray(U8 portID, U8 * destinationPtr, U8 byteNb);

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
U8 usartPullFrame(U8 portID, U8 * destinationPtr, U8 delimiter);
// =========================== //



// ======== OLD STUFF ======== //
// ---- Status Functions ---- //

// Function to check if there is data in the reception buffer
#define	eusart1_datardy()				PIR1bits.RC1IF

// Function to check if the send buffer is empty
#define eusart1_rdytosend()				TXSTA1bits.TRMT


// ---- Transfert Functions ---- //

// Function to send a break character
#define eusart1_send_break()				TXSTA1bits.SENDB = SET; TXREG1 = 0x00

// Function to start continous reception on EUSART 1
#define eusart1_start_reception()			RCSTA1bits.CREN = SET

// Function to start transmission on EUSART 1
#define eusart1_start_transmission()			TXSTA1bits.TXEN = SET

// Function to start a single reception on EUSART 1
#define eusart1_start_single_reception()		RCSTA1bits.SREN = SET

// Function to return the received data from the EUSART 1
#define eusart1_get_byte()				RCREG1

#define eusart1SendByte(x)				TXREG1 = x
// =========================== //

// ############################################## //


#endif
