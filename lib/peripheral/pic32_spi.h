/*!
 @file		pic32_uart.h
 @brief		Uart Control lib for C32

 @version	0.1
 @note		

 @date		February 8th 2011
 @author	Laurence DV
*/


#ifndef _PIC32_SPI_H
#define _PIC32_SPI_H 1

// ################## Includes ################## //
// Hardware
#include <hardware.h>

// Librairies
#include <soft/pic32_ringBuffer.h>
#include <peripheral/pic32_clock.h>
#include <peripheral/pic32_interrupt.h>

// Definition
#include <definition/stddef_megaxone.h>
#include <definition/datatype_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>
#include <tool/bitmanip_megaxone.h>
// ############################################## //


// ################## Defines ################### //
// == Application dependant  == //
#define SPI_MAX_TRANSACTION			10		//Maximum number of pending transaction
// ============================ //


// ==== Init Option ==== //
#define	SPI_MODE_MASTER				(BIT5)
#define SPI_MODE_SLAVE				0

#define SPI_IDLE_RUN				0
#define SPI_IDLE_STOP				(BIT13)

#define SPI_NORMAL_BUF				0
#define SPI_ENHANCED_BUF			(BIT16)

//Normal mode data width
#define SPI_DATA_WIDTH_8BIT			0
#define SPI_DATA_WIDTH_16BIT			(BIT10)
#define SPI_DATA_WIDTH_32BIT			(BIT11)		//Important to be 0b10 not 0b11

// -- Audio mode control -- //
#define SPI_AUDIO_ON				(BIT39)
#define	SPI_AUDIO_OFF				0

//Audio data format
#define SPI_AUDIO_STEREO			0
#define SPI_AUDIO_MONO				(BIT35)

//Audio protocol
#define SPI_AUDIO_I2S				0
#define SPI_AUDIO_LEFT_JUSTIFIED		(BIT32)
#define SPI_AUDIO_RIGHT_JUSTIFIED		(BIT33)
#define SPI_AUDIO_PCM				(BIT32|BIT33)

//Audio mode data width
#define SPI_DATA_AUD_16BIT_16CH_32FRM		0
#define SPI_DATA_AUD_16BIT_32CH_64FRM		(BIT10)
#define SPI_DATA_AUD_32BIT_32CH_64FRM		(BIT11)
#define SPI_DATA_AUD_24BIT_32CH_64FRM		(BIT10|BIT11)
// ------------------------ //

//Data input phase
#define SPI_RX_DATA_AT_END			(BIT9)
#define SPI_RX_DATA_AT_MIDDLE			0
#define SPI_TX_DATA_ACTIVE_TO_IDLE		(BIT8)
#define SPI_TX_DATA_IDLE_TO_ACTIVE		0

//Clock control
#define SPI_CLK_IDLE_HIGH			(BIT6)
#define SPI_CLK_IDLE_LOW			0
#define SPI_CLK_SOURCE_PBCLK			0
#define SPI_CLK_SOURCE_MCLK			(BIT23)

//Tx buffer interrupt selection
#define SPI_TX_BUF_INT_SPISR_EMPTY		0
#define SPI_TX_BUF_INT_BUF_EMPTY		(BIT2)
#define SPI_TX_BUF_INT_BUF_HALF_EMPTY		(BIT3)
#define SPI_TX_BUF_INT_BUF_NOT_FULL		(BIT2|BIT3)

//Rx buffer interrupt selection
#define SPI_RX_BUF_INT_BUF_EMPTY		0
#define SPI_RX_BUF_INT_BUF_NOT_EMPTY		(BIT0)
#define SPI_RX_BUF_INT_BUF_HALF_FULL		(BIT1)
#define SPI_RX_BUF_INT_BUF_FULL			(BIT0|BIT1)

//Interrupt control
#define SPI_INT_UNDERRUN_ON			(BIT42)
#define SPI_INT_UNDERRUN_OFF			0
#define SPI_INT_OVERFLOW_ON			(BIT43)
#define SPI_INT_OVERFLOW_OFF			0
#define SPI_INT_FRAME_ERR_ON			(BIT44)
#define SPI_INT_FRAME_ERR_OFF			0
#define SPI_INT_IGNORE_UNDERRUN			(BIT40)
#define SPI_INT_IGNORE_OVERFLOW			(BIT41)

//Pin control
#define SPI_PIN_SDI_ON				0
#define SPI_PIN_SDI_OFF				(BIT4)
#define SPI_PIN_SDO_ON				0
#define SPI_PIN_SDO_OFF				(BIT12)
#define SPI_PIN_SS_ON				(BIT7|BIT28)
#define SPI_PIN_SS_OFF				0

//Frame control
#define SPI_FRAME_ON				(BIT31)
#define SPI_FRAME_OFF				0

#define SPI_PULSE_EACH_DATA			0
#define SPI_PULSE_EACH_2_DATA			(BIT24)
#define SPI_PULSE_EACH_4_DATA			(BIT25)
#define SPI_PULSE_EACH_8_DATA			(BIT24|BIT25)
#define SPI_PULSE_EACH_16_DATA			(BIT26)
#define SPI_PULSE_EACH_32_DATA			(BIT26|BIT24)

#define SPI_PULSE_1_CLK_WIDE			0
#define SPI_PULSE_1_WORD_WIDE			(BIT27)

#define SPI_PULSE_ACTIVE_LOW			0
#define SPI_PULSE_ACTIVE_HIGH			(BIT29)

#define SPI_PULSE_OUTPUT			0
#define SPI_PULSE_INPUT				(BIT30)

#define SPI_PULSE_BEFORE_CLOCK			0
#define SPI_PULSE_WITH_CLOCK			(BIT17)
// ===================== //

// == Slave Status == //
#define SPI_SLAVE_LOCKED			1
#define SPI_SLAVE_UNLOCKED			0
#define	SPI_SLAVE_BUSY				1
#define SPI_SLAVE_IDLE				0
// ================== //

// == SPI Transaction Status == //
#define SPI_TRANSACTION_REMOVABLE		1
#define SPI_TRANSACTION_ABORTED			1
#define SPI_TRANSACTION_BUSY			1
#define SPI_TRANSACTION_IDLE			0
#define SPI_TRANSACTION_DONE			1
// ============================ //

// == SPI Module Statue == //
#define SPI_MODULE_READY			1
#define SPI_MODULE_NOT_READY			0
// ======================= //

// == SPI HW ID == //
#define SPI_1					0
#define SPI_2					1
#if CPU_FAMILY == PIC32MX5xxH || CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
	#define SPI_3				2
#endif
#if CPU_FAMILY == PIC32MX5xxL || CPU_FAMILY == PIC32MX6xx || CPU_FAMILY == PIC32MX7xx
	#define SPI_4				3
#endif
// =============== //
// ############################################## //


// ################# Data Type ################## //
// -- Registers pointer -- //
// SPIxCON reg
typedef union
{
	U32 all;
	struct
	{
		U32 SRXISEL:2;			//Rx Interrupt selection
		U32 STXISEL:2;			//Tx Interrupt selection
		U32 DISSDI:1;			//SDI pin Disable
		U32 MSTEN:1;			//Master mode Enable
		U32 CKP:1;			//Clock polarity control
		U32 SSEN:1;			//Slave Select Enable
		U32 CKE:1;			//Clock edge control
		U32 SMP:1;			//Rx Sample phase control
		U32 MODE:2;			//Data size selection
		U32 DISSDO:1;			//SDO pin Disable
		U32 SIDL:1;			//Idle state control
		U32 :1;
		U32 ON:1;			//SPI Enable
		U32 ENHBUF:1;			//Enhanced Buffer Enable
		U32 SPIFE:1;			//Frame Sync Pulse edge selection
		U32 :5;
		U32 MCLKSEL:1;			//Clock selection
		U32 FRMCNT:3;			//Frame Sync Pulse counter
		U32 FRMSYPW:1;			//Frame Sync Pulse width control
		U32 MSSEN:1;			//Master mode SS Enable
		U32 FRMPOL:1;			//Frame Sync Pulse polarity control
		U32 FRMSYNC:1;			//Frame Sync Pulse direction control
		U32 FRMEN:1;			//Frame SPI Enable
	};
}tSPIxCON;

// SPIxCON2 reg
typedef union
{
	U32 all;
	struct
	{
		U32 AUDMOD:2;			//Audio Protocol selection
		U32 :1;
		U32 AUDMONO:1;			//Audio data format control
		U32 :3;
		U32 AUDEN:1;			//Audio Protocol Enable
		U32 IGNTUR:1;			//Ignore Tx underrun control
		U32 IGNROV:1;			//Ignore Rx overflow control
		U32 SPITUREN:1;			//Tx underrun interrupt Enable
		U32 SPIROVEN:1;			//Rx overflow interrupt Enable
		U32 FRMERREN:1;			//Frame error interrupt Enable
		U32 :2;
		U32 SPISGNEXT:1;		//Sign extend Rx Data control
	};
}tSPIxCON2;

// SPIxSTAT reg
typedef union
{
	U32 all;
	struct
	{
		U32 SPIRBF:1;			//Rx Buffer full flag
		U32 SPITBF:1;			//Tx Buffer full flag
		U32 :1;
		U32 SPITBE:1;			//Tx Buffer empty flag
		U32 :1;
		U32 SPIRBE:1;			//Rx Buffer empty flag
		U32 SPIROV:1;			//Rx overflow flag
		U32 SRMT:1;			//Shift register empty flag
		U32 SPITUR:1;			//Transmit underrun flag
		U32 :2;
		U32 SPIBUSY:1;			//SPI Activity flag
		U32 FRMERR:1;			//Frame error flag
		U32 :3;
		U32 TXBUFELM:5;			//Number of element in Tx Buffer
		U32 :3;
		U32 RXBUFELM:5;			//Number of element in Rx Buffer
		U32 :3;
	};
}tSPIxSTAT;
// ----------------------- //

// -- Control and Status -- //
// Config format
typedef union
{
	U64 all;
	struct
	{
		U32 :6;
		U32 polarity:1;			//Clock polarity Selection
		U32 :16;
		U32	source:1;		//Clock source Selection
		U32 :8;
		U32 :32;
	}clock;

	struct
	{
		U32 :17;
		U32 syncPulseEdge:1;		//Sync pulse edge Selection
		U32 :6;
		U32 syncPulseCnt:3;		//Sync pulse counter
		U32 syncPulseWidth:1;		//Sync pulse width Selection
		U32 :1;
		U32 syncPulsePol:1;		//Sync pulse polarity Selection
		U32 syncPulseDir:1;		//Sync pulse direction Selection
		U32 enable:1;			//SPI frame mode Enable
		U32 :32;
	}frame;

	struct
	{
		U32 :4;
		U32 SDI:1;			//SDI Enable
		U32 :2;
		U32 SlaveSS:1;			//Slave mode SS Enable
		U32 :4;
		U32 SDO:1;			//SDO Enable
		U32 :15;
		U32 MasterSS:1;			//Master mode SS Enable
		U32 :3;
		U32 :32;
	}pin;

	struct
	{
		U32 rxBuffer:2;			//Rx buffer interrupt Selection
		U32 txBuffer:2;			//Tx buffer interrupt Selection
		U32 :28;
		U32 :8;
		U32 ignoreUdr:1;		//Ignore undderrun interrupt Enable
		U32 ignoreOvf:1;		//Ignore overflow interrupt Enable
		U32 bufferUdr:1;		//Tx undderun interrupt Enable
		U32 bufferOvf:1;		//Rx overflow interrupt Enable
		U32 frameErr:1;			//Frame error interrupt Enable
		U32 :19;
	}interrupts;

	struct
	{
		U32 :8;
		U32 outEdge:1;			//Data transmission edge Selection
		U32 inPhase:1;			//Data reception phase Selection
		U32 width:2;			//Data width
		U32 :20;
		U32 :32;
	}data;

	struct
	{
		U32 :32;
		U32 protocol:2;			//Audio protocol Selection
		U32 :1;
		U32 dataFormat:1;		//Audio data format Selection
		U32 :3;
		U32 enable:1;			//SPI audio mode Enable
		U32 :7;
		U32 signExtend:1;		//Extend sign to the rx data Enable
		U32 :16;
	}audio;

	struct
	{
		U32 :5;
		U32 mode:1;			//Master/Slave selection
		U32 :7;
		U32 idleState:1;		//SPI module state in idle mode Selection
		U32 :2;
		U32 enhancedBuf:1;		//Enhanced buffering mode Enable
		U32 :15;
		U32 :32;
	};

	struct
	{
		tSPIxCON spiCon;		//SPIxCON register alignement
		tSPIxCON2 spiCon2;		//SPIxCON2 register alignement
	}registers;
}tSPIConfig;

// Status format
typedef union
{
	U32 all;
	struct
	{
		U32 multiTransactionOK:1;	//Multi transaction system initialized	
		U32 FIFOlevel:5;		//Number of level of the HW FIFO
		U32 :24;
		U32 busy:1;			//Currently in transaction flag
		U32 ready:1;			//General ready flag
	};
}tSPIStatus;

// Slave informations format
typedef union
{
	U64 all;
	struct
	{
		union
		{
			U8 all;
			struct
			{
				U8 busy:1;	//Slave is busy flag
				U8 lock:1;	//Slave is locked flag
				U8 :7;
			};
		}control;
		U8 spiPort;			//SPI port to which the Slave is attached
		U16 SSpinMask;			//Mask to apply to that Port reg
		volatile U32 * SSpinPort;	//Address of the Port reg the SS pin is on
	};
}tSPISlaveControl;

// Transaction format
typedef struct
{
	void * pRX;				//Receiving data pointer
	void * pTX;				//Transmitting data pointer
	U16 transferNb;				//Total number of transfer
	U16 rxNbDone;				//Number of reception done
	U16 txNbRemaining;			//Number of transmission done
	tSPISlaveControl * pSlave;		//Selected Slave control pointer
	
	union
	{
		U8 all;
		struct
		{
			U8 remove:1;		//This transaction information must be removed after completion
			U8 :2;
			U8 removable:1;		//This structure can be remove
			U8 done:1;		//This transaction is done
			U8 abort:1;		//Abort this transaction
			U8 busy:1;		//This transaction is currently active
			U8 error:1;		//This transaction had an error
		};
	}control;
}tSPITransaction;

// Transaction FSM states
typedef enum
{
	SPISinit,
	SPISfetch,
	SPIStransfer
}tSPITransactionState;
// ------------------------ //
// ############################################## //


// ################# Prototypes ################# //
// === Interrupt Handler ===== //
/**
* \fn		void spiMasterEngine(U8 spiPort, U32 interruptFlags)
* @brief	SPI Transaction Engine, control the transaction system in master mode
* @note		This function must be placed in the correct ISR
*			This Function assume the Enhanced Buffer is enable
* @arg		U8 spiPort						Hardware SPI ID
* @arg		U32 interruptFlags				Interrupt flags for the corresponding SPI port (Aligned to 0)
* @return	nothing
*/
void spiMasterEngine(U8 spiPort, U32 interruptFlags);
// =========================== //


// === Control Functions ==== //
/**
* \fn		void spiSetConfig(U8 spiPort, U64 spiLocalConfig)
* @brief	Save the configuration to the config struct of the specified SPI port
* @note		No sanity check for any settings and it does not apply those config
*			The config can also be directly access via the config struct
*			For manual configuration, option must be | or + (ex: spiSetConfig(SPI_1, SPI_MODE_MASTER|SPI_ENHANCED_BUF))
* @arg		U8 spiPort					Hardware SPI ID
* @arg		tSPIConfig spiLocalConfig	Setting to configure for the SPI
* @return	nothing
*/
void spiSetConfig(U8 spiPort, U64 spiLocalConfig);

/**
* \fn		tSPIConfig spiGetConfig(U8 spiPort)
* @brief	Return the configuration of the specified SPI port
* @note		The config can also be directly access via the config struct
* @arg		U8 spiPort					Hardware SPI ID
* @return	tSPIConfig spiLocalConfig	Setting to configure for the SPI
*/
tSPIConfig spiGetConfig(U8 spiPort);

/**
* \fn		U8 spiStart(U8 spiPort)
* @brief	Configure and start the selected SPI with it's corresponding config.
* @note		If any error occur during the init, will shutdown the specified SPI port and return the error code
*			This function enable the corresponding interrupt, priority must be set in the main
*			and multi-vectored interrupt mode be enabled
* @arg		U8 spiPort			Hardware SPI ID
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiStart(U8 spiPort);

/**
* \fn		U8 spiSetBaudRate(U8 spiPort, U32 baudRate)
* @brief	Compute the correct BRG value for the desired baudrate
* @note		Will round up to the nearest possible, use spiGetBaudRate() to have the exact one.
*			Will return STD_EC_NOTFOUND if invalid timer ID is inputed.
 *			Use the actual PBCLK freq for it's computation
* @arg		U8 spiPort			Hardware SPI ID
* @arg		U32 baudRate		Desired baudrate (in bps)
* @return	U8 errorCode		STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiSetBaudRate(U8 spiPort, U32 baudRate);

/**
* \fn		U32 spiGetBaudRate(U8 spiPort)
* @brief	Return the actual baudrate of the selected SPI
* @note		Use the actual PBCLK freq for it's computation
* @arg		U8 spiPort			Hardware SPI ID
* @return	U32 baudRate		Actual Baudrate (in bps)
*/
U32 spiGetBaudRate(U8 spiPort);



/**
* \fn		U8 spiAddSlave(U8 spiPort, U32 * SSpinPortPtr, U32 SSpinPortMask)
* @brief	Save communication information for a slave, return a pointer to the allocated structure
* @note		If allocation failed, will return a NULL pointer
* @arg		U8 spiPort						SPI port the Slave is connected to
* @arg		U32 * SSpinPortPtr				Pointer to the correct port register (ex: &LATA)
* @arg		U32 SSpinPortMask				Mask to apply to the port register (ex: BIT0)
* @return	tSPISlaveInfo * tempSlavePtr	Pointer to the allocated memory
*/
tSPISlaveControl * spiAddSlave(U8 spiPort, volatile U32 * SSpinPortPtr, U16 SSpinPortMask);

/**
* \fn		void spiDelSlave(tSPISlaveControl * slavePtr)
* @brief	Delete all communication information for a slave
* @note		This function will not delete the slave if it is busy (will return STD_EC_BUSY)
* @arg		tSPISlaveInfo * slavePtr		Pointer to the allocated memory
* @return	U8 errorCode					STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiDelSlave(tSPISlaveControl * slavePtr);
// ========================== //


// === Transfer Function ==== //
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
tSPITransaction * spiCreateTransaction(tSPISlaveControl * slavePtr, void * incomingPtr, void * outgoingPtr, U16 transferNb);

/**
* \fn		U8 spiStartTransaction(tSPITransaction * transactionPtr)
* @brief	Add a transaction to the pending list, and activate the SPI module
* @note		Return STD_EC_BUSY if the transaction was currently busy
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	U8 errorCode								STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiStartTransaction(tSPITransaction * transactionPtr);

/**
* \fn		void spiAbortTransaction(tSPITransaction * transactionPtr)
* @brief	This function flag the transaction for abortion
* @note		The abortion will be done in the SPIengine
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	nothing
*/
void spiAbortTransaction(tSPITransaction * transactionPtr);

/**
* \fn		U8 spiRemoveTransaction(tSPITransaction * transactionPtr)
* @brief	Remove a transaction control structure from the heap
* @note		Return STD_EC_INVALID if the transaction was not in the heap
*			Return STD_EC_BUSY if the transaction was currently busy
* @arg		tSPITransaction * tempSPITransactionPtr		Pointer to the transaction control structure
* @return	U8 errorCode								STD Error Code (return STD_EC_SUCCESS if successful)
*/
U8 spiRemoveTransaction(tSPITransaction * transactionPtr);
// ========================== //
// ############################################## //


// ############### Internal Define ############## //
// Interrupt Masking
#define INT_MASK_SPI_ERR		BIT0
#define INT_MASK_SPI_RX			BIT1
#define INT_MASK_SPI_TX			BIT2

// Error Masking
#define SPI_MASK_FRAME_ERROR	BIT12
#define SPI_MASK_UNDERRUN_ERROR	BIT8
#define SPI_MASK_OVERFLOW_ERROR	BIT6
// ############################################## //

#endif
