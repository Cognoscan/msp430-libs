/*
 * @file UARTA0.c
 * @brief Driver for UART on MSP430 USCI A0.
 * @author Scott Teal (Scott@Teals.org)
 * @date 2014-04-06
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
#include "UARTA0.h"

void UARTA0_EnableInterrupts() {
  IE2 |= UCA0RXIE;
}

void UARTA0_DisableInterrupts() {
  IE2 |= UCA0RXIE;
}

//=============================================================================
// UART Transmit Function
//=============================================================================
void UARTA0_Send(uint8_t data)
{
	// Put straight to UART if not yet transmitting
	if (!UARTA0_transmitting)
	{
		UCA0TXBUF = data;
		UARTA0_transmitting = 1;
		IE2 |= UCA0TXIE;
	  // Load data into TX buffer
	} else {
		// Make sure the buffer isn't full. Loop continuously if it is.
    while(FIFO_Full(&UARTA0_tx_buffer));
    FIFO_Put(&UARTA0_tx_buffer, data);
	}
}

uint8_t UARTA0_Receive()
{
	return FIFO_Get(&UARTA0_rx_buffer);
}

