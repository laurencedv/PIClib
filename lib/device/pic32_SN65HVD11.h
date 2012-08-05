/*!
 @file		pic32_clock.h
 @brief		Clock control lib for pic32

 @version	0.1
 @note		
 @todo		

 @date		February 2th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_SN65HVD11_H
#define _PIC32_SN65HVD11_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <peripheral/pic32_uart.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //

// ############################################## //


// ################# Data Type ################## //

// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ==== //

// ========================== //


// === Control Functions ==== //


#define rs485SetDir(direction)		CONCAT(_rs485SetDir,direction)
// ========================== //


// === Secondary Function === //

// ========================== //
// ############################################## //


// ############### Internal Macro ############### //
#define _rs485SetDir0				setPIN()


// ############################################## //

#endif
