/*!
 @file		pic18_lcd_hd44780.c
 @brief		Library for any Character LCD based on Hitachi HD44780 controller. Builded for Microchip PIC C18 C compiler
 @version	0.2
 @date		Februrary 9th 2011
 @author	Laurence DV
*/

// ================= Includes ================= //
#include <pic18_lcd_hd44780.h>

// ================= LCD functions ================= //

// ------ BASIC FUNCTION ------ //

// LCD initialisation
void lcd_hd44780_init(void)
{
    //Init Port Direction
    LCD_E_TRIS = OUTPUT;
#ifdef LCD_RW
    LCD_RW_TRIS = OUTPUT;
#endif
    LCD_RS_TRIS = OUTPUT;

//If the transmit mode is 8 bit
#if LCD_MODE == LCD_MODE_8BITS
    LCD_D0_TRIS = OUTPUT;
    LCD_D1_TRIS = OUTPUT;
    LCD_D2_TRIS = OUTPUT;
    LCD_D3_TRIS = OUTPUT;
#endif

    LCD_D4_TRIS = OUTPUT;
    LCD_D5_TRIS = OUTPUT;
    LCD_D6_TRIS = OUTPUT;
    LCD_D7_TRIS = OUTPUT;

    //Set the default value for the communication
    LCD_RS = LCD_RS_CHARACTER;              //Set the RS to send a command
#ifdef LCD_RW
    LCD_RW = LCD_RW_WRITE;                  //Set the RW to write a command
#endif

    //Set the default state of the data line and E
#if LCD_MODE == LCD_MODE_8BITS
    LCD_D0 = LOW;
    LCD_D1 = LOW;
    LCD_D2 = LOW;
    LCD_D3 = LOW;
#endif
    LCD_D4 = LOW;
    LCD_D5 = LOW;
    LCD_D6 = LOW;
    LCD_D7 = LOW;
    LCD_E = LOW;

    //Wait for VCC on the LCD to rise
    delay_nMS(40);
    lcd_hd44780_write_command(LCD_INIT_MESSAGE);
    delay_nMS(5);
    lcd_hd44780_write_command(LCD_INIT_MESSAGE);
    delay_nUS(100);
    lcd_hd44780_write_command(LCD_INIT_MESSAGE);

    //Check the mode and send the correct functions set
#if LCD_MODE == LCD_MODE_4BITS
    //Switch to data line of 4bits
    lcd_hd44780_write_command(LCD_MOD_4BITS);
    //Set the default options
    lcd_hd44780_write_command(LCD_MOD_4BITS | LCD_NBLN_2 | LCD_FONT_SML);
#elif LCD_MODE == LCD_MODE_8BITS
    //Set the default options
    lcd_hd44780_write_command(LCD_MOD_8BITS | LCD_NBLN_2 | LCD_FONT_SML);
#endif

    //Send the default options
    lcd_hd44780_write_command(LCD_DIS_ON | LCD_CUR_OFF);    //Diplay control
    lcd_hd44780_write_command(LCD_WRT_RIGHT);		    	//Entry mode

	//Reset the display
    lcd_hd44780_clear();
    lcd_hd44780_home();
}

// Send a byte to the LCD
void lcd_hd44780_write(U8 character)
{

    //Splitting the character in bits
    split8 to_send_to_lcd;
    to_send_to_lcd.all = character;

    //8 bits mode
    #if LCD_MODE == LCD_MODE_8BITS
    //Put the value on the pins
    LCD_D7 = to_send_to_lcd.b7;
    LCD_D6 = to_send_to_lcd.b6;
    LCD_D5 = to_send_to_lcd.b5;
    LCD_D4 = to_send_to_lcd.b4;
    LCD_D3 = to_send_to_lcd.b3;
    LCD_D2 = to_send_to_lcd.b2;
    LCD_D1 = to_send_to_lcd.b1;
    LCD_D0 = to_send_to_lcd.b0;

    lcd_hd44780_pulse();

    #else
    // 4 bits mode
    //Put the MSB on the pins
    LCD_D7 = to_send_to_lcd.b7;
    LCD_D6 = to_send_to_lcd.b6;
    LCD_D5 = to_send_to_lcd.b5;
    LCD_D4 = to_send_to_lcd.b4;

    lcd_hd44780_pulse();

    //Put the LSB on the pins
    LCD_D7 = to_send_to_lcd.b3;
    LCD_D6 = to_send_to_lcd.b2;
    LCD_D5 = to_send_to_lcd.b1;
    LCD_D4 = to_send_to_lcd.b0;

    lcd_hd44780_pulse();
    #endif

    delay_nUS(37);	    //Wait for the execution of the command before exiting the function
}

// Send a command to the LCD
void lcd_hd44780_write_command(U8 command)
{
    LCD_RS = LCD_RS_COMMAND;                //Set the RS to send a command
    lcd_hd44780_write(command);             //Send the actual data
    LCD_RS = LCD_RS_CHARACTER;              //Reset RS to default value (Character)
}

// Write a character to the LCD
void lcd_hd44780_write_character(U8 character)
{
    lcd_hd44780_write(character);
}


// LCD E pulse
void lcd_hd44780_pulse(void)
{
    LCD_E = HIGH;
    delay_nUS(2);
    LCD_E = LOW;
    delay_nUS(2);
}


// ------ COMMAND ------ //

// Set the LCD to write to the left
void lcd_hd44780_write_left(void)
{
    lcd_hd44780_write_command(LCD_WRT_LEFT);
}

// Set the LCD to write to the right
void lcd_hd44780_write_right(void)
{
    lcd_hd44780_write_command(LCD_WRT_RIGHT);
}

// Erase the last Char
void lcd_hd44780_backspace(void)
{
	lcd_hd44780_write_command(LCD_CUR_LEFT);
	lcd_hd44780_write_character(' ');
	lcd_hd44780_write_command(LCD_CUR_LEFT);
}

// Cursor
// Set the cursor to this position
void lcd_hd44780_set_cursor(U8 row, U8 col)
{
    //Address of the first position of each row
    U8 row_offset[4]={0,0x40,0x14,0x54};
    //Check for non-valid row
    if (row > 3)
	row = 3;
    lcd_hd44780_write_command(0x80 | (row_offset[row] + col));

}

// Display the cursor
void lcd_hd44780_cursor_on(void)
{
    lcd_hd44780_write_command(LCD_DIS_ON | LCD_CUR_ON);
}

// Hide the cursor
void lcd_hd44780_cursor_off(void)
{
    lcd_hd44780_write_command(LCD_DIS_ON | LCD_CUR_OFF);
}

// Make the cursor blink
void lcd_hd44780_cursor_blink(void)
{
    lcd_hd44780_write_command(LCD_DIS_ON | LCD_CUR_BLK);
}

// Shift the cursor 1 position to the right
void lcd_hd44780_cursor_shift_right(void)
{
    lcd_hd44780_write_command(LCD_CUR_RIGHT);
}

// Shift the cursor 1 position to the left
void lcd_hd44780_cursor_shift_left(void)
{
    lcd_hd44780_write_command(LCD_CUR_LEFT);
}

// Diplay
// Display the content of the DDRAM
void lcd_hd44780_display_on(void)
{
    lcd_hd44780_write_command(LCD_DIS_ON);
}

// Hide the content of the DDRAM
void lcd_hd44780_display_off(void)
{
    lcd_hd44780_write_command(LCD_DIS_OFF);
}

// Shift the display 1 position to the right
void lcd_hd44780_display_shift_right(void)
{
    lcd_hd44780_write_command(LCD_DIS_RIGHT);
}

// Shift the display 1 position to the left
void lcd_hd44780_display_shift_left(void)
{
    lcd_hd44780_write_command(LCD_DIS_LEFT);
}

// Clear all the display
void lcd_hd44780_clear(void)
{
    lcd_hd44780_write_command(0x01);	    //Clear command
    delay_nMS(1);
    delay_nUS(250);
	delay_nUS(250);
    delay_nUS(233);			    //Wait a total of 1.77ms defore exiting the function
}

// Reset the cursor and display to original position
void lcd_hd44780_home(void)
{
    lcd_hd44780_write_command(0x02);	    //Home command

    delay_nMS(1);
    delay_nUS(250);
	delay_nUS(250);
    delay_nUS(233);			    //Wait a total of 1.77ms defore exiting the function
}

