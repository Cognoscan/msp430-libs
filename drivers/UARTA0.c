#include "UARTA0.h"

#ifndef UARTA0_TX_BUFFER_SIZE
#define UARTA0_TX_BUFFER_SIZE  32
#endif
#ifndef UARTA0_RX_BUFFER_SIZE
#define UARTA0_RX_BUFFER_SIZE  32
#endif

uint8_t tx_buffer[UARTA0_TX_BUFFER_SIZE];
uint8_t rx_buffer[UARTA0_RX_BUFFER_SIZE];

void UARTA0_Init(
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

