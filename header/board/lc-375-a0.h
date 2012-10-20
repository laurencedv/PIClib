/*!
 @file		uOpioid-revB0.h
 @brief		Hardware definition file for the Opioid A0 node
 
 @version	0.1
 @note		This file define each generic pin available on the Opioid revision A0 and
 		include all specific definition for different EXT or COM wings

 @date		May 14th 2011
 @author	Laurence DV
*/

#ifndef _OPIOID_A0_H
#define _OPIOID_A0_H 1

// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// --- Peripheral mapping --- //
#define AQCA_CTL_UART_ID	UART_2

#define CWHT_OC_ID		OC_1
#define WWHT_OC_ID		OC_2
#define BLUE_OC_ID		OC_3
// -------------------------- //

// --- Basic peripherals --- //
#define LED_B			BIT4		//A4
#define LED_B_PORT		A
#define LED_G			BIT3		//A3
#define LED_G_PORT		A
#define LED_R			BIT4		//B4
#define LED_R_PORT		B
#define BTN_ENTER		BIT2		//INT2 - A2
#define BTN_ENTER_PORT		A
#define BTN_UP			BIT5		//B5
#define BTN_UP_PORT		B
#define BTN_DOWN		BIT6		//B6
#define BTN_DOWN_PORT		B
// ------------------------- //

// --- Controlling --------- //
#define CWHT_OC			BIT7		//OC1 - B7
#define CWHT_OC_PORT		B
#define WWHT_OC			BIT8		//OC2 - B8
#define WWHT_OC_PORT		B
#define BLUE_OC			BIT9		//OC3 - B9
#define BLUE_OC_PORT		B
#define AQCA_CTL_TX		BIT10		//UART2 TX - B10
#define AQCA_CTL_TX_PORT	B
#define AQCA_CTL_RX		BIT11		//UART2 RX - B11
#define AQCA_CTL_RX_PORT	B
// ------------------------- //

// --- Monitoring ---------- //
#define TEMP_MOD_1		BIT2		//AN4 - B2
#define TEMP_MOD_1_PORT		B
#define TEMP_MOD_1_AN		an4
#define TEMP_MOD_1_AN_MUX	muxAn4
#define TEMP_MOD_2		BIT3		//AN5 - B3
#define TEMP_MOD_2_PORT		B
#define TEMP_MOD_2_AN		an5
#define TEMP_MOD_2_AN_MUX	muxAn5
#define TEMP_MOD_3		BIT15		//AN9 - B15
#define TEMP_MOD_3_PORT		B
#define TEMP_MOD_3_AN		an9
#define TEMP_MOD_3_AN_MUX	muxAn9
#define TEMP_MOD_4		BIT14		//AN10 - B14
#define TEMP_MOD_4_PORT		B
#define TEMP_MOD_4_AN		an10
#define TEMP_MOD_4_AN_MUX	muxAn10
#define TEMP_MOD_5		BIT13		//AN11 - B13
#define TEMP_MOD_5_PORT		B
#define TEMP_MOD_5_AN		an11
#define TEMP_MOD_5_AN_MUX	muxAn11
#define TEMP_MOD_6		BIT12		//AN12 - B12
#define TEMP_MOD_6_PORT		B
#define TEMP_MOD_6_AN		an12
#define TEMP_MOD_6_AN_MUX	muxAn12
#define CUR_CWHT_1		BIT0		//AN0 - A0
#define CUR_CWHT_1_PORT		A
#define CUR_CWHT_1_AN		an0
#define CUR_CWHT_1_AN_MUX	muxAn0
#define CUR_CWHT_2		BIT1		//AN1 - A1
#define CUR_CWHT_2_PORT		A
#define CUR_CWHT_2_AN		an1
#define CUR_CWHT_2_AN_MUX	muxAn1
#define CUR_WWHT		BIT0		//AN2 - B0
#define CUR_WWHT_PORT		B
#define CUR_WWHT_AN		an2
#define CUR_WWHT_AN_MUX		muxAn2
#define CUR_BLUE		BIT1		//AN3 - B1
#define CUR_BLUE_PORT		B
#define CUR_BLUE_AN		an3
#define CUR_BLUE_AN_MUX		muxAn3
// ------------------------- //

// --- Interrupt mapping --- //
#define AQCA_CTL_UART_VECTOR	INT_VEC_UART_2
#define AQCA_CTL_UART_INT_ID	INT_UART_2
#define AQCA_CTL_UART_INT_IRQ	IRQ_UART_2

#define BTN_ENTER_VECTOR	INT_VEC_EXT_INT_2
#define BTN_ENTER_INT_ID	INT_EXT_INT_2
#define BTN_ENTER_INT_IRQ	IRQ_EXT_INT_2
// ------------------------- //

// ############################################## //


#endif
