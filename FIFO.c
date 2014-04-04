/*
 * FIFO.c
 *
 *  Created on: Sep 25, 2012
 *
 * @copyright
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Cognoscan
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
#include <stdbool.h>
#include "fifo.h"

unsigned FIFO_Count (
		FIFO_BUFFER const *b)
{
	if (b)
	{
		if (b->head >= b->tail)
		{
			return (b->head - b->tail);
		}
		return (b->buffer_len - b->tail + b->head);
	}
	return 0;
}

bool FIFO_Full (
		FIFO_BUFFER const *b)
{
	return (b ? (FIFO_Count(b) == (b->buffer_len - 1)) : true);
}

bool FIFO_Empty(
		FIFO_BUFFER const *b)
{
	return (b ? (FIFO_Count(b) == 0) : true);
}

uint8_t FIFO_Peek(
		FIFO_BUFFER const *b)
{
	if (b) {
		return (b->buffer[b->tail]);
	}
	return 0;
}

uint8_t FIFO_Get(
		FIFO_BUFFER * b)
{
	uint8_t data_byte = 0;

	if (!FIFO_Empty(b)) {
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
		FIFO_BUFFER * b,
		uint8_t data_byte)
{
	bool status = false;

	if (b)
	{
		if (!FIFO_Full(b)) {
			b->buffer[b->head] = data_byte;
			b->head++;
			if (b->head >= b->buffer_len)
			{
				b->head = 0;
			}
			status=true;
		}
	}

	return status;
}

// Buffer length must be power of two
void FIFO_Init(
		FIFO_BUFFER * b,
		volatile uint8_t *buffer,
		unsigned buffer_len)
{
	if (b) {
		b->head = 0;
		b->tail = 0;
		b->buffer = buffer;
		b->buffer_len = buffer_len;
	}

	return;
}

