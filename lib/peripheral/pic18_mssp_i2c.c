/*!
 @file pic18_mssp_i2c.c
 @brief		I2C functions of the MSSP for PIC18F using C18
 @version 0.3.0
 @date March 2th 2011
 @author Laurence DV
*/



// ================= Includes ================= //
#include "pic18_mssp_i2c.h"



// ================= Variables ================= //
volatile U8 i2c1_retry_count = 0;				//Number of time we tried a I2C frame
volatile U8 i2c1_slave_add = 0;					//Slave address for the current communication
volatile U8 i2c1_data_total = 0;				//Number of byte to communicate in total
volatile U8 i2c1_data_index = 0;				//Index of the current byte in communication
volatile U8 *i2c1_data_ptr = 0;					//Pointer to the destination/source of the data

volatile U8 i2c1_pending_slave = 0;				//Number of pending communication
volatile U8 i2c1_pending_byte = 0;				//Number of pending byte
volatile U8 i2c1_queue_slave[I2C1_QUEUE_SIZE];	//Ring Buffer declaration
volatile U8 *i2c1_queue_slave_ptr_in;			//Writing pointer of the slave address ring-buffer
volatile U8 *i2c1_queue_slave_ptr_out;			//Reading pointer of the slave address ring-buffer
volatile U8 *i2c1_queue_slave_ptr_orig;			//Origin pointer of the slave address ring-buffer
volatile U8 i2c1_queue_nb[I2C1_QUEUE_SIZE];		//Ring Buffer declaration
volatile U8 *i2c1_queue_nb_ptr_in;				//Writing pointer of the number of byte ring-buffer
volatile U8 *i2c1_queue_nb_ptr_out;				//Reading pointer of the number of byte ring-buffer
volatile U8 *i2c1_queue_nb_ptr_orig;			//Origin pointer of the number of byte ring-buffer
volatile U16 i2c1_queue_data[I2C1_QUEUE_SIZE];	//Ring Buffer declaration
volatile U16 *i2c1_queue_data_ptr_in;			//Writing pointer of the data location ring-buffer
volatile U16 *i2c1_queue_data_ptr_out;			//Reading pointer of the data location ring-buffer
volatile U16 *i2c1_queue_data_ptr_orig;			//Origin pointer of the data location ring-buffer
volatile U8 i2c1_queue_byte[I2C1_QUEUE_SIZE];	//Ring Buffer declaration
volatile U8 *i2c1_queue_byte_ptr_in;			//Writing pointer of the slave address ring-buffer
volatile U8 *i2c1_queue_byte_ptr_out;			//Reading pointer of the slave address ring-buffer
volatile U8 *i2c1_queue_byte_ptr_orig;			//Origin pointer of the slave address ring-buffer

union i2c_control_flag_struct
{
    U8 all;
    struct
    {
	U8 frame_cnt:2;	    	//Inside-Frame counter 
	U8 :1;	    			//Filling
	U8 retry:1;				//Retry the actual frame
	U8 queue_underrun:1;	//I2C queue is underrunning
	U8 queue_overrun:1;		//I2C queue is overrunning
	U8 mode:1;				//Mode flag (0: Master Write / 1: Master Read)
	U8 working:1;			//Frame actually running
    };
}volatile i2c1_control_flag;



// ================= MSSP I2C Functions ================= //



// --- I2C Master Functions --- //

// Function to initialize the MSSP1 in I2C Master mode			OK
void i2c1_init_master(void)
{
    //Clear all the controls register
    SSP1CON1 = CLEAR;
    SSP1CON2 = CLEAR;
    SSP1STAT = CLEAR;
    SSP1ADD = CLEAR;
    i2c1_control_flag.all = CLEAR;	//Reinitialize the controls variables
	i2c1_pending_slave = CLEAR;
	i2c1_pending_byte = CLEAR;
	i2c1_retry_count = CLEAR;

    SSP1CON1bits.SSPM = 0b1000;	    //Set the MSSP in I2C Master Mode

    //Enable interrupts for interrupt-driven functions
    int_peripheral_en();
    int_mssp1_init();	    	//Enable the MSSP 1 Interrupts
    int_mssp1col_init();	    	//Enable the MSSP 1 Bus collision Interrupt
    int_global_en();

    //Set the MSSP pins for open drain configuration
    MSSP1_SCL_TRIS = INPUT;	    
    MSSP1_SDA_TRIS = INPUT;
	
    //Init the rings buffers by pointing the pointer to the good memory space
    i2c1_queue_slave_ptr_orig = &i2c1_queue_slave[0];
    i2c1_queue_slave_ptr_in = i2c1_queue_slave_ptr_orig;
    i2c1_queue_slave_ptr_out = i2c1_queue_slave_ptr_orig;

    i2c1_queue_nb_ptr_orig = i2c1_queue_nb;
    i2c1_queue_nb_ptr_in = i2c1_queue_nb_ptr_orig;
    i2c1_queue_nb_ptr_out = i2c1_queue_nb_ptr_orig;

    i2c1_queue_data_ptr_orig = i2c1_queue_data;
    i2c1_queue_data_ptr_in = i2c1_queue_data_ptr_orig;
    i2c1_queue_data_ptr_out = i2c1_queue_data_ptr_orig;

    i2c1_queue_byte_ptr_orig = i2c1_queue_byte;
    i2c1_queue_byte_ptr_in = i2c1_queue_byte_ptr_orig;
    i2c1_queue_byte_ptr_out = i2c1_queue_byte_ptr_orig;
    
}

// Function to set the I2C clock to $mssp_kbit_s				OK (need I2C_FOSC_KHZ)
void i2c1_set_clk(U16 mssp_kbit_s)
{
	SSP1CON1bits.SSPEN = CLEAR;				//Disable the MSSP

	//Compute and store the value for the BRG (FOSC/(4*BAUD))-1
	SSP1ADD = (I2C_FOSC_KHZ/(4*mssp_kbit_s))-1;
    
	//SET SSPaSTAT Slew-rate control depending on the Kbits/s
	SSP1STATbits.SMP = SET;					//Disable the Slew rate control for standard mode (100Kbit/s and 1Mbit/s)

	SSP1CON1bits.SSPEN = SET;				//Enable the MSSP
}



// --- Control Functions --- //

// Function to reset variables for the I2C framer				OK
void i2c1_reset_framer(void)
{
    i2c1_data_index = CLEAR;
    i2c1_control_flag.all &= 0b11110100;		//Clear the frame_cnt and the retry bit
}

// Function to put data into the hardware buffer				OK (verify the break)
void i2c1_put_data(void)
{
	U8 write_retry = 0;

	//Write to hardware buffer and handle possible error
	do {
		SSP1BUF = i2c1_data_ptr[i2c1_data_index];	//Put the data into the buffer
		write_retry++;								//Count the try
		if (write_retry >= I2C1_MAX_WRITE_RETRY)	//Check if we tried the max number of time
		{
			SSP1CON2bits.PEN = SET;					//Restart the frame
			i2c1_control_flag.retry = SET;
			break;									//Exit the loop
		}
	} while (SSP1CON1bits.WCOL);						//Loop back if a Write collision as occured
}

// Function to save the parameters of a communication in the queue
void i2c1_put_queue(U8 address, U8 nb_of_byte, U8 *location)
{
	int_peripheral_dis();		    //Disable Peripheral/Low priority Interrupts
	int_global_dis();			    //Disable Global Interrupts

    // The queue is not full
    if (i2c1_pending_slave < I2C1_QUEUE_SIZE)
    {
		i2c1_control_flag.queue_overrun = CLEAR;

		//Save in queue
		*i2c1_queue_slave_ptr_in = address;
		*i2c1_queue_nb_ptr_in = nb_of_byte;
		*i2c1_queue_data_ptr_in = (U16)location;
		
		//Count the pending communication
		i2c1_pending_slave++;

		//Move foward the pointers
		i2c1_queue_slave_ptr_in++;
		i2c1_queue_nb_ptr_in++;
		i2c1_queue_data_ptr_in++;

		//Check if we are out of the buffer boundary
		if (i2c1_queue_slave_ptr_in >= (i2c1_queue_slave_ptr_orig + I2C1_QUEUE_SIZE))
		{
			//Move backward the pointer
			i2c1_queue_slave_ptr_in = i2c1_queue_slave_ptr_orig;
			i2c1_queue_nb_ptr_in = i2c1_queue_nb_ptr_orig;
			i2c1_queue_data_ptr_in = i2c1_queue_data_ptr_orig;
		}
    }
    // The queue is full
    else
		i2c1_control_flag.queue_overrun = SET;
		
	int_peripheral_en();			    //Enable Peripheral/Low priority Interrupts
	int_global_en();			    //Enable Global Interrupts
}

// Function to recover the parameters							OK
// No Interrupt protection
void i2c1_get_queue(void)
{
	//The buffer is not empty
    if (i2c1_pending_slave)
    {
		i2c1_control_flag.queue_underrun = CLEAR;

		//Check if the next frame is from the byte queue
		if (*i2c1_queue_data_ptr_out == (U16)i2c1_queue_byte_ptr_out)
		{
			//Point the data pointer to the current position
			i2c1_data_ptr = i2c1_queue_byte_ptr_out;

			//Move foward the queue pointer
			i2c1_queue_byte_ptr_out++;

			//Remove the byte from the count
			i2c1_pending_byte--;

			//Check if we are out of the buffer boundary
			if (i2c1_queue_byte_ptr_out >= (i2c1_queue_byte_ptr_orig + I2C1_QUEUE_SIZE))
				i2c1_queue_byte_ptr_out = i2c1_queue_byte_ptr_orig;		//Move backward the pointer
		}

		//The next frame is from the data queue
		else
		{
			i2c1_data_ptr = (U8*)*i2c1_queue_data_ptr_out;
			
		}

		//Point the frame pointers to the current queue position
		i2c1_slave_add = *i2c1_queue_slave_ptr_out;
		i2c1_data_total = *i2c1_queue_nb_ptr_out;

		i2c1_pending_slave--;

		//Move foward the pointers
		i2c1_queue_data_ptr_out++;
		i2c1_queue_nb_ptr_out++;
		i2c1_queue_slave_ptr_out++;

		//Check if we are out of the buffer boundary
		if (i2c1_queue_slave_ptr_out >= (i2c1_queue_slave_ptr_orig + I2C1_QUEUE_SIZE))
		{
			//Move backward the pointer
			i2c1_queue_slave_ptr_out = i2c1_queue_slave_ptr_orig;
			i2c1_queue_nb_ptr_out = i2c1_queue_nb_ptr_orig;
			i2c1_queue_data_ptr_out = i2c1_queue_data_ptr_orig;
		}
    }

    //The buffer is empty
    else
		i2c1_control_flag.queue_underrun = SET;
}

// Function to time and control the I2C frame					OK (remove the delay in stage STOP)
void i2c1_interrupt_framer(void)
{
//--//After STOP bit
	if (SSP1STATbits.P)
	{		
		//Retry the frame
		if (i2c1_control_flag.retry && (i2c1_retry_count < I2C1_MAX_RETRY))
		{
			i2c1_control_flag.retry = CLEAR;		//retry flag handled
			i2c1_retry_count++;						//Count this try
			i2c1_reset_framer;
			delay_nCY(60);							//Wait min 5us			//TODO remove the delay!!!!
			SSP1CON2bits.SEN = SET;					//Start a START bit
		}

		//Stop the MSSP
		else
		{
			i2c1_control_flag.working = CLEAR;		//Tell the system the MSSP isn't working anymore
			SSP1CON1bits.SSPEN = CLEAR;				//Shutdown the hardware
		}
	}
	
//--//STAGE 3 of the frame	(after Master ACK/NACK)
	else if(i2c1_control_flag.frame_cnt == 3)
	{
		//There is communication pending
		if (i2c1_pending_slave)
		{
			i2c1_get_queue();						//Load another frame from the queue
			i2c1_reset_framer();					//Reset the frame
			SSP1CON2bits.RSEN = SET;				//Start a ReStart bit
		}

		//No more communication
		else
			SSP1CON2bits.PEN = SET;					//Start a STOP bit
	}
	
//--//STAGE 2 of the frame	(after reception or Slave ACK/NACK)
	else if(i2c1_control_flag.frame_cnt == 2)
	{
		//Reception mode (buffer full)
		if (SSP1STATbits.BF)
		{
			//Save the received byte into the designated location
			i2c1_data_ptr[i2c1_data_index] = SSP1BUF;

			//Check for reception overflow
			if (SSP1CON1bits.SSPOV)
			{
				SSP1CON1bits.SSPOV = CLEAR;			//Clear the overflow flag
				SSP1CON2bits.PEN = SET;				//Start a STOP bit
				i2c1_control_flag.retry = SET;		//Tell the framer to retry the frame
			}

			//Reception was correct
			else
			{
				//Count the received byte
				i2c1_data_index++;

				//We didn't receive all of the bytes
				if (i2c1_data_index < i2c1_data_total)
				{
					i2c1_send_ACK();				//Tell the slave we need more byte
					i2c1_control_flag.frame_cnt--;	//Move to previous stage
				}

				//We received all of the bytes
				else
				{
					i2c1_send_NACK();				//Tell the slave we don't need any more byte
					i2c1_control_flag.frame_cnt++;	//Move to next stage
				}
			}
		}

		//Transmission mode (buffer empty)
		else
		{
			//Slave received the byte correctly
			if (!SSP1CON2bits.ACKSTAT)
			{
				//Count the transmisted byte
				i2c1_data_index++;

				//There is more byte to send
				if (i2c1_data_index < i2c1_data_total)
					i2c1_put_data();				//Send another byte

				//All byte have been sent
				else
				{
					//There is communication pending
					if (i2c1_pending_slave)
					{
						i2c1_get_queue();			//Load another frame from the queue
						i2c1_reset_framer();		//Reset the frame
						SSP1CON2bits.RSEN = SET;	//Start a ReStart bit
					}

					//No more communication
					else
						SSP1CON2bits.PEN = SET;		//Start a STOP bit
				}
			}

			//Slave didn't received the byte correctly
			else
			{
				SSP1CON2bits.PEN = SET;				//Start a STOP bit
				i2c1_control_flag.retry = SET;		//Tell the framer to retry the frame
			}
		}
	}
	
//--//STAGE 1 of the frame	(after Slave ACK/NACK)
	else if(i2c1_control_flag.frame_cnt == 1)
	{
		//Slave is ready
		if (!SSP1CON2bits.ACKSTAT)
		{
			//I2C Master Read
			if (i2c1_control_flag.mode)
				SSP1CON2bits.RCEN = SET;			//Start the reception

			//I2C Master Write
			else
				i2c1_put_data();					//Start the transmission

			//Move to next stage
			i2c1_control_flag.frame_cnt++;
		}
		//Slave is not ready
		else
		{
			SSP1CON2bits.PEN = SET;					//Start a STOP bit
			i2c1_control_flag.retry = SET;			//Tell the framer to retry the frame
		}
	}
	
//--//STAGE 0 of the frame (after START/RE-START bit)
    else
    {
		//Extract the desired mode from the address byte
		i2c1_control_flag.mode = i2c1_slave_add & 0x01;

		//Load the address into the hardware buffer
		SSP1BUF = i2c1_slave_add;

		i2c1_control_flag.frame_cnt++;				//Move to next stage
    }
}

// Function to arbitrate the I2C BUS							OK
void i2c1_bus_arbiter(void)
{
	i2c1_reset_framer();				//Restart the frame
	SSP1CON2bits.SEN = SET;				//Start the START bit sequence
}



// --- Communication Functions --- //

//Function to send "number_of_byte" number of byte to "address" slave located at "*first_byte"		    OK
void i2c1_send_data(U8 address, U8 nb_of_byte, U8 *location)
{
	
	
	//MSSP is not working, start a frame
	if (!i2c1_control_flag.working)
	{
		i2c1_slave_add = address;			//Load the address
		i2c1_data_total = nb_of_byte;		//Load the total number of byte of the frame
		i2c1_data_ptr = location;			//Load the location of the data
		
		i2c1_control_flag.working = SET;	//Tell the system that the mssp is currently working
		
		//Start the frame
		i2c1_reset_framer();
		SSP1CON1bits.SSPEN = SET;
		SSP1CON2bits.SEN = SET;		
	}
	//MSSP is working, put the frame in queue
	else
	    i2c1_put_queue(address,nb_of_byte,location);
}

//Function to send 1 byte to "address" slave
void i2c1_send_byte(U8 address, U8 data)
{
    //MSSP is working (Put in Queue)
    if (i2c1_control_flag.working)
    {		
		//The Byte Queue if full
		if (i2c1_pending_byte >= (I2C1_QUEUE_SIZE -1))
			i2c1_control_flag.queue_overrun = SET;

		//The Byte Queue if not full
		else
		{
			//Save the byte into the queue
			*i2c1_queue_byte_ptr_in = data;

			//Put the information into the queue
			i2c1_put_queue(address,1,i2c1_queue_byte_ptr_in);

			//Move foward the byte queue pointer in
			i2c1_queue_byte_ptr_in++;

			//Count the byte
			i2c1_pending_byte++;

			//Check if we are out of the buffer boundary
			if (i2c1_queue_byte_ptr_in >= (i2c1_queue_byte_ptr_orig + I2C1_QUEUE_SIZE))
				i2c1_queue_byte_ptr_in = i2c1_queue_byte_ptr_orig;		//Move backward the pointer
		}
	}

    //MSSP is not working (Start the frame)
    else
    {
		//Store data temporarly into the byte queue
		*i2c1_queue_byte_ptr_in = data;

		//Store other data directly into the frame variable
		i2c1_data_total = 1;
		i2c1_slave_add = address;

		//Point the frame data pointer to the current queue position
		i2c1_data_ptr = i2c1_queue_byte_ptr_out;

		//Move foward the byte queue pointers (both because we don't count the byte in i2c1_pending_byte)
		i2c1_queue_byte_ptr_in++;
		i2c1_queue_byte_ptr_out++;

		//Check if we are out of the buffer boundary
		if (i2c1_queue_byte_ptr_in >= (i2c1_queue_byte_ptr_orig + I2C1_QUEUE_SIZE))
			i2c1_queue_byte_ptr_in = i2c1_queue_byte_ptr_orig;		//Move backward the pointer
		
		//Check if we are out of the buffer boundary
		if (i2c1_queue_byte_ptr_out >= (i2c1_queue_byte_ptr_orig + I2C1_QUEUE_SIZE))
			i2c1_queue_byte_ptr_out = i2c1_queue_byte_ptr_orig;		//Move backward the pointer

		i2c1_control_flag.working = SET;							//Tell the system that the mssp is currently working

		//Start the frame
		i2c1_reset_framer();
		SSP1CON1bits.SSPEN = SET;
		SSP1CON2bits.SEN = SET;
    }
}

//Function to receive "number_of_byte" number of byte from "address" slave and store it at "*first_byte"    OK
U8 i2c1_receive_data(U8 address, U8 nb_of_byte, U8 *location)
{
	//Set the "Read" bit on the address
	address++;
	
	i2c1_send_data(address, nb_of_byte, location);
	
}

// Function to send a ACK										    OK
void i2c1_send_ACK(void)
{
    //Send an ACK
    SSP1CON2bits.ACKDT = CLEAR;	//Set the ACK Data to "ACK"
    SSP1CON2bits.ACKEN = SET;	//Start the transmission
}

// Function to send a NACK										    OK
void i2c1_send_NACK(void)
{
    //Send an NACK
    SSP1CON2bits.ACKDT = SET;	//Set the ACK Data to "NACK"
    SSP1CON2bits.ACKEN = SET;	//Start the transmission
}
