/**
 * @file	VT100.c
 * VT Terminal Commands Library.
 * This library provides several commands for making a nice terminal interface
 * through the use of VT100 escape sequences. It features a system for creating
 * a simple GUI, with options for a command line, horizontal and vertical
 * sliders, horizontal and vertical status bars, and 2D data plots.
 *
 * @author	Scott Teal
 * @date	Sep 26, 2012
 * @copyright
 * Copyright (c) 2014 Cognoscan
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Includes
// ========
#include <stdint.h>		// Standardizes sizes for variables
#include "BCDConv.h"	// Binary to BCD Conversion functions
#include "VT100.h"		// VT100 Terminal Library (which this file defines)

/** Address of the function for sending characters to terminal. */
static void (*VT_Transmit)(char);

/**
 * Sets the VT_Transmit function.
 * @param	*TX_func	The function for sending characters to terminal.
 */
void VT_SetTXFunc(void (*TX_func)(char))
{
	VT_Transmit = TX_func;
}

/**
 * Send a single character to terminal.
 * @param	c	The character to send.
 */
void VT_SendChar(char c)
{
	VT_Transmit(c);							// Just a wrapper for VT_Transmit.
}

/**
 * Print a string to terminal.
 * @param *string	The string to send.
 */
void VT_Print(char *string)
{
    while (*string) {
        VT_Transmit(*string++);				// Send each character
    }
}

/**
 * Look up the ASCII character for a hex value.
 * @param	byte	Lower nibble is looked up on ASCII table.
 * @return	The ASCII character of the nibble (ex. 0x0A returns 'A')
 */
char Lookup_Hex_Low(uint8_t byte)
{
	const char hex[16] = "0123456789ABCDEF";	// Hex lookup table
	byte = byte & 0x0F;						// Just use lower nibble for lookup
	return hex[byte];						// Look up char value on hex table.
}

/**
 * Print the ASCII representation of a byte in hex format.
 * @param	hex		The hex value to print out.
 */
void VT_Print_Hex_Val(uint8_t hex)
{
	VT_Transmit(Lookup_Hex_Low(hex>>4));
	VT_Transmit(Lookup_Hex_Low(hex));
}

/**
 * Move the cursor to a specific row and column in the terminal.
 * @param	x	The column location to go to (generally 1-80).
 * @param	y	The row location to go to (generally 1-24).
 */
void VT_Pos(uint8_t x, uint8_t y)
{
	x = bin2bcd8(x);
	y = bin2bcd8(y);
	VT_Transmit('\x1B');
	VT_Transmit('[');
	VT_Print_Hex_Val(y);
	VT_Transmit(';');
	VT_Print_Hex_Val(x);
	VT_Transmit('H');
}

/**
 * Move the cursor in a particular direction by a set amount.
 * @param	num		Distance to move the cursor.
 * @param	dir		Direction to move in (use VT_UP/DN/LT/RT).
 */
void VT_Move(uint8_t num, char dir)
{
	VT_Transmit('\x1B');
	VT_Transmit('[');
	if (num > 1)
	{
		num = bin2bcd8(num);
		VT_Print_Hex_Val(num);
	}
	VT_Transmit(dir);
}

/**
 * Sets the Color of either the foreground or background.
 * @param	fg_bg	'3' to set foreground, '4' to set background.
 * @param	color	Character for the new color. See the VT color set.
 */
void VT_ColorSet(char fg_bg, char color)
{
	VT_Transmit('\x1B');
	VT_Transmit('[');
	VT_Transmit(fg_bg);
	VT_Transmit(color);
	VT_Transmit('m');
}

/**
 * Draws a box with the current position as the upper left corner.
 * @param	width	The width of the box in characters.
 * @param	height	The height of the box in characters.
 */
void VT_Draw_Box(uint8_t width, uint8_t height)
{
	uint8_t i;
	height-=2;	// Subtract top and bottom borders

	VT_Save_Cur();
	// Top border
	for (i=width; i>0; i--)
	{
		VT_Transmit('=');
	}
	// Right border
	VT_Move(1,VT_DN);
	VT_Move(1,VT_LT);
	for (i=height; i>0; i--)
	{
			VT_Transmit('|');
			VT_Move(1,VT_DN);
			VT_Move(1,VT_LT);
	}
	// Left border
	VT_Unsave_Cur();
	VT_Move(1,VT_DN);
	for (i=height; i>0; i--)
	{
		VT_Transmit('|');
		VT_Move(1,VT_DN);
		VT_Move(1,VT_LT);
	}
	// Bottom border
	for (i=width; i>0; i--)
	{
			VT_Transmit('=');
	}
	VT_Unsave_Cur();

}

/**
 * Prints out all character codes from 0x20 to 0xFF.
 */
void Show_Characters(void)
{
	VT_Print("All Available Characters\r\n");
		int16_t i;
		for (i=32; i<256; i++)
		{
			if ((i & 0x0F) == 0)
			{
				VT_Print("\r\n");
			}
			VT_SendChar(i);
		}
		VT_Print("\r\n");
}

/**
 * Initializes a horizontal slider GUI item.
 * Value of slider stored in val->val2, and can be 0 to width-1.
 * @param	x		The column of the leftmost slider character.
 * @param	y		The row of the slider.
 * @param	width	The width of the slider, in characters.
 * @param	item	The gui_item object to hold the slider.
 */
void VT_HSlide_Init(uint8_t x, uint8_t y, uint8_t width, gui_item * val)
{
	val->x = x;					// Set x location (column)
	val->y = y;					// Set y location (row)
	val->val1 = width;				// Set val1 to width.
	val->val2 = width >> 1;		// Start slider at midpoint.
	VT_HSlide_Draw(val);			// Make the initial draw call.
}

/**
 * Handles any input to a horizontal slider.
 * Slider will react to A,B,C, and D. The arrow keys generate these through
 * their escape sequences (<esc>[A <esc>[B etc.)
 * @param	in		The character input to handle.
 * @param	values	The gui item values for the slider being handled.
 */
int8_t VT_HSlide_Handle(uint8_t in, gui_item * values)
{
	uint8_t update = 0;
	switch(in)
	{
		// Left Arrow: decrease slider
		case 'D':
			if (values->val2 > 0)
			{
				values->val2--;
				update = 1;
			}
			break;
		// Right Arrow: increase slider
		case 'C':
			if (values->val2 < (values->val1-1))
			{
				values->val2++;
				update = 1;
			}
			break;
		// Up Arrow: return -1 to go to previous GUI item
		case 'A':
			return -1;
		// Down Arrow: return 1 to go to next GUI item
		case 'B':
			return 1;
	}
	// Redraw slider only if value changed.
	if (update)
	{
		VT_HSlide_Draw(values);
	}
	return 0;
}

/**
 * Draws a horizontal slider.
 * @param	values	The gui item values to use in drawing.
 */
void VT_HSlide_Draw(gui_item * values)
{
	uint8_t i;								// Temporary count variable

	VT_Hide_Cur();							// Hide cursor to avoid flicker
	VT_Pos(values->x,values->y);			// Position to x,y for GUI item
	for(i=values->val1; i>0; i--)			// Loop to place down slider bar
	{
		VT_Transmit('-');
	}
	VT_ColorSet(VT_FG,VT_STD_HL);			// Use highlight color for slider
											// position
	VT_Move((values->val1 - values->val2),VT_LT);
	VT_Transmit('|');
	VT_ColorSet(VT_FG,VT_STD_FG);			// Return to standard color
	VT_Move(1,VT_LT);						// Move cursor onto slider position
	VT_Show_Cur();							// Show cursor again
}

/**
 * Initializes a vertical slider GUI item.
 * Value of slider stored in val->val2, and can be 0 to height-1.
 * @param	x		The column of the slider.
 * @param	y		The row of the topmost character of the slider.
 * @param	height	The height of the slider, in characters.
 * @param	item	The gui_item object to hold the slider.
 */
void VT_VSlide_Init(uint8_t x, uint8_t y, uint8_t height, gui_item * val)
{
	val->x = x;					// Set x location (column)
	val->y = y;					// Set y location (row)
	val->val1 = height;			// Set val1 to height.
	val->val2 = height >> 1;		// Start slider at midpoint.
	VT_VSlide_Draw(val);			// Make the initial draw call.
}

/**
 * Handles any input to a vertical slider.
 * Slider will react to A,B,C, and D. The arrow keys generate these through
 * their escape sequences (<esc>[A <esc>[B etc.)
 * @param	in		The character input to handle.
 * @param	values	The gui item values for the slider being handled.
 */
int8_t VT_VSlide_Handle(uint8_t in, gui_item * values)
{
	uint8_t update = 0;
	switch(in)
	{
		// Down Arrow: decrease slider value
		case 'B':
			if (values->val2 > 0)
			{
				values->val2--;
				update = 1;
			}
			break;
		// Up Arrow: increase slider value
		case 'A':
			if (values->val2 < (values->val1-1))
			{
				values->val2++;
				update = 1;
			}
			break;
		// Left Arrow: return -1 to go to previous GUI item
		case 'D':
			return -1;
		// Right Arrow: return 1 to go to next GUI item
		case 'C':
			return 1;
	}
	// Redraw only if value changed
	if (update)
	{
		VT_VSlide_Draw(values);
	}
	return 0;
}

/**
 * Draws a vertical slider.
 * @param	values	The gui item values to use in drawing.
 */
void VT_VSlide_Draw(gui_item * values)
{
	uint8_t i;								// Temporary count variable

	VT_Hide_Cur();							// Hide cursor to avoid flicker
	VT_Pos(values->x,values->y);			// Position to x,y for GUI item
	for(i=values->val1; i>0; i--)			// Loop to place down slider bar
	{
		VT_Transmit('|');
		VT_Move(1,VT_DN);
		VT_Move(1,VT_LT);
	}
	VT_ColorSet(VT_FG,VT_STD_HL);			// Use highlight color for slider
											// position
	VT_Move((values->val2+1),VT_UP);
	VT_Transmit('=');
	VT_ColorSet(VT_FG,VT_STD_FG);			// Return to standard color
	VT_Move(1,VT_LT);						// Move cursor onto slider position
	VT_Show_Cur();							// Show cursor again

}

/**
 * Initializes a horizontal bar GUI item.
 * Value of bar stored in val->val2, and can be 0 to width-1.
 * @param	x		The column of the leftmost bar character.
 * @param	y		The row of the bar.
 * @param	width	The width of the bar, in characters.
 * @param	item	The gui_item object to hold the bar.
 */
void VT_HBar_Init(uint8_t x, uint8_t y, uint8_t width, gui_item * val)
{
	val->x = x;					// Set x location (column)
	val->y = y;					// Set y location (row)
	val->val1 = width;				// Set val1 to width.
	val->val2 = width >> 1;		// Start bar at midpoint.
	VT_HBar_Draw(val);			// Make the initial draw call.
}

/**
 * Draws a horizontal slider.
 * @param	values	The gui item values to use in drawing.
 */
void VT_HBar_Draw(gui_item * values)
{
	uint8_t i;								// Temporary count variable

	VT_Hide_Cur();							// Hide cursor to avoid flicker
	VT_Pos(values->x,values->y);			// Position to x,y for GUI item
	for(i=values->val1; i>0; i--)			// Loop to place down slider bar
	{
		VT_Transmit('-');
	}
	VT_ColorSet(VT_FG,VT_STD_HL);			// Use highlight color for slider
											// position
	VT_Move((values->val1 - values->val2),VT_LT);
	VT_Transmit('|');
	VT_ColorSet(VT_FG,VT_STD_FG);			// Return to standard color
	VT_Move(1,VT_LT);						// Move cursor onto slider position
	VT_Show_Cur();							// Show cursor again
}

/**
 * Set up parameters for a scatter plot.
 * @param	x	x (column) location for upper left corner of plot area.
 * @param	y	y (row) location for upper left corner of plot area.
 * @param	w	Width of the plot area.
 * @param	h	Height of the plot area.
 * @param	val	The GUI item values for the plot being set up.
 */
void VT_Scatter_Init(uint8_t x, uint8_t y, uint8_t w, uint8_t h, gui_item * val)
{
	val->x = x;
	val->y = y;
	val->val1 = w;
	val->val2 = h;
	VT_Pos(x-1,y-1);
	VT_Draw_Box(w+2,h+2);
}

/**
 * Make a mark on the scatter plot.
 * The coordinate system is like a screen: x is increasing from left to right,
 * and y is increasing from top to bottom.
 * @param	x	The x location for the mark within the plot.
 * @param	y	The y location for the mark within the plot.
 * @param	val	The GUI item values for the plot being drawn on.
 */
void VT_Scatter_Update(uint8_t x, uint8_t y, gui_item * val)
{
	VT_Save_Cur();							// Save to return to proper spot
	VT_Hide_Cur();							// Hide cursor to avoid flicker

	VT_Pos(val->x + x, val->y + y);			// Set to x,y within graph
	VT_ColorSet(VT_FG,VT_STD_HL);			// Use highlight color for mark
	VT_Transmit('X');
	VT_ColorSet(VT_FG,VT_STD_FG);			// Return to standard color

	VT_Unsave_Cur();						// Return cursor to original pos
	VT_Show_Cur();							// Show cursor again
}

/**
 * Clear a scatter plot of marks.
 * @param	val	The GUI item values of the plot being cleared.
 */
void VT_Scatter_Clear(gui_item * val)
{
	VT_Save_Cur();							// Save to return to proper spot
	VT_Hide_Cur();							// Hide cursor to avoid flicker
	uint8_t i;
	uint8_t j;
	for (i=0; i < val->val2; i++)			// Loop for clearing by rows
	{
		VT_Pos(val->x, val->y + i);
		for (j=0; j < val->val1; j++)		// Loop for clearing a row out
		{
			VT_Transmit(' ');
		}
	}

	VT_Unsave_Cur();						// Return cursor to original pos
	VT_Show_Cur();							// Show cursor again
}
