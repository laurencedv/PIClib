
/*!
 @file		pic18_spi.h
 @brief		SPI lib for C18 using MSSP interrupts and transactions id

 @version	0.1
 @note		

 @date		September 12th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_SPI_H
#define _PIC18_SPI_H


// ================== Include =================== //
#include <hardware.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>

// Lib
#include <peripheral/pic18_interrupt.h>
// ============================================== //


// =================== Define =================== //
// -- Application Variable -- //
#define		SPI_SLAVE_TABLE_SIZE	10						//Maximum 254
#define		SPI_BUFFER_SIZE			10
// -------------------------- //


// -- Options -- //
#define		SPI_CKP_HIGH			0b00010000
#define		SPI_CKP_LOW				0
#define		SPI_SMP_MIDDLE			0
#define		SPI_SMP_END				0b10000000
#define		SPI_CKE_ACV_TO_IDLE		0b01000000
#define		SPI_CKE_IDLE_TO_ACV		0
#define		SPI_OPEN_DRAIN_EN		0b00100000
#define		SPI_OPEN_DRAIN_DIS		0
// ------------- //


// -- Clock -- //
#define		SPI_CLK_SLAVE			0b00000100
#define		SPI_CLK_TMR2			0b00000011
#define		SPI_CLK_DIV4			0
#define		SPI_CLK_DIV8			0b00001010
#define		SPI_CLK_DIV16			0b00000001
#define		SPI_CLK_DIV64			0b00000010
// ----------- //


// -- Hardware pin -- //
// MSSP 1
#define	    SPI1_SCK				PORTBbits.RB4
#define	    SPI1_SCK_DIR			TRISBbits.TRISB4
#define	    SPI1_SDI				PORTBbits.RB5
#define	    SPI1_SDI_DIR			TRISBbits.TRISB5
#define		SPI1_SDO				PORTCbits.RC7
#define		SPI1_SDO_DIR			TRISCbits.TRISC7
#define		SPI1_SS					PORTAbits.RA5
#define		SPI1_SS_DIR				TRISAbits.TRISA5

// MSSP 2
#define	    SPI2_SCK				PORTBbits.RB4
#define	    SPI2_SCK_DIR			TRISBbits.TRISB4
#define	    SPI2_SDI				PORTBbits.RB5
#define	    SPI2_SDI_DIR			TRISBbits.TRISB5
#define		SPI2_SDO				PORTCbits.RC7
#define		SPI2_SDO_DIR			TRISCbits.TRISC7
#define		SPI2_SS					PORTAbits.RA5
#define		SPI2_SS_DIR				TRISAbits.TRISA5
// ------------------ //


// -- Variable Type -- //
// External SPI Status Flag //
typedef union
{
	U8 all;
	struct
	{
		U8 complete:1;				//Communication finished flag (0: comm in progress | 1: comm finished)
		U8 success:1;				//Completion Status (0: error encountered | 1: success! )
		U8 :6;
	};
}tSPIStatusFlag;

// Slave Table //
typedef union
{
	U8 all[6];
	struct
	{
		U16 portAdd;
		U8 portMask;
		U8 msspPort;
		tSPIStatusFlag * controlPtr;
	};
}tSPISlaveTable;

// Internal SPI Status Flag //
typedef union
{
    U8 all;
    struct
    {
		U8 finish:1;				//Flag to finish the current communication
		U8 ready:1;					//Flag telling the MSSP is ready for an other communication
		U8 :6;
    };
} tSPICtlFlag;

// Internal SPI FSM States //
typedef enum
{
	ready,
	communication,
	write_collision,
	overflow,
	stop
} tSPIMSSPstate;
// ------------------- //


// -- Registers Pointers -- //
// SSPSTAT //
typedef union
{
	U8 all;
	struct
	{
		U8 BF:1;
		U8 UA:1;
		U8 RW:1;
		U8 S:1;
		U8 P:1;
		U8 DA:1;
		U8 CKE:1;
		U8 SMP:1;
	};
}tSSPSTAT;

// SSPCON1 //
typedef union
{
	U8 all;
	struct
	{
		U8 SSPM:4;
		U8 CKP:1;
		U8 SSPEN:1;
		U8 SSPOV:1;
		U8 WCOL:1;
	};
	struct
	{
		U8 SSPM0:1;
		U8 SSPM1:1;
		U8 SSPM2:1;
		U8 SSPM3:1;
		U8 :4;
	};
}tSSPCON1;
// -------------------------- //
// ============================================== //


// ================= Prototype  ================= //
/**
* \fn		U8 spiInit(U8 port, U8 clockRate, U8 option)
* @brief	Initialise the designated MSSP ($port) with $clockRate speed and designated option
* @note
* @arg		U8 port				MSSP port ID
* @arg		U8 clockRate		Clock settings
* @arg		U8 option			Polarity options
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 spiInit(U8 port, U8 clockRate, U8 option);

/**
* \fn		U8 spiAddSlave(U8 port, U8 SSport, U8 SSpin)
* @brief	Append a Slave's SS pin into the SPISlaveTable
* @note
* @arg		U8 port				MSSP port ID
* @arg		U8 SSport			Pointer to the Port Register
* @arg		U8 SSpin			Pin number on the designated port
* @return	U8 SlaveID			ID of the newly added slave on the bus (return STD_EC_OVERFLOW if the table is full)
*/
U8 spiAddSlave(U8 port, U8 * SSport, U8 SSpin, U8 * statusPtr);

/**
* \fn		U8 spiStatus(U8 port)
* @brief	Used to check the status of the MSSP
* @note		can return any of the tSpiMSSPstate value (ready,communication,write_collision,overflow)
* @arg		U8 port				MSSP Port ID
* @return	U8 spiMSSPstate		Actual State of the MSSP
*/
U8 spiStatus(U8 port);

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
U8 spiSend(U8 * dataInPtr, U8 * dataOutPtr, U8 byteNb, U8 slaveID);

/**
* \fn		void spiHandler(U8 port)
* @brief	Interrupt hangler for SPI communication
* @note
* @arg		U8 port				MSSP port ID
* @return	nothing
*/
void spiISR(U8 port);

// ============================================== //

#endif
