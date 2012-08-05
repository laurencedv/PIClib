/*!
 @file		pic18_spi.c
 @brief		SPI lib for C18 using MSSP Interrupt and transaction id

 @version	0.1.1
 @note

 @date		September 13th 2011
 @author	Laurence DV
*/

//TODO		integrated the STD SPI mode instead of the different option

// ================== Include =================== //
#include "pic18_spi.h"
// ============================================== //


// ================= Variables ================== //
U8 SPISlaveNb = 0;								//Number of slave currently initialised

// Data manipulation
U8 * spiINptr[2];								//Pointer to save the incoming data
U8 * spiOUTptr[2];								//Pointer to load the outgoing data
U8 spiByteNb[2] = {0,0};						//Number of byte to transfert for the current com
U8 spiByteSent[2] = {0,0};						//Number of byte already sent

// MSSP control
U8 spiCurrentSlaveID[2];						//Currently Selected Slave
tSPIMSSPstate spiMSSPstate[2] = {stop,stop};	//Current State of the MSSPs
tSPICtlFlag spiCtlFlag[2];						//Control Flag for MSSPs

tSPISlaveTable spiSlaveTable[SPI_SLAVE_TABLE_SIZE];

tSSPSTAT * spiSSPSTAT;
tSSPCON1 * spiSSPCON1;
U8 * spiSSPBUF;
// ============================================== //


// ============ Control Sub-routine ============= //
/**
* \fn		U8 spiInit(U8 port, U8 clockRate, U8 option)
* @brief	Initialise the designated MSSP ($port) with $clockRate speed and designated option 
* @note		
* @arg		U8 port				MSSP port ID
* @arg		U8 clockRate		Clock settings
* @arg		U8 option			Polarity options 
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 spiInit(U8 port, U8 clockRate, U8 option)
{
	split8 splitedOption;

	splitedOption.all = option;

	// -- Point to Ctl Reg -- //
	switch (port)
	{
		//MSSP1
		case 0:
		{
			spiSSPSTAT = (tSSPSTAT*)&SSP1STAT;
			spiSSPCON1 = (tSSPCON1*)&SSP1CON1;

			// -- Stop the MSSP -- //
			SSP1CON1bits.SSPEN = CLEAR;
			// ------------------- //

			int_mssp1_init();

			// -- Set the OD -- //
			ODCON3bits.SPI1OD = splitedOption.b5;
			// ---------------- //

			// -- Set the IO -- //
			SPI1_SCK = splitedOption.b4;				//Set the SCK to idle state
			SPI1_SDO = LOW;
			SPI1_SDI_DIR = INPUT;
			SPI1_SDO_DIR = OUTPUT;

			//Slave
			if (clockRate & 0x04)
			{
				SPI1_SCK_DIR = INPUT;
				SPI1_SS = HIGH;							//Init SS to deselected
				SPI1_SS_DIR = INPUT;			
			}
			//Master
			else
				SPI1_SCK_DIR = OUTPUT;
			// ---------------- //
			
			break;
		}
		//MSSP2
		case 1:
		{
			spiSSPCON1 = (tSSPCON1*)&SSP2CON1;
			spiSSPSTAT = (tSSPSTAT*)&SSP2STAT;

			// -- Stop the MSSP -- //
			SSP2CON1bits.SSPEN = CLEAR;
			// ------------------- //

			int_mssp2_init();

			// -- Set the OD -- //
			ODCON3bits.SPI2OD = splitedOption.b5;
			// ---------------- //

			// -- Set the IO -- //
			SPI2_SCK = splitedOption.b4;				//Set the SCK to idle state
			SPI2_SDO = LOW;
			SPI2_SDO_DIR = OUTPUT;
			SPI2_SDI_DIR = INPUT;

			//Slave
			if (clockRate & 0x04)
			{
				SPI2_SCK_DIR = INPUT;
				SPI2_SS = HIGH;							//Init SS to deselected
				SPI2_SS_DIR = INPUT;
			}
			//Master
			else
				SPI2_SCK_DIR = OUTPUT;
			// ---------------- //

			break;
		}
		//Error
		default:	return STD_EC_FAIL;					//Invalid port nb, exit with error
	}
	// ---------------------- //


	// -- Set the Clock Rate -- //
	spiSSPCON1->SSPM = clockRate & 0x0F;
	// ------------------------ //


	// -- Set the Options -- //
	spiSSPCON1->CKP = splitedOption.b4;					//Load the Idle Clock polarity
	spiSSPSTAT->CKE = splitedOption.b6;					//Load the Transmission edge
	spiSSPSTAT->SMP = splitedOption.b7;					//Load the Reception timing
	// --------------------- //

	
	// -- Start the MSSP -- //
	spiMSSPstate[port] = ready;
	spiSSPCON1->SSPEN = SET;
	// -------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U8 spiAddSlave(U8 port, U8 SSport, U8 SSpin)
* @brief	Append a Slave's SS pin into the SPISlaveTable
* @note		Save all important information for controlling this slave.
			Status of the communication will be outputed in the *statusPtr after all comm
* @arg		U8 port				MSSP port ID
* @arg		U8 * SSport			Pointer to the Port Register
* @arg		U8 SSpin			Pin number on the designated port
* @arg		U8 * statusPtr		Pointer to the status byte of this particular slave
* @return	U8 SlaveID			ID of the newly added slave on the bus (return STD_EC_OVERFLOW if the table is full)
*/
U8 spiAddSlave(U8 port, U8 * SSport, U8 SSpin, U8 * statusPtr)
{
	U8 slaveID;
	U8 *tempPtr;

	//*-- Append the Slave to the Table -- *//
	if (SPISlaveNb < SPI_SLAVE_TABLE_SIZE)
	{
		slaveID = SPISlaveNb;
		spiSlaveTable[slaveID].msspPort = port;					//Save the MSSP port ID
		spiSlaveTable[slaveID].portAdd = (U16)SSport;			//Save the Address of the IO port
		spiSlaveTable[slaveID].portMask = 0;
		spiSlaveTable[slaveID].portMask = 1 << SSpin;			//Save the Mask to apply on IO port
		spiSlaveTable[slaveID].controlPtr = (tSPIStatusFlag*)statusPtr;	//Save a pointer to the status byte of this slave

		//Count the new Slave
		SPISlaveNb++;

		//Set the SS Direction
		tempPtr = SSport + 0x12;								//Offset to TRIS register
		*tempPtr &= (spiSlaveTable[slaveID].portMask ^ 0xFF);	//Clear the correct bit (output)

		return slaveID;											//Return the Slave ID
	}
	//* -- No more space in the Table ----- *//
	else
		return STD_EC_OVERFLOW;
	//* ----------------------------------- *//
}



/**
* \fn		U8 spiStatus(U8 port)
* @brief	Used to check the status of the MSSP
* @note		can return any of the tSpiMSSPstate value (ready,communication,write_collision,overflow)
* @arg		U8 port				MSSP Port ID
* @return	U8 spiMSSPstate		Actual State of the MSSP
*/
U8 spiStatus(U8 port)
{
	return spiMSSPstate[port];
}

// ============================================== //


// ============ Transfer Sub-routine ============ //
/**
* \fn		U8 spiSend(U8 *dataInPtr, U8 *dataOutPtr, U8 byteNb, U8 slaveID)
* @brief	Send $byteNb number fo byte to $slaveID, loading byte from *dataOutPtr and saving it to *dataInPtr.
* @note		MSSP must be ready if not will output STD_EC_BUSY.
* @arg		U8 *dataInPtr		Pointer to where we save byte
* @arg		U8 *dataOutPtr		Pointer from wich we send byte
* @arg		U8 byteNb			Number of byte to send/receive
* @arg		U8 slaveID			ID of the slave with wich we want to communicate
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 spiSend(U8 * dataInPtr, U8 * dataOutPtr, U8 byteNb, U8 slaveID)
{
	U8 tempID = spiSlaveTable[slaveID].msspPort;
	U8 *tempPtr;


	// -- Check the state of the MSSP -- //
	switch (spiMSSPstate[tempID])
	{
		//* -- Ready Start the com -- *//
		case ready:
		{
			// -- Select the correct MSSP -- //
			switch (tempID)
			{
				//MSSP 1
				case 0:
				{
					spiSSPCON1 = (tSSPCON1*)&SSP1CON1;
					spiSSPSTAT = (tSSPSTAT*)&SSP1STAT;
					spiSSPBUF = (U8*)&SSP1BUF;
					break;
				}
				//MSSP 2
				case 1:
				{
					spiSSPCON1 = (tSSPCON1*)&SSP2CON1;
					spiSSPSTAT = (tSSPSTAT*)&SSP2STAT;
					spiSSPBUF = (U8*)&SSP2BUF;
					break;
				}
				//Error
				default:	return STD_EC_NOTFOUND;
			}
			// ----------------------------- //


			// -- Save information for the com -- //
			spiINptr[tempID] = dataInPtr;
			spiOUTptr[tempID] = dataOutPtr;
			spiByteNb[tempID] = byteNb;
			spiCurrentSlaveID[tempID] = slaveID;
			// ---------------------------------- //

			// -- Reset the Slave Status -- //
			spiByteSent[tempID] = 0;						//Reset the byte sent counter
			spiSlaveTable[slaveID].controlPtr->all = 0;		//Reset the status byte
			// ---------------------------- //


			// -- Select the Slave -- //
			tempPtr = (U8*)(spiSlaveTable[spiCurrentSlaveID[tempID]].portAdd);
			*tempPtr &= ((spiSlaveTable[spiCurrentSlaveID[tempID]].portMask) ^ 0xFF);
			// ---------------------- //


			// -- Start the com -- //
			*spiSSPBUF = spiOUTptr[tempID][0];				//Load the first byte into the HW buffer
			// ------------------- //


			spiMSSPstate[tempID] = communication;			//The MSSP is not ready anymore
			return STD_EC_SUCCESS;
		}
		//* -- Already in communication -- *//
		case communication:
			return STD_EC_BUSY;
		//* -- Write collision Detected -- *//
		case write_collision:
		{
			return STD_EC_COLLISION;
		}
		case stop:
		{
			return STD_EC_SHUTDOWN;
		}
		//* -- Invalid State -- *//
		default:
			return STD_EC_FAIL;
	}
	// --------------------------------- //
}

/**
* \fn		void spiISR(U8 port)
* @brief	Interrupt hangler for SPI communication
* @note
* @arg		U8 port				MSSP port ID
* @return	nothing
*/
void spiISR(U8 port)
{
	U8 *tempPtr;
	U8 tempID;
	
	// -- Select the correct MSSP -- //
	switch (port)
	{
		//MSSP1
		case 0:
		{
			spiSSPBUF = (U8*)&SSP1BUF;
			spiSSPCON1 = (tSSPCON1*)&SSP1CON1;
			break;
		}
		//MSSP2
		case 1:
		{
			spiSSPBUF = (U8*)&SSP2BUF;
			spiSSPCON1 = (tSSPCON1*)&SSP2CON1;
			break;
		}
	}
	// ----------------------------- //

	
	// Check for reception overflow
	if (spiSSPCON1->SSPOV)
		spiMSSPstate[port] = overflow;						//Save the Receive overflow error

	// Localy save the byte offset for faster execution
	tempID = spiByteSent[port];

	// Save the incoming byte
	spiINptr[port][tempID] = *spiSSPBUF;

	//Count the byte
	tempID++;


	//* -- Close the communication ----- *//
	if (tempID >= spiByteNb[port])
	{
		// Deselect the Slave
		tempPtr = (U8*)spiSlaveTable[spiCurrentSlaveID[port]].portAdd;
		*tempPtr |= spiSlaveTable[spiCurrentSlaveID[port]].portMask;

		// Reset the MSSP State
		if (spiMSSPstate[port] == communication)
			spiMSSPstate[port] = ready;
		spiByteNb[port] = 0;
		spiByteSent[port] = 0;

		// -- Output the Com Status -- //
		spiSlaveTable[spiCurrentSlaveID[port]].controlPtr->complete = 1;
		spiSlaveTable[spiCurrentSlaveID[port]].controlPtr->success = 1;
		// --------------------------- //
	}
	//* -- Continue the communication -- *//
	else
	{
		//Load the next Byte
		*spiSSPBUF = spiOUTptr[port][tempID];

		spiByteSent[port] = tempID;							//Save back the good offset value in global variable

		if (spiSSPCON1->WCOL)
		{
			spiMSSPstate[port] = write_collision;			//Save the Write collision error
			spiSSPCON1->SSPEN = CLEAR;						//Restart the MSSP
			spiSSPCON1->SSPEN = SET;
		}
	}
	//* -------------------------------- *//
}

// ============================================== //

