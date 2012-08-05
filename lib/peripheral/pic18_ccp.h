/*!
 @file pic18_ccp.h
 @brief		CCP functions for C18
 *              
 @version 0.1
 @date April 2th 2011
 @author Laurence DV
*/

#ifndef _PIC18_CCP_H
#define _PIC18_CCP_H

// ================= Includes ================= //
// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_C18.h>


// ================= Defines ================= //



// ================= Prototype ================= //
void ccp_init_capture(U8 ccp_id, U8 timer, U8 prescaler);
void ccp_init_compare(U8 ccp_id, U8 timer, U8 action);
void ccp_init_pwm(U8 ccp_id, U8 timer, U8 option);





#endif
