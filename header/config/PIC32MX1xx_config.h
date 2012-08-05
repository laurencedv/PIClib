/*!
 @file		PIC32MX1xx_config.h
 @brief		Configuration bits for PIC32MX1xx PIC
 
 @version	0.1
 @note
 
 @date		January 8th 2012
 @author	Laurence DV
*/

#ifndef _PIC32MX1xx_CONFIG_H
#define _PIC32MX1xx_CONFIG_H 1

// ---------- Clock ---------- //
//Main Clock Source Selection
#pragma config FNOSC = FRCPLL		// FRC | FRCPLL | PRI | PRIPLL | SOSC | LPRC | FRCDIV16 | FRCDIV
//Primary OSC Configuration
#pragma config POSCMOD = OFF		// OFF | EC | XT | HS
//Main PLL Input Divider
#pragma config FPLLIDIV = DIV_2		// DIV_1 | DIV_2 | DIV_3 | DIV_4 | DIV_5 | DIV_6 | DIV_10 | DIV_12
//Main PLL Multiplier
#pragma config FPLLMUL = MUL_20		// MUL_15 | MUL_16 | MUL_17 | MUL_18 | MUL_19 | MUL_20 | MUL_21 | MUL_24
//Main PLL Output Divider
#pragma config FPLLODIV = DIV_2		// DIV_1 | DIV_2 | DIV_4 | DIV_8 | DIV_16 | DIV_32 | DIV_64 | DIV_256
//Secondary OSC Enable
#pragma config FSOSCEN = OFF		// ON | OFF
//CLKO Output signal
#pragma config OSCIOFNC = OFF		// ON | OFF
//Peripheral Clock Divider
#pragma config FPBDIV = DIV_1		// DIV_1 | DIV_2 | DIV_4 | DIV_8


// ---------- Execution Monitoring ---------- //
//Internal/External Clock Switch Over
#pragma config IESO = ON			// ON | OFF
//Clock Switching and Monitor Selection
#pragma config FCKSM = CSECME		// CSECME | CSECMD | CSDCMD
//Watchdog Timer Enable
#pragma config FWDTEN = OFF			// ON | OFF
//Watchdog Timer Window Enable
#pragma config WINDIS = OFF			// ON | OFF
//Watchdog Timer Window Size
#pragma config FWDTWINSZ = WINSZ_75	// WINSZ_75 | WINSZ_50 | WINSZ_37 | WINSZ_25
//Watchdog Timer Postscaler
#pragma config WDTPS = PS1			// PS1 | PS2 | PS4 | PS8 | PS16 | PS32 | PS64 | PS128 | PS256 | PS512 | PS1024 | PS2048 | PS4096 | PS8192 | PS16384 | PS32768 | PS65536 | PS131072 | PS262144 | PS524288 | PS1048576


// ---------- Special function ---------- //
//Background Debugger Enable
#pragma config DEBUG = OFF			// ON | OFF
//ICE/ICD Comm Channel
#pragma config ICESEL = ICS_PGx1	// ICS_PGx1 | ICS_PGx2 | ICS_PGx3 | RESERVED
//JTAG Enable
#pragma config JTAGEN = OFF			// ON | OFF
//Peripheral Module Disable Configuration
#pragma config PMDL1WAY = OFF		// ON | OFF
//IOLOCK one-way set enable
#pragma config IOL1WAY = OFF		// ON | OFF
//USB USBID Selection
#pragma config FUSBIDIO = OFF		// ON | OFF
//USB VBUS ON Selection
#pragma config FVBUSONIO = OFF		// ON | OFF


// ---------- Code protection ---------- //
//Program Flash Write Protect
#pragma config PWP = OFF			// PWP1K > PWP32K | OFF
//Boot Flash Write Protect
#pragma config BWP = OFF			// ON | OFF
//Code Protect
#pragma config CP = OFF				// ON | OFF

#endif
