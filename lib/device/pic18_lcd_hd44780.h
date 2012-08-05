/*!
 @file		pic18_lcd_hd44780.h
 @brief		Library for any Character LCD based on Hitachi HD44780 controller. Builded for Microchip PIC C18 C compiler
			Define pins and mode before using
 @version	0.2.1
 @note
 
 @date		Februrary 9th 2011
 @author	Laurence DV
*/

#ifndef _PIC18_LCD_HD44780_H
#define _PIC18_LCD_HD44780_H

// ================= Includes ================= //
#include <datatype_megaxone.h>
#include <stddef_megaxone.h>
#include <splitvar_megaxone.h>

#include <pic18_delay.h>
#include <p18cxxx.h>

// ================= Define ================= //

// ------ TO DEFINE BEFORE USE ------ //
#define LCD_MODE				LCD_MODE_4BITS

#define LCD_E					PORTAbits.RA0
#define LCD_E_TRIS				TRISAbits.TRISA0
//#define LCD_RW
//#define LCD_RW_TRIS
#define LCD_RS					PORTAbits.RA5
#define LCD_RS_TRIS				TRISAbits.TRISA5
/*
#define LCD_D0
#define LCD_D0_TRIS
#define LCD_D1
#define LCD_D1_TRIS
#define LCD_D2
#define LCD_D2_TRIS
#define LCD_D3
#define LCD_D3_TRIS
*/
#define LCD_D4					PORTCbits.RC7
#define LCD_D4_TRIS				TRISCbits.TRISC7
#define LCD_D5					PORTCbits.RC6
#define LCD_D5_TRIS				TRISCbits.TRISC6
#define LCD_D6					PORTCbits.RC5
#define LCD_D6_TRIS				TRISCbits.TRISC5
#define LCD_D7					PORTCbits.RC4
#define LCD_D7_TRIS				TRISCbits.TRISC4
// ---------------------------------- //

//LCD Command mask
//Entry mode
#define LCD_WRT_RIGHT			0b00000110	//Command mask to increment the cursor address (to the right)
#define LCD_WRT_LEFT			0b00000100	//Command mask to decrement the cursor address (to the left)
//Display control
#define LCD_DIS_ON				0b00001100	//Command mask to display the DDRAM on the screen
#define LCD_DIS_OFF				0b00001000	//Command mask to hie the DDRAM on the screen
#define LCD_CUR_ON				0b00001010	//Command mask to display the cursor
#define LCD_CUR_OFF				0b00001000	//Command mask to hide the cursor
#define LCD_CUR_BLK				0b00001011	//Command mask to display and make blink the cursor
//Display and cursor shift
#define LCD_CUR_RIGHT			0b00010100	//Command mask to shift the cursor 1 position to the right
#define LCD_CUR_LEFT			0b00010000	//Command mask to shift the cursor 1 position to the left
#define LCD_DIS_RIGHT			0b00011100	//Command mask to shift the display 1 position to the right
#define LCD_DIS_LEFT			0b00011000	//Command mask to shift the display 1 position to the left
//Function Set
#define LCD_MOD_4BITS			0b00100000	//Command mask to set the Data-Line to 4bits
#define LCD_MOD_8BITS			0b00110000	//Command mask to set the Data-Line to 8bits
#define LCD_NBLN_1				0b00100000	//Command mask to set the number of lines to 1 (or 2 in 4 lines display)
#define LCD_NBLN_2				0b00101000	//Command mask to set the number of lines to 2 (or 4 in 4 lines display)
#define LCD_FONT_SML			0b00100000	//Command mask to set the font to 5x8
#define LCD_FONT_BIG			0b00100100	//Command mask to set the font to 5x10

//LCD Constant
#define LCD_INIT_MESSAGE		0b00110000

#define LCD_MODE_4BITS			0
#define LCD_MODE_8BITS			1

#define LCD_RS_COMMAND			0
#define LCD_RS_CHARACTER		1

#define LCD_RW_WRITE			0
#define LCD_RW_READ				1

// ================= Prototype ================= //
void lcd_hd44780_init(void);
void lcd_hd44780_write(U8 character);
void lcd_hd44780_write_character(U8 character);
//void lcd_hd44780_write_string(U16* table_address);
void lcd_hd44780_write_command(U8 command);
//U8 lcd_hd44780_read(void);
//U8 lcd_hd44780_read_character(void);
//U8 lcd_hd44780_read_command(void);
void lcd_hd44780_write_left(void);
void lcd_hd44780_write_right(void);
void lcd_hd44780_backspace(void);
void lcd_hd44780_set_cursor(U8 row, U8 character);
void lcd_hd44780_cursor_on(void);
void lcd_hd44780_cursor_off(void);
void lcd_hd44780_cursor_blink(void);
void lcd_hd44780_cursor_shift_right(void);
void lcd_hd44780_cursor_shift_left(void);
void lcd_hd44780_display_on(void);
void lcd_hd44780_display_off(void);
void lcd_hd44780_display_shift_right(void);
void lcd_hd44780_display_shift_left(void);
void lcd_hd44780_clear(void);
void lcd_hd44780_home(void);
void lcd_hd44780_pulse(void);

#endif
