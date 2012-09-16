/*!
 @file		uOpioid-revB0.h
 @brief		Hardware definition file for the Opioid rev.A0 node
 
 @version	0.1
 @note		This file define each generic pin available on the Opioid rev.A0 and
 			include all specific definition for different EXT or COM wings

 @date		May 14th 2011
 @author	Laurence DV
*/

#ifndef _OPIOID_REVA0_H
#define _OPIOID_REVA0_H 1

// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Lib
//#include <peripheral/pic32_spi.h>
//#include <peripheral/pic32_uart.h>
//#include <peripheral/pic32_timer.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>

// COM & EXT board definition
#include <board/com-rs485-a0.h>
#include <board/com-nrf-a0.h>

// ############################################## //


// ################## Defines ################### //
// --- Basic peripherals --- //
#define LED_B			BIT3		//OC4 - D3
#define LED_B_PORT		D
#define LED_G			BIT2		//OC3 - D2
#define LED_G_PORT		D
#define LED_R			BIT1		//OC2 - D1
#define LED_R_PORT		D
#define BTN			BIT14		//INT3 - A14
#define BTN_PORT		A
#define VIN_SENSE		BIT3		//AN3 - B3
#define VIN_SENSE_PORT		B
#define VIN_SENSE_AN		an3
#define VIN_SENSE_AN_MUX	muxAn3
// ------------------------- //

// -- OneWire -- //
#define OW_BUS			BIT15		//A15
#define OW_BUS_PORT		A
// ------------- //

// --- EXT wing --- //
#define EXT0_RX			BIT7		//U3RX - G7
#define EXT0_RX_PORT		G
#define EXT0_TX			BIT8		//U3TX - G8
#define EXT0_TX_PORT		G
#define EXT0_IRQ		BIT8		//INT1 - E8
#define EXT0_IRQ_PORT		E

#define EXT1_RX			BIT9		//U6RX - G9
#define EXT1_RX_PORT		G
#define EXT1_TX			BIT6		//U6TX - G6
#define EXT1_TX_PORT		G
#define EXT1_IRQ		BIT9		//INT2 - E9
#define EXT1_IRQ_PORT		E
// ------------------ //

// --- COM wing --- //
#define COM0_RX			BIT2		//U1RX - F2
#define COM0_RX_PORT		F
#define COM0_TX			BIT8		//U1TX - F8
#define COM0_TX_PORT		F
#define COM0_IO0		BIT7		//CN16 - R7
#define COM0_IO0_PORT		D
#define COM0_IO1		BIT6		//CN15 - D6
#define COM0_IO1_PORT		D
#define COM0_IO2		BIT5		//CN14 - D5
#define COM0_IO2_PORT		D
#define COM0_MISO		BIT4		//SDI1 - C4
#define COM0_MISO_PORT		C
#define COM0_MOSI		BIT0		//SDO1 - D0
#define COM0_MOSI_PORT		D
#define COM0_SCK		BIT10		//SCK1 - D10
#define COM0_SCK_PORT		D
#define COM0_ID			BIT9		//AN9 - B9
#define COM0_ID_PORT		B
#define COM0_ID_AN		an9
#define COM0_ID_AN_MUX		muxAn9

#define COM1_RX			BIT14		//U4RX - D14
#define COM1_RX_PORT		D
#define COM1_TX			BIT15		//U4TX - D15
#define COM1_TX_PORT		D
#define COM1_IO0		BIT0		//CN2 - B0
#define COM1_IO0_PORT		B
#define COM1_IO1		BIT1		//CN3 - B1
#define COM1_IO1_PORT		B
#define COM1_IO2		BIT2		//CN4 - B2
#define COM1_IO2_PORT		B
#define COM1_MISO		BIT4		//SDI4 - F4
#define COM1_MISO_PORT		F
#define COM1_MOSI		BIT5		//SDO4 - F5
#define COM1_MOSI_PORT		F
#define COM1_SCK		BIT13		//SCK4 - F13
#define COM1_SCK_PORT		F
#define COM1_ID			BIT8		//AN8 - B8
#define COM1_ID_PORT		B
#define COM1_ID_AN		an8
#define COM1_ID_AN_MUX		muxAn8
// ------------------ //

// --- Interrupt mapping --- //
#define COM0_SPI_VECTOR		INT_VEC_SPI_1
#define COM0_SPI_INT_ID		INT_SPI_1
#define COM0_SPI_INT_IRQ	INT_IRQ_SPI_1
#define COM0_IRQ0_VECTOR	INT_VEC_INPUT_CHANGE
#define COM0_IRQ0_INT_ID	INT_INPUT_CHANGE
#define COM0_IRQ0_INT_IRQ	INT_IRQ_INPUT_CHANGE
#define COM0_IRQ1_VECTOR	INT_VEC_INPUT_CHANGE
#define COM0_IRQ1_INT_ID	INT_INPUT_CHANGE
#define COM0_IRQ1_INT_IRQ	INT_IRQ_INPUT_CHANGE
#define COM0_UART_VECTOR	INT_VEC_UART_1
#define COM0_UART_INT_ID	INT_UART_1
#define COM0_UART_INT_IRQ	INT_IRQ_UART_1

#define COM1_SPI_VECTOR		INT_VEC_SPI_4
#define COM1_SPI_INT_ID		INT_SPI_4
#define COM1_SPI_INT_IRQ	INT_IRQ_SPI_4
#define COM1_IRQ0_VECTOR	INT_VEC_INPUT_CHANGE
#define COM1_IRQ0_INT_ID	INT_INPUT_CHANGE
#define COM1_IRQ0_INT_IRQ	INT_IRQ_INPUT_CHANGE
#define COM1_IRQ1_VECTOR	INT_VEC_INPUT_CHANGE
#define COM1_IRQ1_INT_ID	INT_INPUT_CHANGE
#define COM1_IRQ1_INT_IRQ	INT_IRQ_INPUT_CHANGE
#define COM1_UART_VECTOR	INT_VEC_UART_4
#define COM1_UART_INT_ID	INT_UART_4
#define COM1_UART_INT_IRQ	INT_IRQ_UART_4

#define EXT0_IRQ_VECTOR		INT_VEC_EXT_INT_1
#define EXT0_IRQ_INT_ID		INT_EXT_INT_1
#define EXT0_IRQ_INT_IRQ	INT_IRQ_EXT_INT_1
#define EXT0_UART_VECTOR	INT_VEC_UART_3
#define EXT0_UART_INT_ID	INT_UART_3
#define EXT0_UART_INT_IRQ	INT_IRQ_UART_3

#define EXT1_IRQ_VECTOR		INT_VEC_EXT_INT_2
#define EXT1_IRQ_INT_ID		INT_EXT_INT_2
#define EXT1_IRQ_INT_IRQ	INT_IRQ_EXT_INT_2
#define EXT1_UART_VECTOR	INT_VEC_UART_6
#define EXT1_UART_INT_ID	INT_UART_6
#define EXT1_UART_INT_IRQ	INT_IRQ_UART_6

#define BTN_VECTOR		INT_VEC_EXT_INT_3
#define BTN_INT_ID		INT_EXT_INT_3
#define BTN_INT_IRQ		INT_IRQ_EXT_INT_3
// ------------------------- //

// --- Peripheral mapping --- //
#define COM0_UART_ID		UART_1
#define COM0_SPI_ID		SPI_1
#define COM0_TIMER_ID		TIMER_4
#define COM1_UART_ID		UART_4
#define COM1_SPI_ID		SPI_4
#define COM1_TIMER_ID		TIMER_5

#define EXT0_UART_ID		UART_3
#define EXT1_UART_ID		UART_6

#define LED_R_OC_ID		OC_2
#define LED_G_OC_ID		OC_3
#define LED_B_OC_ID		OC_4
// -------------------------- //

// --- OpIUM Layer Mapping --- //
#define COM_WING_NB		2
#define EXT_WING_NB		2
// --------------------------- //

// ############################################## //

#endif
