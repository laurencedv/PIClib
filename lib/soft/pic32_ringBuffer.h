/*!
 @file		pic32_ringBuffer.h
 @brief		Ring Buffer functions lib

 @version	0.3
 @note		All Function are Non-Blocking
 			All push/pull operation are in a FIFO manner
 			Buffers will wrap around but never write over valid data

 @todo		Disable interrupt (except IPL7) to differ parrallel task on the same buffer

 @date		March 23th 2011
 @author	Laurence DV
*/

#ifndef _PIC32_RINGBUFFER_H
#define _PIC32_RINGBUFFER_H 1
// ################## Includes ################## //
// Lib
#include <stdlib.h>
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

//Dev Macro
#include <tool/splitvar_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// Function Option //
#define RBUF_FIXED_PTR			1
#define RBUF_FREERUN_PTR		0
// --------------- //

// Buffer Status //
#define RBUF_LOCKED				1
#define RBUF_UNLOCKED			0
// ------------- //
// ############################################## //


// ################# Data Type ################## //
// Control Part //
typedef struct
{
	struct
	{
		U16 writeLock:1;
		U16 readLock:1;
		U16 :14;
		U16 freeElement;		//Free space available in the buffer (in elements)
	}status;

	struct
	{
		U16 elementSize;		//Size of an element (in byte)
		U16 elementNb;			//Total number of elements in the buffer	
		void * in;				//Writing pointer
		void * out;				//Reading pointer
		void * end;				//End pointer
	}control;

	void * bufPtr;				//Buffer pointer (in heap)
}tRBufCtl;
// ----------- //

// Function option
typedef union
{
	U8 all;
	struct
	{
		U8 fixedPtr:1;					//Disable the destination/source ptr movement (usefull for HW buffer operation)
		U8 :7;
	};
}tRBufFunctionOption;
// ############################################## //


// ################# Prototypes ################# //
// ==== Control Functions ==== //
/**
* \fn		tRBufCtl * rBufCreate(U16 elementNb, U16 elementSize)
* @brief	Create a ring buffer of the specified size in the heap, return the pointer to the control reg of the buffer
* @note		Use the control reg to access the ring buffer
*			If using an elementSize larger than 2, will round up to 4byte (ex: 6 will round up to 8)
* @arg		U16 elementNb		Total number of elements in the buffer
* @arg		U16 elementSize		Size of an element (in byte)
* @return	void * bufPtr		Pointer to the initialised buffer
*/
tRBufCtl * rBufCreate(U16 elementNb, U16 elementSize);

/**
* \fn		U8 rBufResize(tRBufCtl * bufCtlPtr, U16 newElementNb)
* @brief	Resize the specified buffer to the new size
* @note		Will not resize if the new size is too small to contain the actual data
* @arg		tRBufCtl * bufCtlPtr	Buffer to resize
* @arg		U16 newElementNb		Desired new size fo the buffer (in element)
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 rBufResize(tRBufCtl * bufCtlPtr, U16 newElementNb);

/**
* \fn		U8 rBufDelete(tRBufCtl * bufCtlPtr)
* @brief	Delete the specified Ring buffer
* @note		Warning: Will not check if there is data inside the buffer
* @arg		tRBufCtl * bufCtlPtr	Buffer to destroy
* @return	U8 errorCode			STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 rBufDelete(tRBufCtl * bufCtlPtr);

/**
* \fn		U32 rBufGetFreeSpace(tRBufCtl * bufCtlPtr)
* @brief	Return the actual free space in the buffer
* @note		The free space number is in elements.
*			The freespace can also be determine directly in the control reg (control.freeSpace).
* @arg		tRBufCtl * bufPtr		Buffer to select
* @return	U16 bufSpace			Free space in the buffer (in elements)
*/
U16 rBufGetFreeSpace(tRBufCtl * bufCtlPtr);

/**
* \fn		U16 rBufGetUsedSpace(tRBufCtl * bufCtlPtr)
* @brief	Return the actual used space in the buffer
* @note		The used space number is in elements(byte).
* @arg		tRBufCtl * bufPtr			Buffer to select
* @return	U16 bufSpace				Used space in the buffer (in elements)
*/
U16 rBufGetUsedSpace(tRBufCtl * bufCtlPtr);

/**
* \fn		U16 rBufGetElementSize(tRBufCtl * bufCtlPtr)
* @brief	Return the element size of a buffer
* @note		The element size is in byte.
*			The element size can also be determine directly in the control reg (control.elementSize).
* @arg		tRBufCtl * bufPtr		Buffer to select
* @return	U16 elementSize			Element size (in byte)
*/
U16 rBufGetElementSize(tRBufCtl * bufCtlPtr);

/**
* \fn		void rBufReset(tRBufCtl * bufCtlPtr)
* @brief	Reset the Ring Buffer to initial state
* @note		Fill the buffers with 0
*			WARNING: Will not check if the buffer contain valid data
* @arg		tRBufCtl * bufPtr		Buffer to select
* @return	nothing
*/
void rBufReset(tRBufCtl * bufCtlPtr);
// =========================== //


// === Transfer Functions ==== //
/**
* \fn		U8 rBufPushU8(tRBufCtl * bufCtlPtr, U8 * sourcePtr, U16 elementNb, U8 option)
* @brief	Push a specified number of Byte on top of the ring buffer
* @note		Return STD_EC_OVERFLOW if there is not enough space for the array without copying it to the buffer
*			Return STD_EC_TOOLARGE if the elementSize is not 1
*			Return STD_EC_BUSY if the buffer is write-locked
* @arg		tRBufCtl * bufPtr		Ring Buffer to select
* @arg		U8 * sourcePtr			Source pointer from which we take the byte
* @arg		U16 elementNb			Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 rBufPushU8(tRBufCtl * bufCtlPtr, U8 * sourcePtr, U16 elementNb, U8 option);

/**
* \fn		U8 rBufPull(tRBufCtl * bufCtlPtr)
* @brief	Pull a specified number of Byte from the bottom of the ring buffer
* @note		External size check must be done prior to calling this function, to ensure validity of the data.
*			Return STD_EC_TOOLARGE if the elementSize is not 1
*			Return STD_EC_BUSY if the buffer is read-locked
* @arg		tRBufCtl * bufCtlPtr	Ring Buffer to select
* @arg		U8 * destinationPtr		Destination pointer to which we save the byte
* @arg		U16 elementNb			Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode			STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 rBufPullU8(tRBufCtl * bufCtlPtr, U8 * destinationPtr, U16 elementNb, U8 option);

/**
* \fn		U8 rBufPushElement(tRBufCtl * bufCtlPtr, void * sourcePtr, U16 elementNb, U8 option)
* @brief	Push a specified number of elements into the top of the buffer from sourcePtr
* @note		The element size are assume correct for the buffer
*			Return STD_EC_TOOSMALL if the elementSize is smaller than 4
*			Return STD_EC_OVERFLOW if there is not enough space for the array (does not copy anything to the buffer)
*			Return STD_EC_BUSY if the buffer is write-locked
* @arg		tRBufCtl * bufCtlPtr	Ring Buffer to select
* @arg		void * sourcePtr		Array to save to the buffer
* @arg		U16 elementNb			Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode			STD Error Code
*/
U8 rBufPushElement(tRBufCtl * bufCtlPtr, void * sourcePtr, U16 elementNb, U8 option);

/**
* \fn		U8 rBufPullElement(tRBufCtl * bufCtlPtr, void * destinationPtr, U16 elementNb, U8 option)
* @brief	Pull a specified number of elements from to bottom of the buffer and place it in the destinationPtr
* @note		The destination is assumed to be large enough
*			Return STD_EC_TOOSMALL if the elementSize is smaller than 4
*			Return STD_EC_BUSY if the buffer is read-locked
*			External size check must be done prior to calling this function, to ensure the validity of the data.
* @arg		tRBufCtl * bufCtlPtr	Ring Buffer to select
* @arg		void * destinationPtr	Destination to save the data from the buffer
* @arg		U16 elementNb			Number of element to save
* @arg		U8 option				Special option (refer to define Function Options)
* @return	U8 errorCode			STD Error Code
*/
U8 rBufPullElement(tRBufCtl * bufCtlPtr, void * destinationPtr, U16 elementNb, U8 option);
// ============================ //
// ############################################## //


#endif
