/*!
 @file		pic32_interrupts.c
 @brief		Interrupt lib for pic32

 @version	0.2.1
 @note		Use the name in the Interrupts Source List to access an interrupt with any macro
			Use the Vector number in the _ISR Macro wherever you want
			Use the IRQ only with the run-time functions (not done yet)
 @todo		Run-time functions
			Init functions
			Test usefulness and exactness of IRQ number for group selection (only pic32mx1xx and 2xx for now)

 @date		March 7th 2012
 @author	Jonathan LL && Laurence DV
*/


// ################## Includes ################## //
#include "pic32_interrupt.h"
// ############################################## //


// ################# Variables ################## //
// ############################################## //


// ############# Interrupts Functions ########### //
/**
* \fn		void intSetExternalEdge(U8 intSource, U8 edgeDirection)
* @brief	Set the specified external interrupt source to trigger on a specified edge direction
* @note		Use the tIntIRQ type to select the correct interrupt source
* @arg		tIntIRQ intIRQSource	The external interrupt to configure
* @arg		U8 edgeDirection		The edge direction to select
* @return	nothing
*/
void intSetExternalEdge(tIntIRQ intIRQSource, U8 edgeDirection)
{
	switch (intIRQSource)
	{
		case INT_IRQ_EXT_INT_0:	INTCONbits.INT0EP = edgeDirection;	break;
		case INT_IRQ_EXT_INT_1:	INTCONbits.INT1EP = edgeDirection;	break;
		case INT_IRQ_EXT_INT_2:	INTCONbits.INT2EP = edgeDirection;	break;
		case INT_IRQ_EXT_INT_3:	INTCONbits.INT3EP = edgeDirection;	break;
		case INT_IRQ_EXT_INT_4:	INTCONbits.INT4EP = edgeDirection;	break;
	}
}
// ############################################## //


// ############## Exception handler ############# //
/**
* \fn		void _general_exception_handler (U32 cause, U32 status)
* @brief	Exception handler for the M4K core
* @note		The processor branch here when an exception other than interrupt or bootstrap code
*			is generated
* @return	nothing
*/
void _general_exception_handler (void)
{
	U8 exceptionCause;
	U32 exceptionAddress;

	exceptionCause = (_CP0_GET_CAUSE() & 0x7C) >> 2;
	exceptionAddress = _CP0_GET_ERROREPC();

	for (;;);
}
// ############################################## //
