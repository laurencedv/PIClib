
/*!
 @file PIC Stik-18.h
 @brief		Macro for easy use of the PIC Stik-18 on C18
 @version 0.1
 @date January 29th 2011
 @author Laurence DV
*/

#ifndef _PICSTAMP_18_H
#define _PICSTAMP_18_H

// ================= Includes ================= //
#include <p18f27j53.h>

// ================= Defines ================= //

// --- On-board peripherals --- //
#define	    LED_G			LATCbits.LATC0
#define	    LED_G_TRIS		TRISCbits.TRISC0
#define	    LED_B			LATCbits.LATC2
#define	    LED_B_TRIS		TRISCbits.TRISC2
#define	    LED_R			LATCbits.LATC1
#define	    LED_R_TRIS		TRISCbits.TRISC1

#define		LED_ON			0
#define		LED_OFF			1



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



#endif
