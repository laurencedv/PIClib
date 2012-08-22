/*!
 @file		upicoid-a0.h
 @brief		Hardware definition file for the µPICoid rev.A0 Dev Board
 
 @version	0.1
 @note		This file define each on-board peripherals

 @date		August 21th 2011
 @author	Laurence DV
*/

#ifndef _UPICOID_REVA0_H
#define _UPICOID_REVA0_H 1

// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// --- Basic peripherals --- //
#define LED_B				BIT9		//RB9 - OC3
#define LED_B_PORT			B
#define LED_G				BIT8		//RB8 - OC2
#define LED_G_PORT			B
#define LED_R				BIT7		//RB7 - OC1
#define LED_R_PORT			B
#define BTN					BIT4		//RB4 - INT4/IC4
#define BTN_PORT			B
// ------------------------- //

// --- SDCARD --- //
#define SDCARD_MISO			BIT4		//RA4 - SDI2
#define SDCARD_MISO_PORT	A
#define SDCARD_MOSI			BIT1		//RB1 - SDO2
#define SDCARD_MOSI_PORT	B
#define SDCARD_SCK			BIT15		//RB15 - SCK2
#define SDCARD_SCK_PORT		B
#define SDCARD_CS			BIT0		//B0
#define SDCARD_CS_PORT		B
// -------------- //

// --- Interrupt mapping --- //
#define SDCARD_SPI_VECTOR	INT_VEC_SPI_2
#define SDCARD_SPI_INT_ID	INT_SPI_2
#define SDCARD_SPI_INT_IRQ	INT_IRQ_SPI_2

#define BTN_VECTOR			INT_VEC_EXT_INT_4
#define BTN_INT_ID			INT_EXT_INT_4
#define BTN_INT_IRQ			INT_IRQ_EXT_INT_4

#define LED_R_VECTOR		INT_VEC_OC_1
#define LED_R_INT_ID		INT_OC_1
#define LED_R_INT_IRQ		INT_IRQ_OC_1
#define LED_G_VECTOR		INT_VEC_OC_2
#define LED_G_INT_ID		INT_OC_2
#define LED_G_INT_IRQ		INT_IRQ_OC_2
#define LED_B_VECTOR		INT_VEC_OC_3
#define LED_B_INT_ID		INT_OC_3
#define LED_B_INT_IRQ		INT_IRQ_OC_3
// ------------------------- //

// --- Peripheral mapping --- //
#define SDCARD_SPI_ID		SPI_2
#define LED_R_OC_ID			OC_1
#define LED_G_OC_ID			OC_2
#define LED_B_OC_ID			OC_3
#define OC_TIMER_ID			TIMER_2
// -------------------------- //

// ############################################## //

#endif
