/*!
 @file		pic18_lin.c
 @brief		LIN functions for C18

 @note		
 @version	0.2

 @date		September 12th 2011
 @author	Laurence DV
*/

//TODO Shield the ID in linAddCommand


// ################## Includes ################## //
#include "pic18_lin.h"
// ############################################## //


// ################# Variables ################## //
// -- Control -- //
union
{
	U8 all;
	struct
	{
		U8 inComm:1;							//Flag for receiving a packet (1 when in packet)
		U8 newPacket:1;							//A New packet has been received
		U8 enTimeout:1;							//Enable the Timeout
		U8 timeout:1;							//Timeout Flag for different operation
		U8 step:1;								//Make a step in the FSM
		U8 :3;
	};
}linCtlFlag;

U16 linTimeoutCnt = 0;							//Timeout Counter Value
U8 linNewCommandID = 0xFF;					//ID of the last completed command
enum tLINState linSlaveState = sync;
tLinCommandTable linCommandTable[LIN_COMMAND_TABLE_SIZE];
U8 linCurrentCommand = 0;
U8 linCurrentCommandByteNb = 0;				//MASTER only
U8 linCurrentDir = LIN_COMMAND_DIR_OUT;		//MASTER only
U8 linCommandNb = 0;
enum tLINState linMasterState = idle;						//State of the MASTER FSM
split8 linShieldedID;
// -------------- //

// -- Data -- //
tLINPacket LINPacket;						//Buffered packet
U8 LINPacketNb = 0;							//Number of byte sent

U8 *linDataPtr;
U8 *linCurrentFlagPtr;
// ---------- //



U8 linDataBuf[8] = {0,0,0,0,0,0,0,0};		//Lin Data buffer
U8 linDataNb = 0;							//Number of byte received or Sent
split16 linTempChecksum;
U8 linComputedChecksum = 0;					//Checksum for the outgoing packet (MASTER)
// ############################################## //


// ################ Sub-routine ################# //
// ---- Control Functions ---- //
/**
* \fn		U8 linAddCommand(U8 *packetPtr,U8 command, U8 byteNb)
* @brief	Append a command with it's information into the linCommandTable
* @note
* @arg		U8 *dataPtr		Pointer to save or load the data
* @arg		U8 command			Value of the command
* @arg		U8 byteNb			Number of byte to received for the command
* @arg		U8 direction		Direction for the data (Send or Receive)
* @return	U8 commandID		The ID of the command in the linCommandTable
*/
U8 linAddCommand(U8 *dataPtr,U8 command, U8 byteNb, U8 direction, U8 *flagPtr)
{
	U8 commandID;

	linCtlFlag.inComm = CLEAR;

	//* -- Table is not full -- *//
	if (linCommandNb < LIN_COMMAND_TABLE_SIZE)
	{
		// -- Save the information for the command -- //

		// -- Shield the ID -- //
		linCommandTable[linCommandNb].command = command;
		// ------------------- //

		linCommandTable[linCommandNb].byteNb = byteNb;
		linCommandTable[linCommandNb].direction = direction;
		linCommandTable[linCommandNb].packetPtr = dataPtr;
		linCommandTable[linCommandNb].flagPtr = flagPtr;
		// ------------------------------------------ //

		commandID = linCommandNb;		//Save the command ID
		linCommandNb++;					//Count the new command

		return commandID;
	}
	//* -- Table is full  ----- *//
	else
		return STD_EC_OVERFLOW;
	//* ----------------------- *//
}

// ---- Transfert Functions ---- //
/**
* \fn		U8 linParsePacket(U8 receivedByte)
* @brief	Receive and parse a packet, will react accronding to the Command Table
* @note
* @arg		U8 receivedData		Byte received on the RxD pin of the EUSART
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 linSlaveReceive(U8 receivedByte)
{
	U8 wu0 = 0;
	split16 localChecksum;

	//Different stage of packet reception
	switch (linSlaveState)
	{
		//* -- Sync Field ----- *//
		case idle:
		{
			LED_R = LED_OFF;
			LED_G = LED_OFF;
			LED_B = LED_OFF;
			linDataNb = 0;								//Reset the number of Data byte
			linTempChecksum.all = 0;

			if (receivedByte == LIN_SYNC_FIELD)
				linSlaveState = ID;						//Advance to the next state (ID)

			break;
		}
		//* -- ID ------------- *//
		case ID:
		{
			LED_R = LED_ON;
			// -- Check if the ID correspond to a Command -- //
			wu0 = 0;
			while (receivedByte != linCommandTable[wu0].command)
			{
				if (wu0 >= linCommandNb)
				{
					linSlaveState = idle;				//Invalid Command reset the FSM
					return STD_EC_NOTFOUND;				//Command not found return the error
				}
				wu0++;									//Check the next command
			}
			// --------------------------------------------- //


			//Save the ID of the command
			linCurrentCommand = wu0;


			// -- Set the state depending on direction -- //
			if (linCommandTable[linCurrentCommand].direction == LIN_COMMAND_DIR_OUT)
				linSlaveState = dataTx;						//Advance to the next State (dataTx)
			else if (linCommandTable[linCurrentCommand].direction == LIN_COMMAND_DIR_IN)
				linSlaveState = dataRx;						//Advance to the next State (dataRx)
			else
				return STD_EC_INVALID;
			// ------------------------------------------ //

			break;
		}
		//* -- Receive Data --- *//
		case dataRx:
		{
			linDataBuf[linDataNb] = receivedByte;		//Save the Byte into the packet buffer
			linDataNb++;								//Count the byte

			// -- Check if the packet is complete -- //
			if (linDataNb == linCommandTable[linCurrentCommand].byteNb)
				linSlaveState = checkChecksum;			//Advance to the next State (checkChecksum)
			else if (linDataNb > LIN_MAX_DATA)			//Data overflow
			{
				linSlaveState = idle;
				return STD_EC_OVERFLOW;
			}
			// ------------------------------------- //

			break;
		}
		//* -- Transmit Data -- *//
		case dataTx:
		{
			LED_G = LED_ON;

			if (eusart1_rdytosend())
			{
				eusart1SendByte(linDataBuf[linDataNb]);

				linTempChecksum.all += linDataBuf[linDataNb];

				linDataNb++;

				if (linDataNb == linCommandTable[linCurrentCommand].byteNb)
					linSlaveState = sendChecksum;
				else if (linDataNb > LIN_MAX_DATA)			//Data overflow
				{
					linSlaveState = idle;
					return STD_EC_OVERFLOW;
				}
			}
			else
				linSlaveState = idle;
			
			break;
		}
		//* -- Checksum ------- *//
		case checkChecksum:
		{
			// -- Compute the checksum -- //
			localChecksum.all = 0;
			for (wu0 = 0; wu0 < linCommandTable[linCurrentCommand].byteNb; wu0++)
			{
				localChecksum.all += linDataBuf[wu0];
				if (localChecksum.all > 0xFF)			//If the result overflow 8bit substract 0xFF from it
					localChecksum.all -= 0xFF;
			}
			// -------------------------- //


			// -- Verify the Data Validity -- //
			if ((localChecksum.lvl1 + receivedByte) == 0xFF)
			{
				for (wu0 = 0; wu0 < linCommandTable[linCurrentCommand].byteNb; wu0++)
					linCommandTable[linCurrentCommand].packetPtr[wu0] = linDataBuf[wu0];
				linCtlFlag.newPacket = SET;
				*(linCommandTable[linCurrentCommand].flagPtr) = SET;
				linNewCommandID = linCurrentCommand;
				linSlaveState = idle;
			}
			else
				return STD_EC_INVALID;
			// ------------------------------ //

			break;
		}
		//* -- Send Checksum -- *//
		case sendChecksum:
		{
			LED_B = LED_ON;
			//Compute the checksum
			linComputedChecksum = linTempChecksum.lvl1 ^ 0xFF;

			// All data byte as been sent send the Checksum
			if (eusart1_rdytosend())
				eusart1SendByte(linComputedChecksum);

			//Reset the FSM
			linMasterState = idle;

			//Raise the flag
			*(linCommandTable[linCurrentCommand].flagPtr) = SET;//Raise the complete designated flag

			break;
		}
		//* -- Invalid State -- *//
		default:
		{
			linSlaveState = idle;

			return STD_EC_INVALID;
		}
		//* ------------------- *//
	}
	// ---------------------------------- //
	return STD_EC_SUCCESS;
}

// Init Different LIN Variable
void linInit(void)
{
	linTimeoutCnt = 0;
	linNewCommandID = 0xFF;
	linCtlFlag.all = CLEAR;
	linSlaveState = idle;
	linMasterState = idle;
	LINPacketNb = 0;
}

////////////////////////////
// -- MASTER Functions -- //
////////////////////////////

// Function to send a packet as MASTER
void linSendCommand(U8 *dataPtr, U8 linCommandID, U8 byteNb, U8 commandDir, U8 *flagPtr)
{
	U8 wu0;
	split8 workID;
	split16 localChecksum = 0;

	// ----- Packet not started ----- //
	if (!linCtlFlag.inComm)
	{

		// -- Set packet constant -- //
		LINPacket.linBreak = LIN_BREAK_CHAR;
		LINPacket.sync = LIN_SYNC_FIELD;
		// ------------------------- //


		// -- Set the Command ID -- //
		//Shield the ID
		workID.all = (linCommandTable[linCommandID].command) & 0x3F;
		linShieldedID.all = workID.all;
		linShieldedID.b6 = workID.b0 ^ workID.b1 ^ workID.b2 ^ workID.b4;
		linShieldedID.b7 = (workID.b1 ^ workID.b3 ^ workID.b4 ^ workID.b5) ^ 0xFF;
		
		//Save the shielded ID
		LINPacket.ID = linShieldedID.all;
		// ------------------------ //


		// -- treat the data part -- //
		switch (commandDir)
		{
			case LIN_COMMAND_DIR_OUT:
			{
				//Buffer the command data
				for (wu0 = 0; wu0 < byteNb; wu0++)
					LINPacket.data[wu0] = dataPtr[wu0];

				//Compute the checksum
				for (wu0 = 0; wu0 < byteNb; wu0++)
				{
					localChecksum.all += linDataBuf[wu0];
					if (localChecksum.all > 0xFF)			//If the result overflow 8bit substract 0xFF from it
						localChecksum.all -= 0xFF;
				}
				LINPacket.checksum = localChecksum.lvl1;

				break;
			}
			case LIN_COMMAND_DIR_IN:
			{
				linDataPtr = dataPtr;						//Point the ptr to the correct datasaving variable
				break;
			}
		}
		// ------------------------- //
		

		// -- Save the command parameters -- //
		linCurrentCommand = linCommandID;
		linCurrentCommandByteNb = byteNb;

		//Save the dir
		linCurrentDir = commandDir;

		//Save the flag pointer
		linCurrentFlagPtr = flagPtr;
		// --------------------------------- //
		

		// -- Start the transmission -- //
		linCtlFlag.inComm = SET;							//Flag the packet
		LINPacketNb = 0;
		linCtlFlag.enTimeout = CLEAR;						//Disable the timeout

		linMasterState = idle;
		eusart1_send_break();
		TXSTA1bits.TXEN = SET;
		// ---------------------------- //
	}
	// ------------------------------- //
}

// Interrupt data receiver
U8 linDataReceiver(U8 dataReceived)
{
	U8 wu0;
	split16 localChecksum;

	if (linCtlFlag.inComm)
	{
		switch (linMasterState)
		{
			//* -- Break ---------- *//
			case idle:
			{
				//Discard
				if (dataReceived == LIN_BREAK_CHAR)
				{
					linMasterState = sync;
					linCtlFlag.step = SET;
				}
				break;
			}
			//* -- Sync ----------- *//
			case sync:
			{
				//Discard
				if (dataReceived == LIN_SYNC_FIELD)
				{
					linMasterState = ID;
					linCtlFlag.step = SET;
				}
				break;
			}
			//* -- ID ------------- *//
			case ID:
			{
				if (dataReceived == linShieldedID.all)
				{
					linCtlFlag.step = SET;
					switch (linCurrentDir)
					{
						case LIN_COMMAND_DIR_OUT:
						{
							linMasterState = dataTx;
							break;
						}
						case LIN_COMMAND_DIR_IN:
						{
							linMasterState = dataRx;

							//Start the timeout
							linCtlFlag.enTimeout = SET;
							linTimeoutCnt = 10;
							break;
						}
					}
				}
				break;
			}
			//* -- Data RX -------- *//
			case dataRx:
			{
				// -- Save the incoming data -- //
				linDataPtr[LINPacketNb] = dataReceived;
				// ---------------------------- //

				LINPacketNb++;													//Count the byte

				// Check if we received all the data byte //
				if (LINPacketNb >= linCurrentCommandByteNb)
				{
					linCtlFlag.step = SET;
					linMasterState = checkChecksum;
				}
				// ---------------------------------- //

				break;
			}
			//* -- Data TX -------- *//
			case dataTx:
			{
				//Nothing to do
				break;
			}
			//* -- CheckChecksum -- *//
			case checkChecksum:
			{
				LINPacket.checksum = dataReceived;

				// -- Verify the Data Validity -- //
				for (wu0 = 0; wu0 < linCurrentCommandByteNb; wu0++)
					localChecksum.all += linDataPtr[wu0];

				if (localChecksum.lvl1 + LINPacket.checksum == 0xFF)
					*linCurrentFlagPtr = SET;					//Set High the defined flag
				else
				// ------------------------------ //

				linMasterState = idle;
				linCtlFlag.inComm = CLEAR;

				int_eusart1tx_dis();
				break;
			}
			//* -- SendChecksum --- *//
			case sendChecksum:
			{
				//Nothing to do
				break;
			}
			//* -- Invalid State -- *//
			default:
			{
				return STD_EC_INVALID;
			}
		}
	}
	else
	{
		//Discard
		wu0 = dataReceived;
	}
	return STD_EC_SUCCESS;
}

// Lin Master FSM
U8 linMasterEngine(void)
{
	if (linCtlFlag.inComm)
	{
		if (linCtlFlag.step)
		{
			linCtlFlag.step = CLEAR;
			
			switch (linMasterState)
			{
				//* -- Break ---------- *//
				case idle:
				{
					//Nothing to do
					break;
				}
				//* -- Sync ----------- *//
				case sync:
				{
					eusart1_send_byte(LINPacket.sync);
					break;
				}
				//* -- ID ------------- *//
				case ID:
				{
					eusart1_send_byte(LINPacket.ID);
					break;
				}
				//* -- Data RX -------- *//
				case dataRx:
				{
					//Nothing
					break;
				}
				//* -- Data TX -------- *//
				case dataTx:
				{
					eusart1_send_byte(LINPacket.data[LINPacketNb]);
					LINPacketNb++;

					// Check if we sent all the data byte //
					if (LINPacketNb >= linCurrentCommandByteNb)
						linMasterState = sendChecksum;
					// ---------------------------------- //

					break;
				}
				//* -- CheckChecksum -- *//
				case checkChecksum:
				{
					//Nothing
					break;
				}
				//* -- SendChecksum --- *//
				case sendChecksum:
				{
					eusart1_send_byte(LINPacket.checksum);
					linMasterState = idle;

					linCtlFlag.inComm = CLEAR;						//Transmission finish

					*linCurrentFlagPtr = SET;						//Raise the complete designated flag

					break;
				}
				//* -- Invalid State -- *//
				default:
				{
					return STD_EC_INVALID;
				}
			}
		}
	}
	return STD_EC_SUCCESS;
}

// Lin Timeout interrupt handler
void linTimeout(void)
{
	if (linCtlFlag.enTimeout)
	{
		linTimeoutCnt--;

		if (!linTimeoutCnt)
			linCtlFlag.timeout = SET;
		else
			linCtlFlag.timeout = CLEAR;
	}
}

// Lin Timeout checker
U16 linTimeoutCheck(void)
{
	if (linCtlFlag.enTimeout)
	{
		//TIMEOUT!! Clear everything
		if (linCtlFlag.timeout)
		{
			linCtlFlag.enTimeout = CLEAR;					//Stop the timeout
			linCtlFlag.inComm = CLEAR;						//Stop the comm
			linMasterState = idle;							//Reset to idle state
			linSlaveState = idle;
			int_eusart1tx_dis();
		}
	}
	return linCtlFlag.timeout;
}

/**
* \fn		U8 linCheckStatus(void)
* @brief	Return the state of the LIN Finite State Machine
* @note		Can be used to check for new packet
* @arg		nothing
* @return	U8 linStatus		Status of the LIN Finite State Machine
*/
U8 linCheckStatus(void)
{
	return linSlaveState;
}

/**
* \fn		U8 linCheckCommand(void)
* @brief	Return the ID of the last completed command and clear the newPacket flag if so.
* @note		Must be used after being alterted of a new packet
* @arg		nothing
* @return	U8 newCommandID		Command ID of the last completed packet (return 0xFF is no new packet)
*/
U8 linCheckCommand(void)
{
	if (linCtlFlag.newPacket)
	{
		linCtlFlag.newPacket = CLEAR;
		return linNewCommandID;
	}
	else
		return 0xFF;
}
// ############################################## //
