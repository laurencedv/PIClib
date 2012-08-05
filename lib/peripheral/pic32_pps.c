/*!
 @file		pic32_pps.c
 @brief		Peripheral Pin Select lib for pic32

 @version	0.1
 @note		Always respect the group or you could attach an unknow peripheral to a pins.
 			Ex: Peripherals in "PPS in Group 1" can only be attached to the pins in "PPS in Group 1"
 			Use only the significant part of the names (ex: PPS_IN_INT4 would be INT4)
 @todo		

 @date		February 13th 2011
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_pps.h"
// ############################################## //
