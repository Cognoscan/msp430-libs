
#ifndef _RS485A_H_
#define _RS485A_H_

#include <msp430.h>
#include "../FIFO.h"

/// @name UART Clock Configuration Constants
/// @{
static const uint8_t RS485A_UCLK  = 0x00;
static const uint8_t RS485A_ACLK  = 0x40;
static const uint8_t RS485A_SMCLK = 0x80;
/// @}

/// @name UART Configuration Constants
/// @{
static const uint8_t RS485A_NO_PARITY        = 0;
static const uint8_t RS485A_ODD_PARITY       = UCPEN;
static const uint8_t RS485A_EVEN_PARITY      = UCPEN | UCPAR;
static const uint8_t RS485A_MSB_FIRST        = UCMSB;
static const uint8_t RS485A_LSB_FIRST        = 0;
static const uint8_t RS485A_ONE_STOP_BIT     = 0;
static const uint8_t RS485A_TWO_STOP_BITS    = UCSPB;
static const uint8_t RS485A_OVERSAMPLING_ON  = UCOS16;
static const uint8_t RS485A_OVERSAMPLING_OFF = 0;
/// @}

FIFO_Buffer RS485A_rx_buffer;
FIFO_Buffer RS485A_tx_buffer;

bool RS485A_transmitting;

void RS485A_Init(
    uint8_t clock_source,
    uint8_t clock_prescaler0,
    uint8_t clock_prescaler1,
    uint8_t first_mod_reg,
    uint8_t second_mod_reg,
    uint8_t parity,
    uint8_t msb_or_lsb_first,
    uint8_t oversampling);

void RS485A_Send(uint8_t data);
uint8_t RS485A_Receive();
void RS485A_EnableInterrupts();
void RS485A_DisableInterrupts();
//void RS485A_SendBreak(); Not yet implemented

#endif
