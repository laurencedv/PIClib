
/*!
 @file		pic18_delay.c
 @brief		Delay functions for C18
 @version	0.2
 @date		February 9th 2011
 @author	Laurence DV
*/

// ================= Includes ================= //
#include "pic18_delay.h"

// ================= Delay Functions ================= //

// Delay for n number of Cycle (max 255)
void delay_nCY(U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
       _asm NOP _endasm
    }
}

//Delay for n x10 number of Cycle (max 2 550)
void delay_n10CY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_nCY(10);
    }
}

//Delay for n x100 number of Cycle (max 25 500)
void delay_n100CY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_n10CY(10);
    }
}

//Delay for n x1000 number of Cycle (max 255 000)
void delay_n1KCY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_n100CY(10);
    }
}

//Delay for n x10000 number of Cycle (max 2 550 000)
void delay_n10KCY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_n1KCY(10);
    }
}

//Delay for n x100000 number of Cycle (max 25 500 000)
void delay_n100KCY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_n10KCY(10);
    }
}

//Delay for n x1000000 number of Cycle (max 255 000 000)
void delay_n1MCY (U8 number_of_cycle)
{
    U8 i;
    for (i=0 ; i<number_of_cycle ; i++)
    {
        delay_n100KCY(10);
    }
}

//Delay of 1us (in assembly)
void delay_us(void)
{

    #if CPU_MIPS == 12
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 11
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 10
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 9
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 8
    Nop();
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 7
    Nop();
    Nop();
    Nop();
    #elif CPU_MIPS == 6
    Nop();
    Nop();
    #elif CPU_MIPS == 5
    Nop();
    #elif CPU_MIPS == 4
    Nop();
    #elif CPU_MIPS == 3
    Nop();
    #elif CPU_MIPS == 2
    Nop();
    #elif CPU_MIPS == 1
    Nop();
    #endif
}

//Delay of 1us corrected to be called and looped (in assembly)
void delay_us_loopable(void)
{

}

//Delay for n number of us (max 255)
void delay_nUS(U8 number_of_us)
{
    U8 i;
    for (i=0 ; i<number_of_us ; i++)                  //Loop for the number of microseconds
    {
        delay_us();
    }
}

//Delay for n number of ms (max 255)
void delay_nMS(U8 number_of_ms)
{
	U8 i;
	for (i=0 ; i<number_of_ms ; i++)                  //Loop for the number of milliseconds
	{
		delay_nUS(250);
		delay_nUS(250);
		delay_nUS(250);
		delay_nUS(250);
    }
}

//Delay for n number of s (max 60)
void delay_nS(U8 number_of_s)
{
	U8 i;
	for (i=0 ; i < number_of_s ; i++)               //Loop for the number of seconds
	{
        delay_nMS(250);				    //delay for 250 ms 4 times
		delay_nMS(250);
		delay_nMS(250);
		delay_nMS(250);
	}
}
