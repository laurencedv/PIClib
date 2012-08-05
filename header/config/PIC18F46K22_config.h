// Configuration of Fuses and device function
// PIC18(L)F46K22

// ---------- Clock ---------- //
#pragma config FOSC = INTIO67		//Setting OSC to internal with IO on RA6 and RA7
#pragma config PLLCFG = OFF			//PLL x4 disabled
#pragma config PRICLKEN = ON		//Primary Clock always ON
#pragma config FCMEN = ON			//Fail-safe Clock Monitor enabled
#pragma config IESO = OFF			//OSC switch over mode disabled

// ----------Execution Monitoring ---------- //
#pragma config PWRTEN = ON			//Power-up Timer enabled
#pragma config BOREN = SBORDIS		//Brown-out reset enabled in hardware only
#pragma config BORV = 250			//Brown-out level to 2.5V
#pragma config WDTEN = OFF			//Watchdog disabled

// ---------- Special function ---------- //
#pragma config MCLRE = EXTMCLR		//MCLR pin enabled (RE3 disabled)
#pragma config PBADEN = OFF			//Port B 0-5 Digital IO on reset
#pragma config STVREN = ON			//Stack over/underflow cause reset
#pragma config XINST = ON			//Instruction set extended and indexed adressing mode enable

// ---------- Programming ---------- //
#pragma config LVP = OFF			//Low Voltage Programming disabled
#pragma config DEBUG = OFF			//Debugging mode disabled

// ---------- Code protection ---------- //
//Program block code-protection
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CPB = OFF
//Program block code-protection from others program block
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTRB = OFF
//Program block write-protection
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRTB = OFF, WRTC = OFF
//Data EEPROM protection
#pragma config CPD = OFF, WRTD = OFF
