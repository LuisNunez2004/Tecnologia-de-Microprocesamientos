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

    ldi r16, 0
    out PORTB, r16
    sts TCCR1A, r16
    ldi r16, (1<<CS12)
    sts TCCR1B, r16
    ldi r16, 0
    sts TCCR1C, r16
    sts TIMSK1, r16
    ldi r16, high(3036)
    sts TCNT1H, r16
    ldi r16, low(3036)
    sts TCNT1L, r16

    sei

main:
    sbi PORTB, 0
    rcall delay
    sbi PORTB, 1
    rcall delay
    sbi PORTB, 2
    rcall delay
    sbi PORTB, 3
    rcall delay
    sbi PORTB, 4
    rcall delay
    sbi PORTB, 5
    rcall delay
    sbi PORTB, 6
    rcall delay
    sbi PORTB, 7
    rcall delay
    cbi PORTB, 0
    rcall delay
    cbi PORTB, 1
    rcall delay
    cbi PORTB, 2
    rcall delay
    cbi PORTB, 3
    rcall delay
    cbi PORTB, 4
    rcall delay
    cbi PORTB, 5
    rcall delay
    cbi PORTB, 6
    rcall delay
    cbi PORTB, 7
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

