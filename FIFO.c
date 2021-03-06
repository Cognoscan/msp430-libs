/*
 * @file FIFO.c
 * @brief FIFO Buffer implementation source file
 * @author Scott Teal (Scott@Teals.org)
 * @date 2012-09-25
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

#include <stdint.h>
#include "FIFO.h"


uint8_t FIFO_Get(
    FIFO_Buffer * b)
{
  uint8_t data_byte = 0;
  if (!FIFO_Empty(b)) {
    b->count -= 1;
    data_byte = b->buffer[b->tail];
    b->tail++;
    if (b->tail >= b->buffer_len)
    {
      b->tail = 0;
    }
  }
  return data_byte;
}

bool FIFO_Put(
    FIFO_Buffer * b,
    uint8_t data_byte)
{
  if (!FIFO_Full(b)) {
    b->count += 1;
    b->buffer[b->head] = data_byte;
    b->head++;
    if (b->head >= b->buffer_len)
    {
      b->head = 0;
    }
    return true;
  }

  return false;
}

// Buffer length must be power of two
void FIFO_Init(
    FIFO_Buffer * b,
    volatile uint8_t *buffer,
    unsigned buffer_len)
{
  b->head = 0;
  b->tail = 0;
  b->buffer = buffer;
  b->buffer_len = buffer_len;
  b->count = 0;
}

