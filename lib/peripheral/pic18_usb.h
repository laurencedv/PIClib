/*!
 @file		pic18_usb.h
 @brief		USB 2.0 lib for use with C18

 @version	0.1
 @note		Really Basic at the moment contain only function for the Device CDC class
			Not working still

 @date		October 24th 2011
 @author	Laurence DV
*/


// ---------------------------------- USB Class Code ---------------------------------- //
//	Code	||	Desc. Type	||	Note													//
// ------------------------------------------------------------------------------------ //
//	0x0		||	Device		||	Class information will be relevant to each interface	//
//	0x1		||	Interface	||	Audio													//
//	0x2		||	Both		||	Communication and CDC Control							//
//	0x3		||	Interface	||	Human Interface Device									//
//	0x5		||	Interface	||	Physical												//
//	0x6		||	Interface	||	Image													//
//	0x7		||	Interface	||	Printer													//
//	0x8		||	Interface	||	Mass Storage											//
//	0x9		||	Device		||	Hub														//
//	0xA		||	Interface	||	CDC-Data												//
//	0xB		||	Interface	||	Smart Card												//
//	0xD		||	Interface	||	Content Security										//
//	0xE		||	Interface	||	Video													//
//	0xF		||	Interface	||	Personal Healthcare										//
//	0xDC	||	Both		||	Diagnostic Device										//
//	0xE0	||	Interface	||	Wireless Controller										//
//	0xEF	||	Both		||	Miscellanous											//
//	0xFE	||	Interface	||	Application Specific									//
//	0xFF	||	Both		||	Vendor Specific											//
// ------------------------------------------------------------------------------------ //




#ifndef _PIC18_USB_H
#define _PIC18_USB_H

// ################## Includes ################## //
#include <hardware.h>

// Definition
#include <definition/datatype_megaxone.h>
#include <definition/stddef_megaxone.h>

// Dev Macro
#include <tool/splitvar_megaxone.h>

// Lib
#include <peripheral/pic18_interrupt.h>
#include <soft/pic18_delay.h>
// ############################################## //


// ################## Defines ################### //
// -- Application Variable -- //
#define	USB_ATTACH_DELAY_MAX			10000		//Maximum number of cycle to wait the PLL to be lock before attach the device to the Bus
// -------------------------- //


// -- Options -- //
#define USB_OPT_TRANSCEIVER_INT			0x01
#define USB_OPT_TRANSCEIVER_EXT			0x00
#define USB_OPT_SPEED_FULL				0x02
#define USB_OPT_SPEED_LOW				0x00
// ------------- //


// -- Clock -- //

// ----------- //


// -- Hardware pin -- //

// ------------------ //


// -- Endpoint Transfer Types -- //
#define USB_TRANSFER_CONTROL			0x00
#define USB_TRANSFER_ISOCHRONOUS		0x01
#define USB_TRANSFER_BULK				0x02
#define USB_TRANSFER_INTERRUPT			0x03
// ----------------------------- //


// -- PID Types -- //
#define USB_PID_OUT						0x01		// PID for an OUT token
#define USB_PID_ACK						0x02		// PID for an ACK handshake
#define USB_PID_DATA0					0x03		// PID for DATA0 data
#define USB_PID_PING					0x04		// Special PID PING
#define USB_PID_SOF						0x05		// PID for a SOF token
#define USB_PID_NYET					0x06		// PID for a NYET handshake
#define USB_PID_DATA2					0x07		// PID for DATA2 data
#define USB_PID_SPLIT					0x08		// Special PID SPLIT
#define USB_PID_IN						0x09		// PID for a IN token
#define USB_PID_NAK						0x0A		// PID for a NAK handshake
#define USB_PID_DATA1					0x0B		// PID for DATA1 data
#define USB_PID_PRE						0x0C		// Special PID PRE (Same as USB_PID_ERR)
#define USB_PID_ERR						0x0C		// Special PID ERR (Same as USB_PID_PRE)
#define USB_PID_SETUP					0x0D		// PID for a SETUP token
#define USB_PID_STALL					0x0E		// PID for a STALL handshake
#define USB_PID_MDATA					0x0F		// PID for MDATA data
// --------------- //

// -- Request Types -- //
#define USB_STD_REQ_GET_STATUS			0x00		//Device, Interface, Endpoint
#define USB_STD_REQ_CLEAR_FEATURE		0x01		//Device, Interface, Endpoint
#define USB_STD_REQ_SET_FEATURE			0x03		//Device, Interface, Endpoint
#define USB_STD_REQ_SET_ADDRESS			0x05		//Device
#define USB_STD_REQ_GET_DESCRIPTOR		0x06		//Device
#define USB_STD_REQ_SET_DESCRIPTOR		0x07		//Device
#define USB_STD_REQ_GET_CONFIGURATION	0x08		//Device
#define USB_STD_REQ_SET_CONFIGURATION	0x09		//Device
#define USB_STD_REQ_GET_INTERFACE		0x0A		//Interface
#define USB_STD_REQ_SET_INTERFACE		0x11		//Interface
#define USB_STD_REQ_SYNCH_FRAME			0x12		//Endpoint
// ------------------- //

// -- Descriptor Types -- //
#define USB_DESC_TYPE_DEVICE			0x01		//Device Descriptor
#define USB_DESC_TYPE_CONFIGURATION		0x02		//Configuration Descriptor
#define USB_DESC_TYPE_STRING			0x03		//String Descriptor
#define USB_DESC_TYPE_INTERFACE			0x04		//Interface Descriptor
#define USB_DESC_TYPE_ENDPOINT			0x05		//Endpoint Descriptor
#define USB_DESC_TYPE_HID				0x21		//HID Descriptor
// ---------------------- //

// -- Device Class Code -- //
#define USB_CLASS_MULTI					0x00
#define USB_CLASS_AUDIO					0x01
#define USB_CLASS_CDC					0x02
#define USB_CLASS_HID					0x03
#define USB_CLASS_PHYSICAL				0x05
#define USB_CLASS_IMAGE					0x06
#define USB_CLASS_PRINTER				0x07
#define USB_CLASS_MASS_STORAGE			0x08
#define USB_CLASS_HUB					0x09
#define USB_CLASS_CDC_DATA				0x0A
#define USB_CLASS_SMART_CARD			0x0B
#define USB_CLASS_CONTENT_SECURITY		0x0D
#define USB_CLASS_VIDEO					0x0E
#define USB_CLASS_PERSONAL_HEALTH		0x0F
#define USB_CLASS_DIAGNOSTIC_TOOL		0xDC
#define USB_CLASS_WIRELESS				0xE0
#define USB_CLASS_MISCELLANOUS			0xEF
#define USB_CLASS_APPLICATION_SPEC		0xFE
#define USB_CLASS_VENDOR_SPEC			0xFF
// ----------------------- //
// ############################################## //


// ################# Data Type ################## //
// -- Descriptor Type -- //
// Device Descriptor Type
typedef union
{
	U8 all[18];
	struct
	{
		U8 bLength;				//Size of the Descriptor in Byte (18 Byte)
		U8 bDescriptorType;		//Type of Description (Device: 1 | Configuration: 2 | String: 3 | Interface: 4 | EndPoint: 5 | HID: 0x21)
		U16 bcdUSB;				//USB Specification in BCD (ex: 0x0200 for 2.0)
		U8 bDeviceClass;		//Class Code (see the USB Class Code table above)
		U8 bDeviceSubClass;		//Sub-Class Code (assigned by USB.org)
		U8 bDeviceProtocol;		//Protocol Code (assigned by USB.org) (0: no protocol | 0xFF: Vendor-Specific)
		U8 bMaxPacketSize0;		//Maximum number of Byte/packet for the Endpoint 0
		U16 idVendor;			//Vendor ID (assigned by USB.org)
		U16 idProduct;			//Product ID (assigned by Vendor)
		U16 bcdDevice;			//Device version number in BCD (ex: 0x0302 for 3.2)
		U8 iManufacturer;		//Index of string descriptor for the Vendor
		U8 iProduct;			//Index of string descriptor for the Product
		U8 iSerialNumber;		//Index of string descriptor for the Serial Number
		U8 bNumConfigurations;	//Number of possible configuration
	};
}tUSBDeviceDescriptor;

// Configuration Descriptor Type
typedef union
{
	U8 all[9];
	struct
	{
		U8 bLength;				//Size of the Descriptor in Byte (9 Byte)
		U8 bDescriptorType;		//Type of Description (Device: 1 | Configuration: 2 | String: 3 | Interface: 4 | EndPoint: 5 | HID: 0x21)
		U16 wTotalLength;		//Total size of the data returned (include: configuration, interface, endpoint and class/vendor specific descriptors)
		U8 bNumInterfaces;		//Number of interface for this configuration
		U8 bConfigurationValue;	//Value to use to select this configuration (conf ID ????)
		U8 iConfiguration;		//Index of string descriptor for this configuration
		U8 bmAttributes;		//BitMap Attributes (b0>b4: Reserver(0) | b5: Remote Wakeup | b6: Self Powered | b7: Reserved(1))
		U8 MaxPower;			//Power sunk by the Product when in bus-powered operation (by 2mA ex: 50 = 100mA)
	};
}tUSBConfigurationDescriptor;

// Interface Descriptor Type
typedef union
{
	U8 all[9];
	struct
	{
		U8 bLength;				//Size of the Descriptor in Byte (9 Byte)
		U8 bDescriptorType;		//Type of Description (Device: 1 | Configuration: 2 | String: 3 | Interface: 4 | EndPoint: 5 | HID: 0x21)
		U8 bInterfaceNumber;	//ID of this interface in the configuration
		U8 bAlternateSetting;	//Value to use to select this setting for the interface selected in $bInterfaceNumber
		U8 bNumEndpoints;		//Number of End Point used by this interface (excluding EndP 0)
		U8 bInterfaceClass;		//Class Code for this interface (assigned by USB.org)
		U8 bInterfaceSubClass;	//Sub-Class Code for this interface (assigned by USB.org)
		U8 bInterfaceProtocol;	//Protocol for this interface (assigned by USB.org) (0: no protocol | 0xFF: Vendor-Specific)
		U8 iInterface;
	};
}tUSBInterfaceDescriptor;

// End Point Descriptor Type
typedef union
{
	U8 all[7];
	struct
	{
		U8 bLength;				//Size of the Descriptor in Byte (7 Byte)
		U8 bDescriptorType;		//Type of Description (Device: 1 | Configuration: 2 | String: 3 | Interface: 4 | EndPoint: 5 | HID: 0x21)
		U8 bEndpointAddress;	//EndPoint Address (b0>b3: EndPoint Address | b4>b6: Reserved | b7: Direction [1:IN | 2:OUT])
		U8 bmAttributes;		//BitMap Attributes		b0>b1: Transfert Type [ 00: Control | 01: ISOchronous | 10: Bulk | 11: Interrupt ]
								//						b2>b3: ISO Mode: Synchronisation Type [00: No Sync | 01: Async | 10: Adaptive | 11: Sync]
								//						b4>b5: ISO Mode: Usage Type [00: Data EndP | 01: Feedback EndP | 10: Implicit Feedback Data EndP | 11: Reserved(0)]
		U16 wMaxPacketSize;		//Maximum number of Byte/packet (b0>b10: B/pkt | b11>b12: ISO&Int Mode: Transaction number per micro-frame [ 00: 1 | 01: 2 | 10: 3 | 11: Reserved] | b13>b15: Reserved(0))
		U8 bInterval;			//ISO Mode: Interval between polling of EndP | Bulk Mode: maximum rate of NAK by $bInterval number of micro-frame
	};
}tUSBEndPointDescriptor;

// HID Descriptor Type
typedef union
{
	U8 all[11];
	struct
	{
		U8 bLength;				//Size of the Descriptor in Byte (8-11 Byte)
		U8 bDescriptorType;		//Type of Description (Device: 1 | Configuration: 2 | String: 3 | Interface: 4 | EndPoint: 5 | HID: 0x21)
		U16 bcdHID;				//HID serial/version number in BCD (ex: 0x050C for 5.12)
		U8 bCountry;			//Country Code for localization
		U8 bNumDescriptors;		//Number of descriptor for this class
		U16 wDescriptorLength;	//Size of the Report Descriptor
		U8 bOptionalDescType;	//Optional Descriptor Type
		U16 wOptionalDescLength;//Optional Descriptor Size
	};
}tUSBHIDDescriptor;

// Setup Packet
typedef union
{
	U8 all[8];
	struct
	{
		U8 bmRequestType;		//Type of Request
								//BitMap Attributes		b0>4:	Recipient [ 00: Device | 01: Interface | 10: Endpoint | 11: Other | other: reserverd)
								//						b5>b6:	Type [ 00: Standard | 01: Class | 10: Vendor | 11: Reserved ]
								//						b7:		Transfert Direction [ 0: Host to Device | 1: Device to Host ]
		U8 bRequest;			//Request
		U16 wValue;				//Value
		U16 wIndex;				//Index
		U16 wLength;			//Nb of byte to transfer if there is a data phase
	};
}tUSBSetupPacket;
// --------------------- //

// -- Registers Pointers -- //
// Buffer Descriptor
typedef union
{
	U8 all[4];
	struct
	{
		U8 status;					//Status of the Buffer
		U8 byteCount;				//Byte counter
		U8 addressLow;				//LSB of the address of the first byte of the buffer
		U8 addressHigh;				//MSB
	};
	struct
	{
		struct						//CPU Owned buffer
		{
			U8 BC:2;				//Upper part of the total of byte to send/receive
			U8 BSTALL:1;			//Force a STALL Setup Token response
			U8 DTSEN:1;				//Enable the Data Toggle Synchronization
			U8 :2;
			U8 DTS:1;				//Data Toggle Synchronization status
			U8 UOWN:1;				//Current Owner of the buffer (0: CPU)
		};
		U8 :8;
		U16 address;
 	};
	struct
	{
		struct						//SIE Owned buffer
		{
			U8 BC:2;				//Upper part of the actual total of byte to sent/received
			U8 PID:4;				//PID of the received packet
			U8 :1;
			U8 UOWN:1;				//Current Owner of the buffer (1: SIE)
		};
		U8 :24;
	};
}tUSBBufferDescriptor;

// Endpoint Control
typedef union
{
	U8 all;
	struct
	{
		U8 EPSTALL:1;				//Stall condition indication
		U8 EPINEN:1;				//Input enable
		U8 EPOUTEN:1;				//Output enable
		U8 EPCONDIS:1;				//Control disable
		U8 EPHSHK:1;				//HandShake enable
		U8 :3;
	};
}tUSBendpointControl;
// -------------------------- //
// ############################################## //


// ################# Prototypes ################# //
// ============ Interrupt Functions ============= //

// ============================================== //


// ======== Packet and Condition Handler ======== //

// ============================================== //


// ============= Control Functions ============== //
/**
* \fn		U8 usbDeviceInit(U8 deviceClass)
* @brief	Initialise the USB
* @note
* @arg		U8 deviceClass		Device Class needed to be init
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 USBDeviceInit(U8 deviceClass);

/**
* \fn		U8 usbAttach(void)
* @brief	Attach the Device to the USB Bus
* @note		Will wait a maximum of USB_ATTACH_DELAY_MAX cycle for the PLL to be locked
* @arg		nothing				
* @return	U8 errorCode		STD Error Code (STD_EC_SUCCESS if successful)
*/
U8 USBAttach(void);

/**
* \fn		void usbDetach(void)
* @brief	Detach the Device from the USB Bus
* @note		Will Abort all transfert
* @arg		nothing
* @return	nothing
*/
void USBDetach(void);

/**
* \fn		void USBSetAddress(address)
* @brief	Set the USB Device address
* @note		USB Deviec address is given by the host and automaticly set by the SIE
*			Can be use to force the address to a specified value
* @arg		U8 address			Desired USB Device address
* @return	nothing
*/
#define USBSetAddress(address)	(UADDR = (address & 0x7F))

/**
* \fn		void USBGetAddress(address)
* @brief	Return the USB Device address
* @note		
* @arg		nothing
* @return	U8 address			Actual USB Device address
*/
#define USBGetAddress()			(UADDR & 0x7F)
// ============================================== //

// ############################################## //

#endif
