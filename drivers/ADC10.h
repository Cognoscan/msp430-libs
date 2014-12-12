/*
 * @file ADC10.h
 * @brief Driver for ADC10 on MSP430.
 * @author Scott Teal (Scott@Teals.org)
 * @date 2014-04-07
 * @copyright
 *
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Scott Teal
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

#ifndef _ADC10_H_
#define _ADC10_H_

#include <msp430.h>
#include <stdint.h>

/**
 * Read from the given analog channel.
 * @param channel The channel number to read from (0 to 15)
 * @returns The raw 10-bit analog value from the ADC
 */
int16_t ADC10_AnalogRead(const uint8_t channel);

/**
 * Read the internal temperature sensor.
 * @returns the temperature in degrees celsius.
 */
float ADC10_TempRead();

/// Initialize the temperature compensation constants
void ADC10_TempInit();

static inline void ADC10_EnableAnalog(const uint8_t channel)
{
  ADC10AE0 |= (0x1 << channel);
}

static inline void ADC10_DisableAnalog(const uint8_t channel)
{
  ADC10AE0 &= ~(0x1 << channel);
}

#endif
