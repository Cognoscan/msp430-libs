
#ifndef _UARTA0_H_
#define _UARTA0_H_

#include <msp430.h>
#include "../FIFO.h"

/// @name UART Clock Configuration Constants
/// @{
static const uint8_t UARTA0_UCLK  = 0x00;
static const uint8_t UARTA0_ACLK  = 0x40;
static const uint8_t UARTA0_SMCLK = 0x80;
/// @}

/// @name UART Configuration Constants
/// @{
static const uint8_t UARTA0_NO_PARITY        = 0;
static const uint8_t UARTA0_ODD_PARITY       = UCPEN;
static const uint8_t UARTA0_EVEN_PARITY      = UCPEN | UCPAR;
static const uint8_t UARTA0_MSB_FIRST        = UCMSB;
static const uint8_t UARTA0_LSB_FIRST        = 0;
static const uint8_t UARTA0_ONE_STOP_BIT     = 0;
static const uint8_t UARTA0_TWO_STOP_BITS    = UCSPB;
static const uint8_t UARTA0_OVERSAMPLING_ON  = UCOS16;
static const uint8_t UARTA0_OVERSAMPLING_OFF = 0;
/// @}

FIFO_Buffer UARTA0_rx_buffer;
FIFO_Buffer UARTA0_tx_buffer;

bool UARTA0_transmitting;

void UARTA0_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling);

void UARTA0_Send(uint8_t data);
uint8_t UARTA0_Receive();
void UARTA0_EnableInterrupts();
void UARTA0_DisableInterrupts();
//void UARTA0_SendBreak(); Not yet implemented

//=============================================================================
// UART RX Interrupt
//=============================================================================
static inline void UARTA0_RX_ISR(void) __attribute__((always_inline));
static inline void UARTA0_RX_ISR(void)
{
	FIFO_Put(&UARTA0_rx_buffer, UCA0RXBUF);
}

//=============================================================================
// UART TX Interrupt
//=============================================================================
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
