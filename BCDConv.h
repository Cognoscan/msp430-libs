/**
 * @file	BCDConv.h
 * Assembly functions for quickly converting from binary to BCD
 * through MSP430 BCD instructions.
 *
 * @author	Scott Teal
 * @date	Sep 26, 2012
 * @copyright
 * The MIT License (MIT)
 * 
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

#ifndef BCDCONV_H_
#define BCDCONV_H_

/**
 * Converts an 8-bit binary value to 16-bit BCD.
 * @param	bin		The 8-bit binary value to convert.
 * @return	The converted 4 character (4 nibble) BCD result.
 */
extern uint16_t bin2bcd8(uint8_t bin);

/**
 * Converts a 16-bit binary value to 16-bit BCD.
 * It does not handle values over 9999 properly, due to not having a 5th digit.
 * @param	bin		The 16-bit binary value to convert.
 * @return	The converted 4 character (4 nibble) BCD result
 */
extern uint16_t bin2bcd16(uint16_t bin);

#endif /* BCDCONV_H_ */
