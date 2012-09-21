/*!
 @file		pic32_uart.h
 @brief		Uart Control lib for C32

 @version	0.1.1
 @note		
 @todo		Transfert function 9 bit mode
 		Remove the temp buffering in the TX ISR

 @date		February 15th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_UART_H
#define _PIC32_UART_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Librairies
#include <soft/pic32_ringBuffer.h>
#include <peripheral/pic32_clock.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// Application dependant //
#define UART_BUF_SIZE				100
// --------------------- //

// ---- Init Option ---- //
// Activity state in Idle mode
#define UART_IDLE_RUN				0
#define UART_IDLE_STOP				0x02000

// IrDA Encoder
#define UART_IRDA_ON				0x01000
#define UART_IRDA_OFF				0

// RTS mode
#define UART_RTS_SIMPLEX			0x00800
#define UART_RTS_FLOW_CTL			0

// Pin control
#define UART_PIN_RXTX_BCLK			0x00300
#define UART_PIN_RXTX_RTS_CTS			0x00200
#define UART_PIN_RXTX_RTS			0x00100
#define UART_PIN_RXTX				0

// Wake on start bit during sleep mode
#define UART_WAKE_ON				0x00080
#define UART_WAKE_OFF				0

// Loopback mode
#define UART_LOOPBACK_ON			0x00040
#define UART_LOOPBACK_OFF			0

// Pin idle state
#define UART_RX_IDLE_LOW			0x00010
#define UART_RX_IDLE_HIGH			0
#define UART_TX_IDLE_LOW			0x00020
#define UART_TX_IDLE_HIGH			0

// Interrupt condition
#define UART_TX_INT_BUF_EMPTY			0x08000
#define UART_TX_INT_TSR_EMPTY			0x04000
#define UART_TX_INT_BUF_HAS_SPACE		0
#if CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
	#define UART_RX_INT_BUF_FULL		0x30000
	#define UART_RX_INT_BUF_ALMOST_FULL	0x20000
	#define UART_RX_INT_DATA_READY		0
#else
	#define UART_RX_INT_BUF_ALMOST_FULL	0x20000
	#define UART_RX_INT_BUF_HALF_FULL	0x10000
	#define UART_RX_INT_DATA_READY		0
#endif

// Address detection mode
#define UART_ADD_DETECT_ON			0x00008
#define UART_ADD_DETECT_OFF			0

// Data format
#define UART_MODE_8N1				0
#define UART_MODE_8N2				0x00001
#define UART_MODE_8O1				0x00004
#define UART_MODE_8O2				0x00005
#define UART_MODE_8E1				0x00002
#define UART_MODE_8E2				0x00003
#define UART_MODE_9N1				0x00006
#define UART_MODE_9N2				0x00007
// --------------------- //

// -- UART HW ID -- //
#define UART_1					0
#define UART_2					1
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
	#define UART_3				2
	#define UART_4				3
	#define UART_5				4
	#define UART_6				5
#endif
// ---------------- //
// ############################################## //


// ################# Data Type ################## //
// UxMODE reg
typedef union
{
	U32 all;
	struct
	{
		U32 STSEL:1;		//Stop bit selection
		U32 PDSEL:2;			//Data format control
		U32 BRGH:1;			//Baudrate formula control
		U32 RXINV:1;			//Rx polarity inversion
		U32 ABAUD:1;			//Auto baudrate detection Enable
		U32 LPBACK:1;			//Loopback mode
		U32 WAKE:1;			//Wake on start bit in sleep mode
		U32 UEN:2;			//Pins control
		U32 :1;
		U32 RTSMD:1;			//RTS mode selection
		U32 IREN:1;			//IrDA Encode/Decode Enable
		U32 SIDL:1;			//Stop in idle mode
		U32 FRZ:1;			//Freeze in debug mode
		U32 ON:1;			//Uart Enable
		U32 :16;
	};
}tUxMODE;

// UxSTA reg
typedef union
{
	U32 all;
	struct
	{
		U32 URXDA:1;			//Rx Buf Data ready flag
		U32 OERR:1;			//Rx Buf Overrun Error flag
		U32 FERR:1;			//Framing Error flag
		U32 PERR:1;			//Parity Error flag
		U32 RIDLE:1;			//Receiver Idle flag
		U32 ADDEN:1;			//Address Character Dectection Enable
		U32 URXISEL:2;			//Rx Interrupt selection
		U32 TRMT:1;			//Tx Shift reg empty flag
		U32 UTXBF:1;			//Tx Buf full flag
		U32 UTXEN:1;			//Transmitter Enable
		U32 UTXBRK:1;			//Transmit Break char Enable
		U32 URXEN:1;			//Receiver Enable
		U32 UTXINV:1;			//Tx polarity inversion
		U32 UTXISEL:2;			//Tx Interrupt selection
		U32 ADDR:8;			//Auto Address detection mask
		U32 ADM_EN:1;			//Auto Address detection Enable
		U32 :7;
	};
}tUxSTA;

// Configuration
typedef union
{
	U64 all;
	struct
	{
		U32 :4;
		U32 rxPolarity:1;		//Idle polarity of RX
		U32 :3;
		U32 pinMode:2;			//General pins mode selection
		U32 :1;
		U32 rtsMode:1;			//RTS mode selection
		U32 :20;
		U32 :13;
		U32 txPolarity:1;		//Idle polarity of TX
		U32 :18;
	}pin;

	struct
	{
		U32 :32;
		U32 :6;
		U32 rxMode:2;			//RX interrupt mode selection
		U32 :6;
		U32 txMode:2;			//TX interrupt mode selection
		U32 :8;
		U32 addressDetect:1;		//Automatic Address Detection Enable
	}interrupts;

	struct
	{
		U32 stopBits:1;
		U32 dataWidth:2;
		U32 :9;
		U32 IrDA:1;
		U32 :19;
		U32 :32;
	}format;

	struct
	{
		U32 :3;
		U32 baudRateDivider:1;
		U32 :1;
		U32 autoBaudRateEn:1;
		U32 loopBackEn:1;
		U32 wakeEn:1;
		U32 :5;
		U32 idleMode:1;
		U32 :18;
		U32 :11;
		U32 sendBreak:1;
		U32 :20;
	}mode;

	struct
	{
		U32 :32;
		U32 :5;
		U32 addressBitDetectEn:1;
		U32 :10;
		U32 address:8;
		U32 addressDectectEn:1;
		U32 :7;
	}address;

	struct
	{
		U32 UxMODE;
		U32 UxSTA;
	}registers;
}tUARTConfig;

// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void uartISR(U8 uartID)
* @brief	Interrupt handler for any uart
* @note
* @arg		U8 uartID			Hardware UART ID
* @return	nothing
*/
void uartISR(U8 uartID);
// =========================== //


// === Control Functions ==== //
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
void uartSetConfig(U8 uartPort, U64 uartLocalConfig);

/**
* \fn		tUARTConfig uartGetConfig(U8 uartPort)
* @brief	Return the configuration of the specified UART port
* @note		The config can also be directly access via the config struct
* @arg		U8 uartPort			Hardware UART ID
* @return	tUARTConfig uartLocalConfig	Setting to configure for the UART
*/
tUARTConfig uartGetConfig(U8 uartPort);

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
U8 uartInit(U8 uartPort, U32 option);

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
U8 uartSetBaudRate(U8 uartPort, U32 baudRate);

/**
* \fn		U32 uartGetBaudRate(U8 uartPort)
* @brief	Return the actual baudrate of the selected UART
* @note		Use the actual PBCLK freq for it's computation
* @arg		U8 uartPort			Hardware UART ID
* @return	U32 baudRate			Actual Baudrate (in bps)
*/
U32 uartGetBaudRate(U8 uartPort);

/**
* \fn		U8 uartSetAddressMask(U8 uartPort, U8 addressMask)
* @brief	Set the Address Mask for the automatic address detection mode
* @note
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 addressMask			Value of the mask
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSetAddressMask(U8 uartPort, U8 addressMask);

/**
* \fn		U8 uartGetAddressMask(U8 uartPort)
* @brief	Return the Address Mask for the automatic address detection mode
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 addressMask			Value of the mask
*/
U8 uartGetAddressMask(U8 uartPort);

/**
* \fn		U16 uartGetRxSize(U8 uartPort)
* @brief	Return the number of byte waiting in the RX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 rxBufSize			Number fo byte in the RX buffer
*/
U16 uartGetRxSize(U8 uartPort);

/**
* \fn		U16 uartGetTxSize(U8 uartPort)
* @brief	Return the number of byte waiting in the TX buffer
* @note
* @arg		U8 uartPort			Hardware UART ID
* @return	U16 txBufSize			Number fo byte in the TX buffer
*/
U16 uartGetTxSize(U8 uartPort);
// ========================== //


// === Transfer Function ==== //
/**
* \fn		U8 uartSendByte(U8 uartPort, U8 byteToSend)
* @brief	Send a single bit on the designated UART
* @note		The byte is pushed into the Tx FIFO Ring Buffer of the UART prior to sending.
* @arg		U8 uartPort			Hardware UART ID
* @arg		U8 byteToSend			Byte to be sent
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 uartSendByte(U8 uartPort, U8 byteToSend);

/**
* \fn		U8 uartRcvByte(U8 uartPort)
* @brief	Receive 1 byte from the designated UART
* @note		The byte is pulled from the Rx FIFO Ring Buffer of the UART.
* @arg		U8 uartPort			Hardware UART ID
* @return	U8 receivedByte			Byte received
*/
U8 uartRcvByte(U8 uartPort);
// ========================== //
// ############################################## //


// ############### Internal Define ############## //
// Interrupt Masking
#define INT_MASK_UART_ERR		BIT0
#define INT_MASK_UART_RX		BIT1
#define INT_MASK_UART_TX		BIT2

// Error Masking
#define UART_MASK_PERR			BIT3
#define UART_MASK_FERR			BIT2
#define UART_MASK_OERR			BIT1

// FIFO HW buffer level
#if CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
	#define UART_FIFO_LVL		4
#else
	#define UART_FIFO_LVL		8
#endif

// Fast bit access macro
#define UTXEN_MASK			BIT10
#define URXEN_MASK			BIT12
// ############################################## //

#endif
