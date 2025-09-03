.include "m324pdef.inc" ; Define device ATmega324P
.equ clock=16000000 ; Define the clock frequency
.cseg
.org 000000
jmp inicio

inicio:
    ldi r16, high(ramend)
    out sph, r16
    ldi r16, low(ramend)
    out spl, r16

    ldi r16, 0xFF
    out DDRB, r16
    out DDRD, r16

    ldi r16, 0
    out PORTB, r16
    sts TCCR1A, r16
    ldi r16, (1<<CS12)
    sts TCCR1B, r16
    ldi r16, 0
    sts TCCR1C, r16
    sts TIMSK1, r16
    ldi r16, high(34286)
    sts TCNT1H, r16
    ldi r16, low(34286)
    sts TCNT1L, r16

    sei

main:
    ldi r17, 0
    ldi r18, 0b11111111
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 1
    ldi r18, 0b11011011
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 2
    ldi r18, 0b11011011
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 3
    ldi r18, 0b11111111
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 4
    ldi r18, 0b11111111
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 5
    ldi r18, 0b10111101
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 6
    ldi r18, 0b11000011
    out PORTB, r17
    out PORTD, r18
    rcall delay

    ldi r17, 7
    ldi r18, 0b11111111
    out PORTB, r17
    out PORTD, r18
    rcall delay
    rjmp main

delay:
    sbis tifr1, tov1
    rjmp delay
    sbi tifr1, tov1
    ldi r16, high(34286)
    sts TCNT1H, r16
    ldi r16, low(34286)
    sts TCNT1L, r16
    ret
