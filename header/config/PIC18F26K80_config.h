/*!
 @file		PIC18F26K80_config.h
 @brief		Configuration bits for PIC18(L)F26K80
 
 @version	0.1
 @note
 
 @date		November 2th 2011
 @author	Laurence DV
*/

#ifndef _PIC18F26K80_CONFIG_H
#define _PIC18F26K80_CONFIG_H

// ---------- Clock ---------- //
// Low-Frequency internal OSC power mode in sleep
#pragma config INTOSCSEL = LOW		// LOW | HIGH
//T1OSC/SOSC Power and Mode selection
#pragma config SOSCSEL = HIGH		// LOW | DIG | HIGH
//Selecting OSC
#pragma config FOSC = INTIO2		// LP | XT | HS2 (16M-25M) | HS1 (4M - 16M) | EC3IO (16M-64M CLKO) | RC | RCIO (CLKO) | INTIO2 | INTIO1 (CLKO) | EC2IO (160K-4M CLKO) | EC1IO (0 - 160K CLKO) | EC1 (0-160K)
//PLL control
#pragma config PLLCFG = ON			// ON | OFF
//Fail-safe Clock Monitor
#pragma config FCMEN = ON			// ON | OFF
//OSC switch over mode
#pragma config IESO = OFF			// ON | OFF


// ---------- Execution Monitoring ---------- //
//Power Up Timer
#pragma config PWRTEN = ON			// ON | OFF
//Brown Out Detect
#pragma config BOREN = ON			// ON | OFF | NOSLP | SBORDIS
//Brown-out voltage trigger
#pragma config BORV = 0				// 0 (3V) | 1 (2.7V) | 2 (2V) | 3 (1.8V)
//Brown-out power level
#pragma config BORPWR = LOW			// LOW | MEDIUM | HIGH | ZPBORMV
//Watchdog Control
#pragma config WDTEN = OFF			// ON | OFF | NOSLP | SWDTDIS
//Watchdog Postscaler
#pragma config WDTPS = 1			// 1048576 | 514288 | 262144 | 131072 | 65536 |32768 | 16384 | 8192 | 4096 | 2048 | 1024 | 512 | 256 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1
//Stack overflow cause reset
#pragma config STVREN = ON			// ON | OFF


// ---------- Special function ---------- //
//Voltage Regulator enable in sleep
#pragma config RETEN = OFF			// ON | OFF
//Extended Instruction set
#pragma config XINST = OFF			// ON | OFF
//ECAN pin selection
#pragma config CANMX = PORTB		// PORTC (TX:C6 RX:C7) | PORTB (TX:B2 RX:B3)
//MSSP address Mask
#pragma config MSSPMSK = MSK5		// MSK5 | MSK7
//Master CLeaR Enable
#pragma config MCLRE = ON			// ON | OFF
//Boot Block Size
#pragma config BBSIZ = BB1K			// BB1K | BB2K


// ---------- Code protection ---------- //
//Code protect 0x0800 - 0x3FFF
#pragma config CP0 = OFF			// ON | OFF
//Code protect 0x4000 - 0x7FFF
#pragma config CP1 = OFF			// ON | OFF
//Code protect 0x8000 - 0xBFFF
#pragma config CP2 = OFF			// ON | OFF
//Code protect 0xc000 - 0xFFFF
#pragma config CP3 = OFF			// ON | OFF

//Code protect Boot Block
#pragma config CPB = OFF			// ON | OFF
//Table Write Protect Boot Block
#pragma config WRTB = OFF			// ON | OFF
//Table Read Protect Boot Block
#pragma config EBTRB = OFF			// ON | OFF

//Data EE Write Protect
#pragma config WRTD = OFF			// ON | OFF
//Data EE Read Protect
#pragma config CPD = OFF			// ON | OFF

//Table Write Protect 0x0800 - 0x3FFF
#pragma config WRT0 = OFF			// ON | OFF
//Table Write Protect 0x4000 - 0x7FFF
#pragma config WRT1 = OFF			// ON | OFF
//Table Write Protect 0x8000 - 0xBFFF
#pragma config WRT2 = OFF			// ON | OFF
//Table Write Protect 0xc000 - 0xFFFF
#pragma config WRT3 = OFF			// ON | OFF
//Table Read Protect 0x0800 - 0x3FFF
#pragma config EBTR0 = OFF			// ON | OFF
//Table Read Protect 0x4000 - 0x7FFF
#pragma config EBTR1 = OFF			// ON | OFF
//Table Read Protect 0x8000 - 0xBFFF
#pragma config EBTR2 = OFF			// ON | OFF
//Table Read Protect 0xC000 - 0xFFFF
#pragma config EBTR3 = OFF			// ON | OFF

//Config Write Protect
#pragma config WRTC = OFF			// ON | OFF		!!!WATCHOUT!!!

#endif

