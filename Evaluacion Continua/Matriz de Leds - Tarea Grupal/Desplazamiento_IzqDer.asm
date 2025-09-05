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

       ldi r16, 0
       sts TCCR1A, r16
       ldi r16, (1<<CS12)
       sts TCCR1B, r16
       ldi r16, 0
       sts TCCR1C, r16
       ldi r16, 0
       sts TIMSK1, r16
       ldi r16, high(34286)
       sts TCNT1H, r16
       ldi r16, low(34286)
       sts TCNT1L, r16

       sei

Desp_IzqDer:
       sbi PORTB, PORTB7
       call retardo1s
       cbi PORTB, PORTB7
       call retardo1s
       sbi PORTB, PORTB6
       call retardo1s
       cbi PORTB, PORTB6
       call retardo1s
       sbi PORTB, PORTB5
       call retardo1s
       cbi PORTB, PORTB5
       call retardo1s
       sbi PORTB, PORTB4
       call retardo1s
       cbi PORTB, PORTB4
       call retardo1s
       sbi PORTB, PORTB3
       call retardo1s
       cbi PORTB, PORTB3
       call retardo1s
       sbi PORTB, PORTB2
       call retardo1s
       cbi PORTB, PORTB2
       call retardo1s
       sbi PORTB, PORTB1
       call retardo1s
       cbi PORTB, PORTB1
       call retardo1s
       sbi PORTB, PORTB0
       call retardo1s
       cbi PORTB, PORTB0
       call retardo1s

       rjmp inicio

retardo1s:
        sbis TIFR1, TOV1
        rjmp retardo1s
        sbi TIFR1, TOV1
        ldi r16,  high(34286)
        sts TCNT1H, r16
        ldi r16, low(34286)
        sts TCNT1L, r16
        ret


