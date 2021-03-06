/*!
 @file		pic32_interrupts.c
 @brief		Interrupt lib for pic32

 @version	0.3.0
 @note		Use the name in the Interrupts Source List to access an interrupt with any macro
		Use the Vector number in the _ISR Macro wherever you want
		Use the IRQ only with the run-time functions
 @todo		Test usefulness and exactness of IRQ number for group selection (only pic32mx1xx and 2xx for now)

 @date		September 15th 2012
 @author	Laurence DV
*/


// ################## Includes ################## //
#include "pic32_interrupt.h"
// ############################################## //


// ################### Define ################### //
#define INT_IRQ_PER_IPC_REG		4
#define INT_IRQ_PER_IEC_REG		32
// ############################################## //


// ################# Variables ################## //
// ############################################## //


// ############# Interrupts Functions ########### //
/**
* \fn		void _intSetReg(U32 * regPtr, tIntIRQ intIRQSource, U8 state)
* @brief	Write access to a interrupt register
* @note		INTERNAL FUNCTION Do not use directly!!!
* @arg		U32 * regPtr			Pointer to the register being written
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 state			State to set the bit
* @return	nothing
*/
void _intSetReg(volatile U32 * regPtr, tIntIRQ intIRQSource, U8 state)
{
	U8 maskTemp = BIT0;				//Default mask for only 1 bit
	
	// -- Intercept the Group flag -- //
	if (intIRQSource & BIT7)
	{
		maskTemp = BIT1|BIT0;			//Group at at least 2 bit wide
		if (intIRQSource > 22)
			maskTemp |= BIT2;		//Higher group are 3 bit wide
		intIRQSource &= 0x7F;			//Remove the flag
	}
	// ------------------------------ //

	// -- Point the correct IEC Reg -- //
	while (intIRQSource > (INT_IRQ_PER_IEC_REG-1))
	{
		regPtr += REG_OFFSET_NEXT_32;		//Point to the next reg
		intIRQSource -= INT_IRQ_PER_IEC_REG;	//Substract the IRQ skipped
	}
	// ------------------------------- //

	// -- Set the state -- //
	state &= maskTemp;				//Leave only the first bit
	*regPtr |= state << intIRQSource;
	// ------------------- //
}

/**
* \fn		U8 _intGetReg(U32 * regPtr, tIntIRQ intIRQSource)
* @brief	Read access to a interrupt register
* @note		INTERNAL FUNCTION Do not use directly!!!
* @arg		U32 * regPtr			Pointer to the register being written
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @return	U8 state			State to set the bit
*/
U8 _intGetReg(volatile U32 * regPtr, tIntIRQ intIRQSource)
{
	U8 maskTemp = BIT0;				//Default mask for only 1 bit

	// -- Intercept the Group flag -- //
	if (intIRQSource & BIT7)
	{
		maskTemp = BIT1|BIT0;			//Group at at least 2 bit wide
		if (intIRQSource > 22)
			maskTemp |= BIT2;		//Higher group are 3 bit wide
		intIRQSource &= 0x7F;			//Remove the flag
	}
	// ------------------------------ //

	// -- Point the correct IFS Reg -- //
	while (intIRQSource > (INT_IRQ_PER_IEC_REG-1))
	{
		regPtr += REG_OFFSET_NEXT_32;		//Point to the next reg
		intIRQSource -= INT_IRQ_PER_IEC_REG;	//Substract the IRQ skipped
	}
	// ------------------------------- //

	return ((*regPtr & (maskTemp << intIRQSource)) >> intIRQSource);
}

/**
* \fn		void intSetPriority(tIntIRQ intIRQSource, U8 priorityLvl, U8 subPriorityLvl)
* @brief	Set the priority and the sub-priority of an interrupt
* @note		If you use IRQ Group you will only set the priority for the first IRQ (ex: UART_1 would be UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 priorityLvl			Level of priority for the interrupt
* @arg		U8 subPriorityLvl		Level of sub-priority for the interrupt
* @return	nothing
*/
void intSetPriority(tIntIRQ intIRQSource, U8 priorityLvl, U8 subPriorityLvl)
{
	subPriorityLvl &= 0x3;				//Leave only the 2 LSb
	priorityLvl &= 0x7;				//Leave only the 3 LSb

	// -- Matrix -- //


	// ------------ //

	switch (intIRQSource)
	{
		case IRQ_CORE_TIMER:		_IPC_INT_CORE_TIMER = priorityLvl;	_SIPC_INT_CORE_TIMER = subPriorityLvl;		break;
		case IRQ_CORE_SOFT_0:		_IPC_INT_CORE_SOFT_0 = priorityLvl;	_SIPC_INT_CORE_SOFT_0 = subPriorityLvl;		break;
		case IRQ_CORE_SOFT_1:		_IPC_INT_CORE_SOFT_1 = priorityLvl;	_SIPC_INT_CORE_SOFT_1 = subPriorityLvl;		break;
		case IRQ_EXT_INT_0:		_IPC_INT_EXT_INT_0 = priorityLvl;	_SIPC_INT_EXT_INT_0 = subPriorityLvl;		break;
		case IRQ_TIMER_1:		_IPC_INT_TIMER_1 = priorityLvl;		_SIPC_INT_TIMER_1 = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_1:	_IPC_INT_INPUT_CAPTURE_1 = priorityLvl;	_SIPC_INT_INPUT_CAPTURE_1 = subPriorityLvl;	break;
		case IRQ_OUTPUT_COMPARE_1:	_IPC_INT_OUTPUT_COMPARE_1 = priorityLvl;_SIPC_INT_OUTPUT_COMPARE_1 = subPriorityLvl;	break;
		case IRQ_EXT_INT_1:		_IPC_INT_EXT_INT_1 = priorityLvl;	_SIPC_INT_EXT_INT_1 = subPriorityLvl;		break;
		case IRQ_TIMER_2:		_IPC_INT_TIMER_2 = priorityLvl;		_SIPC_INT_TIMER_2 = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_2:	_IPC_INT_INPUT_CAPTURE_2 = priorityLvl;	_SIPC_INT_INPUT_CAPTURE_2 = subPriorityLvl;	break;
		case IRQ_OUTPUT_COMPARE_2:	_IPC_INT_OUTPUT_COMPARE_2 = priorityLvl;_SIPC_INT_OUTPUT_COMPARE_2 = subPriorityLvl;	break;
		case IRQ_EXT_INT_2:		_IPC_INT_EXT_INT_2 = priorityLvl;	_SIPC_INT_EXT_INT_2 = subPriorityLvl;		break;		
		case IRQ_TIMER_3:		_IPC_INT_TIMER_3 = priorityLvl;		_SIPC_INT_TIMER_3 = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_3:	_IPC_INT_INPUT_CAPTURE_3 = priorityLvl;	_SIPC_INT_INPUT_CAPTURE_3 = subPriorityLvl;	break;
		case IRQ_OUTPUT_COMPARE_3:	_IPC_INT_OUTPUT_COMPARE_3 = priorityLvl;_SIPC_INT_OUTPUT_COMPARE_3 = subPriorityLvl;	break;
		case IRQ_EXT_INT_3:		_IPC_INT_EXT_INT_3 = priorityLvl;	_SIPC_INT_EXT_INT_3 = subPriorityLvl;		break;
		case IRQ_TIMER_4:		_IPC_INT_TIMER_4 = priorityLvl;		_SIPC_INT_TIMER_4 = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_4:	_IPC_INT_INPUT_CAPTURE_4 = priorityLvl;	_SIPC_INT_INPUT_CAPTURE_4 = subPriorityLvl;	break;
		case IRQ_OUTPUT_COMPARE_4:	_IPC_INT_OUTPUT_COMPARE_4 = priorityLvl;_SIPC_INT_OUTPUT_COMPARE_4 = subPriorityLvl;	break;
		case IRQ_EXT_INT_4:		_IPC_INT_EXT_INT_4 = priorityLvl;	_SIPC_INT_EXT_INT_4 = subPriorityLvl;		break;
		case IRQ_TIMER_5:		_IPC_INT_TIMER_5 = priorityLvl;		_SIPC_INT_TIMER_5 = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_5:	_IPC_INT_INPUT_CAPTURE_5 = priorityLvl;	_SIPC_INT_INPUT_CAPTURE_5 = subPriorityLvl;	break;
		case IRQ_OUTPUT_COMPARE_5:	_IPC_INT_OUTPUT_COMPARE_5 = priorityLvl;_SIPC_INT_OUTPUT_COMPARE_5 = subPriorityLvl;	break;
		
		case IRQ_SPI_1:			_IPC_INT_SPI_1 = priorityLvl;		_SIPC_INT_SPI_1 = subPriorityLvl;		break;
		case IRQ_SPI_2:			_IPC_INT_SPI_2 = priorityLvl;		_SIPC_INT_SPI_2 = subPriorityLvl;		break;
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx		
//		case IRQ_SPI_3:			_IPC_INT_SPI_3 = priorityLvl;		_SIPC_INT_SPI_3 = subPriorityLvl;		break;
//		case IRQ_SPI_4:			_IPC_INT_SPI_4 = priorityLvl;		_SIPC_INT_SPI_4 = subPriorityLvl;		break;
#endif		
		case IRQ_UART_1:		_IPC_INT_UART_1 = priorityLvl;		_SIPC_INT_UART_1 = subPriorityLvl;		break;		
		case IRQ_UART_2:		_IPC_INT_UART_2 = priorityLvl;		_SIPC_INT_UART_2 = subPriorityLvl;		break;
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx		
//		case IRQ_UART_3:		_IPC_INT_UART_3 = priorityLvl;		_SIPC_INT_UART_3 = subPriorityLvl;		break;
		case IRQ_UART_4:		_IPC_INT_UART_4 = priorityLvl;		_SIPC_INT_UART_4 = subPriorityLvl;		break;		
		case IRQ_UART_5:		_IPC_INT_UART_5 = priorityLvl;		_SIPC_INT_UART_5 = subPriorityLvl;		break;		
		case IRQ_UART_6:		_IPC_INT_UART_6 = priorityLvl;		_SIPC_INT_UART_6 = subPriorityLvl;		break;
#endif		
		case IRQ_I2C_1:			_IPC_INT_I2C_1 = priorityLvl;		_SIPC_INT_I2C_1 = subPriorityLvl;		break;		
		case IRQ_I2C_2:			_IPC_INT_I2C_2 = priorityLvl;		_SIPC_INT_I2C_2 = subPriorityLvl;		break;
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
//		case IRQ_I2C_3:			_IPC_INT_I2C_3 = priorityLvl;		_SIPC_INT_I2C_3 = subPriorityLvl;		break;
//		case IRQ_I2C_4:			_IPC_INT_I2C_4 = priorityLvl;		_SIPC_INT_I2C_4 = subPriorityLvl;		break;
//		case IRQ_I2C_5:			_IPC_INT_I2C_5 = priorityLvl;		_SIPC_INT_I2C_5 = subPriorityLvl;		break;
#endif
		case IRQ_INPUT_CHANGE:		_IPC_INT_INPUT_CHANGE = priorityLvl;	_SIPC_INT_INPUT_CHANGE = subPriorityLvl;	break;
		case IRQ_ADC_1:			_IPC_INT_ADC_1 = priorityLvl;		_SIPC_INT_ADC_1 = subPriorityLvl;		break;
		case IRQ_PMP:			_IPC_INT_PMP = priorityLvl;		_SIPC_INT_PMP = subPriorityLvl;			break;
		case IRQ_PMP_ERR:		_IPC_INT_PMP_ERR = priorityLvl;		_SIPC_INT_PMP_ERR = subPriorityLvl;		break;
		case IRQ_COMPARATOR_1:		_IPC_INT_COMPARATOR_1 = priorityLvl;	_SIPC_INT_COMPARATOR_1 = subPriorityLvl;	break;
		case IRQ_COMPARATOR_2:		_IPC_INT_COMPARATOR_2 = priorityLvl;	_SIPC_INT_COMPARATOR_2 = subPriorityLvl;	break;
		case IRQ_FAIL_SAFE_CLOCK_MON:	_IPC_INT_FAIL_SAFE_CLOCK_MON = priorityLvl;_SIPC_INT_FAIL_SAFE_CLOCK_MON = subPriorityLvl;break;
		case IRQ_RTCC:			_IPC_INT_RTCC = priorityLvl;		_SIPC_INT_RTCC = subPriorityLvl;		break;
		case IRQ_DMA_0:			_IPC_INT_DMA_0 = priorityLvl;		_SIPC_INT_DMA_0 = subPriorityLvl;		break;
		case IRQ_DMA_1:			_IPC_INT_DMA_1 = priorityLvl;		_SIPC_INT_DMA_1 = subPriorityLvl;		break;
		case IRQ_DMA_2:			_IPC_INT_DMA_2 = priorityLvl;		_SIPC_INT_DMA_2 = subPriorityLvl;		break;
		case IRQ_DMA_3:			_IPC_INT_DMA_3 = priorityLvl;		_SIPC_INT_DMA_3 = subPriorityLvl;		break;
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case IRQ_DMA_4:			_IPC_INT_DMA_4 = priorityLvl;		_SIPC_INT_DMA_4 = subPriorityLvl;		break;
		case IRQ_DMA_5:			_IPC_INT_DMA_5 = priorityLvl;		_SIPC_INT_DMA_5 = subPriorityLvl;		break;
		case IRQ_DMA_6:			_IPC_INT_DMA_6 = priorityLvl;		_SIPC_INT_DMA_6 = subPriorityLvl;		break;
		case IRQ_DMA_7:			_IPC_INT_DMA_7 = priorityLvl;		_SIPC_INT_DMA_7 = subPriorityLvl;		break;
#endif
		case IRQ_FLASH_CONTROL_EVENT:	_IPC_INT_FLASH_CONTROL_EVENT = priorityLvl;_SIPC_INT_FLASH_CONTROL_EVENT = subPriorityLvl;break;
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case IRQ_USB:			_IPC_INT_USB = priorityLvl;		_SIPC_INT_USB = subPriorityLvl;			break;
		case IRQ_CAN_1:			_IPC_INT_CAN_1 = priorityLvl;		_SIPC_INT_CAN_1 = subPriorityLvl;		break;
#endif
//		case IRQ_CAN_2:			_IPC_INT_CAN_2 = priorityLvl;		_SIPC_INT_CAN_2 = subPriorityLvl;		break;
//		case IRQ_ETHERNET:		_IPC_INT_ETHERNET = priorityLvl;	_SIPC_INT_ETHERNET = subPriorityLvl;		break;
		case IRQ_INPUT_CAPTURE_1_ERR:	_IPC_INT_INPUT_CAPTURE_1_ERR = priorityLvl;_SIPC_INT_INPUT_CAPTURE_1_ERR = subPriorityLvl;break;
		case IRQ_INPUT_CAPTURE_2_ERR:	_IPC_INT_INPUT_CAPTURE_2_ERR = priorityLvl;_SIPC_INT_INPUT_CAPTURE_2_ERR = subPriorityLvl;break;
		case IRQ_INPUT_CAPTURE_3_ERR:	_IPC_INT_INPUT_CAPTURE_3_ERR = priorityLvl;_SIPC_INT_INPUT_CAPTURE_3_ERR = subPriorityLvl;break;
		case IRQ_INPUT_CAPTURE_4_ERR:	_IPC_INT_INPUT_CAPTURE_4_ERR = priorityLvl;_SIPC_INT_INPUT_CAPTURE_4_ERR = subPriorityLvl;break;
		case IRQ_INPUT_CAPTURE_5_ERR:	_IPC_INT_INPUT_CAPTURE_5_ERR = priorityLvl;_SIPC_INT_INPUT_CAPTURE_5_ERR = subPriorityLvl;break;
		default: return;
	}

}

/**
* \fn		U8 intGetPriority(tIntIRQ intIRQSource)
* @brief	Return the priority level of an interrupt
* @note		If you use IRQ Group you will only get the priority of the first IRQ (ex: UART_1 would give UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to get
* @return	U8 priorityLvl			Level of priority of the interrupt
*/
U8 intGetPriority(tIntIRQ intIRQSource)
{
	U32 * regPtr = (U32*)&IPC0;
	U8 returnPriority;

	// -- Point the correct IPC Reg -- //
	regPtr += REG_OFFSET_NEXT_32 * (((U8)intIRQSource)/INT_IRQ_PER_IPC_REG);
	intIRQSource %= INT_IRQ_PER_IPC_REG;		//Adjust to the current pointer reg
	// ------------------------------- //

	// -- Format the result -- //
	returnPriority = (*regPtr >> (intIRQSource << 3)) & (BIT4|BIT3|BIT2) ;	//Extract, mask and align the priority
	returnPriority >>= 2;				//Realign the priority
	// ----------------------- //

	return returnPriority;
}

/**
* \fn		U8 intGetSubPriority(tIntIRQ intIRQSource)
* @brief	Return the sub-priority level of an interrupt
* @note		If you use IRQ Group you will only get the subPriority of the first IRQ (ex: UART_1 would give UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to get
* @return	U8 subPriorityLvl		Level of sub-priority of the interrupt
*/
U8 intGetSubPriority(tIntIRQ intIRQSource)
{
	U32 * regPtr = (U32*)&IPC0;

	// -- Point the correct IPC Reg -- //
	regPtr += REG_OFFSET_NEXT_32 * (((U8)intIRQSource)/INT_IRQ_PER_IPC_REG);
	intIRQSource %= INT_IRQ_PER_IPC_REG;		//Adjust to the current pointer reg
	// ------------------------------- //

	return (*regPtr >> (intIRQSource << 3)) & (BIT1|BIT0);	//Extract, mask and align the subPriority
}

/**
* \fn		void intSetExternalEdge(U8 intSource, U8 edgeDirection)
* @brief	Set the specified external interrupt source to trigger on a specific edge transition
* @note		Use the tIntIRQ type to select the correct interrupt source (only IRQ_EXT_INT are valid)
* @arg		tIntIRQ intIRQSource		The external interrupt to configure
* @arg		U8 edgeDirection		The edge transition to select
* @return	nothing
*/
void intSetExternalEdge(tIntIRQ intIRQSource, U8 edgeDirection)
{
	switch (intIRQSource)
	{
		case IRQ_EXT_INT_0:	INTCONbits.INT0EP = edgeDirection;	break;
		case IRQ_EXT_INT_1:	INTCONbits.INT1EP = edgeDirection;	break;
		case IRQ_EXT_INT_2:	INTCONbits.INT2EP = edgeDirection;	break;
		case IRQ_EXT_INT_3:	INTCONbits.INT3EP = edgeDirection;	break;
		case IRQ_EXT_INT_4:	INTCONbits.INT4EP = edgeDirection;	break;
		default: break;
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
	tCP0Error exceptionCause;
	U32 exceptionAddress;

	exceptionCause = (_CP0_GET_CAUSE() & 0x7C) >> 2;
	exceptionAddress = _CP0_GET_EPC();

	cpuSoftReset();					//Reset!
}
// ############################################## //
