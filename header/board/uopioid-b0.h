/*!
 @file		uOpioid-revB0.h
 @brief		Hardware definition file for the µOpioid rev.B0 node
 
 @version	0.1
 @note		This file define each generic pin available on the µOpioid rev.B0 and
 			include all specific definition for different EXT or COM wings

 @date		February 28th 2011
 @author	Laurence DV
*/

#ifndef _UOPIOID_REVB0_H
#define _UOPIOID_REVB0_H 1

// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>

// COM & EXT board definition
#include <board/com-rs485-a0.h>
#include <board/com-nrf-a0.h>

// ############################################## //


// ################## Defines ################### //
// --- Basic peripherals --- //
#define LED_B				BIT0		//OC3 - RPB0
#define LED_B_PORT			B
#define LED_G				BIT1		//OC2 - RPA1
#define LED_G_PORT			A
#define LED_R				BIT0		//OC1 - RPA0
#define LED_R_PORT			A
#define BTN					BIT7		//B7
#define BTN_PORT			B
// ------------------------- //

// --- RAM --- //
#define RAM_MOSI			BIT6		//SDO1 - RPB6
#define RAM_MOSI_PORT		B
#define RAM_MISO			BIT8		//SDI1 - RPB8
#define RAM_MISO_PORT		B
#define RAM_SCK				BIT14		//SCK1 - B14
#define RAM_SCK_PORT		B
#define RAM_SS				BIT5		//B5
#define RAM_SS_PORT			B
#define RAM_HOLD			BIT13		//B13
#define RAM_HOLD_PORT		B
// ----------- //

// -- OneWire -- //
#define OW_BUS				BIT7		//B7
#define OW_BUS_PORT			B
// ------------- //

// --- EXT wing --- //
#define EXT0_RX				BIT11		//U2RX - RPB11
#define EXT0_RX_PORT		B
#define EXT0_TX				BIT10		//U2TX - RPB10
#define EXT0_TX_PORT		B
#define EXT0_IRQ			BIT9		//INT1 - RPB9
#define EXT0_IRQ_PORT		B
// ------------------ //

// --- COM wing --- //
#define COM0_RX				BIT4		//U1RX - RPA4
#define COM0_RX_PORT		A
#define COM0_TX				BIT4		//U1TX - RPB4
#define COM0_TX_PORT		B
#define COM0_IO0			BIT3		//INT4 - RPB3
#define COM0_IO0_PORT		B
#define COM0_IO1			BIT2		//INT2 - RPA2
#define COM0_IO1_PORT		A
#define COM0_IO2			BIT3		//RA3
#define COM0_IO2_PORT		A
#define COM0_MISO			BIT2		//SDI2 - RPB2
#define COM0_MISO_PORT		B
#define COM0_MOSI			BIT1		//SDO2 - RPB1
#define COM0_MOSI_PORT		B
#define COM0_SCK			BIT15		//SCK2 - B15
#define COM0_SCK_PORT		B
#define COM0_ID				BIT12		//AN12 - B12
#define COM0_ID_PORT		B
// ------------------ //

// --- Interrupt mapping --- //
#define COM0_SPI_VECTOR		INT_VEC_SPI_2
#define COM0_SPI_INT_ID		INT_SPI_2
#define COM0_SPI_INT_IRQ	INT_IRQ_SPI_2
#define COM0_IRQ0_VECTOR	INT_VEC_EXT_INT_4
#define COM0_IRQ0_INT_ID	INT_EXT_INT_4
#define COM0_IRQ0_INT_IRQ	INT_IRQ_EXT_INT_4
#define COM0_IRQ1_VECTOR	INT_VEC_EXT_INT_2
#define COM0_IRQ1_INT_ID	INT_EXT_INT_2
#define COM0_IRQ1_INT_IRQ	INT_IRQ_EXT_INT_2
#define COM0_UART_VECTOR	INT_VEC_UART_1
#define COM0_UART_INT_ID	INT_UART_1
#define COM0_UART_INT_IRQ	INT_IRQ_UART_1

#define EXT0_IRQ_VECTOR		INT_VEC_EXT_INT_3
#define EXT0_IRQ_INT_ID		INT_EXT_INT_3
#define EXT0_IRQ_INT_IRQ	INT_IRQ_EXT_INT_3
#define EXT0_UART_VECTOR	INT_VEC_UART_2
#define EXT0_UART_INT_ID	INT_UART_2
#define EXT0_UART_INT_IRQ	INT_IRQ_UART_2

#define RAM_SPI_VECTOR		INT_VEC_SPI_1
#define RAM_SPI_INT_ID		INT_SPI_1
#define	RAM_SPI_INT_IRQ		INT_IRQ_SPI_1

#define BTN_VECTOR			INT_VEC_EXT_INT_0
#define BTN_INT_ID			INT_EXT_INT_0
#define BTN_INT_IRQ			INT_IRQ_EXT_INT_0
// ------------------------- //

// --- Peripheral mapping --- //
#define COM0_UART_ID		UART_1
#define COM0_SPI_ID			SPI_2
#define EXT0_UART_ID		UART_2
#define RAM_SPI_ID			SPI_1
#define LED_R_OC_ID			OC_1
#define LED_G_OC_ID			OC_2
#define LED_B_OC_ID			OC_3
// -------------------------- //

// --- OpIUM Layer Mapping --- //
#define COM_WING_NB			1
#define EXT_WING_NB			1
// --------------------------- //

// ############################################## //

#endif
