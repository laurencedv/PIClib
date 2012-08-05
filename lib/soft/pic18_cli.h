/*!
 @file		pic18_cli.h
 @brief		Command Line Interface for pic18

 @version	0.1
 @note

 @date		November 22th 2011
 @author	Laurence DV
*/


#ifndef _PIC18_CLI_H
#define _PIC18_CLI_H

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Lib
#include <software/pic18_realtime.h>
#include <peripheral/pic18_eusart.h>
#include <peripheral/pic18_interrupt.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
#define	CLI_CURSOR_TICK_RATE		400
#define	CLI_SERIAL_BAUD_RATE		115200
// ############################################## //


// ################# Data Type ################## //

// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //

// =========================== //

// === Control Functions ===== //
U8 cliInit(U8 usartID);
// =========================== //

// === Run-time Functions ==== //
void cliEngine(void);
// =========================== //
// ############################################## //

#endif
