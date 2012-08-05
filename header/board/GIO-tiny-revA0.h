/*!
 @file		GIO-tiny-revA0.h
 @brief		Definiton file for the node Generic IO -tiny rev.A0
 
 @version	0.1
 @note		This file define each generic pin available on the GIO-tiny rev.A0 and
 			include all specific definition for different EXT or COM board

 @date		February 13th 2011
 @author	Laurence DV
*/

#ifndef _GIO_TINY_REVA0_H
#define _GIO_TINY_REVA0_H

// ################## Includes ################## //
// Dev Macro
#include <datatype_megaxone.h>
#include <stddef_megaxone.h>
#include <bitmanip_megaxone.h>

// COM & EXT board definition
#include <board/COM-RS485-revA0.h>
#include <board/COM-NRF-revA0.h>

// ############################################## //


// ################## Defines ################### //
// --- Basic peripherals --- //
#define LED_B				BIT13
#define LED_B_PORT			B
#define LED_G				BIT4
#define LED_G_PORT			B
#define LED_R				BIT3
#define LED_R_PORT			A
#define BTN					BIT7
#define BTN_PORT			B
// ------------------------- //

// --- RAM --- //
#define RAM_SPI_SS			BIT6
#define RAM_SPI_SS_PORT		B
#define RAM_HOLD			BIT0
#define RAM_HOLD_PORT		A
// ----------- //

// -- OneWire -- //
#define OW_BUS				BIT7
#define OW_BUS_PORT			B
// ------------- //

// --- EXT socket --- //
#define EXT0_D0				BIT5
#define EXT0_D0_PORT		B
#define EXT0_D1				BIT8
#define EXT0_D1_PORT		B
#define EXT0_D2				BIT9
#define EXT0_D2_PORT		B
#define EXT0_D3				BIT14
#define EXT0_D3_PORT		B
// ------------------ //

// --- COM socket --- //
#define COM0_UART_RX		BIT2
#define COM0_UART_RX_PORT	B
#define COM0_UART_TX		BIT3
#define COM0_UART_TX_PORT	B
#define COM0_UART_IO0		BIT0
#define COM0_UART_IO0_PORT	B
#define COM0_UART_IO1		BIT1
#define COM0_UART_IO1_PORT	B
#define COM0_SPI_MISO		BIT2
#define COM0_SPI_MISO_PORT	A
#define COM0_SPI_MOSI		BIT1
#define COM0_SPI_MOSI_PORT	A
#define COM0_SPI_SCK		BIT15
#define COM0_SPI_SCK_PORT	B
#define COM0_SPI_IO0		BIT4
#define COM0_SPI_IO0_PORT	A
#define COM0_SPI_IO1		BIT10
#define COM0_SPI_IO1_PORT	B
#define COM0_SPI_IO2		BIT11
#define COM0_SPI_IO2_PORT	B
#define COM0_ID				BIT12
#define COM0_ID_PORT		B
// ------------------ //

// --- Interrupt mapping --- //
#define COM0_SPI_VECTOR		_SPI_2_VECTOR
#define COM0_SPI_IRQ_VECTOR	_EXTERNAL_2_VECTOR
#define COM0_UART_VECTOR	_UART_1_VECTOR

#define EXT0_IRQ_VECTOR		_EXTERNAL_3_VECTOR
#define EXT0_SPI_VECTOR		_SPI_1_VECTOR
#define EXT0_UART_VECTOR	_UART_2_VECTOR
#define EXT0_I2C_VECTOR		_I2C_1_VECTOR

#define BTN_VECTOR			_EXTERNAL_0_VECTOR
// ------------------------- //

// --- Peripheral mapping --- //
#define COM0_UART_PORTID	UART1
#define COM0_SPI_PORTID		SPI2
#define EXT0_UART_PORTID	UART2
#define EXT0_SPI_PORTID		SPI1
#define EXT0_I2C_PORTID		I2C1
// -------------------------- //

// ############################################## //

#endif
