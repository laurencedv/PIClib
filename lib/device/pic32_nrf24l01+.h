/*!
 @file		pic32_nrf24l01+.h
 @brief		nRF24L01+ lib for the OpIUM network

 @version	0.1
 @note		The pin "CSN" in the datasheet is used in the lib as "SS".
 		The lib assume the SPI module is correctly initialised

 @date		March 29th 2012
 @author	Laurence DV
*/

//	-----------------------------------------------------------------------------------------------		Register Map	-----------------------------------------------------------------------------------------------		//
/*	REG NAME	||	VAL	||	BIT 0		||	BIT 1		||	BIT 2		||	BIT 3		||	BIT 4		||	BIT 5		||	BIT	6	||	BIT 7		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
CONFIG			||		||	RX/TX CTL	||	Power CTL	||	CRC Length	||	EN CRC		||	EN int Max ReTX	||	EN int pkt TX	||	EN int pkt RX	||	RESERVED	||
0x00			||	0	||	PTX		||	Power Down	||	1 Byte		||	Disable		||	Disable		||	Disable		||	Disable		||			||
			||	1	||	PRX		||	Power Up	||	2 Byte		||	Enable		||	Enable		||	Enable		||	Enable		||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
EN Auto ACK		||		||	PIPE 0		||	PIPE 1		||	PIPE 2		||	PIPE 3		||	PIPE 4		||	PIPE 5		||	RESERVED	||	RESERVED	||
0x01			||	0	||	Disale		||	Disale		||	Disale		||	Disable		||	Disable		||	Disable		||			||			||
			||	1	||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
EN Pipe RX		||		||	PIPE 0		||	PIPE 1		||	PIPE 2		||	PIPE 3		||	PIPE 4		||	PIPE 5		||	RESERVED	||	RESERVED	||
0x02			||	0	||	Disale		||	Disale		||	Disale		||	Disable		||	Disable		||	Disable		||			||			||
			||	1	||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
Setup Address Size	||		||	Address Size				||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||
0x03			||	0	||	00 Invalid		10 4 Bytes	||			||			||			||			||			||			||
			||	1	||	01 3 Bytes		11 5 Bytes	||			||			||			||			||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
Auto ReTX		||		||	Retransmission delay									||	Maximum number of retransmission							||
0x04			||	0	||	0000 250µs		0100 1250µs		1000 2250µs		1100 3250µs	||	0000 Disable		0100 4			1000 8			1100 12		||
			||	0	||	0001 500µs		0101 1500µs		1001 2500µs		1101 3500µs	||	0001 1			0101 5			1001 9			1101 13		||
			||	1	||	0010 750µs		0110 1750µs		1010 2750µs		1110 3750µs	||	0010 2			0110 6			1010 10			1110 14		||
			||	1	||	0011 1000µs		0111 2000µs		1011 3000µs		1111 4000µs	||	0011 3			0111 7			1011 11			1111 15		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RF Channel		||		||	Channel Number																			||	RESERVED	||
0x05			||	0	||	0000000 Ch 0		0000010 Ch 2		0000100 Ch 4		0000110 Ch 6		0001000 Ch 8		0001010 Ch 10	...	1111110 Ch 126	||			||
			||	1	||	0000001 Ch 1		0000011 Ch 3		0000101 Ch 5		0000111	Ch 7		0001001 Ch 9		0001011 Ch 11	...	1111111 Ch 127	||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RF Config		||		||	RESERVED	||	TX Power				||	High Speed	||	Force PLL Lock	||	Speed		||	RESERVED	||	Cont. Carrier	||
0x06			||	0	||			||	00 -18dBm		10 -6 dBm	||	1 Mbps		||	Disable		||	High Speed	||			||	Disable		||
			||	1	||			||	01 -12dBm		11 0 dBm	||	2 Mbps		||	Enable		||	250 kbps	||			||	Enable		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
STATUS			||		||	TX FIFO Full	||	RX FIFO Data origin						||	int Max ReTX	||	int pkt TX	||	int pkt RX	||	RESERVED	||
0x07			||	0	||	Not Full	||	000 PIPE 0	...	100 PIPE 4		110 Invalid	||	Nothing		||	Nothing		||	Disable		||			||
			||	1	||	Full		||	001 PIPE 1	...	101 PIPE 5		111 FIFO Empty	||	Max ReTX reach	||	ACK Received	||	Data Received	||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
TX STATUS		||		||	Retransmission count									||	Packet lost count									||
0x08			||	0	||	0000 0 ReTX		0100 4 ReTX		1000 8 ReTX		1100 12 ReTX	||	0000 0 pkt		0100 4 pkt		1000 8 pkt		1100 12 pkt	||
			||	0	||	0001 1 ReTX		0101 5 ReTX		1001 9 ReTX		1101 13 ReTX	||	0001 1 pkt		0101 5 pkt		1001 9 pkt		1101 13 pkt	||
			||	1	||	0010 2 ReTX		0110 6 ReTX		1010 10 ReTX		1110 14 ReTX	||	0010 2 pkt		0110 6 pkt		1010 10 pkt		1110 14 pkt	||
			||	1	||	0011 3 ReTX		0111 7 ReTX		1011 11 ReTX		1111 15 ReTX	||	0011 3 pkt		0111 7 pkt		1011 11 pkt		1111 15 pkt	||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
Carrier Detect		||		||	Carrier Detect	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||
0x09			||	0	||	Carrier present	||			||			||			||			||			||			||			||
			||	1	||	No Carrier	||			||			||			||			||			||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 0		||		||	0b 1110 0111 1110 0111 1110 0111 1110 0111 1110 0111	(LSB written first)														||
0x0A			||		||																								||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 1		||		||	0b 1100 0010 1100 0010 1100 0010 1100 0010 1100 0010	(LSB written first)														||
0x0B			||		||																								||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 2		||		||	ADD PIPE 2 b0	||	ADD PIPE 2 b1	||	ADD PIPE 2 b2	||	ADD PIPE 2 b3	||	ADD PIPE 2 b4	||	ADD PIPE 2 b5	||	ADD PIPE 2 b6	||	ADD PIPE 2 b7	||
0x0C			||		||	Only LSB, MSB is the same as MSB of PIPE 1																		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 3		||		||	ADD PIPE 3 b0	||	ADD PIPE 3 b1	||	ADD PIPE 3 b2	||	ADD PIPE 3 b3	||	ADD PIPE 3 b4	||	ADD PIPE 3 b5	||	ADD PIPE 3 b6	||	ADD PIPE 3 b7	||
0x0D			||		||	Only LSB, MSB is the same as MSB of PIPE 1																		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 4		||		||	ADD PIPE 4 b0	||	ADD PIPE 4 b1	||	ADD PIPE 4 b2	||	ADD PIPE 4 b3	||	ADD PIPE 4 b4	||	ADD PIPE 4 b5	||	ADD PIPE 4 b6	||	ADD PIPE 4 b7	||
0x0E			||		||	Only LSB, MSB is the same as MSB of PIPE 1																		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
RX ADD PIPE 5		||		||	ADD PIPE 5 b0	||	ADD PIPE 5 b1	||	ADD PIPE 5 b2	||	ADD PIPE 5 b3	||	ADD PIPE 5 b4	||	ADD PIPE 5 b5	||	ADD PIPE 5 b6	||	ADD PIPE 5 b7	||
0x0F			||		||	Only LSB, MSB is the same as MSB of PIPE 1																		||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
TX ADD			||		||	0b 1110 0111 1110 0111 1110 0111 1110 0111 1110 0111	(LSB written first)														||
0x10			||		||																								||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 0 PKT SIZE		||		||	PIPE 0 RX Packet Size															||	RESERVED	||	RESERVED	||
0x11			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 1 PKT SIZE		||		||	PIPE 1 RX Packet Size															||	RESERVED	||	RESERVED	||
0x12			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 2 PKT SIZE		||		||	PIPE 2 RX Packet Size															||	RESERVED	||	RESERVED	||
0x13			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 3 PKT SIZE		||		||	PIPE 3 RX Packet Size															||	RESERVED	||	RESERVED	||
0x14			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 4 PKT SIZE		||		||	PIPE 4 RX Packet Size															||	RESERVED	||	RESERVED	||
0x15			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
PIPE 5 PKT SIZE		||		||	PIPE 5 RX Packet Size															||	RESERVED	||	RESERVED	||
0x16			||	0	||	00000 Disable		00010 2 Bytes		00100 4 Bytes		00110 6 Bytes		01000 8 Bytes	...	11110 31 Bytes	||			||			||
			||	1	||	00001 1 Byte		00011 3 Bytes		00101 5 Bytes		00111 7 Bytes		01001 9 Bytes	...	11111 32 Bytes	||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
FIFO STATUS		||		||	RX FIFO Empty	||	RX FIFO Full	||	RESERVED	||	RESERVED	||	TX FIFO Empty	||	TX FIFO Full	||	TX ReUse	||	RESERVED	||
0x17			||	0	||	Not Empty	||	Not Full	||			||			||	Not Empty	||	Not Full	||	(Read only)	||			||
			||	1	||	Empty		||	Full		||			||			||	EMpty		||	Full		||	Resend		||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
Dynamic Pkt Size	||		||	EN Dyn. PIPE 0	||	EN Dyn. PIPE 1	||	EN Dyn. PIPE 2	||	EN Dyn. PIPE 3	||	EN Dyn. PIPE 4	||	EN Dyn. PIPE 5	||	RESERVED	||	RESERVED	||
0x1C			||	0	||	Disable		||	Disable		||	Disable		||	Disable		||	Disable		||	Disable		||			||			||
			||	1	||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||	Enable		||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||
Feature			||		||	EN TX + ACK CMD	||	EN TX + ACK	||	EN Dyn Pkt Len	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||	RESERVED	||
0x1D			||	0	||	Disable		||	Disable		||	Disable		||			||			||			||			||			||
			||	1	||	Enable		||	Enable		||	Enable		||			||			||			||			||			||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------||*/
//	------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------	//

#ifndef _PIC32_NRF24L01_H
#define _PIC32_NRF24L01_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Librairies
#include <peripheral/pic32_spi.h>
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// -- Application constant -- //
#define		NRF_PACKET_SIZE_MAX		32			//nRF Enhanced ShockBurst maximum packet size (1-32 byte)
// -------------------------- //

// -- Hardware pin -- //
#define		NRF_PIN_IRQ			PORTBbits.RB0
#define		NRF_PIN_CE			PORTAbits.RA5
#define		NRF_PIN_SS_PORT			PORTB			//PORT on wich the SS is
#define		NRF_PIN_SS_PIN			3			//Pin number on the PORT (0-7)
// ------------------ //

// -- nRF Registers -- //
#define		NRF_REG_CONFIG			0x00			//General config
#define		NRF_REG_EN_AA			0x01			//Auto Ack control of each pipe
#define		NRF_REG_EN_PIPE_RX		0x02			//Pipe RX control
#define		NRF_REG_ADD_SIZE		0x03			//Packet address size control
#define		NRF_REG_RETX_DLY		0x04			//Auto Retransmission delay
#define		NRF_REG_RF_CH			0x05			//RF Channel operating on
#define		NRF_REG_RF_CONFIG		0x06			//RF config
#define		NRF_REG_STATUS			0x07			//General Status
#define		NRF_REG_TX_STATUS		0x08			//Transmission Status
#define		NRF_REG_CARRIER_DETECT		0x09			//Carrier Detection
#define		NRF_REG_PIPE0_RX_ADD		0x0A			//RX Address of the PIPE 0
#define		NRF_REG_PIPE1_RX_ADD		0x0B			//RX Address of the PIPE 1
#define		NRF_REG_PIPE2_RX_ADD		0x0C			//RX Address of the PIPE 2
#define		NRF_REG_PIPE3_RX_ADD		0x0D			//RX Address of the PIPE 3
#define		NRF_REG_PIPE4_RX_ADD		0x0E			//RX Address of the PIPE 4
#define		NRF_REG_PIPE5_RX_ADD		0x0F			//RX Address of the PIPE 5
#define		NRF_REG_TX_ADD			0x10			//TX Address
#define		NRF_REG_PIPE0_PKT_SIZE		0x11			//PIPE 0 Packet size
#define		NRF_REG_PIPE1_PKT_SIZE		0x12			//PIPE 1 Packet size
#define		NRF_REG_PIPE2_PKT_SIZE		0x13			//PIPE 2 Packet size
#define		NRF_REG_PIPE3_PKT_SIZE		0x14			//PIPE 3 Packet size
#define		NRF_REG_PIPE4_PKT_SIZE		0x15			//PIPE 4 Packet size
#define		NRF_REG_PIPE5_PKT_SIZE		0x16			//PIPE 5 Packet size
#define		NRF_REG_FIFO_STATUS		0x17			//FIFO Buffers Status
#define		NRF_REG_DYN_PKT_SIZE		0x1C			//Dynamic Packet Size control
#define		NRF_REG_FEATURE			0x1D			//Misc. Feature control
// ------------------- //

// -- nRF Constant -- //
#define		NRF_DLY_STBtoACTV		130			// Standby to RX or TX delay (in µs)
#define		NRF_DLY_PWDtoSTB		4500			// Power Down to Standby delay (worst case with a Crystal) (150 if using external clock) (in µs)
#define		NRF_DLY_CE_PULSE		10			// Minimum high time for a CE pulse to be valid (in µs)
#define		NRF_DLY_CEtoSS			4			// Minimum time between a rising on CE and a falling on SS (in µs)
// ------------------ //

// -- Variable Type -- //
// Status Reg //
typedef union
{
	U8 all;
	struct
	{
		U8 :1;
		U8 pipe:3;						// Pipe ID
		U8 intReTx:1;						// Max retransmission interrupt flag
		U8 intTx:1;						// TX sent and ACK Received interrupt flag
		U8 intRx:1;						// RX packet received interrupt flag
		U8 :1;
	};
}tNRFStatusReg;

// Tx Status Reg //
typedef union
{
	U8 all;
	struct
	{
		U8 pktLost:4;						// Packet lost count
		U8 reTxCount:4;						// Retransmission count
	};
}tNRFTxStatusReg;

// Config Reg //
typedef union
{
	U8 all;
	struct
	{
		U8 comDir:1;						// Communication Direction (PTX or PRX)
		U8 power:1;						// Power status (0: power down | 1: power up)
		U8 crcEncoding:1;					// CRC Encoding (0: 1 byte | 1: 2 byte)
		U8 crcEn:1;						// Enable CRC computation/transmission
		U8 intReTxEn:1;						// Max retransmission interrupt Enable
		U8 intTxEn:1;						// TX sent and ACK Received interrupt Enable
		U8 intRxEn:1;						// RX packet received interrupt Enable
		U8 :1;
	};
}tNRFConfigReg;

// Address Size Reg //
typedef union
{
	U8 all;
	struct
	{
		U8 size:2;						// Address Size of the RF packet (0: invalid | 1: 3 Byte | 2: 4 Byte | 3: 5 Byte )
		U8 :6;
	};
}tNRFAddressSizeReg;

// Retransmission Config Reg //
typedef union
{
	U8 all;
	struct
	{
		U8 delay:4;						// Delay between retransmission (in 250µs increment)
		U8 maxNb:4;						// Maximum number of time to retransmit
	};
}tNRFReTxConfigReg;

// SPI Command //
typedef enum
{
	NRF_CMD_READ_REG = 0x00,					//Read the Register (000A AAAA) A being the reg address
	NRF_CMD_WRITE_REG = 0x20,					//Write the register (001A AAAA) A being the reg address
	NRF_CMD_READ_RX = 0x61,						//Read the RX FIFO (1-32 B * 3 lvl) LSB first, data deleted after reading
	NRF_CMD_WRITE_TX = 0xA0,					//Write to the TX FIFO (1-32 B * 3 lvl) LSB first
	NRF_CMD_FLUSH_RX = 0xE2,					//Flush the RX FIFO
	NRF_CMD_FLUSH_TX = 0xE1,					//Flush the TX FIFO
	NRF_CMD_RESEND_TX = 0xE3,					//Resend continuously the last packet (must be used in TX mode) (can be clear with FLUSH_TX or WRITE_TX commands)
	NRF_CMD_READ_RX_SIZE = 0x60,					//Read the packet size of the top packet in the RX FIFO (>32 is possible but invalid)
	NRF_CMD_WRITE_TX_WITH_ACK = 0xA8,				//Write data to be sent with the ACK of the last received packet (used only in RX mode)
	NRF_CMD_WRITE_TX_NO_ACK	= 0xB0,					//Write to the TX FIFO but disabling auto-ACK for this only packet
	NRF_CMD_NOP = 0xFF
}tNRFCommand;

// nRF Pipe Buffer
typedef union
{
	U32 all;
	struct
	{
		U8 pipe;
	};
}tNRFPipeBuffer;
// ------------------- //
// ############################################## //


// ################## Variables ################# //
//Initialisation register value for the nRF24L01+ radio chip
const U8 nrfInitReg[8] = {	0b01111011,				// CONFIG :		All interrupt enable, CRC enable, CRC 1 byte, Power up, Primary Rx
				0b00111111,				// AUTO ACK :		All pipe auto ack enable
				0b00111111,				// PIPE RX :		All pipe rx enable
				0b00000001,				// ADDRESS SIZE :	3 byte address width
				0b00000011,				// RETRANSMISSION :	250µS ReTX delay, max 3 ReTX
				0b00000100,				// CHANNEL :		Channel 4
				0b00100110,				// RF CONFIG :		250kbps, 0dBm Tx power
				0b01110000				// STATUS :		Clear all interrupt
			};
// ############################################## //

// ############## Control Function ############## //
/**
* \fn		U8 nrfInit(void)
* @brief	Initialize the various variable for the nRF
* @note
* @arg		U8 spiPort				SPI port the nRF is connected to
* @arg		U32 * SSpinPortPtr			Pointer to the SSpin port register
* @arg		U32 SSpinPortMask			Mask to apply to the SSpin port register
* @return	U8 errorCode				STD Error Code
*/
U8 nrfInit(U8 spiPort, volatile U32 * SSpinPortPtr, U16 SSpinMask);

/**
* \fn
* @brief	Send a command to the local NRF with parameters and save the response in the $nrfResponseBuf
* @note		You can specify 0 to paramNb to send only the command byte and save only the Status REG in response
* @arg		U8 command				Command to send (see nRF Commands section in header)
* @arg		U8 * parametersPtr			Pointer to load the command's parameters to send to the local nRF
* @arg		U8 paramNb				Number of byte of parameters
* @return	U8 errorCode				STD Error Code
*/

// ############################################## //

#endif
