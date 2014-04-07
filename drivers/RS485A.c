#include "RS485A.h"

#ifndef RS485A_TX_BUFFER_SIZE
#define RS485A_TX_BUFFER_SIZE  32
#endif
#ifndef RS485A_RX_BUFFER_SIZE
#define RS485A_RX_BUFFER_SIZE  32
#endif

uint8_t tx_buffer[RS485A_TX_BUFFER_SIZE];
uint8_t rx_buffer[RS485A_RX_BUFFER_SIZE];

void RS485A_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling)
{
  FIFO_Init(&RS485A_tx_buffer, tx_buffer, RS485A_TX_BUFFER_SIZE);
  FIFO_Init(&RS485A_rx_buffer, rx_buffer, RS485A_RX_BUFFER_SIZE);
  RS485A_transmitting = false;

  UCA0CTL0 = parity | msb_or_lsb_first;
  UCA0CTL1 = clock_source | UCSWRST;

  UCA0BR0 = clock_prescaler0;
  UCA0BR1 = clock_prescaler1;
  UCA0MCTL = first_mod_reg | second_mod_reg | oversampling;

  UCA0CTL1 &= ~UCSWRST;
}

void RS485A_EnableInterrupts() {
  IE2 |= UCA0RXIE;
}

void RS485A_DisableInterrupts() {
  IE2 |= UCA0RXIE;
}

//=============================================================================
// UART Transmit Function
//=============================================================================
void RS485A_Send(uint8_t data)
{
	// Put straight to UART if not yet transmitting
	if (!RS485A_transmitting)
	{
		UCA0TXBUF = data;
		RS485A_transmitting = 1;
		IE2 |= UCA0TXIE;
	  // Load data into TX buffer
	} else {
		// Make sure the buffer isn't full. Loop continuously if it is.
    while(FIFO_Full(&RS485A_tx_buffer));
    FIFO_Put(&RS485A_tx_buffer, data);
	}
}

uint8_t RS485A_Receive()
{
	return FIFO_Get(&RS485A_rx_buffer);
}

//=============================================================================
// UART RX Interrupt
//=============================================================================
void RS485A_Rx_ISR(void)
{
	FIFO_Put(&RS485A_rx_buffer, UCA0RXBUF);
}

//=============================================================================
// UART TX Interrupt
//=============================================================================
void RS485A_Tx_ISR(void)
{
  if (!FIFO_Empty(&RS485A_tx_buffer))
  {
    UCA0TXBUF = FIFO_Get(&RS485A_tx_buffer);
	// No more data left in buffer, so disable interrupt and let transmitting=0
	} else {
		RS485A_transmitting = false;
		IE2 &= ~UCA0TXIE;
	}
}
