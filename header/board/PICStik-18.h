
/*!
 @file		PIC Stik-18.h
 @brief		Macro for easy use of the PIC Stik-18 on C18
 
 @version	0.1
 @note

 @date		January 29th 2011
 @author	Laurence DV
*/

#ifndef _PICSTIK_18_H
#define _PICSTIK_18_H

// ================= Includes ================= //
#include <p18f47j53.h>

// ================= Defines ================= //

// --- On-board peripherals --- //
#define	    LED_G			LATBbits.LATB1
#define	    LED_G_TRIS		TRISBbits.TRISB1
#define	    LED_B			LATDbits.LATD2
#define	    LED_B_TRIS		TRISDbits.TRISD2
#define	    LED_R			LATDbits.LATD3
#define	    LED_R_TRIS		TRISDbits.TRISD3

#define		LED_ON			1
#define		LED_OFF			0

#define	    BTN				PORTBbits.RB0
#define	    BTN_TRIS		TRISBbits.TRISB0


// --- MSSP 1 --- //
#define	    SCK1			PORTBbits.SCK1
#define	    SCK1_TRIS		TRISBbits.TRISB4
#define	    SDI1			PORTBbits.SDI1
#define	    SDI1_TRIS		TRISBbits.TRISB5
#define	    SDO1			PORTCbits.SDO1
#define	    SDO1_TRIS		TRISCbits.TRISC7
#define	    SS1				PORTAbits.NOT_SS1
#define	    SS1_TRIS		TRISAbits.TRISA5

#define	    SDA1			PORTBbits.SDA1
#define	    SDA1_TRIS		TRISBbits.TRISB5
#define	    SCL1			PORTBbits.SCL1
#define	    SCL1_TRIS		TRISBbits.TRISB4

// --- MSSP 2 --- //
#define	    SCK2
#define	    SCK2_TRIS
#define	    SDI2
#define	    SDI2_TRIS
#define	    SDO2
#define	    SDO2_TRIS
#define	    SS2
#define	    SS2_TRIS

#define	    SDA2			PORTDbits.SDA2
#define	    SDA2_TRIS		TRISDbits.TRISD1
#define	    SCL2			PORTDbits.SCL2
#define	    SCL2_TRIS		TRISDbits.TRISD0

// --- ADC --- //

// ================= Debug Macro ================= //


#define		PS18DCC_blank		LED_R = 0;LED_G = 0;LED_B = 0
#define		PS18DCC_black		LED_R = 0;LED_G = 0;LED_B = 0
#define		PS18DCC_blue		LED_R = 0;LED_G = 0;LED_B = 1
#define		PS18DCC_green		LED_R = 0;LED_G = 1;LED_B = 0
#define		PS18DCC_cyan		LED_R = 0;LED_G = 1;LED_B = 1
#define		PS18DCC_red			LED_R = 1;LED_G = 0;LED_B = 0
#define		PS18DCC_purple		LED_R = 1;LED_G = 0;LED_B = 1
#define		PS18DCC_yellow		LED_R = 1;LED_G = 1;LED_B = 0
#define		PS18DCC_white		LED_R = 1;LED_G = 1;LED_B = 1


#endif
