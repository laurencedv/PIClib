
/*!
 @file		BiTizoid-revA0.h
 @brief		Macro for easy use of the BiTizoid rev.A0 on C18
 
 @version	0.1
 @note

 @date		December 15th 2011
 @author	Laurence DV
*/

#ifndef _BITIZOID_REVA0_H
#define _BITIZOID_REVA0_H

// ================== Include =================== //
#include <p18f27j53.h>
// ============================================== //


// =================== Define =================== //

// --- On-board peripherals --- //
#define	    LED_G			PORTCbits.RC1
#define	    LED_G_DIR		TRISCbits.TRISC1
#define	    LED_B			PORTCbits.RC2
#define	    LED_B_DIR		TRISCbits.TRISC2
#define	    LED_R			PORTCbits.RC0
#define	    LED_R_DIR		TRISCbits.TRISC0

#define		LED_ON			0
#define		LED_OFF			1

#define	    BTN				PORTBbits.RB0
#define	    BTN_DIR			TRISBbits.TRISB0

#define		BT_RESET		PORTAbits.RA0
#define		BT_RESET_DIR	TRISAbits,TRISA0

#define		BT_SPI_SS		PORTBbits.RB4
#define		BT_SPI_SS_DIR	TRISBbits.TRISB4

#define		BT_AIO0			PORTAbits.RA2
#define		BT_AIO0_DIR		TRISAbits.TRISA2
#define		BT_AIO1			PORTAbits.RA1
#define		BT_AIO1_DIR		TRISAbits.TRISA1

#define		BT_PIO0			PORTBbits.RB3
#define		BT_PIO0_DIR		TRISBbits.TRISB3
#define		BT_PIO1			PORTBbits.RB2
#define		BT_PIO1_DIR		TRISBbits.TRISB2
#define		BT_PIO2			PORTBbits.RB1
#define		BT_PIO2_DIR		TRISBbits.TRISB1
#define		BT_PIO4			PORTAbits.RA7
#define		BT_PIO4_DIR		TRISAbits.TRISA7
#define		BT_PIO5			PORTAbits.RA6
#define		BT_PIO5_DIR		TRISAbits.TRISA6
#define		BT_PIO6			PORTAbits.RA5
#define		BT_PIO6_DIR		TRISAbits.TRISA5
#define		BT_PIO8			PORTAbits.RA3
#define		BT_PIO8_DIR		TRISAbits.TRISA3

// ============================================== //



#endif
