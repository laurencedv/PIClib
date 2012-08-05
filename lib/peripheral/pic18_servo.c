
/*!
 @file pic18_servo.c
 @brief		Servo Library for max 10 Servo, using Timer 3 Interrupt for C18
 @version 0.1
 @date March 14th 2011
 @author Laurence DV
*/
// ################# Includes ################### //

#include "pic18_servo.h"
// ############################################## //


// ################# Variables ################## //

U8 servo_ontime_high[10];					//MSB of the on-time for the timer
U8 servo_ontime_low[10];					//LSB
U8 servo_offtime_high[10]={0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2};					//MSB of the off-time for the timer
U8 servo_offtime_low[10]={0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F};					//LSB

U8 servo_address[10];						//Address of the pins of the servo

U8 servo_actual = 0;						//Current servo selected
U8 servo_started = 0;						//Number of initialised servo

union servo_ctl_flag_struct
{
	U8 all;
	struct
	{
		U8 rise:1;
		U8 skip:1;
		U8 :6;
	};

}servo_ctl_flag;

// ############################################## //


// ############## Servo Functions ############### //
// Initialisation of the Servo Control
void servo_init(void)
{
	//Reset control var
	servo_ctl_flag.all = CLEAR;

	//Set Timer 3 for the correct freq (min 1ms max 2ms)
	T3CONbits.TMR3CS = 0;		//Set the clk source to FOSC/4
	T3CONbits.T3CKPS = 0;		//Set the prescaler to 1:1
	T3CONbits.RD16 = CLEAR;		//Disable 16bit read/write

	//Enable the Timer 3 overflow interrupt
	INTCONbits.PEIE = SET;
	PIE2bits.TMR3IE = SET;
	INTCONbits.GIE = SET;

	//Set TRIS
#ifdef SERVO0_TRIS
	SERVO0_TRIS = OUTPUT;
#endif
#ifdef SERVO1_TRIS
	SERVO1_TRIS = OUTPUT;
#endif
#ifdef SERVO2_TRIS
	SERVO2_TRIS = OUTPUT;
#endif
#ifdef SERVO3_TRIS
	SERVO3_TRIS = OUTPUT;
#endif
#ifdef SERVO4_TRIS
	SERVO4_TRIS = OUTPUT;
#endif
#ifdef SERVO5_TRIS
	SERVO5_TRIS = OUTPUT;
#endif
#ifdef SERVO6_TRIS
	SERVO6_TRIS = OUTPUT;
#endif
#ifdef SERVO7_TRIS
	SERVO7_TRIS = OUTPUT;
#endif
#ifdef SERVO8_TRIS
	SERVO8_TRIS = OUTPUT;
#endif
#ifdef SERVO9_TRIS
	SERVO9_TRIS = OUTPUT;
#endif
}

// Set the number of servo
void servo_add(U8 number_of_servo)
{
	U8 w1;

	//If the asked number of servo is less than the max
	if (!(servo_started + number_of_servo > 10))
	{
		for (w1 = servo_started ; w1 < (servo_started+number_of_servo); w1++)
		{
			//Set default (00 deg) on-time (1,5ms)
			servo_ontime_high[w1] = SERVO_0_POINT>>8;
			servo_ontime_low[w1] = (U8)SERVO_0_POINT;

			//Set default (0 deg) off-time (0,5ms)
			servo_offtime_high[w1] = (U8)((SERVO_0_POINT+SERVO_FULL_VAL)>>8);
			servo_offtime_low[w1] = (U8)(SERVO_0_POINT+SERVO_FULL_VAL);
		}

		//count the number of servo currently active
		servo_started =+ number_of_servo;

		//Start the Timer
		T3CONbits.TMR3ON = SET;
	}
}

// Update the angle of a servo
void servo_angle_abs(U8 servo_id, S8 angle)
{
	//Temp varibles
	union split16 on_value, off_value;
	S16 angle_value, angle_temp;

	//Check bounadaries
	if (angle > 90)
		angle = 90;
	else if (angle < -90)
		angle = -90;

	//Compute the angle data
	angle_temp = (S16)(angle * (S16)SERVO_ANGLE_VAL);					//Compute the int part of the time/deg value
	angle_value = angle_temp + (((S8)(angle_temp>>5))/3);				//Add the frac part
	on_value.all = (SERVO_0_POINT - angle_value);						//Compute the corresponding timer values
	off_value.all = (angle_value+SERVO_0_POINT+SERVO_FULL_VAL);

	//Update variables
	servo_ontime_high[servo_id] = on_value.lvl2;
	servo_ontime_low[servo_id] = on_value.lvl1;
	servo_offtime_high[servo_id] = off_value.lvl2;
	servo_offtime_low[servo_id] = off_value.lvl1;
}

// Update a relative angle of a servo
void servo_angle_rel(U8 servo_id, S8 angle)
{
	//Compute the angle data

	//Update varibles
}

// Interrupt Handler for the Servo timing
void servo_interrupt(void)
{
	do
	{
		servo_ctl_flag.skip = CLEAR;					//Clear the skip flag

		//Rising edge
		if (servo_ctl_flag.rise)
		{
			//Set high the servo pin
			switch (servo_actual)
			{
				case 0:	SERVO0_PIN = HIGH;	break;
				case 1:	SERVO1_PIN = HIGH;	break;
			/*	case 2:	SERVO2_PIN = HIGH;	break;
				case 3:	SERVO3_PIN = HIGH;	break;
				case 4:	SERVO4_PIN = HIGH;	break;
				case 5:	SERVO5_PIN = HIGH;	break;
				case 6:	SERVO6_PIN = HIGH;	break;
				case 7:	SERVO7_PIN = HIGH;	break;
				case 8:	SERVO8_PIN = HIGH;	break;
				case 9:	SERVO9_PIN = HIGH;	break;*/
			}
			
			TMR3H = servo_ontime_high[servo_actual];	//Load the Timer for the on-time
			TMR3L = servo_ontime_low[servo_actual];
			servo_ctl_flag.rise = CLEAR;				//Next interrupt will be for the falling edge
		}
		//Falling edge
		else
		{
			//Set low the servo pin
			switch (servo_actual)
			{
				case 0:	SERVO0_PIN = LOW;	break;
				case 1:	SERVO1_PIN = LOW;	break;
			/*	case 2:	SERVO2_PIN = LOW;	break;
				case 3:	SERVO3_PIN = LOW;	break;
				case 4:	SERVO4_PIN = LOW;	break;
				case 5:	SERVO5_PIN = LOW;	break;
				case 6:	SERVO6_PIN = LOW;	break;
				case 7:	SERVO7_PIN = LOW;	break;
				case 8:	SERVO8_PIN = LOW;	break;
				case 9:	SERVO9_PIN = LOW;	break;*/
			}

			//Off-time is less than a degree
			if (servo_offtime_high[servo_actual] == 0xFF)
			{
				if (servo_offtime_low[servo_actual] >=0xBC)
					servo_ctl_flag.skip = SET;				//Skip the off-time for this servo and switch to the next right away
			}

			//Off-time is more than a degree
			else
			{
				TMR3H = servo_offtime_high[servo_actual];	//Load the Timer for the off-time
				TMR3L = servo_offtime_low[servo_actual];
			}

			//Select the next servo
			servo_actual++;

			// We reached the last servo
			if (servo_actual == 10)
				servo_actual = 0;							//Wrap to the first servo

			if (servo_actual >= servo_started)
				servo_ctl_flag.rise = CLEAR;				//Next interrupt will be for a falling edge (or stay low)
			else
				servo_ctl_flag.rise = SET;					//Next interrupt will be for the rising edge

		}
	} while (servo_ctl_flag.skip);
}
// ############################################## //

