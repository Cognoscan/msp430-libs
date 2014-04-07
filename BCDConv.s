; *****************************************************************************
; File: BCDConv.s43
; Author: Scott Teal
; Date: 2012-09-26
;
; Functions for efficient conversion from binary to BCD
; Copyright:
; The MIT License (MIT)
; 
; Copyright (c) 2014 Cognoscan
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
;
; *****************************************************************************

  ;.cdecls C,LIST,  "msp430.h"

  .text

; *****************************************************************************
; bin2bcd8
; Takes in a binary 8-bit value and converts it to BCD.
; The result can be up to 3 nibbles long.
; *****************************************************************************

  .global bin2bcd8

bin2bcd8:
  mov     #0,R13      ; BCD result
  mov     #8,R14      ; loop counter
bin2bcd8L:
  rla.b   R12        ; Loads highest bit into C
  dadd.w  R13, R13    ; BCD routine
  dec     R14        ; decrement loop counter
  jnz     bin2bcd8L    ; LOOP
  mov     R13,R12      ; Put result in R12
  ret

; *****************************************************************************
; bin2bcd16
; Takes in a binary 16-bit value and converts it to BCD.
; The result can be up to 4 nibbles long, meaning that anything over 9999 will
; not convert properly.
; *****************************************************************************

  .global bin2bcd16

bin2bcd16:
  mov     #0,R13      ; BCD result
  mov     #16,R14      ; loop counter
bin2bcd16L:
  rla.w   R12        ; Loads highest bit into C
  dadd.w  R13, R13    ; BCD routine
  dec     R14        ; decrement loop counter
  jnz     bin2bcd16L    ; LOOP
  mov     R13,R12      ; Put result in R12
  ret


