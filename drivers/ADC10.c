/**
 * @file ADC10.h
 * @brief Driver for ADC10 on MSP430.
 * @author Scott Teal (Scott@Teals.org)
 * @date 2014-04-18
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

#include "ADC10.h"
#include "../utils.h"

/// Value to multiply raw ADC value by for temperature
static float ADC10_temp_compensation_scalar;

/// Value to add to scaled ADC value
static float ADC10_temp_compensation_offset;

int16_t ADC10_AnalogRead(const uint8_t channel)
{
  ADC10CTL0 |= ADC10ON | ADC10SHT_0;
  ADC10CTL1 = (channel << 12) | ADC10SSEL_2;
  ADC10CTL0 |= ADC10SC | ENC;
  DelayCycles(64);
  while (ADC10CTL1 & ADC10BUSY);
  return ADC10MEM;

}

float ADC10_TempRead()
{
  ADC10CTL0 |= ADC10ON | REFON | ADC10SHT_2;
  ADC10CTL1 = (10 << 12) | ADC10SSEL_2;
  ADC10CTL0 |= ADC10SC | ENC;
  while (ADC10CTL1 & ADC10BUSY);
  return ADC10MEM * ADC10_temp_compensation_scalar
    + ADC10_temp_compensation_offset;
}

void ADC10_TempInit()
{
  // Hard code in TLV lookup
	const char *CALADC_15T85 = (char *)(TLV_ADC10_1_TAG_ + 0x02 + CAL_ADC_15T85);
	const char *CALADC_15T30 = (char *)(TLV_ADC10_1_TAG_ + 0x02 + CAL_ADC_15T30);

  // Calculate scalar and offset from calibrated values - this will take a while
  ADC10_temp_compensation_scalar = (85.0f - 30.0f)/(*CALADC_15T85 - *CALADC_15T30);
  ADC10_temp_compensation_offset = 30.0f - ADC10_temp_compensation_scalar * *CALADC_15T30;
}
