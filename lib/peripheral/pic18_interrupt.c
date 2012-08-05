/*!
 @file pic18_interrupt.c
 @brief		Interrupt functions and Pragma code for C18
 *			Use "INT_NAME_FLAG" to check if the interrupt is valid, use "int_name_clr" to clear the flag after processing an interrupt
 *			Use "int_name_en/dis" to temporarly disable an interrupt, use "int_name_init" to enable it the first time
 * 			Create a "interrupt_high_handler" and "interrupt_low_handler" functions in your code for interrupt
 @version 0.1
 @date April 2th 2011
 @author Laurence DV
*/

// ================= Includes ================= //
#include "pic18_interrupt.h"



// ================= Interrupt Functions ================= //



