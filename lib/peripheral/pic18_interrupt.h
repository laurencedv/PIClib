/*!
 @file		pic18_interrupt.h
 @brief		Interrupt functions for C18
 			Use "INT_NAME_FLAG" to check if the interrupt is valid, use "int_name_clr" to clear the flag after processing an interrupt
 			Use "int_name_en/dis" to temporarly disable an interrupt, use "int_name_init" to enable it the first time
  			Create a "interrupt_high_handler" and "interrupt_low_handler" functions in your code for interrupt
 
 @version	0.1.1
 @note
 @todo		Refactor everything in generic assembly macro

 @date		December 18th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_INTERRUPT_H
#define _PIC18_INTERRUPT_H

// ################## Includes ################## //

#include <hardware.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################# Flag ####################### //
// Timer
#define		INT_TMR0_FLAG			(INTCONbits.TMR0IF && INTCONbits.TMR0IE)
#define		INT_TMR1_FLAG			(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
#define		INT_TMR1GATE_FLAG		(PIR5bits.TMR1GIF && PIE5bits.TMR1GIE)
#define		INT_TMR2_FLAG			(PIR1bits.TMR2IF && PIE1bits.TMR2IE)
#define		INT_TMR3_FLAG			(PIR2bits.TMR3IF && PIE2bits.TMR3IE)
#define		INT_TMR3GATE_FLAG		(PIR3bits.TMR3GIF && PIE3bits.TMR3GIE)
#define		INT_TMR4_FLAG			(PIR3bits.TMR4IF && PIE3bits.TMR4IE)
#define		INT_TMR5_FLAG			(PIR5bits.TMR5IF && PIE5bits.TMR5IE)
#define		INT_TMR5GATE_FLAG		(PIR5bits.TMR5GIF && PIE5bits.TMR5GIE)
#define		INT_TMR6_FLAG			(PIR5bits.TMR6IF && PIE5bits.TMR6IE)
#define		INT_TMR8_FLAG			(PIR5bits.TMR8IF && PIE5bits.TMR8IE)

// IO Port
#define		INT_RB_FLAG				(INTCONbits.RBIF && INTCONbits.RBIE)
#define		INT_INT0_FLAG			(INTCONbits.INT0IF && INTCONbits.INT0IE)
#define		INT_INT1_FLAG			(INTCON3bits.INT1IF && INTCON3bits.INT1IE)
#define		INT_INT2_FLAG			(INTCON3bits.INT2IF && INTCON3bits.INT2IE)
#define		INT_INT3_FLAG			(INTCON3bits.INT3IF && INTCON3bits.INT3IE)

// Analogue
#define		INT_ADC_FLAG			(PIR1bits.ADIF && PIE1bits.ADIE)
#define		INT_HLVD_FLAG			(PIR2bits.HLVDIF && PIE2bits.HLVDIE)
#define		INT_CTMU_FLAG			(PIR3bits.CTMUIF && PIE3bits.CTMUIE)

// Comparator
#define		INT_COMP1_FLAG			(PIR2bits.CM1IF && PIE2bits.CM1IE)
#define		INT_COMP2_FLAG			(PIR2bits.CM2IF && PIE2bits.CM2IE)
#define		INT_COMP3_FLAG			(PIR5bits.CM3IF && PIE5bits.CM3IE)

// EUSART
#define		INT_EUSART1RX_FLAG		(PIR1bits.RC1IF && PIE1bits.RC1IE)
#define		INT_EUSART1TX_FLAG		(PIR1bits.TX1IF && PIE1bits.TX1IE)
#define		INT_EUSART2RX_FLAG		(PIR3bits.RC2IF && PIE3bits.RC2IE)
#define		INT_EUSART2TX_FLAG		(PIR3bits.TX2IF && PIE3bits.TX2IE)

// MSSP
#define		INT_MSSP1_FLAG			(PIR1bits.SSP1IF && PIE1bits.SSP1IE)
#define		INT_MSSP1COL_FLAG		(PIR2bits.BCL1IF && PIE2bits.BCL1IE)
#define		INT_MSSP2_FLAG			(PIR3bits.SSP2IF && PIE3bits.SSP2IE)
#define		INT_MSSP2COL_FLAG		(PIR3bits.BCL2IF && PIE3bits.BCL2IE)

// ECCP & CCP
#define		INT_ECCP1_FLAG			(PIR1bits.CCP1IF && PIE1bits.CCP1IE)
#define		INT_ECCP2_FLAG			(PIR2bits.CCP2IF && PIE2bits.CCP2IE)
#define		INT_ECCP3_FLAG			(PIR4bits.CCP3IF && PIE4bits.CCP3IE)
#define		INT_CCP4_FLAG			(PIR4bits.CCP4IF && PIE4bits.CCP4IE)
#define		INT_CCP5_FLAG			(PIR4bits.CCP5IF && PIE4bits.CCP5IE)
#define		INT_CCP6_FLAG			(PIR4bits.CCP6IF && PIE4bits.CCP6IE)
#define		INT_CCP7_FLAG			(PIR4bits.CCP7IF && PIE4bits.CCP7IE)
#define		INT_CCP8_FLAG			(PIR4bits.CCP8IF && PIE4bits.CCP8IE)
#define		INT_CCP9_FLAG			(PIR4bits.CCP9IF && PIE4bits.CCP9IE)
#define		INT_CCP10_FLAG			(PIR4bits.CCP10IF && PIE4bits.CCP10IE)

// Other
#define		INT_PMP_FLAG			(PIR1bits.PMPIF && PIE1bits.PMPIE)
#define		INT_OSC_FLAG			(PIR2bits.OSCFIF && PIE2bits.OSCFIE)
#define		INT_RTCC_FLAG			(PIR3bits.RTCCIF && PIE3bits.RTCCIE)

// USB
#define		INT_USB_FLAG			(PIR2bits.USBIF && PIE2bits.USBIE)
#define		INT_USB_SOF_FLAG		(UIRbits.SOFIF && UIEbits.SOFIE)
#define		INT_USB_STALL_FLAG		(UIRbits.STALLIF && UIEbits.STALLIE)
#define		INT_USB_IDLE_FLAG		(UIRbits.IDLEIF && UIEbits.IDLEIE)
#define		INT_USB_TRANSACTION_FLAG (UIRbits.TRNIF && UIEbits.TRNIE)
#define		INT_USB_ACTIVITY_FLAG	(UIRbits.ACTVIF && UIEbits.ACTVIE)
#define		INT_USB_ERROR_FLAG		(UIRbits.UERRIF && UIEbits.UERRIE)
#define		INT_USB_RESET_FLAG		(UIRbits.URSTIF && UIEbits.URSTIE)

// USB Errors
#define		INT_USB_BITSTUFF_FLAG	(UEIRbits.BTSEF && UEIEbits.BTSEE)
#define		INT_USB_BUSTIMEOUT_FLAG	(UEIRbits.BTOEF && UEIEbits.BTOEE)
#define		INT_USB_DATASIZE_FLAG	(UEIRbits.DFNEF && UEIEbits.DFNEE)
#define		INT_USB_CRC16_FLAG		(UEIRbits.CRC16EF && UEIEbits.CRC16EE)
#define		INT_USB_CRC5_FLAG		(UEIRbits.CRC5EF && UEIEbits.CRC5EE)
#define		INT_USB_PIDCHECK_FLAG	(UEIRbits.PIDEF && UEIEbits.PIDEE)
// ############################################## //


// ################## Macro ##################### //
// ------ General Control ------ //
#define		int_global_en()			INTCONbits.GIE = 1
#define		int_global_dis()		INTCONbits.GIE = 0
#define		int_high_prio_en()		INTCONbits.GIEH = 1
#define		int_high_prio_dis()		INTCONbits.GIEH = 0
#define		int_peripheral_en()		INTCONbits.PEIE = 1
#define		int_peripheral_dis()	INTCONbits.PEIE = 0
#define		int_low_prio_en()		INTCONbits.GIEL = 1
#define		int_low_prio_dis()		INTCONbits.GIEL = 0
#define		int_priority_en()		RCONbits.IPEN = 1
#define		int_priority_dis()		RCONbits.IPEN = 0


// ------ Priority Control ------ //
// Timer
#define		int_tmr0_prio(_int_prio_lvl)		INTCON2bits.TMR0IP = _int_prio_lvl
#define		int_tmr1_prio(_int_prio_lvl)		IPR1bits.TMR1IP = _int_prio_lvl
#define		int_tmr1gate_prio(_int_prio_lvl)	IPR5bits.TMR1GIP = _int_prio_lvl
#define		int_tmr2_prio(_int_prio_lvl)		IPR1bits.TMR2IP = _int_prio_lvl
#define		int_tmr3_prio(_int_prio_lvl)		IPR2bits.TMR3IP = _int_prio_lvl
#define		int_tmr3gate_prio(_int_prio_lvl)	IPR3bits.TMR3GIP = _int_prio_lvl
#define		int_tmr4_prio(_int_prio_lvl)		IPR3bits.TMR4IP = _int_prio_lvl
#define		int_tmr5_prio(_int_prio_lvl)		IPR5bits.TMR5IP = _int_prio_lvl
#define		int_tmr5gate_prio(_int_prio_lvl)	IPR5bits.TMR5GIP = _int_prio_lvl
#define		int_tmr6_prio(_int_prio_lvl)		IPR5bits.TMR6IP = _int_prio_lvl
#define		int_tmr8_prio(_int_prio_lvl)		IPR5bits.TMR8IP = _int_prio_lvl

// IO Port
#define		int_rb_prio(_int_prio_lvl)			INTCON2bits.RBIP = _int_prio_lvl
#define		int_int1_prio(_int_prio_lvl)		INTCON3bits.INT1IP = _int_prio_lvl
#define		int_int2_prio(_int_prio_lvl)		INTCON3bits.INT2IP = _int_prio_lvl
#define		int_int3_prio(_int_prio_lvl)		INTCON2bits.INT3IP = _int_prio_lvl

// Analogue
#define		int_adc_prio(_int_prio_lvl)			IPR1bits.ADIP = _int_prio_lvl
#define		int_hlvd_prio(_int_prio_lvl)		IPR2bits.HLVDIP = _int_prio_lvl
#define		int_ctmu_prio(_int_prio_lvl)		IPR3bits.CTMUIP = _int_prio_lvl

// Comparator
#define		int_comp1_prio(_int_prio_lvl)		IPR2bits.CM1IP = _int_prio_lvl
#define		int_comp2_prio(_int_prio_lvl)		IPR2bits.CM2IP = _int_prio_lvl
#define		int_comp3_prio(_int_prio_lvl)		IPR5bits.CM3IP = _int_prio_lvl

// EUSART
#define		int_eusart1rx_prio(_int_prio_lvl)	IPR1bits.RC1IP = _int_prio_lvl
#define		int_eusart1tx_prio(_int_prio_lvl)	IPR1bits.TX1IP = _int_prio_lvl
#define		int_eusart2rx_prio(_int_prio_lvl)	IPR3bits.RC2IP = _int_prio_lvl
#define		int_eusart2tx_prio(_int_prio_lvl)	IPR3bits.TX2IP = _int_prio_lvl

// MSSP
#define		int_mssp1_prio(_int_prio_lvl)		IPR1bits.SSP1IP = _int_prio_lvl
#define		int_mssp1col_prio(_int_prio_lvl)	IPR2bits.BCL1IP = _int_prio_lvl
#define		int_mssp2_prio(_int_prio_lvl)		IPR3bits.SSP2IP = _int_prio_lvl
#define		int_mssp2col_prio(_int_prio_lvl)	IPR3bits.BCL2IP = _int_prio_lvl

// ECCP & CCP
#define		int_eccp1_prio(_int_prio_lvl)		IPR1bits.CCP1IP = _int_prio_lvl
#define		int_eccp2_prio(_int_prio_lvl)		IPR2bits.CCP2IP = _int_prio_lvl
#define		int_eccp3_prio(_int_prio_lvl)		IPR4bits.CCP3IP = _int_prio_lvl
#define		int_ccp4_prio(_int_prio_lvl)		IPR4bits.CCP4IP = _int_prio_lvl
#define		int_ccp5_prio(_int_prio_lvl)		IPR4bits.CCP5IP = _int_prio_lvl
#define		int_ccp6_prio(_int_prio_lvl)		IPR4bits.CCP6IP = _int_prio_lvl
#define		int_ccp7_prio(_int_prio_lvl)		IPR4bits.CCP7IP = _int_prio_lvl
#define		int_ccp8_prio(_int_prio_lvl)		IPR4bits.CCP8IP = _int_prio_lvl
#define		int_ccp9_prio(_int_prio_lvl)		IPR4bits.CCP9IP = _int_prio_lvl
#define		int_ccp10_prio(_int_prio_lvl)		IPR4bits.CCP10IP = _int_prio_lvl

// Other
#define		int_pmp_prio(_int_prio_lvl)			IPR1bits.PMPIP = _int_prio_lvl
#define		int_osc_prio(_int_prio_lvl)			IPR2bits.OSCFIP = _int_prio_lvl
#define		int_rtcc_prio(_int_prio_lvl)		IPR3bits.RTCCIP = _int_prio_lvl

// USB
#define		int_usb_prio(_int_prio_lvl)			IPR2bits.USBIP = _int_prio_lvl


// ------ General Interrupt ------ //
#define		int_tmr0_init()			INTCONbits.TMR0IF = 0; INTCONbits.TMR0IE = 1
#define		int_tmr0_en()			INTCONbits.TMR0IE = 1
#define		int_tmr0_dis()			INTCONbits.TMR0IE = 0
#define		int_tmr0_clr()			INTCONbits.TMR0IF = 0

#define		int_rb_init()			INTCONbits.RBIF = 0; INTCONbits.RBIE = 1
#define		int_rb_en()				INTCONbits.RBIE = 1
#define		int_rb_dis()			INTCONbits.RBIE = 0
#define		int_rb_clr()			INTCONbits.RBIF = 0

#define		int_int0_init()			INTCONbits.INT0IF = 0; INTCONbits.INT0IE = 1
#define		int_int0_en()			INTCONbits.INT0IE = 1
#define		int_int0_dis()			INTCONbits.INT0IE = 0
#define		int_int0_clr()			INTCONbits.INT0IF = 0
#define		int_int0_rising()		INTCON2bits.INTEDG0 = 1
#define		int_int0_falling()		INTCON2bits.INTEDG0 = 0
#define		int_int1_init()			INTCON3bits.INT1IF = 0; INTCON3bits.INT1IE = 1
#define		int_int1_en()			INTCON3bits.INT1IE = 1
#define		int_int1_dis()			INTCON3bits.INT1IE = 0
#define		int_int1_clr()			INTCON3bits.INT1IF = 0
#define		int_int1_rising()		INTCON2bits.INTEDG1 = 1
#define		int_int1_falling()		INTCON2bits.INTEDG1 = 0
#define		int_int2_init()			INTCON3bits.INT2IF = 0; INTCON3bits.INT2IE = 1
#define		int_int2_en()			INTCON3bits.INT2IE = 1
#define		int_int2_dis()			INTCON3bits.INT2IE = 0
#define		int_int2_clr()			INTCON3bits.INT2IF = 0
#define		int_int2_rising()		INTCON2bits.INTEDG2 = 1
#define		int_int2_falling()		INTCON2bits.INTEDG2 = 0
#define		int_int3_init()			INTCON3bits.INT3IF = 0; INTCON3bits.INT3IE = 1
#define		int_int3_en()			INTCON3bits.INT3IE = 1
#define		int_int3_dis()			INTCON3bits.INT3IE = 0
#define		int_int3_clr()			INTCON3bits.INT3IF = 0
#define		int_int3_rising()		INTCON2bits.INTEDG3 = 1
#define		int_int3_falling()		INTCON2bits.INTEDG3 = 0


// ------ Peripheral Interrupt ------ //
// Timer
#define		int_tmr1_init()			PIR1bits.TMR1IF = 0; PIE1bits.TMR1IE = 1
#define		int_tmr1_en()			PIE1bits.TMR1IE = 1
#define		int_tmr1_dis()			PIE1bits.TMR1IE = 0
#define		int_tmr1_clr()			PIR1bits.TMR1IF = 0
#define		int_tmr1gate_init()		PIR5bits.TMR1GIF = 0; PIE5bits.TMR1GIE = 1
#define		int_tmr1gate_en()		PIE5bits.TMR1GIE = 1
#define		int_tmr1gate_dis()		PIE5bits.TMR1GIE = 0
#define		int_tmr1gate_clr()		PIR5bits.TMR1GIF = 0
#define		int_tmr2_init()			PIR1bits.TMR2IF = 0; PIE1bits.TMR2IE = 1
#define		int_tmr2_en()			PIE1bits.TMR2IE = 1
#define		int_tmr2_dis()			PIE1bits.TMR2IE = 0
#define		int_tmr2_clr()			PIR1bits.TMR2IF = 0
#define		int_tmr3_init()			PIR2bits.TMR3IF = 0; PIE2bits.TMR3IE = 1
#define		int_tmr3_en()			PIE2bits.TMR3IE = 1
#define		int_tmr3_dis()			PIE2bits.TMR3IE = 0
#define		int_tmr3_clr()			PIR2bits.TMR3IF = 0
#define		int_tmr3gate_init()		PIR3bits.TMR3GIF = 0; PIE3bits.TMR3GIE = 1
#define		int_tmr3gate_en()		PIE3bits.TMR3GIE = 1
#define		int_tmr3gate_dis()		PIE3bits.TMR3GIE = 0
#define		int_tmr3gate_clr()		PIR3bits.TMR3GIF = 0
#define		int_tmr4_init()			PIR3bits.TMR4IF = 0; PIE3bits.TMR4IE = 1
#define		int_tmr4_en()			PIE3bits.TMR4IE = 1
#define		int_tmr4_dis()			PIE3bits.TMR4IE = 0
#define		int_tmr4_clr()			PIR3bits.TMR4IF = 0
#define		int_tmr5_init()			PIR5bits.TMR5IF = 0; PIE5bits.TMR5IE = 1
#define		int_tmr5_en()			PIE5bits.TMR5IE = 1
#define		int_tmr5_dis()			PIE5bits.TMR5IE = 0
#define		int_tmr5_clr()			PIR5bits.TMR5IF = 0
#define		int_tmr5gate_init()		PIR5bits.TMR5GIF = 0; PIE5bits.TMR5GIE = 1
#define		int_tmr5gate_en()		PIE5bits.TMR5GIE = 1
#define		int_tmr5gate_dis()		PIE5bits.TMR5GIE = 0
#define		int_tmr5gate_clr()		PIR5bits.TMR5GIF = 0
#define		int_tmr6_init()			PIR5bits.TMR6IF = 0; PIE5bits.TMR6IE = 1
#define		int_tmr6_en()			PIE5bits.TMR6IE = 1
#define		int_tmr6_dis()			PIE5bits.TMR6IE = 0
#define		int_tmr6_clr()			PIR5bits.TMR6IF = 0
#define		int_tmr8_init()			PIR5bits.TMR8IF = 0; PIE5bits.TMR8IE = 1
#define		int_tmr8_en()			PIE5bits.TMR8IE = 1
#define		int_tmr8_dis()			PIE5bits.TMR8IE = 0
#define		int_tmr8_clr()			PIR5bits.TMR8IF = 0


// Other
#define		int_pmp_init()			PIR1bits.PMPIF = 0; PIE1bits.PMPIE = 1
#define		int_pmp_en()			PIE1bits.PMPIE = 1
#define		int_pmp_dis()			PIE1bits.PMPIE = 0
#define		int_pmp_clr()			PIR1bits.PMPIF = 0
#define		int_osc_init()			PIR2bits.OSCFIF = 0; PIE2bits.OSCFIE = 1
#define		int_osc_en()			PIE2bits.OSCFIE = 1
#define		int_osc_dis()			PIE2bits.OSCFIE = 0
#define		int_osc_clr()			PIR2bits.OSCFIF = 0
#define		int_rtcc_init()			PIR3bits.RTCCIF = 0; PIE3bits.RTCCIE = 1
#define		int_rtcc_en()			PIE3bits.RTCCIE = 1
#define		int_rtcc_dis()			PIE3bits.RTCCIE = 0
#define		int_rtcc_clr()			PIR3bits.RTCCIF = 0

// Analogue
#define		int_adc_init()			PIR1bits.ADIF = 0; PIE1bits.ADIE = 1
#define		int_adc_en()			PIE1bits.ADIE = 1
#define		int_adc_dis()			PIE1bits.ADIE = 0
#define		int_adc_clr()			PIR1bits.ADIF = 0
#define		int_hlvd_init()			PIR2bits.HLVDIF = 0; PIE2bits.HLVDIE = 1
#define		int_hlvd_en()			PIE2bits.HLVDIE = 1
#define		int_hlvd_dis()			PIE2bits.HLVDIE = 0
#define		int_hlvd_clr()			PIR2bits.HLVDIF = 0
#define		int_ctmu_init()			PIR3bits.CTMUIF = 0; PIE3bits.CTMUIE = 1
#define		int_ctmu_en()			PIE3bits.CTMUIE = 1
#define		int_ctmu_dis()			PIE3bits.CTMUIE = 0
#define		int_ctmu_clr()			PIR3bits.CTMUIF = 0

// EUSART
#define		int_eusart1rx_init()	PIE1bits.RC1IE = 1
#define		int_eusart1rx_en()		PIE1bits.RC1IE = 1
#define		int_eusart1rx_dis()		PIE1bits.RC1IE = 0
#define		int_eusart1tx_init()	TXREG1 = 0; PIE1bits.TX1IE = 1
#define		int_eusart1tx_en()		PIE1bits.TX1IE = 1
#define		int_eusart1tx_dis()		PIE1bits.TX1IE = 0
#define		int_eusart2rx_init()	PIE3bits.RC2IE = 1
#define		int_eusart2rx_en()		PIE3bits.RC2IE = 1
#define		int_eusart2rx_dis()		PIE3bits.RC2IE = 0
#define		int_eusart2tx_init()	TXREG2 = 0; PIE3bits.TX2IE = 1
#define		int_eusart2tx_en()		PIE3bits.TX2IE = 1
#define		int_eusart2tx_dis()		PIE3bits.TX2IE = 0

// MSSP
#define		int_mssp1_init()		PIR1bits.SSP1IF = 0; PIE1bits.SSP1IE = 1
#define		int_mssp1_en()			PIE1bits.SSP1IE = 1
#define		int_mssp1_dis()			PIE1bits.SSP1IE = 0
#define		int_mssp1_clr()			PIR1bits.SSP1IF = 0
#define		int_mssp1col_init()		PIR2bits.BCL1IF = 0; PIE2bits.BCL1IE = 1
#define		int_mssp1col_en()		PIE2bits.BCL1IE = 1
#define		int_mssp1col_dis()		PIE2bits.BCL1IE = 0
#define		int_mssp1col_clr()		PIR2bits.BCL1IF = 0
#define		int_mssp2_init()		PIR3bits.SSP2IF = 0; PIE3bits.SSP2IE = 1
#define		int_mssp2_en()			PIE3bits.SSP2IE = 1
#define		int_mssp2_dis()			PIE3bits.SSP2IE = 0
#define		int_mssp2_clr()			PIR3bits.SSP2IF = 0
#define		int_mssp2col_init()		PIR3bits.BCL2IF = 0; PIE3bits.BCL2IE = 1
#define		int_mssp2col_en()		PIE3bits.BCL2IE = 1
#define		int_mssp2col_dis()		PIE3bits.BCL2IE = 0
#define		int_mssp2col_clr()		PIR3bits.BCL2IF = 0

// ECCP & CCP
#define		int_eccp1_init()		PIR1bits.CCP1IF = 0; PIE1bits.CCP1IE = 1
#define		int_eccp1_en()			PIE1bits.CCP1IE = 1
#define		int_eccp1_dis()			PIE1bits.CCP1IE = 0
#define		int_eccp1_clr()			PIR1bits.CCP1IF = 0
#define		int_eccp2_init()		PIR2bits.CCP2IF = 0; PIE2bits.CCP2IE = 1
#define		int_eccp2_en()			PIE2bits.CCP2IE = 1
#define		int_eccp2_dis()			PIE2bits.CCP2IE = 0
#define		int_eccp2_clr()			PIR2bits.CCP2IF = 0
#define		int_eccp3_init()		PIR4bits.CCP3IF = 0; PIE4bits.CCP3IE = 1
#define		int_eccp3_en()			PIE4bits.CCP3IE = 1
#define		int_eccp3_dis()			PIE4bits.CCP3IE = 0
#define		int_eccp3_clr()			PIR4bits.CCP3IF = 0
#define		int_ccp4_init()			PIR4bits.CCP4IF = 0; PIE4bits.CCP4IE = 1
#define		int_ccp4_en()			PIE4bits.CCP4IE = 1
#define		int_ccp4_dis()			PIE4bits.CCP4IE = 0
#define		int_ccp4_clr()			PIR4bits.CCP4IF = 0
#define		int_ccp5_init()			PIR4bits.CCP5IF = 0; PIE4bits.CCP5IE = 1
#define		int_ccp5_en()			PIE4bits.CCP5IE = 1
#define		int_ccp5_dis()			PIE4bits.CCP5IE = 0
#define		int_ccp5_clr()			PIR4bits.CCP5IF = 0
#define		int_ccp6_init()			PIR4bits.CCP6IF = 0; PIE4bits.CCP6IE = 1
#define		int_ccp6_en()			PIE4bits.CCP6IE = 1
#define		int_ccp6_dis()			PIE4bits.CCP6IE = 0
#define		int_ccp6_clr()			PIR4bits.CCP6IF = 0
#define		int_ccp7_init()			PIR4bits.CCP7IF = 0; PIE4bits.CCP7IE = 1
#define		int_ccp7_en()			PIE4bits.CCP7IE = 1
#define		int_ccp7_dis()			PIE4bits.CCP7IE = 0
#define		int_ccp7_clr()			PIR4bits.CCP7IF = 0
#define		int_ccp8_init()			PIR4bits.CCP8IF = 0; PIE4bits.CCP8IE = 1
#define		int_ccp8_en()			PIE4bits.CCP8IE = 1
#define		int_ccp8_dis()			PIE4bits.CCP8IE = 0
#define		int_ccp8_clr()			PIR4bits.CCP8IF = 0
#define		int_ccp9_init()			PIR4bits.CCP9IF = 0; PIE4bits.CCP9IE = 1
#define		int_ccp9_en()			PIE4bits.CCP9IE = 1
#define		int_ccp9_dis()			PIE4bits.CCP9IE = 0
#define		int_ccp9_clr()			PIR4bits.CCP9IF = 0
#define		int_ccp10_init()		PIR4bits.CCP10IF = 0; PIE4bits.CCP10IE = 1
#define		int_ccp10_en()			PIE4bits.CCP10IE = 1
#define		int_ccp10_dis()			PIE4bits.CCP10IE = 0
#define		int_ccp10_clr()			PIR4bits.CCP10IF = 0

// Comparators
#define		int_comp1_init()		PIR2bits.CM1IF = 0; PIE2bits.CM1IE = 1
#define		int_comp1_en()			PIE2bits.CM1IE = 1
#define		int_comp1_dis()			PIE2bits.CM1IE = 0
#define		int_comp1_clr()			PIR2bits.CM1IF = 0
#define		int_comp2_init()		PIR2bits.CM2IF = 0; PIE2bits.CM2IE = 1
#define		int_comp2_en()			PIE2bits.CM2IE = 1
#define		int_comp2_dis()			PIE2bits.CM2IE = 0
#define		int_comp2_clr()			PIR2bits.CM2IF = 0
#define		int_comp3_init()		PIR5bits.CM3IF = 0; PIE5bits.CM3IE = 1
#define		int_comp3_en()			PIE5bits.CM3IE = 1
#define		int_comp3_dis()			PIE5bits.CM3IE = 0
#define		int_comp3_clr()			PIR5bits.CM3IF = 0

// USB
#define		int_usb_init()			PIR2bits.USBIF = 0; PIE2bits.USBIE = 1
#define		int_usb_en()			PIE2bits.USBIE = 1
#define		int_usb_dis()			PIE2bits.USBIE = 0
#define		int_usb_clr()			PIR2bits.USBIF = 0
// ############################################## //

#endif
