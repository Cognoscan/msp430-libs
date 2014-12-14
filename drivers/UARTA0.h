/*
 * @file UARTA0.h
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
 *
 * @details
 * Using this Driver
 * -----------------
 *
 *  To use this driver, a few routines must be incorporated into your main.c 
 *  code:
 *  
 *  - UARTA0_Init must be called during initialization of MSP430.
 *  - UARTA0_EnableInterrupts must be called during initialization of MSP430.
 *  - UARTA0_TX_ISR must be inserted into the USCIABTX_VECTOR ISR.
 *  - UARTA0_RX_ISR must be inserted into the USCIABRX_VECTOR ISR.
 *
 *  Simple example ISRs:
 * ~~~{.c}
 *
 * __attribute__((interrupt(USCIAB0RX_VECTOR)))
 * void USCI_AB0_RX_ISR(void)
 * {
 * 	UARTA0_RX_ISR();
 * 	_bic_SR_register_on_exit(LPM0_bits);
 * }
 * 
 * __attribute__((interrupt(USCIAB0TX_VECTOR)))
 * void USCI_AB0_TX_ISR(void)
 * {
 * 	UARTA0_TX_ISR();
 * }
 *
 * ~~~
 */

#ifndef _UARTA0_H_
#define _UARTA0_H_

#include <msp430.h>
#include "../FIFO.h"

#ifndef UARTA0_TX_BUFFER_SIZE
#define UARTA0_TX_BUFFER_SIZE  32
#endif
#ifndef UARTA0_RX_BUFFER_SIZE
#define UARTA0_RX_BUFFER_SIZE  32
#endif

uint8_t tx_buffer[UARTA0_TX_BUFFER_SIZE];
uint8_t rx_buffer[UARTA0_RX_BUFFER_SIZE];

FIFO_Buffer UARTA0_rx_buffer; ///< TX FIFO Buffer handler
FIFO_Buffer UARTA0_tx_buffer; ///< RX FIFO Buffer handler

/// @name UART Clock Configuration Constants
/// @{
static const uint8_t UARTA0_UCLK  = 0x00; ///< Use UCLK as clock source
static const uint8_t UARTA0_ACLK  = 0x40; ///< Use ACLK as clock source
static const uint8_t UARTA0_SMCLK = 0x80; ///< Use SMCLK as clock source
/// @}

/// @name UART Configuration Constants
/// @{
static const uint8_t UARTA0_NO_PARITY        = 0;
static const uint8_t UARTA0_ODD_PARITY       = UCPEN;
static const uint8_t UARTA0_EVEN_PARITY      = UCPEN | UCPAR;
static const uint8_t UARTA0_MSB_FIRST        = UCMSB;
static const uint8_t UARTA0_LSB_FIRST        = 0;
static const uint8_t UARTA0_OVERSAMPLING_ON  = UCOS16;
static const uint8_t UARTA0_OVERSAMPLING_OFF = 0;
/// @}


bool UARTA0_transmitting;

/*
 * Send a byte over UART.
 * @param data Byte to send.
 */
void UARTA0_Send(char data);

/* Retrieve a byte from UART buffer.
 * @returns Byte from UART buffer or 0.
 */
uint8_t UARTA0_Receive();

/*
 * Enable UART interrupts. Needed for this driver to run.
 */
void UARTA0_EnableInterrupts();

/*
 * Disable UART interrupts. Stops this driver from receiving.
 */
void UARTA0_DisableInterrupts();

/*
 * Set configuration registers.
 * @param clock_source Should be UARTA0_UCLK, UARTA0_ACLK, or UARTA0_SMCLK.
 * @param clock_prescaler0 Lower byte of clock prescaler divider
 * @param clock_prescaler1 Upper byte of clock prescaler divider
 * @param first_mod_reg First modulation register value (see family datasheet)
 * @param second_mod_reg Second modulation register value (see family datasheet)
 * @param parity Should be UARTA0_NO_PARITY, UARTA0_ODD_PARITY, or 
 *    UARTA0_EVEN_PARITY.
 * @param msb_or_lsb Should be UARTA0_MSB_FIRST or UARTA0_LSB_FIRST.
 * @param oversampling Should be UARTA0_OVERSAMPLING_ON or 
 *    UARTA0_OVERSAMPLING_OFF.
 */
static inline void UARTA0_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling)
{
  FIFO_Init(&UARTA0_tx_buffer, tx_buffer, UARTA0_TX_BUFFER_SIZE);
  FIFO_Init(&UARTA0_rx_buffer, rx_buffer, UARTA0_RX_BUFFER_SIZE);
  UARTA0_transmitting = false;

  UCA0CTL0 = parity | msb_or_lsb_first;
  UCA0CTL1 = clock_source | UCSWRST;

  UCA0BR0 = clock_prescaler0;
  UCA0BR1 = clock_prescaler1;
  UCA0MCTL = first_mod_reg | second_mod_reg | oversampling;

  UCA0CTL1 &= ~UCSWRST;
}


/*
 * Determines if receive buffer is empty or not.
 */
static inline bool UARTA0_Empty()
{
	return FIFO_Empty(&UARTA0_rx_buffer);
}

//void UARTA0_SendBreak(); Not yet implemented

/*
 * ISR for when byte is received.
 */
static inline void UARTA0_RX_ISR(void) __attribute__((always_inline));
static inline void UARTA0_RX_ISR(void)
{
	FIFO_Put(&UARTA0_rx_buffer, UCA0RXBUF);
}

/*
 * ISR for when byte has been sent.
 */
static inline void UARTA0_TX_ISR(void) __attribute__((always_inline));
static inline void UARTA0_TX_ISR(void)
{
  if (!FIFO_Empty(&UARTA0_tx_buffer))
  {
    UCA0TXBUF = FIFO_Get(&UARTA0_tx_buffer);
  // No more data left in buffer, so disable interrupt and let transmitting=0
	} else {
		UARTA0_transmitting = false;
		IE2 &= ~UCA0TXIE;
	}
}

#endif
