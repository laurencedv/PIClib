/*!
 @file		GIO-test.h
 @brief		Macro for easy use of Generic IO Node test (breadboard)
 
 @version	0.1
 @note

 @date		November 9th 2011
 @author	Laurence DV
*/

#ifndef _GIO_TEST_H
#define _GIO_TEST_H

// =================== Define =================== //

// --- On-board peripherals --- //

#define	    LED_O			PORTAbits.RA3
#define	    LED_O_DIR		TRISAbits.TRISA3
#define	    LED_B			PORTAbits.RA1
#define	    LED_B_DIR		TRISAbits.TRISA1
#define	    LED_R			PORTAbits.RA2
#define	    LED_R_DIR		TRISAbits.TRISA2

#define		LED_ON			1
#define		LED_OFF			0
// ---------------------------- //
// ============================================== //

#endif
