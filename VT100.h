/**
 * @file	VT100.h
 * Header file for the VT100 Terminal Commands Library.
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

#include <stdint.h>

/**
 * The data intrinsic to a GUI object
 */
struct gui_item_t {
	uint8_t x;				// X position of GUI item
	uint8_t	y;				// Y position of GUI item
	uint8_t	val1;			// 1st value in item
	uint8_t	val2;			// 2nd value in item
};

typedef struct gui_item_t gui_item;

// Escape Sequences
// ================
#define VT_Clear()       VT_Print("\x1B[2J")   ///< Clear screen
#define VT_Home()        VT_Print("\x1B[;H")   ///< Go to Home Position
#define VT_Save_Cur()    VT_Print("\x1B[s")    ///< Save Cursor
#define VT_Unsave_Cur()  VT_Print("\x1B[u")    ///< Unsave Cursor
#define VT_Hide_Cur()    VT_Print("\x1B[?25l")
#define VT_Show_Cur()    VT_Print("\x1B[?25h")

// Aliases for cursor control characters, used in \x1B[{count}{character}
#define VT_UP     'A' ///< Up character
#define VT_DN     'B' ///< Down character
#define VT_RT     'C' ///< Right character
#define VT_LT     'D' ///< Left character

#define VT_Mup()    VT_Print("\x1B[A")  ///< Move up 1
#define VT_Mdn()    VT_Print("\x1B[B")  ///< Move down 1
#define VT_Mrt()    VT_Print("\x1B[C")  ///< Move right 1
#define VT_Mlt()    VT_Print("\x1B[D")  ///< Move left 1

// Color Value characters, from ASCII 0-7
#define VT_BLK '0' ///< Black
#define VT_RED '1' ///< Red
#define VT_GRN '2' ///< Green
#define VT_YEL '3' ///< Yellow
#define VT_BLU '4' ///< Blue
#define VT_PUR '5' ///< Magenta (Purple)
#define VT_CYA '6' ///< Cyan
#define VT_WHT '7' ///< White

// Display attribute characters
#define VT_RESET   '0' ///< Reset attributes
#define VT_BRIGHT  '1' ///< Bright (a.k.a. bold)
#define VT_DIM     '2' ///< Dim
#define VT_USCORE  '4' ///< Underscore
#define VT_BLINK   '5' ///< Blinking
#define VT_REVERSE '7' ///< Reverse
#define VT_HIDDEN  '8' ///< Hidden
#define VT_FG      '3' ///< Foreground color char
#define VT_BG      '4' ///< Background color char

// Default Display Colors
#define VT_STD_FG				VT_RED
#define VT_STD_BG				VT_BLK
#define VT_STD_HL				VT_WHT

void VT_SetTXFunc(void (*TX_func)(char));

void VT_SendChar(char c);
void VT_Print(char *string);

char Lookup_Hex_Low(uint8_t byte);
void VT_Print_Hex_Val(uint8_t hex);

void VT_Pos(uint8_t x, uint8_t y);
void VT_Move(uint8_t num, char dir);
void VT_ColorSet(char fg_bg, char color);

void VT_Draw_Box(uint8_t width, uint8_t height);
void Show_Characters(void);

void VT_HSlide_Init(uint8_t x, uint8_t y, uint8_t width, gui_item * val);
int8_t VT_HSlide_Handle(uint8_t in, gui_item * values);
void VT_HSlide_Draw(gui_item * values);

void VT_VSlide_Init(uint8_t x, uint8_t y, uint8_t height, gui_item * val);
int8_t VT_VSlide_Handle(uint8_t in, gui_item * values);
void VT_VSlide_Draw(gui_item * values);

void VT_HBar_Init(uint8_t x, uint8_t y, uint8_t width, gui_item * val);
void VT_HBar_Draw(gui_item * values);

void VT_Scatter_Init(uint8_t x, uint8_t y, uint8_t h, uint8_t w, gui_item * val);
void VT_Scatter_Update(uint8_t x, uint8_t y, gui_item * val);
void VT_Scatter_Clear(gui_item * val);
