
/*!
 @file pic18_servo.h
 @brief		Servo Library for max 10 Servo, using Timer 3 Interrupt for C18
 @version 0.1
 @date March 14th 2011
 @author Laurence DV
*/

#ifndef _PIC18_SERVO_H
#define _PIC18_SERVO_H

// ================= Includes ================= //
#include <hardware.h>

//Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

//Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ================= Defines ================= //

// Timing
#define SERVO_0_POINT	47535
#define SERVO_ANGLE_VAL	66
#define SERVO_FULL_VAL	12000

// Channel selection
#define SERVO0_PIN	PORTDbits.RD7
#define SERVO0_TRIS	TRISDbits.TRISD7
#define SERVO1_PIN	PORTDbits.RD6
#define SERVO1_TRIS	TRISDbits.TRISD6
// ############################################## //


// ================= Prototype ================= //
void servo_init(void);
void servo_add(U8 number_of_servo);
void servo_angle_abs(U8 servo_id, S8 angle);
void servo_angle_rel(U8 servo_id, S8 angle);
void servo_interrupt(void);
// ############################################## //

#endif
