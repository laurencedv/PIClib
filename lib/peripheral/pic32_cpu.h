/*!
 @file		pic32_cpu.h
 @brief		CPU Control lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		September 20th 2012
 @author	Laurence DV
*/


#ifndef _PIC32_CPU_H
#define _PIC32_CPU_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define SWRST_MASK		BIT0
// ############################################## //


// ################# Prototypes ################# //
/**
* \fn		void sysUnlock(void)
* @brief	Unlock the SYSLOCK control reg
* @note		Will force lock before unlocking
* @arg		nothing
* @return	nothing
*/
#define sysUnlock()						(SYSKEY = 0); (SYSKEY = 0xAA996655); (SYSKEY = 0x556699AA)

/**
* \fn		void sysLock(void)
* @brief	Lock the SYSLOCK control reg
* @note
* @arg		nothing
* @return	nothing
*/
#define sysLock()						(SYSKEY = 0)

/**
* \fn		void cpuSoftReset(void)
* @brief	Call a software Reset
* @note		This function will never exit, the reset is done inside
* @arg		nothing
* @return	nothing
*/
void cpuSoftReset(void);
// ############################################## //

#endif
