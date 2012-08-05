
/*!
 @file pic18_delay.h
 @brief		Delay functions for C18
 *		ATTENTION! Not Accurate!!!! work harder you dumb ass
 *		TODO: Remake everything....
 @version 0.2
 @date February 9th 2011
 @author Laurence DV
*/

#ifndef _PIC18_DELAY_H
#define _PIC18_DELAY_H

// ================= Includes ================= //
// Definition
#include <definition/datatype_megaxone.h>

//Dev Macro
#include <splitvar_megaxone.h>
// ================= Prototype ================= //

void delay_nCY(U8 number_of_cycle);
void delay_n10CY (U8 number_of_cycle);
void delay_n100CY (U8 number_of_cycle);
void delay_n1KCY (U8 number_of_cycle);
void delay_n10KCY (U8 number_of_cycle);
void delay_n100KCY (U8 number_of_cycle);
void delay_n1MCY (U8 number_of_cycle);
void delay_us(void);
void delay_nUS(U8 number_of_us);
void delay_nMS(U8 number_of_ms);
void delay_nS(U8 number_of_s);

#endif
