/*!
 @file		PIC32MX2xx_config.h
 @brief		Configuration bits for PIC32MX220F032B
 
 @version	0.1
 @note
 
 @date		November 30th 2011
 @author	Laurence DV
*/

#ifndef _PIC32MX575_CONFIG_H
#define _PIC32MX575_CONFIG_H 1

// ---------- Clock ---------- //
//Main Clock Source Selection
#pragma config FNOSC = FRCPLL		// FRC | FRCPLL | PRI | PRIPLL | SOSC | LPRC | FRCDIV16 | FRCDIV
//Primary OSC Configuration
#pragma config POSCMOD = OFF		// OFF | EC | XT |HS
//Main PLL Input Divider
#pragma config FPLLIDIV = DIV_2		// DIV_1 | DIV_2 | DIV_3 | DIV_4 | DIV_5 | DIV_6 | DIV_10 | DIV_12
//Main PLL Multiplier
#pragma config FPLLMUL = MUL_20		// MUL_15 | MUL_16 | MUL_17 | MUL_18 | MUL_19 | MUL_20 | MUL_21 | MUL_24
//Main PLL Output Divider
#pragma config FPLLODIV = DIV_2		// DIV_1 | DIV_2 | DIV_4 | DIV_8 | DIV_16 | DIV_32 | DIV_64 | DIV_256
//USB PLL Enable
#pragma config UPLLEN = OFF			// ON | OFF
//USB PLL Input Divider
#pragma config UPLLIDIV = DIV_1		// DIV_1 | DIV_2 | DIV_3 | DIV_4 | DIV_5 | DIV_6 | DIV_10 | DIV_12
//Secondary OSC Enable
#pragma config FSOSCEN = OFF		// ON | OFF
//Peripheral Clock Divider
#pragma config FPBDIV = DIV_8		// DIV_1 | DIV_2 | DIV_4 | DIV_8
//CLKO Output signal
#pragma config OSCIOFNC = ON		// ON | OFF

// ---------- Execution Monitoring ---------- //
//Internal/External Clock Switch Over
#pragma config IESO = OFF			// ON | OFF
//Clock Switching and Monitor Selection
#pragma config FCKSM = CSDCMD		// CSECME | CSECMD | CSDCMD
//Watchdog Timer Enable
#pragma config FWDTEN = OFF			// ON | OFF
//Watchdog Timer Postscaler
#pragma config WDTPS = PS1			// PS1 | PS2 | PS4 | PS8 | PS16 | PS32 | PS64 | PS128 | PS256 | PS512 | PS1024 | PS2048 | PS4096 | PS8192 | PS16384 | PS32768 | PS65536 | PS131072 | PS262144 | PS524288 | PS1048576

// ---------- Special function ---------- //
//Shadow Set Register assignation
#pragma config FSRSSEL = PRIORITY_0	// PRIORITY_0 > PRIORITY_7
//CAN IO pin Selection
#pragma config FCANIO = ON			// ON | OFF
//USB USBID Selection
#pragma config FUSBIDIO = OFF		// ON | OFF
//USB VBUS ON Selection
#pragma config FVBUSONIO = OFF		// ON | OFF
//Background Debugger Enable
#pragma config DEBUG = OFF			// ON | OFF
//ICE/ICD Comm Channel
#pragma config ICESEL = ICS_PGx2	// ICS_PGx1 | ICS_PGx2

// ---------- Code protection ---------- //
//Program Flash Write Protect
#pragma config PWP = OFF			// PWP4K > PWP512K | OFF
//Boot Flash Write Protect
#pragma config BWP = OFF			// ON | OFF
//Code Protect
#pragma config CP = OFF				// ON | OFF

#endif
