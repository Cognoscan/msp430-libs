/*
 * @file FIFO.h
 * @brief FIFO Buffer implementation
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

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>

struct fifo_buffer_t {
	volatile unsigned head;   // 1st byte location
	volatile unsigned tail;   // last byte location
	volatile uint8_t *buffer; // buffer block
	uint8_t buffer_len;       // length of the buffer
  uint8_t count;            // bytes currently in buffer
};

typedef struct fifo_buffer_t FIFO_Buffer;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline uint8_t FIFO_Count (FIFO_Buffer const *b)
{
  return b->count;
}

static inline bool FIFO_Full (FIFO_Buffer const *b)
{
  return (b->count == (b->buffer_len -1));
}

static inline bool FIFO_Empty(FIFO_Buffer const *b)
{
  return (b->count == 0);
}

static inline uint8_t FIFO_Peek(
    FIFO_Buffer const *b)
{
  return (b->buffer[b->tail]);
}

uint8_t FIFO_Get(FIFO_Buffer * b);

bool FIFO_Put(FIFO_Buffer * b, uint8_t data_byte);

// Buffer length must be power of two
void FIFO_Init(
		FIFO_Buffer * b,
		volatile uint8_t *buffer,
		unsigned buffer_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FIFO_H_ */
