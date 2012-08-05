/*!
 @file		pic32_dma.c
 @brief		DMA Control lib for pic32

 @version	0.1
 @note		DMA control use the physical Address not the virtual one!
 @todo		

 @date		February 16th 2012
 @author	Laurence DV
*/

// ################## Includes ################## //
#include "pic32_dma.h"
// ############################################## //


// ################## Variables ################# //
tDCHxCON * pDCHxCON = NULL;
tDCHxECON * pDCHxECON = NULL;
tDCHxINT * pDCHxINT = NULL;
U32 * pDCHxSSA = NULL;
U32 * pDCHxDSA = NULL;
tDCHxSSIZ * pDCHxSSIZ = NULL;
tDCHxDSIZ * pDCHxDSIZ = NULL;
tDCHxSPTR * pDCHxSPTR = NULL;
tDCHxDPTR * pDCHxDPTR = NULL;
tDCHxCSIZ * pDCHxCSIZ = NULL;
tDCHxCPTR * pDCHxCPTR = NULL;
tDCHxDAT * pDCHxDAT = NULL;
// ############################################## //


// ################ DMA Functions ############### //

// Channel Init

// setup normal transfert
U8 dmaSetupNormalTransfer(U32 option, U8 eventSource, U32 * sourcePtr, U32 * destinationPtr)
{

	return STD_EC_SUCCESS;
}

// setup pattern match transfert
U8 dmaSetupPatternTransfer(U32 option, U8 eventSource, U8 pattern, U32 *sourcePtr, U32 * destinationPtr)
{

	return STD_EC_SUCCESS;
}

// ############################################## //

