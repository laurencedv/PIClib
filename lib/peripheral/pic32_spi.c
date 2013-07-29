/*!
 @file		pic32_spi.c
 @brief		SPI Control lib for C32

 @version	0.1
 @note
 @todo		function pointer for different data width transfer (in the spiMasterEngine)

 @date		February 15th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_spi.h"
// ############################################## //


// ################## Variables ################# //
extern U32 heapAvailable;

//Control tables
tSPIConfig spiConfig[SPI_MAX_PORT];
tSPIStatus spiStatus[SPI_MAX_PORT];

//Reg pointers
tSPIxCON * pSPIxCON = NULL;
tSPIxCON2 * pSPIxCON2 = NULL;
tSPIxSTAT * pSPIxSTAT = NULL;
U32 * pSPIxBRG = NULL;
U32 * pSPIxBUF = NULL;

//Transaction control
tRBufCtl * spiTransactionList[SPI_MAX_TRANSACTION];
tSPITransaction * spiCurrentTransaction[SPI_MAX_PORT];
tSPITransactionState spiFSMState[SPI_MAX_PORT];
// ############################################## //


// ############# Internal Functions ############# //
/**
* \fn		U8 spiSelectPort(U8 spiPort)
* @brief	Correctly point all reg pointers for a designated SPI port
* @note		Will return STD_EC_NOTFOUND if an invalid port is given
* @arg		U8 spiPort					Hardware SPI ID
* @return	U8 errorCode					STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 spiSelectPort(U8 spiPort)
{
	switch (spiPort)
	{
	#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX1xx
		case SPI_1: pSPIxCON = (tSPIxCON*)&SPI1CON;	pSPIxCON2 = (tSPIxCON2*)&SPI1CON2;	pSPIxSTAT = (tSPIxSTAT*)&SPI1STAT; pSPIxBRG = (U32*)&SPI1BRG;	pSPIxBUF = (U32*)&SPI1BUF;	break;
		case SPI_2: pSPIxCON = (tSPIxCON*)&SPI2CON;	pSPIxCON2 = (tSPIxCON2*)&SPI2CON2;	pSPIxSTAT = (tSPIxSTAT*)&SPI2STAT; pSPIxBRG = (U32*)&SPI2BRG;	pSPIxBUF = (U32*)&SPI2BUF;	break;
	#elif CPU_FAMILY == PIC32MX3xx || CPU_FAMILY == PIC32MX4xx
		case SPI_1: pSPIxCON = (tSPIxCON*)&SPI1CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI1STAT; pSPIxBRG = (U32*)&SPI1BRG;	pSPIxBUF = (U32*)&SPI1BUF;	break;
		case SPI_2: pSPIxCON = (tSPIxCON*)&SPI2CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI2STAT; pSPIxBRG = (U32*)&SPI2BRG;	pSPIxBUF = (U32*)&SPI2BUF;	break;
	#elif CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case SPI_1: pSPIxCON = (tSPIxCON*)&SPI1CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI1STAT; pSPIxBRG = (U32*)&SPI1BRG;	pSPIxBUF = (U32*)&SPI1BUF;	break;
		case SPI_2: pSPIxCON = (tSPIxCON*)&SPI2CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI2STAT; pSPIxBRG = (U32*)&SPI2BRG;	pSPIxBUF = (U32*)&SPI2BUF;	break;
		case SPI_3: pSPIxCON = (tSPIxCON*)&SPI3CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI3STAT; pSPIxBRG = (U32*)&SPI3BRG;	pSPIxBUF = (U32*)&SPI3BUF;	break;
	#if CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
		case SPI_4: pSPIxCON = (tSPIxCON*)&SPI4CON;	pSPIxSTAT = (tSPIxSTAT*)&SPI4STAT; pSPIxBRG = (U32*)&SPI4BRG;	pSPIxBUF = (U32*)&SPI4BUF;	break;
	#endif
	#endif
		default : return STD_EC_NOTFOUND;					//Invalid SPI port ID
	}
	return STD_EC_SUCCESS;
}
// ############################################## //


// ################ SPI Functions ############### //
// === Interrupt Handler ===== //
/**
* \fn		void spiMasterEngine(U8 spiPort, U32 interruptFlags)
* @brief	SPI Transaction Engine, control the transaction system in master mode
* @note		This function must be placed in the correct ISR
*		This Function assume the Enhanced Buffer is enable
* @arg		U8 spiPort					Hardware SPI ID
* @arg		U32 interruptFlags				Interrupt flags for the corresponding SPI port (Aligned to 0)
* @return	nothing
*/
void spiMasterISR(U8 spiPort, U32 interruptFlags)
{
	U8 transferNb;
	U8 wu0;
	U8 loop = 1;
	static tSPISlaveControl * localCurrentSlavePtr;

	// -- Select the correct Port -- //
	spiSelectPort(spiPort);
	// ----------------------------- //

	// -- Finite State Machine -- //
	while (loop)
	{
		if (spiStatus[spiPort].busy)
		{
			// -- Transaction is done -- //
			if (spiCurrentTransaction[spiPort]->control.done)
			{
				//fetch a new transaction
				spiFSMState[spiPort] = SPISfetch;

				//Deselect the Slave
				(localCurrentSlavePtr->SSpinPort)[REG_OFFSET_SET_32] = localCurrentSlavePtr->SSpinMask;

				//Set the module status
				spiStatus[spiPort].busy = SPI_TRANSACTION_IDLE;
			}
			// ------------------------- //
		}

		// -- Transaction states -- //
		switch (spiFSMState[spiPort])
		{
			// == SPISfetch a new Transaction ===== //

			{
				// -- Check for pending transaction -- //
				if (rBufGetUsedSpace(spiTransactionList[spiPort]))
				{
					// Load a new transaction
					if (rBufPullElement(spiTransactionList[spiPort], (void*)(&spiCurrentTransaction[spiPort]), 1, RBUF_FREERUN_PTR) == STD_EC_BUSY)
						Nop(); // TODO : Delay the state (10ms)

					//Save the slave localy
					localCurrentSlavePtr = spiCurrentTransaction[spiPort]->pSlave;

					// Set the module status
					spiStatus[spiPort].busy = SPI_TRANSACTION_BUSY;

					// Move to initialization
					spiFSMState[spiPort] = SPISinit;
				}
				// -- No more transaction -- //
				else
				{
					// -- Close the SPI -- //
					switch (spiPort)
					{
						case SPI_1: intFastDisable(INT_SPI_1);	break;
						case SPI_2: intFastDisable(INT_SPI_2);	break;
					#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
						case SPI_3: intFastDisable(INT_SPI_3);	break;
					#endif
					#if CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
						case SPI_4: intFastDisable(INT_SPI_4);	break;
					#endif
					}
					// ------------------- //

					// Stop the loop
					loop = 0;
				}
				// ------------------------- //
				break;
			}
			// == Initialize the Transaction == //
			case SPISinit:
			{
				// -- Set the transaction Status -- //
				spiCurrentTransaction[spiPort]->control.busy = SPI_TRANSACTION_BUSY;
				// -------------------------------- //

				// Select the Slave
				(localCurrentSlavePtr->SSpinPort)[REG_OFFSET_CLR_32] = localCurrentSlavePtr->SSpinMask;

				// Move to transfer
				spiFSMState[spiPort] = SPIStransfer;

				break;
			}
			// == In Transfer ================= //
			case SPIStransfer:
			{
				// === TX State ====== //
				if (spiCurrentTransaction[spiPort]->txNbRemaining > 0)
				{
					// -- Count the remaining transfer -- //
					transferNb = (spiStatus[spiPort].FIFOlevel - pSPIxSTAT->TXBUFELM);
					if (transferNb > spiCurrentTransaction[spiPort]->txNbRemaining)
						transferNb = spiCurrentTransaction[spiPort]->txNbRemaining;
					// ---------------------------------- //

					// -- Load new data to be transmitted -- //
					switch ((spiStatus[spiPort]).FIFOlevel)
					{
						// 8bit transfer width
						case 16:
						{
							for (; wu0 < transferNb; wu0++)
								*pSPIxBUF = ((U8*)(spiCurrentTransaction[spiPort]->pTX))[wu0];
							break;
						}
						// 16bit transfer width
						case 8:
						{
							for (; wu0 < transferNb; wu0++)
								*pSPIxBUF = ((U16*)(spiCurrentTransaction[spiPort]->pTX))[wu0];
							break;
						}
						// 32bit transfer width
						case 4:
						{
							for (; wu0 < transferNb; wu0++)
								*pSPIxBUF = ((U32*)(spiCurrentTransaction[spiPort]->pTX))[wu0];
							break;
						}
					}

					//Adjust the pointer
					spiCurrentTransaction[spiPort]->pTX += (transferNb<<(pSPIxCON->MODE));
					// ------------------------------------- //

					// Adjust the tx number
					spiCurrentTransaction[spiPort]->txNbRemaining -= transferNb;
				}
				// === RX State ====== //
				if ((pSPIxSTAT->RXBUFELM) > 0)
				{
					// -- Count the transfer to make ---- //
					transferNb = pSPIxSTAT->RXBUFELM;
					// ---------------------------------- //

					// -- Save received data -- //
					wu0 = 0;
					switch (spiStatus[spiPort].FIFOlevel)
					{
						// 8bit transfer width
						case 16:
						{
							Nop();
							for (; wu0 < transferNb; wu0++)
								((U8*)(spiCurrentTransaction[spiPort]->pRX))[wu0] = *pSPIxBUF;
							
							break;
						}
						// 16bit transfer width
						case 8:
						{
							Nop();
							for (; wu0 < transferNb; wu0++)
								((U16*)(spiCurrentTransaction[spiPort]->pRX))[wu0] = *pSPIxBUF;
							break;
						}
						// 32bit transfer width
						case 4:
						{
							Nop();
							for (; wu0 < transferNb; wu0++)
								((U32*)(spiCurrentTransaction[spiPort]->pRX))[wu0] = *pSPIxBUF;
							break;
						}
					}

					// Adjust the pointer
					spiCurrentTransaction[spiPort]->pRX += (transferNb<<(pSPIxCON->MODE));
					// ------------------------ //

					// Adjust the rx number
					spiCurrentTransaction[spiPort]->rxNbDone += transferNb;

					// -- Check for completion -- //
					if (spiCurrentTransaction[spiPort]->rxNbDone >= spiCurrentTransaction[spiPort]->transferNb)
					{
						// -- Set the transaction Status -- //
						spiCurrentTransaction[spiPort]->control.busy = SPI_TRANSACTION_IDLE;
						spiCurrentTransaction[spiPort]->control.done = SPI_TRANSACTION_DONE;
						// -------------------------------- //
					}
					else
						loop = 0;						//Wait for more rx/tx interrupt
					// -------------------------- //
				}
				// === ERROR State === //
				if (interruptFlags & INT_MASK_SPI_ERR)
				{
					// -- Handle the errors -- //

					// ----------------------- //

					// Clear the local flag
					interruptFlags -= INT_MASK_SPI_ERR;
				}
				// =================== //
			}
			// ================================ //
		}
		// ------------------------ //
	}
	// -------------------------- //	
}

// =========================== //


// === Control Functions ===== //
/**
* \fn		void spiSetConfig(U8 spiPort, U64 spiLocalConfig)
* @brief	Save the configuration to the config struct of the specified SPI port
* @note		No sanity check for any settings and it does not apply those config
*			The config can also be directly access via the config struct
*			For manual configuration, option must be | or + (ex: spiSetConfig(SPI_1, SPI_MODE_MASTER|SPI_ENHANCED_BUF))
* @arg		U8 spiPort						Hardware SPI ID
* @arg		tSPIConfig spiLocalConfig		Setting to configure for the SPI
* @return	nothing
*/
void spiSetConfig(U8 spiPort, U64 spiLocalConfig)
{
	spiConfig[spiPort].all = spiLocalConfig;
}

/**
* \fn		tSPIConfig spiGetConfig(U8 spiPort)
* @brief	Return the configuration of the specified SPI port
* @note		The config can also be directly access via the config struct
* @arg		U8 spiPort						Hardware SPI ID
* @return	tSPIConfig spiLocalConfig		Setting to configure for the SPI
*/
tSPIConfig spiGetConfig(U8 spiPort)
{
	return spiConfig[spiPort];
}

/**
* \fn		U8 spiStart(U8 spiPort)
* @brief	Configure and start the selected SPI with it's corresponding config.
* @note		If any error occur during the starting, will shutdown the specified SPI port and return the error code
*		This function assume multi-vectored interrupt is enable and the ISR is correctly set in the main project file
* @arg		U8 spiPort					Hardware SPI ID
* @return	U8 errorCode					STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiStart(U8 spiPort)
{
	U16 intState;
	U8 errorCode;

	// -- Select the correct SPI -- //
	errorCode = spiSelectPort(spiPort);
	if (errorCode == STD_EC_SUCCESS)
	// ---------------------------- //
	{
		// Disable Global Interrupt
		intState = intFastDisableGlobal();

		// -- Shutdown the SPI -- //
		spiStatus[spiPort].all = 0;
		pSPIxCON->ON = 0;
		// ---------------------- //

		// -- Set the config -- //
		*pSPIxCON = spiConfig[spiPort].registers.spiCon;
		#if CPU_FAMILY == PIC32MX1xx || CPU_FAMILY == PIC32MX1xx
			*pSPIxCON2 = spiConfig[spiPort].registers.spiCon2;
		#endif
		spiStatus[spiPort].FIFOlevel = 16>>(pSPIxCON->MODE);
		// -------------------- //

		// -- Init the transactions buffer -- //
		spiTransactionList[spiPort] = rBufCreate(SPI_MAX_TRANSACTION,sizeof(tSPITransaction*));
		if (spiTransactionList[spiPort] != NULL)
		{
			spiStatus[spiPort].multiTransactionOK = 1;
			errorCode = STD_EC_MEMORY;
		}
		// ---------------------------------- //

		// -- Configure Interrupt -- //
		switch (spiPort)
		{
			case SPI_1: intFastClearFlag(INT_SPI_1);	break;
			case SPI_2: intFastClearFlag(INT_SPI_2);	break;
		#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
			case SPI_3: intFastClearFlag(INT_SPI_3);	break;
		#endif
		#if  CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
			case SPI_4: intFastClearFlag(INT_SPI_4);	break;
		#endif
		}
		// ------------------------ //

		// -- Start the SPI -- //
		pSPIxCON->ON = 1;
		spiStatus[spiPort].ready = 1;
		// ------------------- //

		// Restore Interrupt state
		intFastRestoreGlobal(intState);
	}

	return errorCode;
}

/**
* \fn		U8 spiSetBaudRate(U8 spiPort, U32 baudRate)
* @brief	Compute the correct BRG value for the desired baudrate
* @note		Will round up to the nearest possible, use spiGetBaudRate() to have the exact one.
*			Will return STD_EC_NOTFOUND if invalid timer ID is inputed.
 *			Use the actual PBCLK freq for it's computation
* @arg		U8 spiPort						Hardware SPI ID
* @arg		U32 baudRate					Desired baudrate (in bps)
* @return	U8 errorCode					STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiSetBaudRate(U8 spiPort, U32 baudRate)
{
	U8 spiSaveState;
	U8 errorCode;

	// -- Handle division by 0 -- //
	if (baudRate == 0)
		return STD_EC_INVALID;
	// -------------------------- //

	// -- Select the correct SPI -- //
	errorCode = spiSelectPort(spiPort);
	if (errorCode == STD_EC_SUCCESS)
	// ---------------------------- //
	{
		// Stop the SPI
		spiStatus[spiPort].ready = 0;
		spiSaveState = pSPIxCON->ON;
		pSPIxCON->ON = 0;

		// -- Compute the BDGEN value -- //
		*pSPIxBRG = ((U32)( ((F32)clockGetPBCLK())/(F32)baudRate ) >> 2 ) -1 ;
		// ----------------------------- //

		//Restore the SPI State
		spiStatus[spiPort].ready = 1;
		pSPIxCON->ON = spiSaveState;
	}
	return errorCode;
}

/**
* \fn		U32 spiGetBaudRate(U8 spiPort)
* @brief	Return the actual baudrate of the selected SPI
* @note		Use the actual PBCLK freq for it's computation
* @arg		U8 spiPort						Hardware SPI ID
* @return	U32 baudRate					Actual Baudrate (in bps)
*/
U32 spiGetBaudRate(U8 spiPort)
{
	U32 baudRate = 0;

	// -- Select the correct SPI -- //
	if (spiSelectPort(spiPort) == STD_EC_SUCCESS)
	// ---------------------------- //
	{
		// -- Compute the Baud rate value -- //
		baudRate = (U32)( ((F32)clockGetPBCLK())/((F32)((*pSPIxBRG +1)<<1 )) );
		// --------------------------------- //
	}
	return baudRate;
}

/**
* \fn		U8 spiAddSlave(U8 spiPort, U32 * SSpinPortPtr, U32 SSpinPortMask)
* @brief	Save communication information for a slave, return a pointer to the allocated structure
* @note		If allocation failed, will return a NULL pointer
* @arg		U8 spiPort						SPI port the Slave is connected to
* @arg		U32 * SSpinPortPtr				Pointer to the correct port register (ex: &LATA)
* @arg		U32 SSpinPortMask				Mask to apply to the port register (ex: BIT0)
* @return	tSPISlaveInfo * tempSlavePtr	Pointer to the allocated memory
*/
tSPISlaveControl * spiAddSlave(U8 spiPort, volatile U32 * SSpinPortPtr, U16 SSpinPortMask)
{
	tSPISlaveControl * tempSlavePtr;
	
	// -- Allocate Memory -- //
	tempSlavePtr = (tSPISlaveControl*)malloc(sizeof(tSPISlaveControl));
	if (tempSlavePtr != NULL)
	// --------------------- //
	{
		//Count the allocated ram
		heapAvailable -= sizeof(tSPISlaveControl);

		// -- Save data -- //
		tempSlavePtr->spiPort = spiPort;
		tempSlavePtr->SSpinMask = SSpinPortMask;
		tempSlavePtr->SSpinPort = SSpinPortPtr;
		// --------------- //

		// -- Reset SSpin to a known state -- //
		SSpinPortPtr[REG_OFFSET_SET_32] = SSpinPortMask;	//High is the idle state for a SSpin
		// ---------------------------------- //

		// -- Init the slave -- //
		tempSlavePtr->control.lock = SPI_SLAVE_UNLOCKED;
		tempSlavePtr->control.busy = SPI_SLAVE_IDLE;
		// -------------------- //
	}

	return tempSlavePtr;
}

/**
* \fn		void spiDelSlave(tSPISlaveControl * slavePtr)
* @brief	Delete all communication information for a slave
* @note		This function will not delete the slave if it is busy (will return STD_EC_BUSY)
* @arg		tSPISlaveInfo * slavePtr		Pointer to the allocated memory
* @return	U8 errorCode					STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiDelSlave(tSPISlaveControl * slavePtr)
{
	// -- Only delete if the slave is idle -- //
	if (slavePtr->control.busy == SPI_SLAVE_IDLE)
	{
		// Lock the slave
		slavePtr->control.lock = SPI_SLAVE_LOCKED;

		// -- Set SSpin to a known state -- //
		slavePtr->SSpinPort[REG_OFFSET_SET_32] = slavePtr->SSpinMask;	//High is the idle state for a SSpin
		// -------------------------------- //

		// -- Delete the information -- //
		free(slavePtr);
		heapAvailable += sizeof(tSPISlaveControl);		//Count the unallocated ram
		// ---------------------------- //
		return STD_EC_SUCCESS;
	}
	// -------------------------------------- //
	return STD_EC_BUSY;
}
// =========================== //


// === Transfer Functions ==== //
/**
* \fn		tSPITransaction * spiCreateTransaction(tSPISlaveControl * slavePtr, void * incomingPtr, void * outgoingPtr, U16 transferNb)
* @brief	Create and init an SPI Transaction control structure in the heap
* @note		This function can be usefull for single shot communication or for dynamic slave
* @arg		tSPISlaveControl * slavePtr					Slave to select for this transaction
* @arg		void * incomingPtr							Pointer to store the incoming data
* @arg		void * outgoingPtr							Pointer to load the outgoing data
* @arg		U16 transferNb								Number of transfer to do
* @return	tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
*/
tSPITransaction * spiCreateTransaction(tSPISlaveControl * slavePtr, void * incomingPtr, void * outgoingPtr, U16 transferNb)
{
	tSPITransaction * tempSPITransactionPtr = NULL;

	// -- Only process if the slave is unlocked -- //
	if (slavePtr->control.lock == SPI_SLAVE_UNLOCKED)
	{
		// -- Allocate memory -- //
		tempSPITransactionPtr = (tSPITransaction*)malloc(sizeof(tSPITransaction));
		if (tempSPITransactionPtr != NULL)
		// --------------------- //
		{
			//Count the allocated ram
			heapAvailable -= sizeof(tSPITransaction);

			// -- Save information -- //
			tempSPITransactionPtr->pRX = incomingPtr;
			tempSPITransactionPtr->pTX = outgoingPtr;
			tempSPITransactionPtr->pSlave = slavePtr;
			tempSPITransactionPtr->transferNb = transferNb;
			tempSPITransactionPtr->txNbRemaining = transferNb;
			tempSPITransactionPtr->rxNbDone = 0;
			tempSPITransactionPtr->control.all = 0;
			tempSPITransactionPtr->control.removable = SPI_TRANSACTION_REMOVABLE;	//Flag this struct as removable
			// ---------------------- //
		}
	}
	// ------------------------------------------- //
	return tempSPITransactionPtr;
}

/**
* \fn		U8 spiStartTransaction(tSPITransaction * transactionPtr)
* @brief	Add a transaction to the pending list, and activate the SPI module
* @note		Return STD_EC_BUSY if the transaction was currently busy
*			Return STD_EC_OVERFLOW if the transaction list is full
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	U8 errorCode								STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiStartTransaction(tSPITransaction * transactionPtr)
{
	U8 tempSPIPort = transactionPtr->pSlave->spiPort;
	U8 errorCode;
	U32 tempAddress = (U32*)transactionPtr;					// @todo Why don't use directly (&transactionPtr) ?

	// -- Only process if the transaction is idle -- //
	if (transactionPtr->control.busy == SPI_TRANSACTION_IDLE)
	{
		// -- Add it to the list -- //
		errorCode = rBufPushElement(spiTransactionList[tempSPIPort],(void*)(&tempAddress),1,RBUF_FREERUN_PTR);
		if (errorCode == STD_EC_SUCCESS)
		{
			transactionPtr->control.busy = SPI_TRANSACTION_BUSY;
		// ------------------------ //

			// -- Enable SPI module -- //
			if (spiStatus[tempSPIPort].ready == SPI_MODULE_READY)
			{
				switch (tempSPIPort)
				{
					case SPI_1: intFastEnable(INT_SPI_1);	break;
					case SPI_2: intFastEnable(INT_SPI_2);	break;
				#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
					case SPI_3: intFastEnable(INT_SPI_3);	break;
				#endif
				#if CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
					case SPI_4: intFastEnable(INT_SPI_4);	break;
				#endif
				}
			}
			// ----------------------- //
		}
		return errorCode;
	}
	// --------------------------------------------- //
	return STD_EC_BUSY;
}

/**
* \fn		void spiAbortTransaction(tSPITransaction * transactionPtr)
* @brief	This function flag the transaction for abortion
* @note		The abortion will be done in the SPIengine
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	nothing
*/
void spiAbortTransaction(tSPITransaction * transactionPtr)
{
	transactionPtr->control.abort = SPI_TRANSACTION_ABORTED;
	transactionPtr->control.done = SPI_TRANSACTION_DONE;
}

/**
* \fn		U8 spiRemoveTransaction(tSPITransaction * transactionPtr)
* @brief	Remove a transaction control structure from the heap
* @note		Return STD_EC_INVALID if the transaction was not in the heap
*			Return STD_EC_BUSY if the transaction was currently busy
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	U8 errorCode								STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiRemoveTransaction(tSPITransaction * transactionPtr)
{
	// -- Only process if the transaction is idle -- //
	if (transactionPtr->control.busy == SPI_TRANSACTION_IDLE)
	{
		if (transactionPtr->control.removable == SPI_TRANSACTION_REMOVABLE)
		{
			// -- Desallocate the ram -- //
			free(transactionPtr);
			heapAvailable += sizeof(tSPITransaction);				//Count the desallocated ram
			// ------------------------- //
			return STD_EC_SUCCESS;
		}
		return STD_EC_INVALID;
	}
	// --------------------------------------------- //
	return STD_EC_BUSY;
}

// =========================== //
// ############################################## //
