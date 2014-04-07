
#ifndef _UARTA_H_
#define _UARTA_H_

#include <msp430.h>
#include "../FIFO.h"

/// @name UART Clock Configuration Constants
/// @{
static const uint8_t UARTA_UCLK  = 0x00;
static const uint8_t UARTA_ACLK  = 0x40;
static const uint8_t UARTA_SMCLK = 0x80;
/// @}

/// @name UART Configuration Constants
/// @{
static const uint8_t UARTA_NO_PARITY        = 0;
static const uint8_t UARTA_ODD_PARITY       = UCPEN;
static const uint8_t UARTA_EVEN_PARITY      = UCPEN | UCPAR;
static const uint8_t UARTA_MSB_FIRST        = UCMSB;
static const uint8_t UARTA_LSB_FIRST        = 0;
static const uint8_t UARTA_ONE_STOP_BIT     = 0;
static const uint8_t UARTA_TWO_STOP_BITS    = UCSPB;
static const uint8_t UARTA_OVERSAMPLING_ON  = UCOS16;
static const uint8_t UARTA_OVERSAMPLING_OFF = 0;
/// @}

FIFO_Buffer UARTA_rx_buffer;
FIFO_Buffer UARTA_tx_buffer;

bool UARTA_transmitting;

void UARTA_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling);

void UARTA_Send(uint8_t data);
uint8_t UARTA_Receive();
void UARTA_EnableInterrupts();
void UARTA_DisableInterrupts();
//void UARTA_SendBreak(); Not yet implemented

//=============================================================================
// UART RX Interrupt
//=============================================================================
inline static void UARTA_RX_ISR(void) __attribute__((always_inline));
inline static void UARTA_RX_ISR(void)
{
	FIFO_Put(&UARTA_rx_buffer, UCA0RXBUF);
}

//=============================================================================
// UART TX Interrupt
//=============================================================================
inline static void UARTA_TX_ISR(void) __attribute__((always_inline));
inline static void UARTA_TX_ISR(void)
{
  if (!FIFO_Empty(&UARTA_tx_buffer))
  {
    UCA0TXBUF = FIFO_Get(&UARTA_tx_buffer);
	// No more data left in buffer, so disable interrupt and let transmitting=0
	} else {
		UARTA_transmitting = false;
		IE2 &= ~UCA0TXIE;
	}
}

#endif
