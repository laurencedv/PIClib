/*!
 @file		pic32_ringBuffer.c
 @brief		Ring Buffer functions lib

 @version	0.3
 @note		All Function are Non-Blocking
 			All push/pull operation are in a FIFO manner
 			Buffers will wrap around but never write over valid data

 @todo		Disable interrupt (except IPL7) to differ parrallel task on the same buffer

 @date		March 23th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_ringBuffer.h"
// ############################################## //


// ################# Variables ################## //
tRBufCtl * tempBufCtlPtr = NULL;
extern U32 heapAvailable;
// ############################################## //


// ############ Ring Buffer Functions ########### //
// ==== Control Functions ==== //
/**
* \fn		tRBufCtl * rBufCreate(U16 elementNb, U16 elementSize)
* @brief	Create a ring buffer of the specified size in the heap, return the pointer to the control reg of the buffer
* @note		Use the control reg to access the ring buffer
*		If using an elementSize larger than 2, will round up to 4byte (ex: 6 will round up to 8)
* @arg		U16 elementNb			Total number of elements in the buffer
* @arg		U16 elementSize			Size of an element (in byte)
* @return	void * bufPtr			Pointer to the initialised buffer
*/
tRBufCtl * rBufCreate(U16 elementNb, U16 elementSize)
{
	U16 realElementSize = elementSize;

	// -- Allocate the control -- //
	tempBufCtlPtr = (tRBufCtl*) malloc(sizeof(tRBufCtl));
	if (tempBufCtlPtr == NULL)
		return NULL;						//Allocation error, return NULL
	heapAvailable -= sizeof(tRBufCtl);				//Count the allocated ram
	// -------------------------- //

	// -- Compute the correct size -- //
	if (elementSize > 2)						//Round up only if bigger than 16bit
	{
		realElementSize = elementSize>>2;			//Round up the elmentSize to 4byte
		if (elementSize & 0x3)					//If there is a rest, add 1
			realElementSize++;
		realElementSize <<= 2;
	}
	// ------------------------------ //

	// -- Allocate the buffer -- //
	tempBufCtlPtr->bufPtr = malloc(elementNb*realElementSize);	//Allocate the buffer itself
	if (tempBufCtlPtr->bufPtr == NULL)
	{
		heapAvailable += sizeof(tRBufCtl);			//Count the desallocated ram
		free(tempBufCtlPtr);					//Free the priviously allocated control reg
		return NULL;						//Allocation error, return NULL
	}
	heapAvailable -= (elementNb*realElementSize);			//Count the allocated ram
	// ------------------------- //

	// -- Init the buffer -- //
	tempBufCtlPtr->control.elementNb = elementNb;			//Save the number of element in the buffer
	tempBufCtlPtr->control.elementSize = realElementSize;		//Save the size of an element

	tempBufCtlPtr->control.end = (tempBufCtlPtr->bufPtr)+(elementNb*realElementSize);//Save the end pointer

	rBufReset(tempBufCtlPtr);
	// --------------------- //

	return tempBufCtlPtr;
}

/**
* \fn		U8 rBufResize(tRBufCtl * bufCtlPtr, U16 newElementNb)
* @brief	Resize the specified buffer to the new size
* @note		Will not resize if the new size is too small to contain the actual data
* @arg		tRBufCtl * bufCtlPtr			Buffer to resize
* @arg		U16 newElementNb			Desired new size fo the buffer (in element)
* @return	U8 errorCode				STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 rBufResize(tRBufCtl * bufCtlPtr, U16 newElementNb)
{
	U8 errorCode;

	// -- Lock the buffer -- //
	bufCtlPtr->status.writeLock = RBUF_LOCKED;
	bufCtlPtr->status.readLock = RBUF_LOCKED;
	// --------------------- //

	// -- Check if the new size is enough -- //
	if ((bufCtlPtr->control.elementNb - bufCtlPtr->status.freeElement) <= newElementNb)
	// ------------------------------------- //
	{
		// -- Resize the buffer -- //
		if (realloc(bufCtlPtr->bufPtr,(newElementNb*(bufCtlPtr->control.elementSize))) != NULL)
		{
			//Count the allocated ram
			heapAvailable -= (newElementNb - bufCtlPtr->control.elementNb)*(bufCtlPtr->control.elementSize);
		}
		else
			errorCode = STD_EC_MEMORY;			//Memory allocation error occured
		// ----------------------- //
	}
	else
		errorCode = STD_EC_TOOSMALL;

	// -- Unlock the buffer -- //
	bufCtlPtr->status.writeLock = RBUF_UNLOCKED;
	bufCtlPtr->status.readLock = RBUF_UNLOCKED;
	// --------------------- //

	return STD_EC_SUCCESS;
}


/**
* \fn		U8 rBufDelete(tRBufCtl * bufCtlPtr)
* @brief	Delete the specified Ring buffer
* @note		Warning: Will not check if there is data inside the buffer
* @arg		tRBufCtl * bufCtlPtr			Buffer to destroy
* @return	U8 errorCode				STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 rBufDelete(tRBufCtl * bufCtlPtr)
{
	// -- Lock the buffer -- //
	bufCtlPtr->status.writeLock = RBUF_LOCKED;
	bufCtlPtr->status.readLock = RBUF_LOCKED;
	// --------------------- //

	// -- Free the buffer -- //
	free(bufCtlPtr->bufPtr);
	heapAvailable += ((bufCtlPtr->control.elementNb)*(bufCtlPtr->control.elementSize));//Count the desallocated ram
	// --------------------- //

	// -- Free the control reg -- //
	free(bufCtlPtr);
	heapAvailable += sizeof(tRBufCtl);													//Count the desallocated ram
	// -------------------------- //

	return STD_EC_SUCCESS;
}

/**
* \fn		U16 rBufGetFreeSpace(tRBufCtl * bufCtlPtr)
* @brief	Return the actual free space in the buffer
* @note		The free space number is in elements.
*		The freespace can also be determine directly in the control reg (status.freeElement).
* @arg		tRBufCtl * bufPtr			Buffer to select
* @return	U16 bufSpace				Free space in the buffer (in elements)
*/
U16 rBufGetFreeSpace(tRBufCtl * bufCtlPtr)
{
	return bufCtlPtr->status.freeElement;
}

/**
* \fn		U16 rBufGetUsedSpace(tRBufCtl * bufCtlPtr)
* @brief	Return the actual used space in the buffer
* @note		The used space number is in elements.
* @arg		tRBufCtl * bufPtr			Buffer to select
* @return	U16 bufSpace				Used space in the buffer (in elements)
*/
U16 rBufGetUsedSpace(tRBufCtl * bufCtlPtr)
{
	return (bufCtlPtr->control.elementNb - bufCtlPtr->status.freeElement);
}

/**
* \fn		U16 rBufGetElementSize(tRBufCtl * bufCtlPtr)
* @brief	Return the element size of a buffer
* @note		The element size is in byte.
*		The element size can also be determine directly in the control reg (control.elementSize).
* @arg		tRBufCtl * bufPtr			Buffer to select
* @return	U16 elementSize				Element size (in byte)
*/
U16 rBufGetElementSize(tRBufCtl * bufCtlPtr)
{
	return bufCtlPtr->control.elementSize;
}

/**
* \fn		void rBufReset(tRBufCtl * bufCtlPtr)
* @brief	Reset the Ring Buffer to initial state
* @note		Fill the buffers with 0
*		WARNING: Will not check if the buffer contain valid data
* @arg		tRBufCtl * bufPtr			Buffer to select
* @return	nothing
*/
void rBufReset(tRBufCtl * bufCtlPtr)
{
	U32 wu0;

	// -- Lock the buffer -- //
	bufCtlPtr->status.writeLock = RBUF_LOCKED;
	bufCtlPtr->status.readLock = RBUF_LOCKED;
	// --------------------- //

	// -- Reset the control -- //
	bufCtlPtr->status.freeElement = bufCtlPtr->control.elementNb;	//The buffer is free
	bufCtlPtr->control.in = bufCtlPtr->bufPtr;			//Write to the first element
	bufCtlPtr->control.out = bufCtlPtr->bufPtr;			//Read from the first element
	// ----------------------- //

	// -- Refill the buffer with 0 -- //
	for (wu0 = 0; wu0 < (bufCtlPtr->control.elementNb*bufCtlPtr->control.elementSize); wu0++)
		((U8*)(bufCtlPtr->bufPtr))[wu0] = 0;			//Fill 0 everywhere
	// ------------------------------ //

	// -- Unlock the buffer -- //
	bufCtlPtr->status.writeLock = RBUF_UNLOCKED;
	bufCtlPtr->status.readLock = RBUF_UNLOCKED;
	// --------------------- //

}

// =========================== //


// === Transfer Functions ==== //
/**
* \fn		U8 rBufPushU8(tRBufCtl * bufCtlPtr, U8 * sourcePtr, U16 elementNb, U8 option)
* @brief	Push a specified number of Byte on top of the ring buffer
* @note		Return STD_EC_OVERFLOW if there is not enough space for the array without copying it to the buffer
*		Return STD_EC_TOOLARGE if the elementSize is not 1
*		Return STD_EC_BUSY if the buffer is write-locked
* @arg		tRBufCtl * bufPtr			Ring Buffer to select
* @arg		U8 * sourcePtr				Source pointer from which we take the byte
* @arg		U16 elementNb				Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 rBufPushU8(tRBufCtl * bufCtlPtr, U8 * sourcePtr, U16 elementNb, U8 option)
{
	tRBufCtl workBufCtl = *bufCtlPtr;				//Load the control reg to work localy
	U16 elementDone;

	//Check for correct size
	if (workBufCtl.control.elementSize != 1)
		return STD_EC_TOOLARGE;

	// Only process if there is enough free space in the buffer
	if (workBufCtl.status.freeElement > elementNb)
	{
		//Only process if the buffer is available
		if (bufCtlPtr->status.writeLock == RBUF_UNLOCKED)
		{
			//Lock the wrinting
			bufCtlPtr->status.writeLock = RBUF_LOCKED;

			// -- Push the elements -- //
			for (elementDone = 0; elementDone < elementNb; elementDone++)
			{
				*((U8*)workBufCtl.control.in) = *sourcePtr;

				// -- Move source pointer -- //
				if (!((tRBufFunctionOption)(option)).fixedPtr)
					sourcePtr++;
				// ------------------------- //

				// -- Check boundaries -- //
				if (workBufCtl.control.in == workBufCtl.control.end)	//Reach the end of the buffer
					workBufCtl.control.in = workBufCtl.bufPtr;	//Reset to origin
				else
					workBufCtl.control.in++;	//Move the pointer to the next U8
				// ---------------------- //
			}
			// ----------------------- //

			// -- Save the control reg -- //
			workBufCtl.status.freeElement -= elementNb;	//Decrease the free space
			*bufCtlPtr = workBufCtl;
			// -------------------------- //

			//Unlock the wrinting
			bufCtlPtr->status.writeLock = RBUF_UNLOCKED;

			return STD_EC_SUCCESS;
		}
		// Buffer locked
		return STD_EC_BUSY;
	}
	// Not enough space in the buffer
	else
		return STD_EC_OVERFLOW;
}

/**
* \fn		U8 rBufPull(tRBufCtl * bufCtlPtr)
* @brief	Pull a specified number of Byte from the bottom of the ring buffer
* @note		External size check must be done prior to calling this function, to ensure validity of the data.
*		Return STD_EC_TOOLARGE if the elementSize is not 1
*		Return STD_EC_BUSY if the buffer is read-locked
* @arg		tRBufCtl * bufCtlPtr			Ring Buffer to select
* @arg		U8 * destinationPtr			Destination pointer to which we save the byte
* @arg		U16 elementNb				Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode				STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 rBufPullU8(tRBufCtl * bufCtlPtr, U8 * destinationPtr, U16 elementNb, U8 option)
{
	tRBufCtl workBufCtl = *bufCtlPtr;				//Load the control reg to work localy
	U16 elementDone;

	//Check for correct size
	if (workBufCtl.control.elementSize != 1)
		return STD_EC_TOOLARGE;

	//Only process if the buffer is available
	if (bufCtlPtr->status.readLock == RBUF_UNLOCKED)
	{
		//Lock the reading
		bufCtlPtr->status.readLock = RBUF_LOCKED;

		// -- Pull the elements -- //
		for (elementDone = 0; elementDone < elementNb; elementDone++)
		{
			*destinationPtr = *((U8*)workBufCtl.control.out);//Retreive the byte

			// -- Move destination pointer -- //
			if (!((tRBufFunctionOption)(option)).fixedPtr)
				destinationPtr++;
			// ------------------------------ //

			// -- Check boundaries -- //
			if (workBufCtl.control.out == workBufCtl.control.end)	//Reach the end of the buffer
				workBufCtl.control.out = workBufCtl.bufPtr;	//Reset to origin
			else
				workBufCtl.control.out++;		//Move the pointer to the next U8
			// ---------------------- //
		}
		// ---------------------- //

		// -- Save the control reg -- //
		workBufCtl.status.freeElement += elementNb;		//Increase the free space
		*bufCtlPtr = workBufCtl;
		// -------------------------- //

		//Unlock the reading
		bufCtlPtr->status.readLock = RBUF_UNLOCKED;
		
		return STD_EC_SUCCESS;
	}
	// Buffer locked
	return STD_EC_BUSY;
}

/**
* \fn		U8 rBufPushElement(tRBufCtl * bufCtlPtr, void * sourcePtr, U16 elementNb, U8 option)
* @brief	Push a specified number of elements into the top of the buffer from sourcePtr
* @note		The element size are assume correct for the buffer
*		Return STD_EC_TOOSMALL if the elementSize is smaller than 4
*		Return STD_EC_OVERFLOW if there is not enough space for the array (does not copy anything to the buffer)
*		Return STD_EC_BUSY if the buffer is write-locked
* @arg		tRBufCtl * bufCtlPtr			Ring Buffer to select
* @arg		void * sourcePtr			Array to save to the buffer
* @arg		U16 elementNb				Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode				STD Error Code
*/
U8 rBufPushElement(tRBufCtl * bufCtlPtr, void * sourcePtr, U16 elementNb, U8 option)
{
	tRBufCtl workBufCtl = *bufCtlPtr;				//Load the control reg to work localy
	U32 loopDone;							//Reset the loop count
	U32 loopNeeded;

	//Check for correct size
	if (workBufCtl.control.elementSize < 4)
		return STD_EC_TOOSMALL;

	//Only push if there is enough free space
	if (workBufCtl.status.freeElement >= elementNb)
	{
		//Only process if the buffer is available
		if (bufCtlPtr->status.writeLock == RBUF_UNLOCKED)
		{
			//Lock the wrinting
			bufCtlPtr->status.writeLock = RBUF_LOCKED;

			// -- Count the loop number -- //
			loopNeeded = elementNb*(workBufCtl.control.elementSize>>2);
			// --------------------------- //

			// -- Push the elements -- //
			for (loopDone = 0; loopDone < loopNeeded; loopDone++)
			{
				//Push one U32
				*((U32*)workBufCtl.control.in) = *((U32*)sourcePtr);

				// -- Move source pointer -- //
				if (!((tRBufFunctionOption)(option)).fixedPtr)
					sourcePtr += 4;
				// ------------------------- //

				// -- Check boundary -- //
				if (workBufCtl.control.in == workBufCtl.control.end)	//Reach the end of the buffer
					workBufCtl.control.in = workBufCtl.bufPtr;	//Reset to origin
				else
					workBufCtl.control.in += 4;	//Move the pointer to the next U32
				// -------------------- //
			}
			// ----------------------- //

			// -- Save the control reg -- //
			workBufCtl.status.freeElement -= elementNb;	//Update the free Space
			*bufCtlPtr = workBufCtl;
			// -------------------------- //

			//Unlock the wrinting
			bufCtlPtr->status.writeLock = RBUF_UNLOCKED;

			return STD_EC_SUCCESS;
		}
		//Buffer locked
		return STD_EC_BUSY;
	}
	else
		return STD_EC_OVERFLOW;
}

/**
* \fn		U8 rBufPullElement(tRBufCtl * bufCtlPtr, void * destinationPtr, U16 elementNb, U8 option)
* @brief	Pull a specified number of elements from to bottom of the buffer and place it in the destinationPtr
* @note		The destination is assumed to be large enough
*		Return STD_EC_TOOSMALL if the elementSize is smaller than 4
*		Return STD_EC_BUSY if the buffer is read-locked
*		External size check must be done prior to calling this function, to ensure the validity of the data.
* @arg		tRBufCtl * bufCtlPtr			Ring Buffer to select
* @arg		void * destinationPtr			Destination to save the data from the buffer
* @arg		U16 elementNb				Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode				STD Error Code
*/
U8 rBufPullElement(tRBufCtl * bufCtlPtr, void * destinationPtr, U16 elementNb, U8 option)
{
	tRBufCtl workBufCtl = *bufCtlPtr;				//Load the control reg to work localy
	U32 loopDone;							//Reset the loop count
	U32 loopNeeded;							//Number of loop needed

	//Check for correct size
	if (workBufCtl.control.elementSize < 4)
		return STD_EC_TOOSMALL;

	//Only process if the buffer is available
	if (bufCtlPtr->status.readLock == RBUF_UNLOCKED)
	{
		//Lock the reading
		bufCtlPtr->status.readLock = RBUF_LOCKED;

		// -- Count the loop number -- //
		loopNeeded = elementNb * (workBufCtl.control.elementSize>>2);
		// --------------------------- //

		// -- Pull the elements -- //
		for (loopDone = 0; loopDone < loopNeeded; loopDone++)
		{
			//Pull one U32
			*((U32*)destinationPtr) = *((U32*)workBufCtl.control.out);

			// -- Move source pointer -- //
			if (!((tRBufFunctionOption)(option)).fixedPtr)
				destinationPtr += 4;
			// ------------------------- //

			// -- Check boundary -- //
			if (workBufCtl.control.out == workBufCtl.control.end)	//Reach the end of the buffer
				workBufCtl.control.out = workBufCtl.bufPtr;	//Reset to origin
			else
				workBufCtl.control.out += 4;		//Move the pointer to the next U32
			// -------------------- //
		}
		// ----------------------- //

		// -- Save the control reg -- //
		workBufCtl.status.freeElement += elementNb;		//Update the free Space
		*bufCtlPtr = workBufCtl;
		// -------------------------- //

		//Unlock the reading
		bufCtlPtr->status.readLock = RBUF_UNLOCKED;

		return STD_EC_SUCCESS;
	}
	//Buffer locked
	return STD_EC_BUSY;
}
// ============================ //

// ############################################## //
