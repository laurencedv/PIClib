/*!
 @file		opiluminator-a0.h
 @brief		Hardware definition file for the OpiLuminator A0 module
 
 @version	0.1
 @note		

 @date		November 12th 2011
 @author	Laurence DV
*/

#ifndef _OPILUMINATOR_A0_H
#define _OPILUMINATOR_A0_H 1

// ################## Includes ################## //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// --- PWM mapping ---------- //
#define PWM_0_CCP_ID	CCP_7
#define PWM_0		BIT7
#define PWM_0_PORT	B
#define PWM_1_CCP_ID	CCP_6
#define PWM_1		BIT6
#define PWM_1_PORT	B
#define PWM_2_CCP_ID	CCP_5
#define PWM_2		BIT5
#define PWM_2_PORT	B
#define PWM_3_CCP_ID	CCP_1
#define PWM_3		BIT2
#define PWM_3_PORT	B
#define PWM_4_CCP_ID	CCP_2
#define PWM_4		BIT3
#define PWM_4_PORT	B
#define PWM_5_CCP_ID	CCP_4
#define PWM_5		BIT4
#define PWM_5_PORT	B
#define PWM_6_CCP_ID	CCP_9
#define PWM_6		BIT6
#define PWM_6_PORT	C
#define PWM_7_CCP_ID	CCP_10
#define PWM_7		BIT7
#define PWM_7_PORT	C
#define PWM_8_CCP_ID	CCP_3
#define PWM_8		BIT0
#define PWM_8_PORT	B
#define PWM_9_CCP_ID	CCP_8
#define PWM_9		BIT1
#define PWM_9_PORT	C
// -------------------------- //

// --- Controlling --------- //
#define COM_UART_ID	UART_2
#define COM_RX		BIT0
#define COM_RX_PORT	C
#define COM_TX		BIT2
#define COM_TX_PORT	C
#define COM_IRQ_INT_ID	INT_1
#define COM_IRQ		BIT1
#define COM_IRQ_PORT	B
#define POT_0		BIT0
#define POT_0_PORT	A
#define POT_0_AN	an0
#define POT_1		BIT1
#define POT_1_PORT	A
#define POT_1_AN	an1
#define POT_2		BIT2
#define POT_2_PORT	A
#define POT_2_AN	an2
// ------------------------- //
// ############################################## //


#endif
