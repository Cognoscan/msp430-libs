#include "UARTA.h"

#ifndef UARTA_TX_BUFFER_SIZE
#define UARTA_TX_BUFFER_SIZE  32
#endif
#ifndef UARTA_RX_BUFFER_SIZE
#define UARTA_RX_BUFFER_SIZE  32
#endif

uint8_t tx_buffer[UARTA_TX_BUFFER_SIZE];
uint8_t rx_buffer[UARTA_RX_BUFFER_SIZE];

void UARTA_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling)
{
  FIFO_Init(&UARTA_tx_buffer, tx_buffer, UARTA_TX_BUFFER_SIZE);
  FIFO_Init(&UARTA_rx_buffer, rx_buffer, UARTA_RX_BUFFER_SIZE);
  UARTA_transmitting = false;

  UCA0CTL0 = parity | msb_or_lsb_first;
  UCA0CTL1 = clock_source | UCSWRST;

  UCA0BR0 = clock_prescaler0;
  UCA0BR1 = clock_prescaler1;
  UCA0MCTL = first_mod_reg | second_mod_reg | oversampling;

  UCA0CTL1 &= ~UCSWRST;
}

void UARTA_EnableInterrupts() {
  IE2 |= UCA0RXIE;
}

void UARTA_DisableInterrupts() {
  IE2 |= UCA0RXIE;
}

//=============================================================================
// UART Transmit Function
//=============================================================================
void UARTA_Send(uint8_t data)
{
	// Put straight to UART if not yet transmitting
	if (!UARTA_transmitting)
	{
		UCA0TXBUF = data;
		UARTA_transmitting = 1;
		IE2 |= UCA0TXIE;
	  // Load data into TX buffer
	} else {
		// Make sure the buffer isn't full. Loop continuously if it is.
    while(FIFO_Full(&UARTA_tx_buffer));
    FIFO_Put(&UARTA_tx_buffer, data);
	}
}

uint8_t UARTA_Receive()
{
	return FIFO_Get(&UARTA_rx_buffer);
}

//=============================================================================
// UART RX Interrupt
//=============================================================================
void __interrupt(USCIAB0RX_VECTOR) UART_Rx_ISR(void)
{
	FIFO_Put(&UARTA_rx_buffer, UCA0RXBUF);
	_bic_SR_register_on_exit(LPM0_bits);
}

//=============================================================================
// UART TX Interrupt
//=============================================================================
void __interrupt(USCIAB0TX_VECTOR) UART_Tx_ISR(void)
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
