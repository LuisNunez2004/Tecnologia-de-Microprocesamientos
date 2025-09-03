.include "m328pdef.inc"

.org 0x00

jmp inicio

inicio:
       ldi r16, low(RAMEND)
       out SPL, r16
       ldi r16, high(RAMEND)
       out SPH, r16

       ldi r16, 0xFF
       out DDRB, r16
       out DDRD, r16

       ldi r16, 0
       sts TCCR1A, r16
       ldi r16, (1<<CS12)
       sts TCCR1B, r16
       ldi r16, 0
       sts TCCR1C, r16
       ldi r16, 0
       sts TIMSK1, r16
       ldi r16, high(131)
       sts TCNT1H, r16
       ldi r16, low(131)
       sts TCNT1L, r16

       sei

cara_triste:
            ldi r16, 0x00
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x01
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x02
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x03
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x04
            ldi r17, 0b00000000
            rcall retardo2ms

            ldi r16, 0x05
            ldi r17, 0b00111100
            rcall retardo2ms

            ldi r16, 0x06
            ldi r17, 0b01000010
            rcall retardo2ms

            ldi r16, 0x07
            ldi r17, 0b10000001
            rcall retardo2ms

            rjmp rombo

rombo:
      ldi r16, 0x00
            ldi r17, 0b00011000
            rcall retardo2ms

            ldi r16, 0x01
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x02
            ldi r17, 0b01000010
            rcall retardo2ms

            ldi r16, 0x03
            ldi r17, 0b10000001
            rcall retardo2ms

            ldi r16, 0x04
            ldi r17, 0b10000001
            rcall retardo2ms

            ldi r16, 0x05
            ldi r17, 0b01000010
            rcall retardo2ms

            ldi r16, 0x06
            ldi r17, 0b00100100
            rcall retardo2ms

            ldi r16, 0x07
            ldi r17, 0b00011000
            rcall retardo2ms

            rjmp space_invader

space_invader:
              ldi r16, 0x00
              ldi r17, 0b00100100
              rcall retardo2ms

              ldi r16, 0x01
              ldi r17, 0b01111110
              rcall retardo2ms

              ldi r16, 0x02
              ldi r17, 0b11111111
              rcall retardo2ms

              ldi r16, 0x03
              ldi r17, 0b10111101
              rcall retardo2ms

              ldi r16, 0x04
              ldi r17, 0b11111111
              rcall retardo2ms

              ldi r16, 0x05
              ldi r17, 0b01011010
              rcall retardo2ms

              ldi r16, 0x06
              ldi r17, 0b10100101
              rcall retardo2ms

              ldi r16, 0x07
              ldi r17, 0b11000011
              rcall retardo2ms

              rjmp inicio


retardo2ms:
            sbis TIFR1, TOV1
            rjmp retardo2ms
            sbi TIFR1, TOV1
            ldi r16,  high(131)
            sts TCNT1H, r16
            ldi r16, low(131)
            sts TCNT1L, r16
            ret


