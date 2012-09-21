/*!
 @file		pic32_interrupts.h
 @brief		Interrupt lib for pic32

 @version	0.3.0
 @note		Use the name in the Interrupts Source List to access an interrupt with any macro
		Use the Vector number in the _ISR Macro wherever you want
		Use the IRQ only with the run-time functions
 @todo		Test usefulness and exactness of IRQ number for group selection (only pic32mx1xx and 2xx for now)

 @date		September 15th 2012
 @author	Laurence DV
*/


// ########### Interrupts Sources List ########## //
// You must add "INT_" before any of those name (ex: INT_CORE_TIMER)
// Use those to specified which interrupt you want as $intSource in any macro

// -- Individual interrupt -- //
// Core : CORE_TIMER | CORE_SOFT_0 | CORE_SOFT_1
// External Interrupt : EXT_INT_0 | EXT_INT_1 | EXT_INT_2 | EXT_INT_3 | EXT_INT_4
// Input Capture : INPUT_CAPTURE_1 | INPUT_CAPTURE_1_ERR | INPUT_CAPTURE_2 | INPUT_CAPTURE_2_ERR | INPUT_CAPTURE_3 | INPUT_CAPTURE_3_ERR | INPUT_CAPTURE_4 | INPUT_CAPTURE_4_ERR | INPUT_CAPTURE_5 | INPUT_CAPTURE_5_ERR
// Output Compare : OUTPUT_COMPARE_1 | OUTPUT_COMPARE_2 | OUTPUT_COMPARE_3 | OUTPUT_COMPARE_4 | OUTPUT_COMPARE_5
// Timer : TIMER_1 | TIMER_2 | TIMER_3 | TIMER_4 | TIMER_5
// Input Change : INPUT_CHANGE | INPUT_CHANGE_PORT_A | INPUT_CHANGE_PORT_B | INPUT_CHANGE_PORT_C
// ADC : ADC_1
// Fail Safe Clock Monitor : FAIL_SAFE_CLOCK_MON
// Real-time Clock and Calendar : RTCC
// Flash Control Event : FLASH_CONTROL_EVENT
// Comparator : COMPARATOR_1 | COMPARATOR_2 | COMPARATOR_3
// USB : USB
// PMP : PMP | PMP_ERR
// UART : UART_1_ERR | UART_1_RX | UART_1_TX | UART_2_ERR | UART_2_RX | UART_2_TX | UART_3_ERR | UART_3_RX | UART_3_TX | UART_4_ERR | UART_4_RX | UART_4_TX | UART_5_ERR | UART_5_RX | UART_5_TX | UART_6_ERR | UART_6_RX | UART_6_TX
// SPI : SPI_1_ERR | SPI_1_RX | SPI_1_TX | SPI_2_ERR | SPI_2_RX | SPI_2_TX | SPI_3_ERR | SPI_3_RX | SPI_3_TX | SPI_4_ERR | SPI_4_RX | SPI_4_TX
// I2C : I2C_1_COL | I2C_1_SLAVE | I2C_1_MASTER | I2C_2_COL | I2C_2_SLAVE | I2C_2_MASTER | I2C_3_COL | I2C_3_SLAVE | I2C_3_MASTER | I2C_4_COL | I2C_4_SLAVE | I2C_4_MASTER | I2C_5_COL | I2C_5_SLAVE | I2C_5_MASTER
// Ethernet : ETHERNET
// CAN : CAN_1 | CAN_2
// DMA : DMA_0 | DMA_1 | DMA_2 | DMA_3 | DMA_4 | DMA_5 | DMA_6 | DMA_7
// -------------------------- //

//Those special group access control all the relevant interrupt of a peripheral (ex: UART_1 contain UART_1_ERR and UART_1_RX and UART_1_TX)
// -- Group interrupt -- //
// Timer : TIMER_23 | TIMER_45
// Input Change : INPUT_CHANGE
// UART : UART_1 | UART_2 | UART_3 | UART_4 | UART_5 | UART_6
// SPI : SPI_1 | SPI_2 | SPI_3 | SPI_4
// I2C : I2C_1 | I2C_2 | I2C_3 | I2C_4 | I2C_5
// --------------------- //
// ############################################## //

#ifndef _PIC32_INTERRUPTS_H
#define _PIC32_INTERRUPTS_H 1

// ################## Includes ################## //
#include <hardware.h>

// Lib
#include <peripheral/pic32_cpu.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

//Microchip Lib
#include <cp0defs.h>
#include <peripheral/int.h>
// ############################################## //


// ################### Define ################### //
// -- Vector Number -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
	#define INT_VEC_CORE_TIMER			0
	#define INT_VEC_CORE_SOFT_0			1
	#define INT_VEC_CORE_SOFT_1			2
	#define INT_VEC_EXT_INT_0			3
	#define INT_VEC_TIMER_1				4
	#define INT_VEC_INPUT_CAPTURE_1_ERR		5
	#define INT_VEC_INPUT_CAPTURE_1			5
	#define INT_VEC_OUTPUT_COMPARE_1		6
	#define INT_VEC_EXT_INT_1			7
	#define INT_VEC_TIMER_2				8
	#define INT_VEC_INPUT_CAPTURE_2_ERR		9
	#define INT_VEC_INPUT_CAPTURE_2			9
	#define INT_VEC_OUTPUT_COMPARE_2		10
	#define INT_VEC_EXT_INT_2			11
	#define INT_VEC_TIMER_3				12
	#define INT_VEC_INPUT_CAPTURE_3_ERR		13
	#define INT_VEC_INPUT_CAPTURE_3			13
	#define INT_VEC_OUTPUT_COMPARE_3		14
	#define INT_VEC_EXT_INT_3			15
	#define INT_VEC_TIMER_4				16
	#define INT_VEC_INPUT_CAPTURE_4_ERR		17
	#define INT_VEC_INPUT_CAPTURE_4			17
	#define INT_VEC_OUTPUT_COMPARE_4		18
	#define INT_VEC_EXT_INT_4			19
	#define INT_VEC_TIMER_5				20
	#define INT_VEC_INPUT_CAPTURE_5_ERR		21
	#define INT_VEC_INPUT_CAPTURE_5			21
	#define INT_VEC_OUTPUT_COMPARE_5		22
	#define INT_VEC_ADC_1				23
	#define INT_VEC_FAIL_SAFE_CLOCK_MON		24
	#define INT_VEC_RTCC				25
	#define INT_VEC_FLASH_CONTROL_EVENT		26
	#define INT_VEC_COMPARATOR_1			27
	#define INT_VEC_COMPARATOR_2			28
	#define INT_VEC_COMPARATOR_3			29
	#define INT_VEC_USB				30
	#define INT_VEC_SPI_1				31
	#define INT_VEC_SPI_1_ERR			31
	#define INT_VEC_SPI_1_RX			31
	#define INT_VEC_SPI_1_TX			31
	#define INT_VEC_UART_1				32
	#define INT_VEC_UART_1_ERR			32
	#define INT_VEC_UART_1_RX			32
	#define INT_VEC_UART_1_TX			32
	#define INT_VEC_I2C_1				33
	#define INT_VEC_I2C_1_COL			33
	#define INT_VEC_I2C_1_SLAVE			33
	#define INT_VEC_I2C_1_MASTER			33
	#define INT_VEC_INPUT_CHANGE			34
	#define INT_VEC_INPUT_CHANGE_PORT_A		34
	#define INT_VEC_INPUT_CHANGE_PORT_B		34
	#define INT_VEC_INPUT_CHANGE_PORT_C		34
	#define INT_VEC_PMP				35
	#define INT_VEC_PMP_ERR				35
	#define INT_VEC_SPI_2				36
	#define INT_VEC_SPI_2_ERR			36
	#define INT_VEC_SPI_2_RX			36
	#define INT_VEC_SPI_2_TX			36
	#define INT_VEC_UART_2				37
	#define INT_VEC_UART_2_ERR			37
	#define INT_VEC_UART_2_RX			37
	#define INT_VEC_UART_2_TX			37
	#define INT_VEC_I2C_2				38
	#define INT_VEC_I2C_2_COL			38
	#define INT_VEC_I2C_2_SLAVE			38
	#define INT_VEC_I2C_2_MASTER			38
	#define INT_VEC_CTMU				39
	#define INT_VEC_DMA_0				40
	#define INT_VEC_DMA_1				41
	#define INT_VEC_DMA_2				42
	#define INT_VEC_DMA_3				43
#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
	#define INT_VEC_CORE_TIMER			0
	#define INT_VEC_CORE_SOFT_0			1
	#define INT_VEC_CORE_SOFT_1			2
	#define INT_VEC_EXT_INT_0			3
	#define INT_VEC_TIMER_1				4
	#define INT_VEC_INPUT_CAPTURE_1			5
	#define INT_VEC_OUTPUT_COMPARE_1		6
	#define INT_VEC_EXT_INT_1			7
	#define INT_VEC_TIMER_2				8
	#define INT_VEC_INPUT_CAPTURE_2			9
	#define INT_VEC_OUTPUT_COMPARE_2		10
	#define INT_VEC_EXT_INT_2			11
	#define INT_VEC_TIMER_3				12
	#define INT_VEC_INPUT_CAPTURE_3			13
	#define INT_VEC_OUTPUT_COMPARE_3		14
	#define INT_VEC_EXT_INT_3			15
	#define INT_VEC_TIMER_4				16
	#define INT_VEC_INPUT_CAPTURE_4			17
	#define INT_VEC_OUTPUT_COMPARE_4		18
	#define INT_VEC_EXT_INT_4			19
	#define INT_VEC_TIMER_5				20
	#define INT_VEC_INPUT_CAPTURE_5			21
	#define INT_VEC_OUTPUT_COMPARE_5		22
	#define INT_VEC_SPI_1				23
	#define INT_VEC_SPI_1_ERR			23
	#define INT_VEC_SPI_1_RX			23
	#define INT_VEC_SPI_1_TX			23
	#define INT_VEC_UART_1				24
	#define INT_VEC_UART_1_ERR			24
	#define INT_VEC_UART_1_RX			24
	#define INT_VEC_UART_1_TX			24
	#define INT_VEC_I2C_1				25
	#define INT_VEC_I2C_1_COL			25
	#define INT_VEC_I2C_1_SLAVE			25
	#define INT_VEC_I2C_1_MASTER			25
	#define INT_VEC_INPUT_CHANGE			26
	#define INT_VEC_ADC_1				27
	#define INT_VEC_PMP				28
	#define INT_VEC_COMPARATOR_1			29
	#define INT_VEC_COMPARATOR_2			30
	#define INT_VEC_SPI_2				31
	#define INT_VEC_SPI_2_ERR			31
	#define INT_VEC_SPI_2_RX			31
	#define INT_VEC_SPI_2_TX			31
	#define INT_VEC_UART_2				32
	#define INT_VEC_UART_2_ERR			32
	#define INT_VEC_UART_2_RX			32
	#define INT_VEC_UART_2_TX			32
	#define INT_VEC_I2C_2				33
	#define INT_VEC_I2C_2_COL			33
	#define INT_VEC_I2C_2_SLAVE			33
	#define INT_VEC_I2C_2_MASTER			33
	#define INT_VEC_FAIL_SAFE_CLOCK_MON		34
	#define INT_VEC_RTCC				35
	#define INT_VEC_DMA_0				36
	#define INT_VEC_DMA_1				37
	#define INT_VEC_DMA_2				38
	#define INT_VEC_DMA_3				39
	#define INT_VEC_FLASH_CONTROL_EVENT		44
	#define INT_VEC_USB				45
#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
	#define INT_VEC_CORE_TIMER			0
	#define INT_VEC_CORE_SOFT_0			1
	#define INT_VEC_CORE_SOFT_1			2
	#define INT_VEC_EXT_INT_0			3
	#define INT_VEC_TIMER_1				4
	#define INT_VEC_INPUT_CAPTURE_1_ERR		5
	#define INT_VEC_INPUT_CAPTURE_1			5
	#define INT_VEC_OUTPUT_COMPARE_1		6
	#define INT_VEC_EXT_INT_1			7
	#define INT_VEC_TIMER_2				8
	#define INT_VEC_INPUT_CAPTURE_2_ERR		9
	#define INT_VEC_INPUT_CAPTURE_2			9
	#define INT_VEC_OUTPUT_COMPARE_2		10
	#define INT_VEC_EXT_INT_2			11
	#define INT_VEC_TIMER_3				12
	#define INT_VEC_INPUT_CAPTURE_3_ERR		13
	#define INT_VEC_INPUT_CAPTURE_3			13
	#define INT_VEC_OUTPUT_COMPARE_3		14
	#define INT_VEC_EXT_INT_3			15
	#define INT_VEC_TIMER_4				16
	#define INT_VEC_INPUT_CAPTURE_4_ERR		17
	#define INT_VEC_INPUT_CAPTURE_4			17
	#define INT_VEC_OUTPUT_COMPARE_4		18
	#define INT_VEC_EXT_INT_4			19
	#define INT_VEC_TIMER_5				20
	#define INT_VEC_INPUT_CAPTURE_5_ERR		21
	#define INT_VEC_INPUT_CAPTURE_5			21
	#define INT_VEC_OUTPUT_COMPARE_5		22
	#define INT_VEC_SPI_1				23
	#define INT_VEC_SPI_1_ERR			23
	#define INT_VEC_SPI_1_RX			23
	#define INT_VEC_SPI_1_TX			23
	#define INT_VEC_UART_1				24
	#define INT_VEC_UART_1_ERR			24
	#define INT_VEC_UART_1_RX			24
	#define INT_VEC_UART_1_TX			24
	#define INT_VEC_SPI_3				24
	#define INT_VEC_SPI_3_ERR			24
	#define INT_VEC_SPI_3_RX			24
	#define INT_VEC_SPI_3_TX			24
	#define INT_VEC_I2C_3				24
	#define INT_VEC_I2C_3_COL			24
	#define INT_VEC_I2C_3_SLAVE			24
	#define INT_VEC_I2C_3_MASTER			24
	#define INT_VEC_I2C_1				25
	#define INT_VEC_I2C_1_COL			25
	#define INT_VEC_I2C_1_SLAVE			25
	#define INT_VEC_I2C_1_MASTER			25
	#define INT_VEC_INPUT_CHANGE			26
	#define INT_VEC_ADC_1				27
	#define INT_VEC_PMP				28
	#define INT_VEC_PMP_ERR				28
	#define INT_VEC_COMPARATOR_1			29
	#define INT_VEC_COMPARATOR_2			30
	#define INT_VEC_UART_3				31
	#define INT_VEC_UART_3_ERR			31
	#define INT_VEC_UART_3_RX			31
	#define INT_VEC_UART_3_TX			31
	#define INT_VEC_SPI_2				31
	#define INT_VEC_SPI_2_ERR			31
	#define INT_VEC_SPI_2_RX			31
	#define INT_VEC_SPI_2_TX			31
	#define INT_VEC_I2C_4				31
	#define INT_VEC_I2C_4_COL			31
	#define INT_VEC_I2C_4_SLAVE			31
	#define INT_VEC_I2C_4_MASTER			31
	#define INT_VEC_UART_2				32
	#define INT_VEC_UART_2_ERR			32
	#define INT_VEC_UART_2_RX			32
	#define INT_VEC_UART_2_TX			32
	#define INT_VEC_SPI_4				32
	#define INT_VEC_SPI_4_ERR			32
	#define INT_VEC_SPI_4_RX			32
	#define INT_VEC_SPI_4_TX			32
	#define INT_VEC_I2C_5				32
	#define INT_VEC_I2C_5_COL			32
	#define INT_VEC_I2C_5_SLAVE			32
	#define INT_VEC_I2C_5_MASTER			32
	#define INT_VEC_I2C_2				33
	#define INT_VEC_I2C_2_COL			33
	#define INT_VEC_I2C_2_SLAVE			33
	#define INT_VEC_I2C_2_MASTER			33
	#define INT_VEC_FAIL_SAFE_CLOCK_MON		34
	#define INT_VEC_RTCC				35
	#define INT_VEC_DMA_0				36
	#define INT_VEC_DMA_1				37
	#define INT_VEC_DMA_2				38
	#define INT_VEC_DMA_3				39
	#define INT_VEC_DMA_4				40
	#define INT_VEC_DMA_5				41
	#define INT_VEC_DMA_6				42
	#define INT_VEC_DMA_7				43
	#define INT_VEC_FLASH_CONTROL_EVENT		44
	#define INT_VEC_USB				45
	#define INT_VEC_CAN_1				46
	#define INT_VEC_CAN_2				47
	#define INT_VEC_ETHERNET			48
	#define INT_VEC_UART_4				49
	#define INT_VEC_UART_4_ERR			49
	#define INT_VEC_UART_4_RX			49
	#define INT_VEC_UART_4_TX			49
	#define INT_VEC_UART_6				50
	#define INT_VEC_UART_6_ERR			50
	#define INT_VEC_UART_6_RX			50
	#define INT_VEC_UART_6_TX			50
	#define INT_VEC_UART_5				51
	#define INT_VEC_UART_5_ERR			51
	#define INT_VEC_UART_5_RX			51
	#define INT_VEC_UART_5_TX			51
#endif
// ------------------- //
// ############################################## //


// ################# Data Type ################## //
// CP0 Error Type
typedef enum
{
	CP0E_interrupt = 0,
	CP0E_addressLoadFetch = 4,
	CP0E_addressStore = 5,
	CP0E_busFetch = 6,
	CP0E_busLoadStore = 7,
	CP0E_syscall = 8,
	CP0E_breakPoint = 9,
	CP0E_reserveInstruction = 10,
	CP0E_coProcessorUnusable = 11,
	CP0E_arithmeticOverflow = 12,
	CP0E_trap = 13
}tCP0Error;

// -- IRQ Number -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
typedef enum
{
	IRQ_CORE_TIMER = 0,
	IRQ_CORE_SOFT_0 = 1,
	IRQ_CORE_SOFT_1 = 2,
	IRQ_EXT_INT_0 = 3,
	IRQ_TIMER_1 = 4,
	IRQ_INPUT_CAPTURE_1_ERR = 5,
	IRQ_INPUT_CAPTURE_1 = 6,
	IRQ_OUTPUT_COMPARE_1 = 7,
	IRQ_EXT_INT_1 = 8,
	IRQ_TIMER_2 = 9,
	IRQ_TIMER_23 = 0x89,
	IRQ_INPUT_CAPTURE_2_ERR = 10,
	IRQ_INPUT_CAPTURE_2 = 11,
	IRQ_OUTPUT_COMPARE_2 = 12,
	IRQ_EXT_INT_2 = 13,
	IRQ_TIMER_3 = 14,
	IRQ_INPUT_CAPTURE_3_ERR = 15,
	IRQ_INPUT_CAPTURE_3 = 16,
	IRQ_OUTPUT_COMPARE_3 = 17,
	IRQ_EXT_INT_3 = 18,
	IRQ_TIMER_4 = 19,
	IRQ_TIMER_45 = 0x93,
	IRQ_INPUT_CAPTURE_4_ERR = 20,
	IRQ_INPUT_CAPTURE_4 = 21,
	IRQ_OUTPUT_COMPARE_4 = 22,
	IRQ_EXT_INT_4 = 23,
	IRQ_TIMER_5 = 24,
	IRQ_INPUT_CAPTURE_5_ERR = 25,
	IRQ_INPUT_CAPTURE_5 = 26,
	IRQ_OUTPUT_COMPARE_5 = 27,
	IRQ_ADC_1 = 28,
	IRQ_FAIL_SAFE_CLOCK_MON = 29,
	IRQ_RTCC = 30,
	IRQ_FLASH_CONTROL_EVENT = 31,
	IRQ_COMPARATOR_1 = 32,
	IRQ_COMPARATOR_2 = 33,
	IRQ_COMPARATOR_3 = 34,
	IRQ_USB = 35,
	IRQ_SPI_1_ERR = 36,
	IRQ_SPI_1_RX = 37,
	IRQ_SPI_1_TX = 38,
	IRQ_SPI_1 = 0xA4,
	IRQ_UART_1_ERR = 39,
	IRQ_UART_1_RX = 40,
	IRQ_UART_1_TX = 41,
	IRQ_UART_1 = 0xA7,
	IRQ_I2C_1_COL = 42,
	IRQ_I2C_1_SLAVE = 43,
	IRQ_I2C_1_MASTER = 44,
	IRQ_I2C_1 = 0xAA,
	IRQ_INPUT_CHANGE_PORT_A = 45,
	IRQ_INPUT_CHANGE_PORT_B = 46,
	IRQ_INPUT_CHANGE_PORT_C = 47,
	IRQ_INPUT_CHANGE = 0xAD,
	IRQ_PMP = 48,
	IRQ_PMP_ERR = 49,
	IRQ_SPI_2_ERR = 50,
	IRQ_SPI_2_RX = 51,
	IRQ_SPI_2_TX = 52,
	IRQ_SPI_2 = 0xB2,
	IRQ_UART_2_ERR = 53,
	IRQ_UART_2_RX = 54,
	IRQ_UART_2_TX = 55,
	IRQ_UART_2 = 0xB5,
	IRQ_I2C_2_COL = 56,
	IRQ_I2C_2_SLAVE = 57,
	IRQ_I2C_2_MASTER = 58,
	IRQ_I2C_2 = 50xB8,
	IRQ_CTMU = 59,
	IRQ_DMA_0 = 60,
	IRQ_DMA_1 = 61,
	IRQ_DMA_2 = 62,
	IRQ_DMA_3 = 63
}tIntIRQ;
#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
typedef enum
{
	IRQ_CORE_TIMER = 0,
	IRQ_CORE_SOFT_0 = 1,
	IRQ_CORE_SOFT_1 = 2,
	IRQ_EXT_INT_0 = 3,
	IRQ_TIMER_1 = 4,
	IRQ_INPUT_CAPTURE_1 = 5,
	IRQ_OUTPUT_COMPARE_1 = 6,
	IRQ_EXT_INT_1 = 7,
	IRQ_TIMER_2 = 8,
	IRQ_TIMER_23 = 0x88,
	IRQ_INPUT_CAPTURE_2 = 9,
	IRQ_OUTPUT_COMPARE_2 = 10,
	IRQ_EXT_INT_2 = 11,
	IRQ_TIMER_3 = 12,
	IRQ_INPUT_CAPTURE_3 = 13,
	IRQ_OUTPUT_COMPARE_3 = 14,
	IRQ_EXT_INT_3 = 15,
	IRQ_TIMER_4 = 16,
	IRQ_TIMER_45 = 0x90,
	IRQ_INPUT_CAPTURE_4 = 17,
	IRQ_OUTPUT_COMPARE_4 = 18,
	IRQ_EXT_INT_4 = 19,
	IRQ_TIMER_5 = 20,
	IRQ_INPUT_CAPTURE_5 = 21,
	IRQ_OUTPUT_COMPARE_5 = 22,
	IRQ_SPI_1_ERR = 23,
	IRQ_SPI_1_RX = 24,
	IRQ_SPI_1_TX = 25,
	IRQ_SPI_1 = 0x97,
	IRQ_UART_1_ERR = 26,
	IRQ_UART_1_RX = 27,
	IRQ_UART_1_TX = 28,
	IRQ_UART_1 = 0x9A,
	IRQ_I2C_1_COL = 29,
	IRQ_I2C_1_SLAVE = 30,
	IRQ_I2C_1_MASTER = 31,
	IRQ_I2C_1 = 0x9D,
	IRQ_INPUT_CHANGE = 32,
	IRQ_ADC_1 = 33,
	IRQ_PMP = 34,
	IRQ_COMPARATOR_1 = 35,
	IRQ_COMPARATOR_2 = 36,
	IRQ_SPI_2_ERR = 37,
	IRQ_SPI_2_RX = 38,
	IRQ_SPI_2_TX = 39,
	IRQ_SPI_2 = 0xA5,
	IRQ_UART_2_ERR = 40,
	IRQ_UART_2_RX = 41,
	IRQ_UART_2_TX = 42,
	IRQ_UART_2 = 0xA8,
	IRQ_I2C_2_COL = 43,
	IRQ_I2C_2_SLAVE = 44,
	IRQ_I2C_2_MASTER = 45,
	IRQ_I2C_2 = 0xAB,
	IRQ_FAIL_SAFE_CLOCK_MON = 46,
	IRQ_RTCC = 47,
	IRQ_DMA_0 = 48,
	IRQ_DMA_1 = 49,
	IRQ_DMA_2 = 50,
	IRQ_DMA_3 = 51,
	IRQ_FLASH_CONTROL_EVENT = 56,
	IRQ_USB = 57
}tIntIRQ;
#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
typedef enum
{
	IRQ_CORE_TIMER = 0,
	IRQ_CORE_SOFT_0 = 1,
	IRQ_CORE_SOFT_1 = 2,
	IRQ_EXT_INT_0 = 3,
	IRQ_TIMER_1 = 4,
	IRQ_INPUT_CAPTURE_1 = 5,
	IRQ_OUTPUT_COMPARE_1 = 6,
	IRQ_EXT_INT_1 = 7,
	IRQ_TIMER_2 = 8,
	IRQ_TIMER_23 = 0x88,
	IRQ_INPUT_CAPTURE_2 = 9,
	IRQ_OUTPUT_COMPARE_2 = 10,
	IRQ_EXT_INT_2 = 11,
	IRQ_TIMER_3 = 12,
	IRQ_INPUT_CAPTURE_3 = 13,
	IRQ_OUTPUT_COMPARE_3 = 14,
	IRQ_EXT_INT_3 = 15,
	IRQ_TIMER_4 = 16,
	IRQ_TIMER_45 = 0x90,
	IRQ_INPUT_CAPTURE_4 = 17,
	IRQ_OUTPUT_COMPARE_4 = 18,
	IRQ_EXT_INT_4 = 19,
	IRQ_TIMER_5 = 20,
	IRQ_INPUT_CAPTURE_5 = 21,
	IRQ_OUTPUT_COMPARE_5 = 22,
	IRQ_SPI_1_ERR = 23,
	IRQ_SPI_1_RX = 24,
	IRQ_SPI_1_TX = 25,
	IRQ_SPI_1 = 0x97,
	IRQ_UART_1_ERR = 26,
	IRQ_UART_1_RX = 27,
	IRQ_UART_1_TX = 28,
	IRQ_UART_1 = 0x9A,
	IRQ_SPI_3_ERR = 26,
	IRQ_SPI_3_RX = 27,
	IRQ_SPI_3_TX = 28,
	IRQ_SPI_3 = 0x9A,
	IRQ_I2C_3_COL = 26,
	IRQ_I2C_3_SLAVE = 27,
	IRQ_I2C_3_MASTER = 28,
	IRQ_I2C_3 = 0x9A,
	IRQ_I2C_1_COL = 29,
	IRQ_I2C_1_SLAVE = 30,
	IRQ_I2C_1_MASTER = 31,
	IRQ_I2C_1 = 0x9D,
	IRQ_INPUT_CHANGE = 32,
	IRQ_ADC_1 = 33,
	IRQ_PMP = 34,
	IRQ_COMPARATOR_1 = 35,
	IRQ_COMPARATOR_2 = 36,
	IRQ_UART_3_ERR = 37,
	IRQ_UART_3_RX = 38,
	IRQ_UART_3_TX = 39,
	IRQ_UART_3 = 0xA5,
	IRQ_SPI_2_ERR = 37,
	IRQ_SPI_2_RX = 38,
	IRQ_SPI_2_TX = 39,
	IRQ_SPI_2 = 0xA5,
	IRQ_I2C_4_COL = 37,
	IRQ_I2C_4_SLAVE = 38,
	IRQ_I2C_4_MASTER = 39,
	IRQ_I2C_4 = 0xA5,
	IRQ_UART_2_ERR = 40,
	IRQ_UART_2_RX = 41,
	IRQ_UART_2_TX = 42,
	IRQ_UART_2 = 0xA8,
	IRQ_SPI_4_ERR = 40,
	IRQ_SPI_4_RX = 41,
	IRQ_SPI_4_TX = 42,
	IRQ_SPI_4 = 0xA8,
	IRQ_I2C_5_COL = 40,
	IRQ_I2C_5_SLAVE = 41,
	IRQ_I2C_5_MASTER = 42,
	IRQ_I2C_5 = 0xA8,
	IRQ_I2C_2_COL = 43,
	IRQ_I2C_2_SLAVE = 44,
	IRQ_I2C_2_MASTER = 45,
	IRQ_I2C_2 = 0xAB,
	IRQ_FAIL_SAFE_CLOCK_MON = 46,
	IRQ_RTCC = 47,
	IRQ_DMA_0 = 48,
	IRQ_DMA_1 = 49,
	IRQ_DMA_2 = 50,
	IRQ_DMA_3 = 51,
	IRQ_DMA_4 = 52,
	IRQ_DMA_5 = 53,
	IRQ_DMA_6 = 54,
	IRQ_DMA_7 = 55,
	IRQ_FLASH_CONTROL_EVENT = 56,
	IRQ_USB = 57,
	IRQ_CAN_1 = 58,
	IRQ_CAN_2 = 59,
	IRQ_ETHERNET = 60,
	IRQ_INPUT_CAPTURE_1_ERR = 61,
	IRQ_INPUT_CAPTURE_2_ERR = 62,
	IRQ_INPUT_CAPTURE_3_ERR = 63,
	IRQ_INPUT_CAPTURE_4_ERR = 64,
	IRQ_INPUT_CAPTURE_5_ERR = 65,
	IRQ_PMP_ERR = 66,
	IRQ_UART_4_ERR = 67,
	IRQ_UART_4_RX = 68,
	IRQ_UART_4_TX = 69,
	IRQ_UART_4 = 0xC3,
	IRQ_UART_6_ERR = 70,
	IRQ_UART_6_RX = 71,
	IRQ_UART_6_TX = 72,
	IRQ_UART_6 = 0xC6,
	IRQ_UART_5_ERR = 73,
	IRQ_UART_5_RX = 74,
	IRQ_UART_5_TX = 75,
	IRQ_UART_5 = 0xC9
}tIntIRQ;
#endif
// ---------------- //
// ############################################## //


// ################# Fast Macro ################# //
/**
* \fn		intFastEnableGlobal()
* @brief	Macro to enable the global interrupt Enable
* @note		This function also return the state of the Status CP0 Register
* @arg		nothing
* @return	U32 intGlobalState		State of the CP0 Status register before enabling
*/
#define	intFastEnableGlobal()				INTEnableInterrupts()

/**
* \fn		intFastDisableGlobal()
* @brief	Macro to disable the global interrupt Enable
* @note		This function also return the state of the Status CP0 Register
* @arg		nothing
* @return	U32 intGlobalState		State of the CP0 Status register before disabling
*/
#define	intFastDisableGlobal()				INTDisableInterrupts()

/**
* \fn		intFastRestoreGlobal(intGlobalState)
* @brief	Macro to restore the global interrupt Enable to a specified state
* @note
* @arg		U32 intGlobalState		State of CP0 Status register to restore
* @return	nothing
*/
#define	intFastRestoreGlobal(intGlobalState)		INTRestoreInterrupts(intGlobalState)

/**
* \fn		intFastGetGlobal()
* @brief	Macro to return the actual state of global interrupt Enable
* @note		This macro check only the IE bit in the CP0 Status Register
* @arg		nothing
* @return	U8 intGlobalState		The state of the global interrupt Enable (1: Enabled | 0: Disabled)
*/
#define	intFastGetGlobal()				((_CP0_GET_STATUS()) & 0x1)

/**
* \fn		intFastSetGlobal(intGlobalState)
* @brief	Macro to set the state of global interrupt Enable
* @note
* @arg		U8 intGlobalState		The state to set the global interrupt Enable (1: Enabled | 0: Disabled)
* @return	nothing
*/
#define	intFastSetGlobal(intGlobalState)		(_CP0_SET_STATUS(((_CP0_GET_STATUS())|(intGlobalState & 0x1))))

/**
* \fn		intFastInit(intSource)
* @brief	Macro to initialize a designated interrupt source
* @note		Use Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to initialize
* @return	nothing
*/
#define intFastInit(intSource)				(intFastClearFlag(intSource));	(intFastEnable(intSource))

/**
* \fn		intFastEnable(intSource)
* @brief	Macro to enable a designated interrupt source
* @note		Use Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to enable
* @return	nothing
*/
#define intFastEnable(intSource)			((CONCAT3(IEC,CONCAT(_REG_,intSource),SET)) = (CONCAT(_MASK_,intSource)))

/**
* \fn		intFastDisable(intSource)
* @brief	Macro to disable a designated interrupt source
* @note		Use Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to disable
* @return	nothing
*/
#define intFastDisable(intSource)			((CONCAT3(IEC,CONCAT(_REG_,intSource),CLR)) = (CONCAT(_MASK_,intSource)))

/**
* \fn		intFastGetState(intSource)
* @brief	Macro to return the actual enable state of a designated interrupt source
* @note		Use Interrupt source list for intSource
*		The return value is aligned to bit0 not to the corresponding mask
* @arg		intSource			The name of the interrupt source to return
* @return	U32 intState			interrupt source enable state (1: Enabled | 0: Disabled )
*/
#define intFastGetState(intSource)			(((CONCAT(IEC,CONCAT(_REG_,intSource))) & (CONCAT(_MASK_,intSource))) >> (CONCAT(_POS_,intSource)))

/**
* \fn		intFastSetState(intSource,intState)
* @brief	Macro to set the enable state of a designated interrupt source
* @note		Use Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to set
* @arg		U32 intState			The state to set the interrupt source to (1: Enabled | 0: Disabled)
* @return	nothing
*/
#define intFastSetState(intSource,intState)		((CONCAT(IEC,CONCAT(_REG_,intSource))) |= (CONCAT(_MASK_,intSource)) & ((intState) << (CONCAT(_POS_,intSource))))

/**
* \fn		intFastGetPriority(intSource)
* @brief	Macro to return the actual priority level of a designated interrupt source
* @note		Use Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to return
* @return	U8 intPrio			interrupt priority level (0 to 7)
*/
#define intFastGetPriority(intSource)			(CONCAT(_IPC_,intSource))

/**
* \fn		intFastSetPriority(intSource,intPrio)
* @brief	Macro to set the priority level of a designated interrupt source
* @note		Use the Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to return
* @arg		U8 intPrio			interrupt priority level (0 to 7)
* @return	nothing
*/
#define intFastSetPriority(intSource,intPrio)		((CONCAT(_IPC_,intSource)) = (intPrio))

/**
* \fn		intFastGetSubPriority(intSource)
* @brief	Macro to return the actual sub-priority level of a designated interrupt source
* @note		Use the Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to return
* @return	U8 intSubPrio			interrupt sub-priority level (0 to 3)
*/
#define intFastGetSubPriority(intSource)		(CONCAT(_SIPC_,intSource))

/**
* \fn		intFastSetSubPriority(intSource,intSubPrio)
* @brief	Macro to set the sub-priority level of a designated interrupt source
* @note		Use the Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to return
* @arg		U8 intSubPrio			interrupt priority level (0 to 3)
* @return	nothing
*/
#define intFastSetSubPriority(intSource,intSubPrio)	((CONCAT(_SIPC_,intSource)) = (intSubPrio))

/**
* \fn		intFastCheckFlag(intSource)
* @brief	Macro to check the flag and the enable of a designated interrupt source
* @note		Use the Interrupt source list for intSource
*		This macro is 1 instruction shorter than doing intFastGetState() & intFastGetFlag()
*		The return value is aligned to bit0 not to the corresponding mask
* @arg		intSource			The name of the interrupt source to check
* @return	U32 intFlagState		The state of the interrupt (1: Interrupt generated | 0: No interrupt generated)
*/
#define intFastCheckFlag(intSource)			((((CONCAT(IEC,CONCAT(_REG_,intSource))) & (CONCAT(IFS,CONCAT(_REG_,intSource)))) & (CONCAT(_MASK_,intSource))) >> (CONCAT(_POS_,intSource)))

/**
* \fn		intFastGetFlag(intSource)
* @brief	Macro to return the flag of a designated interrupt source
* @note		Use the Interrupt source list for intSource
*		The return value is aligned to bit0 not to the corresponding mask
* @arg		intSource			The name of the interrupt source to return
* @return	U32 intFlagState		The state of the flag (1: Interrupt condition | 0: No interrupt condition)
*/
#define intFastGetFlag(intSource)			(((CONCAT(IFS,CONCAT(_REG_,intSource))) & (CONCAT(_MASK_,intSource))) >> (CONCAT(_POS_,intSource)))

/**
* \fn		intFastSetFlag(intSource)
* @brief	Macro to set the flag of a designated interrupt source
* @note		Use the Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to set
* @return	nothing
*/
#define intFastSetFlag(intSource)			((CONCAT3(IFS,CONCAT(_REG_,intSource),SET)) = (CONCAT(_MASK_,intSource)))

/**
* \fn		intFastClearFlag(intSource)
* @brief	Macro to clear the flag of a designated interrupt source
* @note		Use the Interrupt source list for intSource
* @arg		intSource			The name of the interrupt source to clear
* @return	nothing
*/
#define intFastClearFlag(intSource)			((CONCAT3(IFS,CONCAT(_REG_,intSource),CLR)) = (CONCAT(_MASK_,intSource)))

// ############################################## //




// ############# Interrupts Functions ########### //
/**
* \fn		void intInit(intIRQSource)
* @brief	Initialize an interrupt
* @note		Will clear the flag prior to enabling the interrupt
*		Can accept IRQ group
* @arg		tIntIRQ intIRQSource		Which interrupt to init
* @return	nothing
*/
#define intInit(intIRQSource)				_intSetReg((U32*)&IFS0,intIRQSource,DISABLE); _intSetReg((U32*)&IEC0,intIRQSource,ENABLE)

/**
* \fn		void intSetState(intIRQSource, state)
* @brief	Set the state of a specific interrupt
* @note		Use tIntIRQ to know which interrupt is available
*		Use ENABLE or DISABLE for $state but if you are using
*		an IRQ group,use the correct number of bit (ex: IRQ_UART_1 , BIT2|BIT1|BIT0)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 state			State to set the interrupt
* @return	nothing
*/
#define intSetState(intIRQSource, state)		_intSetReg((U32*)&IEC0,intIRQSource,state)

/**
* \fn		void intGetState(intIRQSource, state)
* @brief	Get the state of a specific interrupt
* @note		Use tIntIRQ to know which interrupt is available
*		The state is return as ENABLE or DISABLE but if you are using
*		an IRQ group, it will return all the bits (ex: IRQ_UART_1 returns 0x7)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @return	U8 state			State of the interrupt
*/
#define intGetState(intIRQSource)			_intGetReg((U32*)&IEC0,intIRQSource)

/**
* \fn		void intSetFlag(intIRQSource, state)
* @brief	Set the state of the flag of a specific interrupt
* @note		Use tIntIRQ to know which interrupt is available
*		Use ENABLE or DISABLE for $state but if you are using
*		an IRQ group,use the correct number of bit (ex: IRQ_UART_1 , BIT2|BIT1|BIT0)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 state			State to set the flag
* @return	nothing
*/
#define intSetFlag(intIRQSource, state)			_intSetReg(&IFS0,intIRQSource,state)

/**
* \fn		void intGetFlag(intIRQSource, state)
* @brief	Get the state of the flag of a specific interrupt
* @note		Use tIntIRQ to know which interrupt is available
*		The state is return as ENABLE or DISABLE but if you are using
*		an IRQ group, it will return all the bits (ex: IRQ_UART_1 returns 0x7)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @return	U8 state			State of the flag
*/
#define intGetFlag(intIRQSource)			_intGetReg(&IFS0,intIRQSource)

/**
* \fn		void intSetPriority(tIntIRQ intIRQSource, U8 priorityLvl, U8 subPriorityLvl)
* @brief	Set the priority and the sub-priority of an interrupt
* @note		If you use IRQ Group you will only set the priority for the first IRQ (ex: UART_1 would be UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 priorityLvl			Level of priority for the interrupt
* @arg		U8 subPriorityLvl		Level of sub-priority for the interrupt
* @return	nothing
*/
void intSetPriority(tIntIRQ intIRQSource, U8 priorityLvl, U8 subPriorityLvl);

/**
* \fn		U8 intGetPriority(tIntIRQ intIRQSource)
* @brief	Return the priority level of an interrupt
* @note		If you use IRQ Group you will only get the priority of the first IRQ (ex: UART_1 would give UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to get
* @return	U8 priorityLvl			Level of priority of the interrupt
*/
U8 intGetPriority(tIntIRQ intIRQSource);

/**
* \fn		U8 intGetSubPriority(tIntIRQ intIRQSource)
* @brief	Return the sub-priority level of an interrupt
* @note		If you use IRQ Group you will only get the subPriority of the first IRQ (ex: UART_1 would give UART_1_ERR)
* @arg		tIntIRQ intIRQSource		Which interrupt to get
* @return	U8 subPriorityLvl		Level of sub-priority of the interrupt
*/
U8 intGetSubPriority(tIntIRQ intIRQSource);

/**
* \fn		void intSetExternalEdge(U8 intSource, U8 edgeDirection)
* @brief	Set the specified external interrupt source to trigger on a specific edge transition
* @note		Use the tIntIRQ type to select the correct interrupt source (only IRQ_EXT_INT are valid)
* @arg		tIntIRQ intIRQSource		The external interrupt to configure
* @arg		U8 edgeDirection		The edge transition to select
* @return	nothing
*/
void intSetExternalEdge(tIntIRQ intIRQSource, U8 edgeDirection);
// ############################################## //


// ############## Exception handler ############# //
/**
* \fn		void _general_exception_handler (U32 cause, U32 status)
* @brief	Exception handler for the M4K core
* @note		The processor branch here when an exception other than interrupt or bootstrap code
*		is generated
* @return	nothing
*/
void _general_exception_handler(void);
// ############################################## //


// ############# Internal Definition ############ //
// -- Interrupts Priority -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
//Priority
	#define	_IPC_INT_CORE_TIMER				(IPC0bits.CTIP)
	#define	_IPC_INT_CORE_SOFT_0				(IPC0bits.CS0IP)
	#define	_IPC_INT_CORE_SOFT_1				(IPC0bits.CS1IP)
	#define	_IPC_INT_EXT_INT_0				(IPC0bits.INT0IP)
	#define	_IPC_INT_TIMER_1				(IPC1bits.T1IP)
	#define	_IPC_INT_INPUT_CAPTURE_1_ERR			(IPC1bits.IC1IP)
	#define	_IPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IP)
	#define	_IPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IP)
	#define	_IPC_INT_EXT_INT_1				(IPC1bits.INT1IP)
	#define	_IPC_INT_TIMER_2				(IPC2bits.T2IP)
	#define	_IPC_INT_INPUT_CAPTURE_2_ERR			(IPC2bits.IC2IP)
	#define	_IPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IP)
	#define	_IPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IP)
	#define	_IPC_INT_EXT_INT_2				(IPC2bits.INT2IP)
	#define	_IPC_INT_TIMER_3				(IPC3bits.T3IP)
	#define	_IPC_INT_INPUT_CAPTURE_3_ERR			(IPC3bits.IC3IP)
	#define	_IPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IP)
	#define	_IPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IP)
	#define	_IPC_INT_EXT_INT_3				(IPC3bits.INT3IP)
	#define	_IPC_INT_TIMER_4				(IPC4bits.T4IP)
	#define	_IPC_INT_INPUT_CAPTURE_4_ERR			(IPC4bits.IC4IP)
	#define	_IPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IP)
	#define	_IPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IP)
	#define	_IPC_INT_EXT_INT_4				(IPC4bits.INT4IP)
	#define	_IPC_INT_TIMER_5				(IPC5bits.T5IP)
	#define	_IPC_INT_INPUT_CAPTURE_5_ERR			(IPC5bits.IC5IP)
	#define	_IPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IP)
	#define	_IPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IP)
	#define	_IPC_INT_ADC_1					(IPC5bits.AD1IP)
	#define	_IPC_INT_FAIL_SAFE_CLOCK_MON			(IPC6bits.FSCMIP)
	#define	_IPC_INT_RTCC					(IPC6bits.RTCCIP)
	#define	_IPC_INT_FLASH_CONTROL_EVENT			(IPC6bits.FCEIP)
	#define	_IPC_INT_COMPARATOR_1				(IPC6bits.CMP1IP)
	#define	_IPC_INT_COMPARATOR_2				(IPC7bits.CMP2IP)
	#define	_IPC_INT_COMPARATOR_3				(IPC7bits.CMP3IP)
	#define	_IPC_INT_USB					(IPC7bits.USBIP)
	#define	_IPC_INT_SPI_1_ERR				(IPC7bits.SPI1IP)
	#define	_IPC_INT_SPI_1_RX				(IPC7bits.SPI1IP)
	#define	_IPC_INT_SPI_1_TX				(IPC7bits.SPI1IP)
	#define _IPC_INT_UART_1_ERR				(IPC8bits.U1IP)
	#define _IPC_INT_UART_1_RX				(IPC8bits.U1IP)
	#define _IPC_INT_UART_1_TX				(IPC8bits.U1IP)
	#define	_IPC_INT_I2C_1_COL				(IPC8bits.I2C1IP)
	#define	_IPC_INT_I2C_1_SLAVE				(IPC8bits.I2C1IP)
	#define	_IPC_INT_I2C_1_MASTER				(IPC8bits.I2C1IP)
	#define	_IPC_INT_INPUT_CHANGE_PORT_A			(IPC8bits.CNIP)
	#define	_IPC_INT_INPUT_CHANGE_PORT_B			(IPC8bits.CNIP)
	#define	_IPC_INT_INPUT_CHANGE_PORT_C			(IPC8bits.CNIP)
	#define	_IPC_INT_PMP					(IPC8bits.PMPIP)
	#define	_IPC_INT_PMP_ERR				(IPC8bits.PMPIP)
	#define	_IPC_INT_SPI_2_ERR				(IPC9bits.SPI2IP)
	#define	_IPC_INT_SPI_2_RX				(IPC9bits.SPI2IP)
	#define	_IPC_INT_SPI_2_TX				(IPC9bits.SPI2IP)
	#define _IPC_INT_UART_2_ERR				(IPC9bits.U2IP)
	#define _IPC_INT_UART_2_RX				(IPC9bits.U2IP)
	#define _IPC_INT_UART_2_TX				(IPC9bits.U2IP)
	#define	_IPC_INT_I2C_2_COL				(IPC9bits.I2C2IP)
	#define	_IPC_INT_I2C_2_SLAVE				(IPC9bits.I2C2IP)
	#define	_IPC_INT_I2C_2_MASTER				(IPC9bits.I2C2IP)
	#define	_IPC_INT_CTMU					(IPC9bits.CTMUIP)
	#define	_IPC_INT_DMA_0					(IPC10bits.DMA0IP)
	#define	_IPC_INT_DMA_1					(IPC10bits.DMA1IP)
	#define	_IPC_INT_DMA_2					(IPC10bits.DMA2IP)
	#define	_IPC_INT_DMA_3					(IPC10bits.DMA3IP)

//Sub-priority
	#define	_SIPC_INT_CORE_TIMER				(IPC0bits.CTIS)
	#define	_SIPC_INT_CORE_SOFT_0				(IPC0bits.CS0IS)
	#define	_SIPC_INT_CORE_SOFT_1				(IPC0bits.CS1IS)
	#define	_SIPC_INT_EXT_INT_0				(IPC0bits.INT0IS)
	#define	_SIPC_INT_TIMER_1				(IPC1bits.T1IS)
	#define	_SIPC_INT_INPUT_CAPTURE_1_ERR			(IPC1bits.IC1IS)
	#define	_SIPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IS)
	#define	_SIPC_INT_EXT_INT_1				(IPC1bits.INT1IS)
	#define	_SIPC_INT_TIMER_2				(IPC2bits.T2IS)
	#define	_SIPC_INT_INPUT_CAPTURE_2_ERR			(IPC2bits.IC2IS)
	#define	_SIPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IS)
	#define	_SIPC_INT_EXT_INT_2				(IPC2bits.INT2IS)
	#define	_SIPC_INT_TIMER_3				(IPC3bits.T3IS)
	#define	_SIPC_INT_INPUT_CAPTURE_3_ERR			(IPC3bits.IC3IS)
	#define	_SIPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IS)
	#define	_SIPC_INT_EXT_INT_3				(IPC3bits.INT3IS)
	#define	_SIPC_INT_TIMER_4				(IPC4bits.T4IS)
	#define	_SIPC_INT_INPUT_CAPTURE_4_ERR			(IPC4bits.IC4IS)
	#define	_SIPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IS)
	#define	_SIPC_INT_EXT_INT_4				(IPC4bits.INT4IS)
	#define	_SIPC_INT_TIMER_5				(IPC5bits.T5IS)
	#define	_SIPC_INT_INPUT_CAPTURE_5_ERR			(IPC5bits.IC5IS)
	#define	_SIPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IS)
	#define	_SIPC_INT_ADC_1					(IPC5bits.AD1IS)
	#define	_SIPC_INT_FAIL_SAFE_CLOCK_MON			(IPC6bits.FSCMIS)
	#define	_SIPC_INT_RTCC					(IPC6bits.RTCCIS)
	#define	_SIPC_INT_FLASH_CONTROL_EVENT			(IPC6bits.FCEIS)
	#define	_SIPC_INT_COMPARATOR_1				(IPC6bits.CMP1IS)
	#define	_SIPC_INT_COMPARATOR_2				(IPC7bits.CMP2IS)
	#define	_SIPC_INT_COMPARATOR_3				(IPC7bits.CMP3IS)
	#define	_SIPC_INT_USB					(IPC7bits.USBIS)
	#define	_SIPC_INT_SPI_1_ERR				(IPC7bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_RX				(IPC7bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_TX				(IPC7bits.SPI1IS)
	#define _SIPC_INT_UART_1_ERR				(IPC8bits.U1IS)
	#define _SIPC_INT_UART_1_RX				(IPC8bits.U1IS)
	#define _SIPC_INT_UART_1_TX				(IPC8bits.U1IS)
	#define	_SIPC_INT_I2C_1_COL				(IPC8bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_SLAVE				(IPC8bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_MASTER				(IPC8bits.I2C1IS)
	#define	_SIPC_INT_INPUT_CHANGE_PORT_A			(IPC8bits.CNIS)
	#define	_SIPC_INT_INPUT_CHANGE_PORT_B			(IPC8bits.CNIS)
	#define	_SIPC_INT_INPUT_CHANGE_PORT_C			(IPC8bits.CNIS)
	#define	_SIPC_INT_PMP					(IPC8bits.PMPIS)
	#define	_SIPC_INT_PMP_ERR				(IPC8bits.PMPIS)
	#define	_SIPC_INT_SPI_2_ERR				(IPC9bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_RX				(IPC9bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_TX				(IPC9bits.SPI2IS)
	#define _SIPC_INT_UART_2_ERR				(IPC9bits.U2IS)
	#define _SIPC_INT_UART_2_RX				(IPC9bits.U2IS)
	#define _SIPC_INT_UART_2_TX				(IPC9bits.U2IS)
	#define	_SIPC_INT_I2C_2_COL				(IPC9bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_SLAVE				(IPC9bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_MASTER				(IPC9bits.I2C2IS)
	#define	_SIPC_INT_CTMU					(IPC9bits.CTMUIS)
	#define	_SIPC_INT_DMA_0					(IPC10bits.DMA0IS)
	#define	_SIPC_INT_DMA_1					(IPC10bits.DMA1IS)
	#define	_SIPC_INT_DMA_2					(IPC10bits.DMA2IS)
	#define	_SIPC_INT_DMA_3					(IPC10bits.DMA3IS)

// Group priorities
	#define _IPC_INT_TIMER_23				_IPC_INT_TIMER_2
	#define _IPC_INT_TIMER_45				_IPC_INT_TIMER_4
	#define _IPC_INT_INPUT_CHANGE				_IPC_INT_INPUT_CHANGE_PORT_A
	#define _IPC_INT_UART_1					_IPC_INT_UART_1_ERR
	#define _IPC_INT_UART_2					_IPC_INT_UART_2_ERR
	#define _IPC_INT_SPI_1					_IPC_INT_SPI_1_ERR
	#define _IPC_INT_SPI_2					_IPC_INT_SPI_2_ERR
	#define _IPC_INT_I2C_1					_IPC_INT_I2C_1_COL
	#define _IPC_INT_I2C_2					_IPC_INT_I2C_2_COL

	#define _SIPC_INT_TIMER_23				_SIPC_INT_TIMER_2
	#define _SIPC_INT_TIMER_45				_SIPC_INT_TIMER_4
	#define _SIPC_INT_INPUT_CHANGE				_SIPC_INT_INPUT_CHANGE_PORT_A
	#define _SIPC_INT_UART_1				_SIPC_INT_UART_1_ERR
	#define _SIPC_INT_UART_2				_SIPC_INT_UART_2_ERR
	#define _SIPC_INT_SPI_1					_SIPC_INT_SPI_1_ERR
	#define _SIPC_INT_SPI_2					_SIPC_INT_SPI_2_ERR
	#define _SIPC_INT_I2C_1					_SIPC_INT_I2C_1_COL
	#define _SIPC_INT_I2C_2					_SIPC_INT_I2C_2_COL

#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
	#define	_IPC_INT_CORE_TIMER				(IPC0bits.CTIP)
	#define	_IPC_INT_CORE_SOFT_0				(IPC0bits.CS0IP)
	#define	_IPC_INT_CORE_SOFT_1				(IPC0bits.CS1IP)
	#define	_IPC_INT_EXT_INT_0				(IPC0bits.INT0IP)
	#define	_IPC_INT_TIMER_1				(IPC1bits.T1IP)
	#define	_IPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IP)
	#define	_IPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IP)
	#define	_IPC_INT_EXT_INT_1				(IPC1bits.INT1IP)
	#define	_IPC_INT_TIMER_2				(IPC2bits.T2IP)
	#define	_IPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IP)
	#define	_IPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IP)
	#define	_IPC_INT_EXT_INT_2				(IPC2bits.INT2IP)
	#define	_IPC_INT_TIMER_3				(IPC3bits.T3IP)
	#define	_IPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IP)
	#define	_IPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IP)
	#define	_IPC_INT_EXT_INT_3				(IPC3bits.INT3IP)
	#define	_IPC_INT_TIMER_4				(IPC4bits.T4IP)
	#define	_IPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IP)
	#define	_IPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IP)
	#define	_IPC_INT_EXT_INT_4				(IPC4bits.INT4IP)
	#define	_IPC_INT_TIMER_5				(IPC5bits.T5IP)
	#define	_IPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IP)
	#define	_IPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IP)
	#define	_IPC_INT_SPI_1_ERR				(IPC5bits.SPI1IP)
	#define	_IPC_INT_SPI_1_RX				(IPC5bits.SPI1IP)
	#define	_IPC_INT_SPI_1_TX				(IPC5bits.SPI1IP)
	#define _IPC_INT_UART_1_ERR				(IPC6bits.U1IP)
	#define _IPC_INT_UART_1_RX				(IPC6bits.U1IP)
	#define _IPC_INT_UART_1_TX				(IPC6bits.U1IP)
	#define	_IPC_INT_I2C_1_COL				(IPC6bits.I2C1IP)
	#define	_IPC_INT_I2C_1_SLAVE				(IPC6bits.I2C1IP)
	#define	_IPC_INT_I2C_1_MASTER				(IPC6bits.I2C1IP)
	#define	_IPC_INT_INPUT_CHANGE				(IPC6bits.CNIP)
	#define	_IPC_INT_ADC_1					(IPC6bits.AD1IP)
	#define	_IPC_INT_PMP					(IPC7bits.PMPIP)
	#define	_IPC_INT_COMPARATOR_1				(IPC7bits.CMP1IP)
	#define	_IPC_INT_COMPARATOR_2				(IPC7bits.CMP2IP)
	#define	_IPC_INT_SPI_2_ERR				(IPC7bits.SPI2IP)
	#define	_IPC_INT_SPI_2_RX				(IPC7bits.SPI2IP)
	#define	_IPC_INT_SPI_2_TX				(IPC7bits.SPI2IP)
	#define _IPC_INT_UART_2_ERR				(IPC8bits.U2IP)
	#define _IPC_INT_UART_2_RX				(IPC8bits.U2IP)
	#define _IPC_INT_UART_2_TX				(IPC8bits.U2IP)
	#define	_IPC_INT_SPI_4_ERR				(IPC8bits.SPI4IP)
	#define	_IPC_INT_SPI_4_RX				(IPC8bits.SPI4IP)
	#define	_IPC_INT_SPI_4_TX				(IPC8bits.SPI4IP)
	#define	_IPC_INT_I2C_2_COL				(IPC8bits.I2C2IP)
	#define	_IPC_INT_I2C_2_SLAVE				(IPC8bits.I2C2IP)
	#define	_IPC_INT_I2C_2_MASTER				(IPC8bits.I2C2IP)
	#define	_IPC_INT_FAIL_SAFE_CLOCK_MON			(IPC8bits.FSCMIP)
	#define	_IPC_INT_RTCC					(IPC8bits.RTCCIP)
	#define	_IPC_INT_DMA_0					(IPC9bits.DMA0IP)
	#define	_IPC_INT_DMA_1					(IPC9bits.DMA1IP)
	#define	_IPC_INT_DMA_2					(IPC9bits.DMA2IP)
	#define	_IPC_INT_DMA_3					(IPC9bits.DMA3IP)
	#define	_IPC_INT_FLASH_CONTROL_EVENT			(IPC11bits.FCEIP)
	#define	_IPC_INT_USB					(IPC11bits.USBIP)


//Sub-priority
	#define	_SIPC_INT_CORE_TIMER				(IPC0bits.CTIS)
	#define	_SIPC_INT_CORE_SOFT_0				(IPC0bits.CS0IS)
	#define	_SIPC_INT_CORE_SOFT_1				(IPC0bits.CS1IS)
	#define	_SIPC_INT_EXT_INT_0				(IPC0bits.INT0IS)
	#define	_SIPC_INT_TIMER_1				(IPC1bits.T1IS)
	#define	_SIPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IS)
	#define	_SIPC_INT_EXT_INT_1				(IPC1bits.INT1IS)
	#define	_SIPC_INT_TIMER_2				(IPC2bits.T2IS)
	#define	_SIPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IS)
	#define	_SIPC_INT_EXT_INT_2				(IPC2bits.INT2IS)
	#define	_SIPC_INT_TIMER_3				(IPC3bits.T3IS)
	#define	_SIPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IS)
	#define	_SIPC_INT_EXT_INT_3				(IPC3bits.INT3IS)
	#define	_SIPC_INT_TIMER_4				(IPC4bits.T4IS)
	#define	_SIPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IS)
	#define	_SIPC_INT_EXT_INT_4				(IPC4bits.INT4IS)
	#define	_SIPC_INT_TIMER_5				(IPC5bits.T5IS)
	#define	_SIPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IS)
	#define	_SIPC_INT_SPI_1_ERR				(IPC5bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_RX				(IPC5bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_TX				(IPC5bits.SPI1IS)
	#define _SIPC_INT_UART_1_ERR				(IPC6bits.U1IS)
	#define _SIPC_INT_UART_1_RX				(IPC6bits.U1IS)
	#define _SIPC_INT_UART_1_TX				(IPC6bits.U1IS)
	#define	_SIPC_INT_I2C_1_COL				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_SLAVE				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_MASTER				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_INPUT_CHANGE				(IPC6bits.CNIS)
	#define	_SIPC_INT_ADC_1					(IPC6bits.AD1IS)
	#define	_SIPC_INT_PMP					(IPC7bits.PMPIS)
	#define	_SIPC_INT_COMPARATOR_1				(IPC7bits.CMP1IS)
	#define	_SIPC_INT_COMPARATOR_2				(IPC7bits.CMP2IS)
	#define	_SIPC_INT_SPI_2_ERR				(IPC7bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_RX				(IPC7bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_TX				(IPC7bits.SPI2IS)
	#define _SIPC_INT_UART_2_ERR				(IPC8bits.U2IS)
	#define _SIPC_INT_UART_2_RX				(IPC8bits.U2IS)
	#define _SIPC_INT_UART_2_TX				(IPC8bits.U2IS)
	#define	_SIPC_INT_SPI_4_ERR				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_SPI_4_RX				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_SPI_4_TX				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_I2C_2_COL				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_SLAVE				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_MASTER				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_FAIL_SAFE_CLOCK_MON			(IPC8bits.FSCMIS)
	#define	_SIPC_INT_RTCC					(IPC8bits.RTCCIS)
	#define	_SIPC_INT_DMA_0					(IPC9bits.DMA0IS)
	#define	_SIPC_INT_DMA_1					(IPC9bits.DMA1IS)
	#define	_SIPC_INT_DMA_2					(IPC9bits.DMA2IS)
	#define	_SIPC_INT_DMA_3					(IPC9bits.DMA3IS)
	#define	_SIPC_INT_FLASH_CONTROL_EVENT			(IPC11bits.FCEIS)
	#define	_SIPC_INT_USB					(IPC11bits.USBIS)

// Group priorities
	#define _IPC_INT_TIMER_23				_IPC_INT_TIMER_2
	#define _IPC_INT_TIMER_45				_IPC_INT_TIMER_4
	#define _IPC_INT_UART_1					_IPC_INT_UART_1_ERR
	#define _IPC_INT_UART_2					_IPC_INT_UART_2_ERR
	#define _IPC_INT_SPI_1					_IPC_INT_SPI_1_ERR
	#define _IPC_INT_SPI_2					_IPC_INT_SPI_2_ERR
	#define _IPC_INT_I2C_1					_IPC_INT_I2C_1_COL
	#define _IPC_INT_I2C_2					_IPC_INT_I2C_2_COL

	#define _SIPC_INT_TIMER_23				_SIPC_INT_TIMER_2
	#define _SIPC_INT_TIMER_45				_SIPC_INT_TIMER_4
	#define _SIPC_INT_UART_1				_SIPC_INT_UART_1_ERR
	#define _SIPC_INT_UART_2				_SIPC_INT_UART_2_ERR
	#define _SIPC_INT_SPI_1					_SIPC_INT_SPI_1_ERR
	#define _SIPC_INT_SPI_2					_SIPC_INT_SPI_2_ERR
	#define _SIPC_INT_I2C_1					_SIPC_INT_I2C_1_COL
	#define _SIPC_INT_I2C_2					_SIPC_INT_I2C_2_COL
#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
	#define	_IPC_INT_CORE_TIMER				(IPC0bits.CTIP)
	#define	_IPC_INT_CORE_SOFT_0				(IPC0bits.CS0IP)
	#define	_IPC_INT_CORE_SOFT_1				(IPC0bits.CS1IP)
	#define	_IPC_INT_EXT_INT_0				(IPC0bits.INT0IP)
	#define	_IPC_INT_TIMER_1				(IPC1bits.T1IP)
	#define	_IPC_INT_INPUT_CAPTURE_1_ERR			(IPC1bits.IC1IP)
	#define	_IPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IP)
	#define	_IPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IP)
	#define	_IPC_INT_EXT_INT_1				(IPC1bits.INT1IP)
	#define	_IPC_INT_TIMER_2				(IPC2bits.T2IP)
	#define	_IPC_INT_INPUT_CAPTURE_2_ERR			(IPC2bits.IC2IP)
	#define	_IPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IP)
	#define	_IPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IP)
	#define	_IPC_INT_EXT_INT_2				(IPC2bits.INT2IP)
	#define	_IPC_INT_TIMER_3				(IPC3bits.T3IP)
	#define	_IPC_INT_INPUT_CAPTURE_3_ERR			(IPC3bits.IC3IP)
	#define	_IPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IP)
	#define	_IPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IP)
	#define	_IPC_INT_EXT_INT_3				(IPC3bits.INT3IP)
	#define	_IPC_INT_TIMER_4				(IPC4bits.T4IP)
	#define	_IPC_INT_INPUT_CAPTURE_4_ERR			(IPC4bits.IC4IP)
	#define	_IPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IP)
	#define	_IPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IP)
	#define	_IPC_INT_EXT_INT_4				(IPC4bits.INT4IP)
	#define	_IPC_INT_TIMER_5				(IPC5bits.T5IP)
	#define	_IPC_INT_INPUT_CAPTURE_5_ERR			(IPC5bits.IC5IP)
	#define	_IPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IP)
	#define	_IPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IP)
	#define	_IPC_INT_SPI_1_ERR				(IPC5bits.SPI1IP)
	#define	_IPC_INT_SPI_1_RX				(IPC5bits.SPI1IP)
	#define	_IPC_INT_SPI_1_TX				(IPC5bits.SPI1IP)
	#define _IPC_INT_UART_1_ERR				(IPC6bits.U1IP)
	#define _IPC_INT_UART_1_RX				(IPC6bits.U1IP)
	#define _IPC_INT_UART_1_TX				(IPC6bits.U1IP)
	#define	_IPC_INT_SPI_3_ERR				(IPC6bits.SPI3IP)
	#define	_IPC_INT_SPI_3_RX				(IPC6bits.SPI3IP)
	#define	_IPC_INT_SPI_3_TX				(IPC6bits.SPI3IP)
	#define	_IPC_INT_I2C_3_COL				(IPC6bits.I2C3IP)
	#define	_IPC_INT_I2C_3_SLAVE				(IPC6bits.I2C3IP)
	#define	_IPC_INT_I2C_3_MASTER				(IPC6bits.I2C3IP)
	#define	_IPC_INT_I2C_1_COL				(IPC6bits.I2C1IP)
	#define	_IPC_INT_I2C_1_SLAVE				(IPC6bits.I2C1IP)
	#define	_IPC_INT_I2C_1_MASTER				(IPC6bits.I2C1IP)
	#define	_IPC_INT_INPUT_CHANGE				(IPC6bits.CNIP)
	#define	_IPC_INT_ADC_1					(IPC6bits.AD1IP)
	#define	_IPC_INT_PMP					(IPC7bits.PMPIP)
	#define	_IPC_INT_PMP_ERR				(IPC7bits.PMPIP)
	#define	_IPC_INT_COMPARATOR_1				(IPC7bits.CMP1IP)
	#define	_IPC_INT_COMPARATOR_2				(IPC7bits.CMP2IP)
	#define _IPC_INT_UART_3_ERR				(IPC7bits.U3IP)
	#define _IPC_INT_UART_3_RX				(IPC7bits.U3IP)
	#define _IPC_INT_UART_3_TX				(IPC7bits.U3IP)
	#define	_IPC_INT_SPI_2_ERR				(IPC7bits.SPI2IP)
	#define	_IPC_INT_SPI_2_RX				(IPC7bits.SPI2IP)
	#define	_IPC_INT_SPI_2_TX				(IPC7bits.SPI2IP)
	#define	_IPC_INT_I2C_4_COL				(IPC7bits.I2C4IP)
	#define	_IPC_INT_I2C_4_SLAVE				(IPC7bits.I2C4IP)
	#define	_IPC_INT_I2C_4_MASTER				(IPC7bits.I2C4IP)
	#define _IPC_INT_UART_2_ERR				(IPC8bits.U2IP)
	#define _IPC_INT_UART_2_RX				(IPC8bits.U2IP)
	#define _IPC_INT_UART_2_TX				(IPC8bits.U2IP)
	#define	_IPC_INT_SPI_4_ERR				(IPC8bits.SPI4IP)
	#define	_IPC_INT_SPI_4_RX				(IPC8bits.SPI4IP)
	#define	_IPC_INT_SPI_4_TX				(IPC8bits.SPI4IP)
	#define	_IPC_INT_I2C_5_COL				(IPC8bits.I2C5IP)
	#define	_IPC_INT_I2C_5_SLAVE				(IPC8bits.I2C5IP)
	#define	_IPC_INT_I2C_5_MASTER				(IPC8bits.I2C5IP)
	#define	_IPC_INT_I2C_2_COL				(IPC8bits.I2C2IP)
	#define	_IPC_INT_I2C_2_SLAVE				(IPC8bits.I2C2IP)
	#define	_IPC_INT_I2C_2_MASTER				(IPC8bits.I2C2IP)
	#define	_IPC_INT_FAIL_SAFE_CLOCK_MON			(IPC8bits.FSCMIP)
	#define	_IPC_INT_RTCC					(IPC8bits.RTCCIP)
	#define	_IPC_INT_DMA_0					(IPC9bits.DMA0IP)
	#define	_IPC_INT_DMA_1					(IPC9bits.DMA1IP)
	#define	_IPC_INT_DMA_2					(IPC9bits.DMA2IP)
	#define	_IPC_INT_DMA_3					(IPC9bits.DMA3IP)
	#define	_IPC_INT_DMA_4					(IPC10bits.DMA4IP)
	#define	_IPC_INT_DMA_5					(IPC10bits.DMA5IP)
	#define	_IPC_INT_DMA_6					(IPC10bits.DMA6IP)
	#define	_IPC_INT_DMA_7					(IPC10bits.DMA7IP)
	#define	_IPC_INT_FLASH_CONTROL_EVENT			(IPC11bits.FCEIP)
	#define	_IPC_INT_USB					(IPC11bits.USBIP)
	#define	_IPC_INT_CAN_1					(IPC11bits.CAN1IP)
	#define	_IPC_INT_CAN_2					(IPC11bits.CAN2IP)
	#define	_IPC_INT_ETHERNET				(IPC12bits.ETHIP)
	#define _IPC_INT_UART_4_ERR				(IPC12bits.U4IP)
	#define _IPC_INT_UART_4_RX				(IPC12bits.U4IP)
	#define _IPC_INT_UART_4_TX				(IPC12bits.U4IP)
	#define _IPC_INT_UART_6_ERR				(IPC12bits.U6IP)
	#define _IPC_INT_UART_6_RX				(IPC12bits.U6IP)
	#define _IPC_INT_UART_6_TX				(IPC12bits.U6IP)
	#define _IPC_INT_UART_5_ERR				(IPC12bits.U5IP)
	#define _IPC_INT_UART_5_RX				(IPC12bits.U5IP)
	#define _IPC_INT_UART_5_TX				(IPC12bits.U5IP)


//Sub-priority
	#define	_SIPC_INT_CORE_TIMER				(IPC0bits.CTIS)
	#define	_SIPC_INT_CORE_SOFT_0				(IPC0bits.CS0IS)
	#define	_SIPC_INT_CORE_SOFT_1				(IPC0bits.CS1IS)
	#define	_SIPC_INT_EXT_INT_0				(IPC0bits.INT0IS)
	#define	_SIPC_INT_TIMER_1				(IPC1bits.T1IS)
	#define	_SIPC_INT_INPUT_CAPTURE_1_ERR			(IPC1bits.IC1IS)
	#define	_SIPC_INT_INPUT_CAPTURE_1			(IPC1bits.IC1IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_1			(IPC1bits.OC1IS)
	#define	_SIPC_INT_EXT_INT_1				(IPC1bits.INT1IS)
	#define	_SIPC_INT_TIMER_2				(IPC2bits.T2IS)
	#define	_SIPC_INT_INPUT_CAPTURE_2_ERR			(IPC2bits.IC2IS)
	#define	_SIPC_INT_INPUT_CAPTURE_2			(IPC2bits.IC2IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_2			(IPC2bits.OC2IS)
	#define	_SIPC_INT_EXT_INT_2				(IPC2bits.INT2IS)
	#define	_SIPC_INT_TIMER_3				(IPC3bits.T3IS)
	#define	_SIPC_INT_INPUT_CAPTURE_3_ERR			(IPC3bits.IC3IS)
	#define	_SIPC_INT_INPUT_CAPTURE_3			(IPC3bits.IC3IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_3			(IPC3bits.OC3IS)
	#define	_SIPC_INT_EXT_INT_3				(IPC3bits.INT3IS)
	#define	_SIPC_INT_TIMER_4				(IPC4bits.T4IS)
	#define	_SIPC_INT_INPUT_CAPTURE_4_ERR			(IPC4bits.IC4IS)
	#define	_SIPC_INT_INPUT_CAPTURE_4			(IPC4bits.IC4IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_4			(IPC4bits.OC4IS)
	#define	_SIPC_INT_EXT_INT_4				(IPC4bits.INT4IS)
	#define	_SIPC_INT_TIMER_5				(IPC5bits.T5IS)
	#define	_SIPC_INT_INPUT_CAPTURE_5_ERR			(IPC5bits.IC5IS)
	#define	_SIPC_INT_INPUT_CAPTURE_5			(IPC5bits.IC5IS)
	#define	_SIPC_INT_OUTPUT_COMPARE_5			(IPC5bits.OC5IS)
	#define	_SIPC_INT_SPI_1_ERR				(IPC5bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_RX				(IPC5bits.SPI1IS)
	#define	_SIPC_INT_SPI_1_TX				(IPC5bits.SPI1IS)
	#define _SIPC_INT_UART_1_ERR				(IPC6bits.U1IS)
	#define _SIPC_INT_UART_1_RX				(IPC6bits.U1IS)
	#define _SIPC_INT_UART_1_TX				(IPC6bits.U1IS)
	#define	_SIPC_INT_SPI_3_ERR				(IPC6bits.SPI3IS)
	#define	_SIPC_INT_SPI_3_RX				(IPC6bits.SPI3IS)
	#define	_SIPC_INT_SPI_3_TX				(IPC6bits.SPI3IS)
	#define	_SIPC_INT_I2C_3_COL				(IPC6bits.I2C3IS)
	#define	_SIPC_INT_I2C_3_SLAVE				(IPC6bits.I2C3IS)
	#define	_SIPC_INT_I2C_3_MASTER				(IPC6bits.I2C3IS)
	#define	_SIPC_INT_I2C_1_COL				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_SLAVE				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_I2C_1_MASTER				(IPC6bits.I2C1IS)
	#define	_SIPC_INT_INPUT_CHANGE				(IPC6bits.CNIS)
	#define	_SIPC_INT_ADC_1					(IPC6bits.AD1IS)
	#define	_SIPC_INT_PMP					(IPC7bits.PMPIS)
	#define	_SIPC_INT_PMP_ERR				(IPC7bits.PMPIS)
	#define	_SIPC_INT_COMPARATOR_1				(IPC7bits.CMP1IS)
	#define	_SIPC_INT_COMPARATOR_2				(IPC7bits.CMP2IS)
	#define _SIPC_INT_UART_3_ERR				(IPC7bits.U3IS)
	#define _SIPC_INT_UART_3_RX				(IPC7bits.U3IS)
	#define _SIPC_INT_UART_3_TX				(IPC7bits.U3IS)
	#define	_SIPC_INT_SPI_2_ERR				(IPC7bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_RX				(IPC7bits.SPI2IS)
	#define	_SIPC_INT_SPI_2_TX				(IPC7bits.SPI2IS)
	#define	_SIPC_INT_I2C_4_COL				(IPC7bits.I2C4IS)
	#define	_SIPC_INT_I2C_4_SLAVE				(IPC7bits.I2C4IS)
	#define	_SIPC_INT_I2C_4_MASTER				(IPC7bits.I2C4IS)
	#define _SIPC_INT_UART_2_ERR				(IPC8bits.U2IS)
	#define _SIPC_INT_UART_2_RX				(IPC8bits.U2IS)
	#define _SIPC_INT_UART_2_TX				(IPC8bits.U2IS)
	#define	_SIPC_INT_SPI_4_ERR				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_SPI_4_RX				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_SPI_4_TX				(IPC8bits.SPI4IS)
	#define	_SIPC_INT_I2C_5_COL				(IPC8bits.I2C5IS)
	#define	_SIPC_INT_I2C_5_SLAVE				(IPC8bits.I2C5IS)
	#define	_SIPC_INT_I2C_5_MASTER				(IPC8bits.I2C5IS)
	#define	_SIPC_INT_I2C_2_COL				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_SLAVE				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_I2C_2_MASTER				(IPC8bits.I2C2IS)
	#define	_SIPC_INT_FAIL_SAFE_CLOCK_MON			(IPC8bits.FSCMIS)
	#define	_SIPC_INT_RTCC					(IPC8bits.RTCCIS)
	#define	_SIPC_INT_DMA_0					(IPC9bits.DMA0IS)
	#define	_SIPC_INT_DMA_1					(IPC9bits.DMA1IS)
	#define	_SIPC_INT_DMA_2					(IPC9bits.DMA2IS)
	#define	_SIPC_INT_DMA_3					(IPC9bits.DMA3IS)
	#define	_SIPC_INT_DMA_4					(IPC10bits.DMA4IS)
	#define	_SIPC_INT_DMA_5					(IPC10bits.DMA5IS)
	#define	_SIPC_INT_DMA_6					(IPC10bits.DMA6IS)
	#define	_SIPC_INT_DMA_7					(IPC10bits.DMA7IS)
	#define	_SIPC_INT_FLASH_CONTROL_EVENT			(IPC11bits.FCEIS)
	#define	_SIPC_INT_USB					(IPC11bits.USBIS)
	#define	_SIPC_INT_CAN_1					(IPC11bits.CAN1IS)
	#define	_SIPC_INT_CAN_2					(IPC11bits.CAN2IS)
	#define	_SIPC_INT_ETHERNET				(IPC12bits.ETHIS)
	#define _SIPC_INT_UART_4_ERR				(IPC12bits.U4IS)
	#define _SIPC_INT_UART_4_RX				(IPC12bits.U4IS)
	#define _SIPC_INT_UART_4_TX				(IPC12bits.U4IS)
	#define _SIPC_INT_UART_6_ERR				(IPC12bits.U6IS)
	#define _SIPC_INT_UART_6_RX				(IPC12bits.U6IS)
	#define _SIPC_INT_UART_6_TX				(IPC12bits.U6IS)
	#define _SIPC_INT_UART_5_ERR				(IPC12bits.U5IS)
	#define _SIPC_INT_UART_5_RX				(IPC12bits.U5IS)
	#define _SIPC_INT_UART_5_TX				(IPC12bits.U5IS)

// Group priorities
	#define _IPC_INT_TIMER_23				_IPC_INT_TIMER_2
	#define _IPC_INT_TIMER_45				_IPC_INT_TIMER_4
	#define _IPC_INT_UART_1					_IPC_INT_UART_1_ERR
	#define _IPC_INT_UART_2					_IPC_INT_UART_2_ERR
	#define _IPC_INT_UART_3					_IPC_INT_UART_3_ERR
	#define _IPC_INT_UART_4					_IPC_INT_UART_4_ERR
	#define _IPC_INT_UART_5					_IPC_INT_UART_5_ERR
	#define _IPC_INT_UART_6					_IPC_INT_UART_6_ERR
	#define _IPC_INT_SPI_1					_IPC_INT_SPI_1_ERR
	#define _IPC_INT_SPI_2					_IPC_INT_SPI_2_ERR
	#define _IPC_INT_SPI_3					_IPC_INT_SPI_3_ERR
	#define _IPC_INT_SPI_4					_IPC_INT_SPI_4_ERR
	#define _IPC_INT_I2C_1					_IPC_INT_I2C_1_COL
	#define _IPC_INT_I2C_2					_IPC_INT_I2C_2_COL
	#define _IPC_INT_I2C_3					_IPC_INT_I2C_3_COL
	#define _IPC_INT_I2C_4					_IPC_INT_I2C_4_COL
	#define _IPC_INT_I2C_5					_IPC_INT_I2C_5_COL

	#define _SIPC_INT_TIMER_23				_SIPC_INT_TIMER_2
	#define _SIPC_INT_TIMER_45				_SIPC_INT_TIMER_4
	#define _SIPC_INT_UART_1				_SIPC_INT_UART_1_ERR
	#define _SIPC_INT_UART_2				_SIPC_INT_UART_2_ERR
	#define _SIPC_INT_UART_3				_SIPC_INT_UART_3_ERR
	#define _SIPC_INT_UART_4				_SIPC_INT_UART_4_ERR
	#define _SIPC_INT_UART_5				_SIPC_INT_UART_5_ERR
	#define _SIPC_INT_UART_6				_SIPC_INT_UART_6_ERR
	#define _SIPC_INT_SPI_1					_SIPC_INT_SPI_1_ERR
	#define _SIPC_INT_SPI_2					_SIPC_INT_SPI_2_ERR
	#define _SIPC_INT_SPI_3					_SIPC_INT_SPI_3_ERR
	#define _SIPC_INT_SPI_4					_SIPC_INT_SPI_4_ERR
	#define _SIPC_INT_I2C_1					_SIPC_INT_I2C_1_COL
	#define _SIPC_INT_I2C_2					_SIPC_INT_I2C_2_COL
	#define _SIPC_INT_I2C_3					_SIPC_INT_I2C_3_COL
	#define _SIPC_INT_I2C_4					_SIPC_INT_I2C_4_COL
	#define _SIPC_INT_I2C_5					_SIPC_INT_I2C_5_COL
#endif
// ------------------------- //

// -- Interrupts Mask -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
//Individual mask
	#define _MASK_INT_CORE_TIMER				BIT0
	#define _MASK_INT_CORE_SOFT_0				BIT1
	#define _MASK_INT_CORE_SOFT_1				BIT2
	#define _MASK_INT_EXT_INT_0				BIT3
	#define _MASK_INT_TIMER_1				BIT4
	#define _MASK_INT_INPUT_CAPTURE_1_ERR			BIT5
	#define _MASK_INT_INPUT_CAPTURE_1			BIT6
	#define _MASK_INT_OUTPUT_COMPARE_1			BIT7
	#define _MASK_INT_EXT_INT_1				BIT8
	#define _MASK_INT_TIMER_2				BIT9
	#define _MASK_INT_INPUT_CAPTURE_2_ERR			BIT10
	#define _MASK_INT_INPUT_CAPTURE_2			BIT11
	#define _MASK_INT_OUTPUT_COMPARE_2			BIT12
	#define _MASK_INT_EXT_INT_2				BIT13
	#define _MASK_INT_TIMER_3				BIT14
	#define _MASK_INT_INPUT_CAPTURE_3_ERR			BIT15
	#define _MASK_INT_INPUT_CAPTURE_3			BIT16
	#define _MASK_INT_OUTPUT_COMPARE_3			BIT17
	#define _MASK_INT_EXT_INT_3				BIT18
	#define _MASK_INT_TIMER_4				BIT19
	#define _MASK_INT_INPUT_CAPTURE_4_ERR			BIT20
	#define _MASK_INT_INPUT_CAPTURE_4			BIT21
	#define _MASK_INT_OUTPUT_COMPARE_4			BIT22
	#define _MASK_INT_EXT_INT_4				BIT23
	#define _MASK_INT_TIMER_5				BIT24
	#define _MASK_INT_INPUT_CAPTURE_5_ERR			BIT25
	#define _MASK_INT_INPUT_CAPTURE_5			BIT26
	#define _MASK_INT_OUTPUT_COMPARE_5			BIT27
	#define _MASK_INT_ADC_1					BIT28
	#define _MASK_INT_FAIL_SAFE_CLOCK_MON			BIT29
	#define _MASK_INT_RTCC					BIT30
	#define _MASK_INT_FLASH_CONTROL_EVENT			BIT31
	#define _MASK_INT_COMPARATOR_1				BIT0
	#define _MASK_INT_COMPARATOR_2				BIT1
	#define _MASK_INT_COMPARATOR_3				BIT2
	#define _MASK_INT_USB					BIT3
	#define _MASK_INT_SPI_1_ERR				BIT4
	#define _MASK_INT_SPI_1_RX				BIT5
	#define _MASK_INT_SPI_1_TX				BIT6
	#define _MASK_INT_UART_1_ERR				BIT7
	#define _MASK_INT_UART_1_RX				BIT8
	#define _MASK_INT_UART_1_TX				BIT9
	#define _MASK_INT_I2C_1_COL				BIT10
	#define _MASK_INT_I2C_1_SLAVE				BIT11
	#define _MASK_INT_I2C_1_MASTER				BIT12
	#define _MASK_INT_INPUT_CHANGE_PORT_A			BIT13
	#define _MASK_INT_INPUT_CHANGE_PORT_B			BIT14
	#define _MASK_INT_INPUT_CHANGE_PORT_C			BIT15
	#define _MASK_INT_PMP					BIT16
	#define _MASK_INT_PMP_ERR				BIT17
	#define _MASK_INT_SPI_2_ERR				BIT18
	#define _MASK_INT_SPI_2_RX				BIT19
	#define _MASK_INT_SPI_2_TX				BIT20
	#define _MASK_INT_UART_2_ERR				BIT21
	#define _MASK_INT_UART_2_RX				BIT22
	#define _MASK_INT_UART_2_TX				BIT23
	#define _MASK_INT_I2C_2_COL				BIT24
	#define _MASK_INT_I2C_2_SLAVE				BIT25
	#define _MASK_INT_I2C_2_MASTER				BIT26
	#define _MASK_INT_CTMU					BIT27
	#define _MASK_INT_DMA_0					BIT28
	#define _MASK_INT_DMA_1					BIT29
	#define _MASK_INT_DMA_2					BIT30
	#define _MASK_INT_DMA_3					BIT31

//Group mask
	#define _MASK_INT_TIMER_23				_MASK_INT_TIMER_2|_MASK_INT_TIMER_3
	#define _MASK_INT_TIMER_45				_MASK_INT_TIMER_4|_MASK_INT_TIMER_5
	#define _MASK_INT_INPUT_CHANGE				_MASK_INT_INPUT_CHANGE_PORT_A|_MASK_INT_INPUT_CHANGE_PORT_B|_MASK_INT_INPUT_CHANGE_PORT_C
	#define _MASK_INT_UART_1				_MASK_INT_UART_1_ERR|_MASK_INT_UART_1_RX|_MASK_INT_UART_1_TX
	#define _MASK_INT_UART_2				_MASK_INT_UART_2_ERR|_MASK_INT_UART_2_RX|_MASK_INT_UART_2_TX
	#define _MASK_INT_SPI_1					_MASK_INT_SPI_1_ERR|_MASK_INT_SPI_1_RX|_MASK_INT_SPI_1_TX
	#define _MASK_INT_SPI_2					_MASK_INT_SPI_2_ERR|_MASK_INT_SPI_2_RX|_MASK_INT_SPI_2_TX
	#define _MASK_INT_I2C_1					_MASK_INT_I2C_1_COL|_MASK_INT_I2C_1_SLAVE|_MASK_INT_I2C_1_MASTER
	#define _MASK_INT_I2C_2					_MASK_INT_I2C_2_COL|_MASK_INT_I2C_2_SLAVE|_MASK_INT_I2C_2_MASTER

#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
//Individual mask
	#define _MASK_INT_CORE_TIMER				BIT0
	#define _MASK_INT_CORE_SOFT_0				BIT1
	#define _MASK_INT_CORE_SOFT_1				BIT2
	#define _MASK_INT_EXT_INT_0				BIT3
	#define _MASK_INT_TIMER_1				BIT4
	#define _MASK_INT_INPUT_CAPTURE_1			BIT5
	#define _MASK_INT_OUTPUT_COMPARE_1			BIT6
	#define _MASK_INT_EXT_INT_1				BIT7
	#define _MASK_INT_TIMER_2				BIT8
	#define _MASK_INT_INPUT_CAPTURE_2			BIT9
	#define _MASK_INT_OUTPUT_COMPARE_2			BIT10
	#define _MASK_INT_EXT_INT_2				BIT11
	#define _MASK_INT_TIMER_3				BIT12
	#define _MASK_INT_INPUT_CAPTURE_3			BIT13
	#define _MASK_INT_OUTPUT_COMPARE_3			BIT14
	#define _MASK_INT_EXT_INT_3				BIT15
	#define _MASK_INT_TIMER_4				BIT16
	#define _MASK_INT_INPUT_CAPTURE_4			BIT17
	#define _MASK_INT_OUTPUT_COMPARE_4			BIT18
	#define _MASK_INT_EXT_INT_4				BIT19
	#define _MASK_INT_TIMER_5				BIT20
	#define _MASK_INT_INPUT_CAPTURE_5			BIT21
	#define _MASK_INT_OUTPUT_COMPARE_5			BIT22
	#define _MASK_INT_SPI_1_ERR				BIT23
	#define _MASK_INT_SPI_1_RX				BIT24
	#define _MASK_INT_SPI_1_TX				BIT25
	#define _MASK_INT_UART_1_ERR				BIT26
	#define _MASK_INT_UART_1_RX				BIT27
	#define _MASK_INT_UART_1_TX				BIT28
	#define _MASK_INT_I2C_1_COL				BIT29
	#define _MASK_INT_I2C_1_SLAVE				BIT30
	#define _MASK_INT_I2C_1_MASTER				BIT31
	#define _MASK_INT_INPUT_CHANGE				BIT0
	#define _MASK_INT_ADC_1					BIT1
	#define _MASK_INT_PMP					BIT2
	#define _MASK_INT_COMPARATOR_1				BIT3
	#define _MASK_INT_COMPARATOR_2				BIT4
	#define _MASK_INT_SPI_2_ERR				BIT5
	#define _MASK_INT_SPI_2_RX				BIT6
	#define _MASK_INT_SPI_2_TX				BIT7
	#define _MASK_INT_UART_2_ERR				BIT8
	#define _MASK_INT_UART_2_RX				BIT9
	#define _MASK_INT_UART_2_TX				BIT10
	#define _MASK_INT_I2C_2_COL				BIT11
	#define _MASK_INT_I2C_2_SLAVE				BIT12
	#define _MASK_INT_I2C_2_MASTER				BIT13
	#define _MASK_INT_FAIL_SAFE_CLOCK_MON			BIT14
	#define _MASK_INT_RTCC					BIT15
	#define _MASK_INT_DMA_0					BIT16
	#define _MASK_INT_DMA_1					BIT17
	#define _MASK_INT_DMA_2					BIT18
	#define _MASK_INT_DMA_3					BIT19
	#define _MASK_INT_FLASH_CONTROL_EVENT			BIT24
	#define _MASK_INT_USB					BIT25

//Group mask
	#define _MASK_INT_TIMER_23				_MASK_INT_TIMER_2|_MASK_INT_TIMER_3
	#define _MASK_INT_TIMER_45				_MASK_INT_TIMER_4|_MASK_INT_TIMER_5
	#define _MASK_INT_UART_1				_MASK_INT_UART_1_ERR|_MASK_INT_UART_1_RX|_MASK_INT_UART_1_TX
	#define _MASK_INT_UART_2				_MASK_INT_UART_2_ERR|_MASK_INT_UART_2_RX|_MASK_INT_UART_2_TX
	#define _MASK_INT_SPI_1					_MASK_INT_SPI_1_ERR|_MASK_INT_SPI_1_RX|_MASK_INT_SPI_1_TX
	#define _MASK_INT_SPI_2					_MASK_INT_SPI_2_ERR|_MASK_INT_SPI_2_RX|_MASK_INT_SPI_2_TX
	#define _MASK_INT_I2C_1					_MASK_INT_I2C_1_COL|_MASK_INT_I2C_1_SLAVE|_MASK_INT_I2C_1_MASTER
	#define _MASK_INT_I2C_2					_MASK_INT_I2C_2_COL|_MASK_INT_I2C_2_SLAVE|_MASK_INT_I2C_2_MASTER

#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
//Individual mask
	#define _MASK_INT_CORE_TIMER				BIT0
	#define _MASK_INT_CORE_SOFT_0				BIT1
	#define _MASK_INT_CORE_SOFT_1				BIT2
	#define _MASK_INT_EXT_INT_0				BIT3
	#define _MASK_INT_TIMER_1				BIT4
	#define _MASK_INT_INPUT_CAPTURE_1			BIT5
	#define _MASK_INT_OUTPUT_COMPARE_1			BIT6
	#define _MASK_INT_EXT_INT_1				BIT7
	#define _MASK_INT_TIMER_2				BIT8
	#define _MASK_INT_INPUT_CAPTURE_2			BIT9
	#define _MASK_INT_OUTPUT_COMPARE_2			BIT10
	#define _MASK_INT_EXT_INT_2				BIT11
	#define _MASK_INT_TIMER_3				BIT12
	#define _MASK_INT_INPUT_CAPTURE_3			BIT13
	#define _MASK_INT_OUTPUT_COMPARE_3			BIT14
	#define _MASK_INT_EXT_INT_3				BIT15
	#define _MASK_INT_TIMER_4				BIT16
	#define _MASK_INT_INPUT_CAPTURE_4			BIT17
	#define _MASK_INT_OUTPUT_COMPARE_4			BIT18
	#define _MASK_INT_EXT_INT_4				BIT19
	#define _MASK_INT_TIMER_5				BIT20
	#define _MASK_INT_INPUT_CAPTURE_5			BIT21
	#define _MASK_INT_OUTPUT_COMPARE_5			BIT22
	#define _MASK_INT_SPI_1_ERR				BIT23
	#define _MASK_INT_SPI_1_RX				BIT24
	#define _MASK_INT_SPI_1_TX				BIT25
	#define _MASK_INT_UART_1_ERR				BIT26
	#define _MASK_INT_UART_1_RX				BIT27
	#define _MASK_INT_UART_1_TX				BIT28
	#define _MASK_INT_SPI_3_ERR				BIT26
	#define _MASK_INT_SPI_3_RX				BIT27
	#define _MASK_INT_SPI_3_TX				BIT28
	#define _MASK_INT_I2C_3_COL				BIT26
	#define _MASK_INT_I2C_3_SLAVE				BIT27
	#define _MASK_INT_I2C_3_MASTER				BIT28
	#define _MASK_INT_I2C_1_COL				BIT29
	#define _MASK_INT_I2C_1_SLAVE				BIT30
	#define _MASK_INT_I2C_1_MASTER				BIT31
	#define _MASK_INT_INPUT_CHANGE				BIT0
	#define _MASK_INT_ADC_1					BIT1
	#define _MASK_INT_PMP					BIT2
	#define _MASK_INT_COMPARATOR_1				BIT3
	#define _MASK_INT_COMPARATOR_2				BIT4
	#define _MASK_INT_UART_3_ERR				BIT5
	#define _MASK_INT_UART_3_RX				BIT6
	#define _MASK_INT_UART_3_TX				BIT7
	#define _MASK_INT_SPI_2_ERR				BIT5
	#define _MASK_INT_SPI_2_RX				BIT6
	#define _MASK_INT_SPI_2_TX				BIT7
	#define _MASK_INT_I2C_4_COL				BIT5
	#define _MASK_INT_I2C_4_SLAVE				BIT6
	#define _MASK_INT_I2C_4_MASTER				BIT7
	#define _MASK_INT_UART_2_ERR				BIT8
	#define _MASK_INT_UART_2_RX				BIT9
	#define _MASK_INT_UART_2_TX				BIT10
	#define _MASK_INT_SPI_4_ERR				BIT8
	#define _MASK_INT_SPI_4_RX				BIT9
	#define _MASK_INT_SPI_4_TX				BIT10
	#define _MASK_INT_I2C_5_COL				BIT8
	#define _MASK_INT_I2C_5_SLAVE				BIT9
	#define _MASK_INT_I2C_5_MASTER				BIT10
	#define _MASK_INT_I2C_2_COL				BIT11
	#define _MASK_INT_I2C_2_SLAVE				BIT12
	#define _MASK_INT_I2C_2_MASTER				BIT13
	#define _MASK_INT_FAIL_SAFE_CLOCK_MON			BIT14
	#define _MASK_INT_RTCC					BIT15
	#define _MASK_INT_DMA_0					BIT16
	#define _MASK_INT_DMA_1					BIT17
	#define _MASK_INT_DMA_2					BIT18
	#define _MASK_INT_DMA_3					BIT19
	#define _MASK_INT_DMA_4					BIT20
	#define _MASK_INT_DMA_5					BIT21
	#define _MASK_INT_DMA_6					BIT22
	#define _MASK_INT_DMA_7					BIT23
	#define _MASK_INT_FLASH_CONTROL_EVENT			BIT24
	#define _MASK_INT_USB					BIT25
	#define _MASK_INT_CAN_1					BIT26
	#define _MASK_INT_CAN_2					BIT27
	#define _MASK_INT_ETHERNET				BIT28
	#define _MASK_INT_INPUT_CAPTURE_1_ERR			BIT29
	#define _MASK_INT_INPUT_CAPTURE_2_ERR			BIT30
	#define _MASK_INT_INPUT_CAPTURE_3_ERR			BIT31
	#define _MASK_INT_INPUT_CAPTURE_4_ERR			BIT0
	#define _MASK_INT_INPUT_CAPTURE_5_ERR			BIT1
	#define _MASK_INT_PMP_ERR				BIT2
	#define _MASK_INT_UART_4_ERR				BIT3
	#define _MASK_INT_UART_4_RX				BIT4
	#define _MASK_INT_UART_4_TX				BIT5
	#define _MASK_INT_UART_6_ERR				BIT6
	#define _MASK_INT_UART_6_RX				BIT7
	#define _MASK_INT_UART_6_TX				BIT8
	#define _MASK_INT_UART_5_ERR				BIT9
	#define _MASK_INT_UART_5_RX				BIT10
	#define _MASK_INT_UART_5_TX				BIT11

//Group mask
	#define _MASK_INT_TIMER_23				_MASK_INT_TIMER_2|_MASK_INT_TIMER_3
	#define _MASK_INT_TIMER_45				_MASK_INT_TIMER_4|_MASK_INT_TIMER_5
	#define _MASK_INT_UART_1				_MASK_INT_UART_1_ERR|_MASK_INT_UART_1_RX|_MASK_INT_UART_1_TX
	#define _MASK_INT_UART_2				_MASK_INT_UART_2_ERR|_MASK_INT_UART_2_RX|_MASK_INT_UART_2_TX
	#define _MASK_INT_UART_3				_MASK_INT_UART_3_ERR|_MASK_INT_UART_3_RX|_MASK_INT_UART_3_TX
	#define _MASK_INT_UART_4				_MASK_INT_UART_4_ERR|_MASK_INT_UART_4_RX|_MASK_INT_UART_4_TX
	#define _MASK_INT_UART_5				_MASK_INT_UART_5_ERR|_MASK_INT_UART_5_RX|_MASK_INT_UART_5_TX
	#define _MASK_INT_UART_6				_MASK_INT_UART_6_ERR|_MASK_INT_UART_6_RX|_MASK_INT_UART_6_TX
	#define _MASK_INT_SPI_1					_MASK_INT_SPI_1_ERR|_MASK_INT_SPI_1_RX|_MASK_INT_SPI_1_TX
	#define _MASK_INT_SPI_2					_MASK_INT_SPI_2_ERR|_MASK_INT_SPI_2_RX|_MASK_INT_SPI_2_TX
	#define _MASK_INT_SPI_3					_MASK_INT_SPI_3_ERR|_MASK_INT_SPI_3_RX|_MASK_INT_SPI_3_TX
	#define _MASK_INT_SPI_4					_MASK_INT_SPI_4_ERR|_MASK_INT_SPI_4_RX|_MASK_INT_SPI_4_TX
	#define _MASK_INT_I2C_1					_MASK_INT_I2C_1_COL|_MASK_INT_I2C_1_SLAVE|_MASK_INT_I2C_1_MASTER
	#define _MASK_INT_I2C_2					_MASK_INT_I2C_2_COL|_MASK_INT_I2C_2_SLAVE|_MASK_INT_I2C_2_MASTER
	#define _MASK_INT_I2C_3					_MASK_INT_I2C_3_COL|_MASK_INT_I2C_3_SLAVE|_MASK_INT_I2C_3_MASTER
	#define _MASK_INT_I2C_4					_MASK_INT_I2C_4_COL|_MASK_INT_I2C_4_SLAVE|_MASK_INT_I2C_4_MASTER
	#define _MASK_INT_I2C_5					_MASK_INT_I2C_5_COL|_MASK_INT_I2C_5_SLAVE|_MASK_INT_I2C_5_MASTER

#endif
// --------------------- //

// -- Interrupts Positions -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
//Individual positions
	#define _POS_INT_CORE_TIMER				0
	#define _POS_INT_CORE_SOFT_0				1
	#define _POS_INT_CORE_SOFT_1				2
	#define _POS_INT_EXT_INT_0				3
	#define _POS_INT_TIMER_1				4
	#define _POS_INT_INPUT_CAPTURE_1_ERR			5
	#define _POS_INT_INPUT_CAPTURE_1			6
	#define _POS_INT_OUTPUT_COMPARE_1			7
	#define _POS_INT_EXT_INT_1				8
	#define _POS_INT_TIMER_2				9
	#define _POS_INT_INPUT_CAPTURE_2_ERR			10
	#define _POS_INT_INPUT_CAPTURE_2			11
	#define _POS_INT_OUTPUT_COMPARE_2			12
	#define _POS_INT_EXT_INT_2				13
	#define _POS_INT_TIMER_3				14
	#define _POS_INT_INPUT_CAPTURE_3_ERR			15
	#define _POS_INT_INPUT_CAPTURE_3			16
	#define _POS_INT_OUTPUT_COMPARE_3			17
	#define _POS_INT_EXT_INT_3				18
	#define _POS_INT_TIMER_4				19
	#define _POS_INT_INPUT_CAPTURE_4_ERR			20
	#define _POS_INT_INPUT_CAPTURE_4			21
	#define _POS_INT_OUTPUT_COMPARE_4			22
	#define _POS_INT_EXT_INT_4				23
	#define _POS_INT_TIMER_5				24
	#define _POS_INT_INPUT_CAPTURE_5_ERR			25
	#define _POS_INT_INPUT_CAPTURE_5			26
	#define _POS_INT_OUTPUT_COMPARE_5			27
	#define _POS_INT_ADC_1					28
	#define _POS_INT_FAIL_SAFE_CLOCK_MON			29
	#define _POS_INT_RTCC					30
	#define _POS_INT_FLASH_CONTROL_EVENT			31
	#define _POS_INT_COMPARATOR_1				0
	#define _POS_INT_COMPARATOR_2				1
	#define _POS_INT_COMPARATOR_3				2
	#define _POS_INT_USB					3
	#define _POS_INT_SPI_1_ERR				4
	#define _POS_INT_SPI_1_RX				5
	#define _POS_INT_SPI_1_TX				6
	#define _POS_INT_UART_1_ERR				7
	#define _POS_INT_UART_1_RX				8
	#define _POS_INT_UART_1_TX				9
	#define _POS_INT_I2C_1_COL				10
	#define _POS_INT_I2C_1_SLAVE				11
	#define _POS_INT_I2C_1_MASTER				12
	#define _POS_INT_INPUT_CHANGE_PORT_A			13
	#define _POS_INT_INPUT_CHANGE_PORT_B			14
	#define _POS_INT_INPUT_CHANGE_PORT_C			15
	#define _POS_INT_PMP					16
	#define _POS_INT_PMP_ERR				17
	#define _POS_INT_SPI_2_ERR				18
	#define _POS_INT_SPI_2_RX				19
	#define _POS_INT_SPI_2_TX				20
	#define _POS_INT_UART_2_ERR				21
	#define _POS_INT_UART_2_RX				22
	#define _POS_INT_UART_2_TX				23
	#define _POS_INT_I2C_2_COL				24
	#define _POS_INT_I2C_2_SLAVE				25
	#define _POS_INT_I2C_2_MASTER				26
	#define _POS_INT_CTMU					27
	#define _POS_INT_DMA_0					28
	#define _POS_INT_DMA_1					29
	#define _POS_INT_DMA_2					30
	#define _POS_INT_DMA_3					31

//Group positions
	#define _POS_INT_TIMER_23				_POS_INT_TIMER_2
	#define _POS_INT_TIMER_45				_POS_INT_TIMER_4
	#define _POS_INT_INPUT_CHANGE				_POS_INT_INPUT_CHANGE_PORT_A
	#define _POS_INT_UART_1					_POS_INT_UART_1_ERR
	#define _POS_INT_UART_2					_POS_INT_UART_2_ERR
	#define _POS_INT_SPI_1					_POS_INT_SPI_1_ERR
	#define _POS_INT_SPI_2					_POS_INT_SPI_2_ERR
	#define _POS_INT_I2C_1					_POS_INT_I2C_1_COL
	#define _POS_INT_I2C_2					_POS_INT_I2C_2_COL

#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
//Individual positions
	#define _POS_INT_CORE_TIMER				0
	#define _POS_INT_CORE_SOFT_0				1
	#define _POS_INT_CORE_SOFT_1				2
	#define _POS_INT_EXT_INT_0				3
	#define _POS_INT_TIMER_1				4
	#define _POS_INT_INPUT_CAPTURE_1			5
	#define _POS_INT_OUTPUT_COMPARE_1			6
	#define _POS_INT_EXT_INT_1				7
	#define _POS_INT_TIMER_2				8
	#define _POS_INT_INPUT_CAPTURE_2			9
	#define _POS_INT_OUTPUT_COMPARE_2			10
	#define _POS_INT_EXT_INT_2				11
	#define _POS_INT_TIMER_3				12
	#define _POS_INT_INPUT_CAPTURE_3			13
	#define _POS_INT_OUTPUT_COMPARE_3			14
	#define _POS_INT_EXT_INT_3				15
	#define _POS_INT_TIMER_4				16
	#define _POS_INT_INPUT_CAPTURE_4			17
	#define _POS_INT_OUTPUT_COMPARE_4			18
	#define _POS_INT_EXT_INT_4				19
	#define _POS_INT_TIMER_5				20
	#define _POS_INT_INPUT_CAPTURE_5			21
	#define _POS_INT_OUTPUT_COMPARE_5			22
	#define _POS_INT_SPI_1_ERR				23
	#define _POS_INT_SPI_1_RX				24
	#define _POS_INT_SPI_1_TX				25
	#define _POS_INT_UART_1_ERR				26
	#define _POS_INT_UART_1_RX				27
	#define _POS_INT_UART_1_TX				28
	#define _POS_INT_I2C_1_COL				29
	#define _POS_INT_I2C_1_SLAVE				30
	#define _POS_INT_I2C_1_MASTER				31
	#define _POS_INT_INPUT_CHANGE				0
	#define _POS_INT_ADC_1					1
	#define _POS_INT_PMP					2
	#define _POS_INT_COMPARATOR_1				3
	#define _POS_INT_COMPARATOR_2				4
	#define _POS_INT_SPI_2_ERR				5
	#define _POS_INT_SPI_2_RX				6
	#define _POS_INT_SPI_2_TX				7
	#define _POS_INT_UART_2_ERR				8
	#define _POS_INT_UART_2_RX				9
	#define _POS_INT_UART_2_TX				10
	#define _POS_INT_I2C_2_COL				11
	#define _POS_INT_I2C_2_SLAVE				12
	#define _POS_INT_I2C_2_MASTER				13
	#define _POS_INT_FAIL_SAFE_CLOCK_MON			14
	#define _POS_INT_RTCC					15
	#define _POS_INT_DMA_0					16
	#define _POS_INT_DMA_1					17
	#define _POS_INT_DMA_2					18
	#define _POS_INT_DMA_3					19
	#define _POS_INT_FLASH_CONTROL_EVENT			24
	#define _POS_INT_USB					25

//Group mask
	#define _POS_INT_TIMER_23				_POS_INT_TIMER_2
	#define _POS_INT_TIMER_45				_POS_INT_TIMER_4
	#define _POS_INT_UART_1					_POS_INT_UART_1_ERR
	#define _POS_INT_UART_2					_POS_INT_UART_2_ERR
	#define _POS_INT_SPI_1					_POS_INT_SPI_1_ERR
	#define _POS_INT_SPI_2					_POS_INT_SPI_2_ERR
	#define _POS_INT_I2C_1					_POS_INT_I2C_1_COL
	#define _POS_INT_I2C_2					_POS_INT_I2C_2_COL

#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
//Individual positions
	#define _POS_INT_CORE_TIMER				0
	#define _POS_INT_CORE_SOFT_0				1
	#define _POS_INT_CORE_SOFT_1				2
	#define _POS_INT_EXT_INT_0				3
	#define _POS_INT_TIMER_1				4
	#define _POS_INT_INPUT_CAPTURE_1			5
	#define _POS_INT_OUTPUT_COMPARE_1			6
	#define _POS_INT_EXT_INT_1				7
	#define _POS_INT_TIMER_2				8
	#define _POS_INT_INPUT_CAPTURE_2			9
	#define _POS_INT_OUTPUT_COMPARE_2			10
	#define _POS_INT_EXT_INT_2				11
	#define _POS_INT_TIMER_3				12
	#define _POS_INT_INPUT_CAPTURE_3			13
	#define _POS_INT_OUTPUT_COMPARE_3			14
	#define _POS_INT_EXT_INT_3				15
	#define _POS_INT_TIMER_4				16
	#define _POS_INT_INPUT_CAPTURE_4			17
	#define _POS_INT_OUTPUT_COMPARE_4			18
	#define _POS_INT_EXT_INT_4				19
	#define _POS_INT_TIMER_5				20
	#define _POS_INT_INPUT_CAPTURE_5			21
	#define _POS_INT_OUTPUT_COMPARE_5			22
	#define _POS_INT_SPI_1_ERR				23
	#define _POS_INT_SPI_1_RX				24
	#define _POS_INT_SPI_1_TX				25
	#define _POS_INT_UART_1_ERR				26
	#define _POS_INT_UART_1_RX				27
	#define _POS_INT_UART_1_TX				28
	#define _POS_INT_SPI_3_ERR				26
	#define _POS_INT_SPI_3_RX				27
	#define _POS_INT_SPI_3_TX				28
	#define _POS_INT_I2C_3_COL				26
	#define _POS_INT_I2C_3_SLAVE				27
	#define _POS_INT_I2C_3_MASTER				28
	#define _POS_INT_I2C_1_COL				29
	#define _POS_INT_I2C_1_SLAVE				30
	#define _POS_INT_I2C_1_MASTER				31
	#define _POS_INT_INPUT_CHANGE				0
	#define _POS_INT_ADC_1					1
	#define _POS_INT_PMP					2
	#define _POS_INT_COMPARATOR_1				3
	#define _POS_INT_COMPARATOR_2				4
	#define _POS_INT_UART_3_ERR				5
	#define _POS_INT_UART_3_RX				6
	#define _POS_INT_UART_3_TX				7
	#define _POS_INT_SPI_2_ERR				5
	#define _POS_INT_SPI_2_RX				6
	#define _POS_INT_SPI_2_TX				7
	#define _POS_INT_I2C_4_COL				5
	#define _POS_INT_I2C_4_SLAVE				6
	#define _POS_INT_I2C_4_MASTER				7
	#define _POS_INT_UART_2_ERR				8
	#define _POS_INT_UART_2_RX				9
	#define _POS_INT_UART_2_TX				10
	#define _POS_INT_SPI_4_ERR				8
	#define _POS_INT_SPI_4_RX				9
	#define _POS_INT_SPI_4_TX				10
	#define _POS_INT_I2C_5_COL				8
	#define _POS_INT_I2C_5_SLAVE				9
	#define _POS_INT_I2C_5_MASTER				10
	#define _POS_INT_I2C_2_COL				11
	#define _POS_INT_I2C_2_SLAVE				12
	#define _POS_INT_I2C_2_MASTER				13
	#define _POS_INT_FAIL_SAFE_CLOCK_MON			14
	#define _POS_INT_RTCC					15
	#define _POS_INT_DMA_0					16
	#define _POS_INT_DMA_1					17
	#define _POS_INT_DMA_2					18
	#define _POS_INT_DMA_3					19
	#define _POS_INT_DMA_4					20
	#define _POS_INT_DMA_5					21
	#define _POS_INT_DMA_6					22
	#define _POS_INT_DMA_7					23
	#define _POS_INT_FLASH_CONTROL_EVENT			24
	#define _POS_INT_USB					25
	#define _POS_INT_CAN_1					26
	#define _POS_INT_CAN_2					27
	#define _POS_INT_ETHERNET				28
	#define _POS_INT_INPUT_CAPTURE_1_ERR			29
	#define _POS_INT_INPUT_CAPTURE_2_ERR			30
	#define _POS_INT_INPUT_CAPTURE_3_ERR			31
	#define _POS_INT_INPUT_CAPTURE_4_ERR			0
	#define _POS_INT_INPUT_CAPTURE_5_ERR			1
	#define _POS_INT_PMP_ERR				2
	#define _POS_INT_UART_4_ERR				3
	#define _POS_INT_UART_4_RX				4
	#define _POS_INT_UART_4_TX				5
	#define _POS_INT_UART_6_ERR				6
	#define _POS_INT_UART_6_RX				7
	#define _POS_INT_UART_6_TX				8
	#define _POS_INT_UART_5_ERR				9
	#define _POS_INT_UART_5_RX				10
	#define _POS_INT_UART_5_TX				11

//Group positions
	#define _POS_INT_TIMER_23				_POS_INT_TIMER_2
	#define _POS_INT_TIMER_45				_POS_INT_TIMER_4
	#define _POS_INT_UART_1					_POS_INT_UART_1_ERR
	#define _POS_INT_UART_2					_POS_INT_UART_2_ERR
	#define _POS_INT_UART_3					_POS_INT_UART_3_ERR
	#define _POS_INT_UART_4					_POS_INT_UART_4_ERR
	#define _POS_INT_UART_5					_POS_INT_UART_5_ERR
	#define _POS_INT_UART_6					_POS_INT_UART_6_ERR
	#define _POS_INT_SPI_1					_POS_INT_SPI_1_ERR
	#define _POS_INT_SPI_2					_POS_INT_SPI_2_ERR
	#define _POS_INT_SPI_3					_POS_INT_SPI_3_ERR
	#define _POS_INT_SPI_4					_POS_INT_SPI_4_ERR
	#define _POS_INT_I2C_1					_POS_INT_I2C_1_COL
	#define _POS_INT_I2C_2					_POS_INT_I2C_2_COL
	#define _POS_INT_I2C_3					_POS_INT_I2C_3_COL
	#define _POS_INT_I2C_4					_POS_INT_I2C_4_COL
	#define _POS_INT_I2C_5					_POS_INT_I2C_5_COL
#endif
// -------------------------- //

// -- Interrupts Registers -- //
#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX2xx
//Individual registers
	#define _REG_INT_CORE_TIMER				0
	#define _REG_INT_CORE_SOFT_0				0
	#define _REG_INT_CORE_SOFT_1				0
	#define _REG_INT_EXT_INT_0				0
	#define _REG_INT_TIMER_1				0
	#define _REG_INT_INPUT_CAPTURE_1_ERR			0
	#define _REG_INT_INPUT_CAPTURE_1			0
	#define _REG_INT_OUTPUT_COMPARE_1			0
	#define _REG_INT_EXT_INT_1				0
	#define _REG_INT_TIMER_2				0
	#define _REG_INT_INPUT_CAPTURE_2_ERR			0
	#define _REG_INT_INPUT_CAPTURE_2			0
	#define _REG_INT_OUTPUT_COMPARE_2			0
	#define _REG_INT_EXT_INT_2				0
	#define _REG_INT_TIMER_3				0
	#define _REG_INT_INPUT_CAPTURE_3_ERR			0
	#define _REG_INT_INPUT_CAPTURE_3			0
	#define _REG_INT_OUTPUT_COMPARE_3			0
	#define _REG_INT_EXT_INT_3				0
	#define _REG_INT_TIMER_4				0
	#define _REG_INT_INPUT_CAPTURE_4_ERR			0
	#define _REG_INT_INPUT_CAPTURE_4			0
	#define _REG_INT_OUTPUT_COMPARE_4			0
	#define _REG_INT_EXT_INT_4				0
	#define _REG_INT_TIMER_5				0
	#define _REG_INT_INPUT_CAPTURE_5_ERR			0
	#define _REG_INT_INPUT_CAPTURE_5			0
	#define _REG_INT_OUTPUT_COMPARE_5			0
	#define _REG_INT_ADC_1					0
	#define _REG_INT_FAIL_SAFE_CLOCK_MON			0
	#define _REG_INT_RTCC					0
	#define _REG_INT_FLASH_CONTROL_EVENT			0
	#define _REG_INT_COMPARATOR_1				1
	#define _REG_INT_COMPARATOR_2				1
	#define _REG_INT_COMPARATOR_3				1
	#define _REG_INT_USB					1
	#define _REG_INT_SPI_1_ERR				1
	#define _REG_INT_SPI_1_RX				1
	#define _REG_INT_SPI_1_TX				1
	#define _REG_INT_UART_1_ERR				1
	#define _REG_INT_UART_1_RX				1
	#define _REG_INT_UART_1_TX				1
	#define _REG_INT_I2C_1_COL				1
	#define _REG_INT_I2C_1_SLAVE				1
	#define _REG_INT_I2C_1_MASTER				1
	#define _REG_INT_INPUT_CHANGE_PORT_A			1
	#define _REG_INT_INPUT_CHANGE_PORT_B			1
	#define _REG_INT_INPUT_CHANGE_PORT_C			1
	#define _REG_INT_PMP					1
	#define _REG_INT_PMP_ERR				1
	#define _REG_INT_SPI_2_ERR				1
	#define _REG_INT_SPI_2_RX				1
	#define _REG_INT_SPI_2_TX				1
	#define _REG_INT_UART_2_ERR				1
	#define _REG_INT_UART_2_RX				1
	#define _REG_INT_UART_2_TX				1
	#define _REG_INT_I2C_2_COL				1
	#define _REG_INT_I2C_2_SLAVE				1
	#define _REG_INT_I2C_2_MASTER				1
	#define _REG_INT_CTMU					1
	#define _REG_INT_DMA_0					1
	#define _REG_INT_DMA_1					1
	#define _REG_INT_DMA_2					1
	#define _REG_INT_DMA_3					1

//Group registers
	#define _REG_INT_TIMER_23				0
	#define _REG_INT_TIMER_45				0
	#define _REG_INT_INPUT_CHANGE				1
	#define _REG_INT_UART_1					1
	#define _REG_INT_UART_2					1
	#define _REG_INT_SPI_1					1
	#define _REG_INT_SPI_2					1
	#define _REG_INT_I2C_1					1
	#define _REG_INT_I2C_2					1

#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
//Individual registers
	#define _REG_INT_CORE_TIMER				0
	#define _REG_INT_CORE_SOFT_0				0
	#define _REG_INT_CORE_SOFT_1				0
	#define _REG_INT_EXT_INT_0				0
	#define _REG_INT_TIMER_1				0
	#define _REG_INT_INPUT_CAPTURE_1			0
	#define _REG_INT_OUTPUT_COMPARE_1			0
	#define _REG_INT_EXT_INT_1				0
	#define _REG_INT_TIMER_2				0
	#define _REG_INT_INPUT_CAPTURE_2			0
	#define _REG_INT_OUTPUT_COMPARE_2			0
	#define _REG_INT_EXT_INT_2				0
	#define _REG_INT_TIMER_3				0
	#define _REG_INT_INPUT_CAPTURE_3			0
	#define _REG_INT_OUTPUT_COMPARE_3			0
	#define _REG_INT_EXT_INT_3				0
	#define _REG_INT_TIMER_4				0
	#define _REG_INT_INPUT_CAPTURE_4			0
	#define _REG_INT_OUTPUT_COMPARE_4			0
	#define _REG_INT_EXT_INT_4				0
	#define _REG_INT_TIMER_5				0
	#define _REG_INT_INPUT_CAPTURE_5			0
	#define _REG_INT_OUTPUT_COMPARE_5			0
	#define _REG_INT_SPI_1_ERR				0
	#define _REG_INT_SPI_1_RX				0
	#define _REG_INT_SPI_1_TX				0
	#define _REG_INT_UART_1_ERR				0
	#define _REG_INT_UART_1_RX				0
	#define _REG_INT_UART_1_TX				0
	#define _REG_INT_I2C_1_COL				0
	#define _REG_INT_I2C_1_SLAVE				0
	#define _REG_INT_I2C_1_MASTER				0
	#define _REG_INT_INPUT_CHANGE				1
	#define _REG_INT_ADC_1					1
	#define _REG_INT_PMP					1
	#define _REG_INT_COMPARATOR_1				1
	#define _REG_INT_COMPARATOR_2				1
	#define _REG_INT_SPI_2_ERR				1
	#define _REG_INT_SPI_2_RX				1
	#define _REG_INT_SPI_2_TX				1
	#define _REG_INT_UART_2_ERR				1
	#define _REG_INT_UART_2_RX				1
	#define _REG_INT_UART_2_TX				1
	#define _REG_INT_I2C_2_COL				1
	#define _REG_INT_I2C_2_SLAVE				1
	#define _REG_INT_I2C_2_MASTER				1
	#define _REG_INT_FAIL_SAFE_CLOCK_MON			1
	#define _REG_INT_RTCC					1
	#define _REG_INT_DMA_0					1
	#define _REG_INT_DMA_1					1
	#define _REG_INT_DMA_2					1
	#define _REG_INT_DMA_3					1
	#define _REG_INT_FLASH_CONTROL_EVENT			1
	#define _REG_INT_USB					1

//Group registers
	#define _REG_INT_TIMER_23				_REG_INT_TIMER_2
	#define _REG_INT_TIMER_45				_REG_INT_TIMER_4
	#define _REG_INT_UART_1					_REG_INT_UART_1_ERR
	#define _REG_INT_UART_2					_REG_INT_UART_2_ERR
	#define _REG_INT_SPI_1					_REG_INT_SPI_1_ERR
	#define _REG_INT_SPI_2					_REG_INT_SPI_2_ERR
	#define _REG_INT_I2C_1					_REG_INT_I2C_1_COL
	#define _REG_INT_I2C_2					_REG_INT_I2C_2_COL

#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
//Individual registers
	#define _REG_INT_CORE_TIMER				0
	#define _REG_INT_CORE_SOFT_0				0
	#define _REG_INT_CORE_SOFT_1				0
	#define _REG_INT_EXT_INT_0				0
	#define _REG_INT_TIMER_1				0
	#define _REG_INT_INPUT_CAPTURE_1			0
	#define _REG_INT_OUTPUT_COMPARE_1			0
	#define _REG_INT_EXT_INT_1				0
	#define _REG_INT_TIMER_2				0
	#define _REG_INT_INPUT_CAPTURE_2			0
	#define _REG_INT_OUTPUT_COMPARE_2			0
	#define _REG_INT_EXT_INT_2				0
	#define _REG_INT_TIMER_3				0
	#define _REG_INT_INPUT_CAPTURE_3			0
	#define _REG_INT_OUTPUT_COMPARE_3			0
	#define _REG_INT_EXT_INT_3				0
	#define _REG_INT_TIMER_4				0
	#define _REG_INT_INPUT_CAPTURE_4			0
	#define _REG_INT_OUTPUT_COMPARE_4			0
	#define _REG_INT_EXT_INT_4				0
	#define _REG_INT_TIMER_5				0
	#define _REG_INT_INPUT_CAPTURE_5			0
	#define _REG_INT_OUTPUT_COMPARE_5			0
	#define _REG_INT_SPI_1_ERR				0
	#define _REG_INT_SPI_1_RX				0
	#define _REG_INT_SPI_1_TX				0
	#define _REG_INT_UART_1_ERR				0
	#define _REG_INT_UART_1_RX				0
	#define _REG_INT_UART_1_TX				0
	#define _REG_INT_SPI_3_ERR				0
	#define _REG_INT_SPI_3_RX				0
	#define _REG_INT_SPI_3_TX				0
	#define _REG_INT_I2C_3_COL				0
	#define _REG_INT_I2C_3_SLAVE				0
	#define _REG_INT_I2C_3_MASTER				0
	#define _REG_INT_I2C_1_COL				0
	#define _REG_INT_I2C_1_SLAVE				0
	#define _REG_INT_I2C_1_MASTER				0
	#define _REG_INT_INPUT_CHANGE				1
	#define _REG_INT_ADC_1					1
	#define _REG_INT_PMP					1
	#define _REG_INT_COMPARATOR_1				1
	#define _REG_INT_COMPARATOR_2				1
	#define _REG_INT_UART_3_ERR				1
	#define _REG_INT_UART_3_RX				1
	#define _REG_INT_UART_3_TX				1
	#define _REG_INT_SPI_2_ERR				1
	#define _REG_INT_SPI_2_RX				1
	#define _REG_INT_SPI_2_TX				1
	#define _REG_INT_I2C_4_COL				1
	#define _REG_INT_I2C_4_SLAVE				1
	#define _REG_INT_I2C_4_MASTER				1
	#define _REG_INT_UART_2_ERR				1
	#define _REG_INT_UART_2_RX				1
	#define _REG_INT_UART_2_TX				1
	#define _REG_INT_SPI_4_ERR				1
	#define _REG_INT_SPI_4_RX				1
	#define _REG_INT_SPI_4_TX				1
	#define _REG_INT_I2C_5_COL				1
	#define _REG_INT_I2C_5_SLAVE				1
	#define _REG_INT_I2C_5_MASTER				1
	#define _REG_INT_I2C_2_COL				1
	#define _REG_INT_I2C_2_SLAVE				1
	#define _REG_INT_I2C_2_MASTER				1
	#define _REG_INT_FAIL_SAFE_CLOCK_MON			1
	#define _REG_INT_RTCC					1
	#define _REG_INT_DMA_0					1
	#define _REG_INT_DMA_1					1
	#define _REG_INT_DMA_2					1
	#define _REG_INT_DMA_3					1
	#define _REG_INT_DMA_4					1
	#define _REG_INT_DMA_5					1
	#define _REG_INT_DMA_6					1
	#define _REG_INT_DMA_7					1
	#define _REG_INT_FLASH_CONTROL_EVENT			1
	#define _REG_INT_USB					1
	#define _REG_INT_CAN_1					1
	#define _REG_INT_CAN_2					1
	#define _REG_INT_ETHERNET				1
	#define _REG_INT_INPUT_CAPTURE_1_ERR			1
	#define _REG_INT_INPUT_CAPTURE_2_ERR			1
	#define _REG_INT_INPUT_CAPTURE_3_ERR			1
	#define _REG_INT_INPUT_CAPTURE_4_ERR			2
	#define _REG_INT_INPUT_CAPTURE_5_ERR			2
	#define _REG_INT_PMP_ERR				2
	#define _REG_INT_UART_4_ERR				2
	#define _REG_INT_UART_4_RX				2
	#define _REG_INT_UART_4_TX				2
	#define _REG_INT_UART_6_ERR				2
	#define _REG_INT_UART_6_RX				2
	#define _REG_INT_UART_6_TX				2
	#define _REG_INT_UART_5_ERR				2
	#define _REG_INT_UART_5_RX				2
	#define _REG_INT_UART_5_TX				2

//Group registers
	#define _REG_INT_TIMER_23				_REG_INT_TIMER_2
	#define _REG_INT_TIMER_45				_REG_INT_TIMER_4
	#define _REG_INT_UART_1					_REG_INT_UART_1_ERR
	#define _REG_INT_UART_2					_REG_INT_UART_2_ERR
	#define _REG_INT_UART_3					_REG_INT_UART_3_ERR
	#define _REG_INT_UART_4					_REG_INT_UART_4_ERR
	#define _REG_INT_UART_5					_REG_INT_UART_5_ERR
	#define _REG_INT_UART_6					_REG_INT_UART_6_ERR
	#define _REG_INT_SPI_1					_REG_INT_SPI_1_ERR
	#define _REG_INT_SPI_2					_REG_INT_SPI_2_ERR
	#define _REG_INT_SPI_3					_REG_INT_SPI_3_ERR
	#define _REG_INT_SPI_4					_REG_INT_SPI_4_ERR
	#define _REG_INT_I2C_1					_REG_INT_I2C_1_COL
	#define _REG_INT_I2C_2					_REG_INT_I2C_2_COL
	#define _REG_INT_I2C_3					_REG_INT_I2C_3_COL
	#define _REG_INT_I2C_4					_REG_INT_I2C_4_COL
	#define _REG_INT_I2C_5					_REG_INT_I2C_5_COL
#endif
// -------------------------- //
// ############################################## //


// ############# Internal Functions ############# //
/**
* \fn		void _intSetReg(U32 * regPtr, tIntIRQ intIRQSource, U8 state)
* @brief	Write access to a interrupt register
* @note		INTERNAL FUNCTION Do not use directly!!!
* @arg		U32 * regPtr			Pointer to the register being written
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @arg		U8 state			State to set the bit
* @return	nothing
*/
void _intSetReg(U32 * regPtr, tIntIRQ intIRQSource, U8 state);

/**
* \fn		U8 _intGetReg(U32 * regPtr, tIntIRQ intIRQSource)
* @brief	Read access to a interrupt register
* @note		INTERNAL FUNCTION Do not use directly!!!
* @arg		U32 * regPtr			Pointer to the register being written
* @arg		tIntIRQ intIRQSource		Which interrupt to set
* @return	U8 state			State to set the bit
*/
U8 _intGetReg(U32 * regPtr, tIntIRQ intIRQSource);
// ############################################## //

#endif
