
/*!
 @file pic18_mssp_i2c.h
 @brief		I2C functions of the MSSP for PIC18F using C18
			Depend:	hardware.h, pic18_delay.h, pic18_interrupt.h

 @version 0.3.0
 @date March 2th 2011
 @author Laurence DV
*/

#ifndef _PIC18_MSSP_I2C_H
#define _PIC18_MSSP_I2C_H


// ################# Includes ################### //
#include <hardware.h>

//Lib
#include <peripheral/pic18_interrupt.h>
#include <soft/pic18_delay.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// Osciator Definition //
#define	    I2C_FOSC_KHZ	    FOSC_KHZ

#define	    I2C1_MAX_RETRY	    3	    //Maximum number of time to try a frame
#define	    I2C1_MAX_WRITE_RETRY    3	//Maximum try to write into the hardware buffer before retrying the frame
#define	    I2C1_QUEUE_SIZE	    24	    //Number of byte for the I2C queue (3 buffers)	(MAX:255)

// I2C 1
#define	    MSSP1_SDA		SDA1
#define	    MSSP1_SDA_TRIS	SDA1_TRIS
#define	    MSSP1_SCL		SCL1
#define	    MSSP1_SCL_TRIS	SCL1_TRIS

// I2C 2
#define	    MSSP2_SDA		SDA2
#define	    MSSP2_SDA_TRIS	SDA2_TRIS
#define	    MSSP2_SCL		SCL2
#define	    MSSP2_SCL_TRIS	SCL2_TRIS
// ############################################## //


// ################# Prototypes ################# //
void i2c1_init_master(void);
void i2c1_set_clk(U16 mssp_kbit_s);

void i2c1_send_data(U8 address, U8 number_of_byte, U8 *location);
void i2c1_send_byte(U8 addres, U8 data);
U8 i2c1_receive_data(U8 address, U8 number_of_byte, U8 *location);

void i2c1_put_queue(U8 address, U8 nb_of_byte, U8 *location);
void i2c1_get_queue(void);
void i2c1_interrupt_framer(void);
void i2c1_bus_arbiter(void);
void i2c1_send_ACK(void);
void i2c1_send_NACK(void);
void i2c1_reset_framer(void);
// ############################################## //

#endif
