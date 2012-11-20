/*!
 @file 		PIC18(L)F47J53_config.h
 @brief		Configuration bits for PIC18(L)F47J53
		Only missing are: MSSP7B_EN
 @version 	0.2
 @date 		February 28th 2011
 @author 	Laurence DV
*/

#ifndef _PIC18F27J53_CONFIG_H
#define _PIC18F27J53_CONFIG_H

// ---------- Clock ---------- //
//Selecting OSC
#pragma config OSC = INTOSCPLL		// INTOSC | INTOSCO | INTOSCPLL | INTOSCPLLO | HS | HSPLL | EC | ECPLL
//PLL control
#pragma config CFGPLLEN = ON		// ON | OFF
//PLL divider
#pragma config PLLDIV = 2		// 12 | 10 | 6 | 5 | 4 | 3 | 2 | 1
//CPU clock divider
#pragma config CPUDIV = OSC1		// OSC4_PLL6 | OSC3_PLL3 | OSC2_PLL2 | OSC1
//T1OSC/SOSC Power selection
#pragma config SOSCSEL = LOW		// LOW | DIG | HIGH
//Main Clock out enable (RA6)
#pragma config CLKOEC = OFF		// ON | OFF
//Fail-safe Clock Monitor
#pragma config FCMEN = ON		// ON | OFF
//OSC switch over mode
#pragma config IESO = OFF		// ON | OFF
//RTCC Clock selection
#pragma config RTCOSC = T1OSCREF	// INTOSCREF | T1OSCREF

// ---------- Execution Monitoring ---------- //
//Deep Sleep Brown-out detect
#pragma config DSBOREN = OFF		// ON | OFF
//Watchdog Control
#pragma config WDTEN = OFF		// ON | OFF
//Watchdog Postscaler
#pragma config WDTPS = 1		// 32768 | 16384 | 8192 | 4096 | 2048 | 1024 | 512 | 256 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1
//Deep Sleep Watchdog Timer
#pragma config DSWDTEN = OFF		// ON | OFF
//Deep Sleep Watchdog Clock selection
#pragma config DSWDTOSC = INTOSCREF	// INTOSCREF | T1OSCREF
//Deep Sleep Watchdof Postscaler
#pragma config DSWDTPS = 2		// G2 (25.7 days) | M536 (6.4 days) | M134 (38.5 hours) | M33 (9.6 hours) | M8 (2.4 hours) | M2 (36 min) | K524 (9 min) | K131 (135 sec) | K32 (34 sec) | 8192 (8.5 sec) | 2048 (2.1 sec) | 512 (528 ms) | 128 (132ms) | 32 (33ms) | 8 (8.3ms) | 2 (2.1ms)

// ---------- Special function ---------- //
//Stack over/underflow cause reset
#pragma config STVREN = ON		// ON | OFF
//Extended Instruction set
#pragma config XINST = OFF		// ON | OFF
//IOLOCK one-way set enable
#pragma config IOL1WAY = OFF		// ON | OFF
//Low Speed USB mode with 48MHz FOSC
#pragma config LS48MHZ = SYS48X8	// SYS48X8 | SYS24X4
//ADC precision
#pragma config ADCSEL = BIT12		// BIT12 | BIT10

// ---------- Code protection ---------- //
//Program block code-protection
#pragma config CP0 = OFF		// ON | OFF
//Write/Erase protect page start/end location
#pragma config WPFP = PAGE_0		// PAGE_0 >> PAGE_127
//Write Protection of the Configuration bits
#pragma config WPCFG = OFF
//Write Protect disable
#pragma config WPDIS = OFF		// ON | OFF
//Write/Erase protect region select
#pragma config WPEND = PAGE_0		// PAGE_0 | PAGE_WPFP

#endif
