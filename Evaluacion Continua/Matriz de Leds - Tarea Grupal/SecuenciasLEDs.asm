.include "m328pdef.inc"

.org 0x0000
rjmp interrupcion
.org 0x0002
rjmp RSI_0

interrupcion:
             sei
             ldi r16, 0xA0
             out DDRD, r16
             ldi r16, 0xFF
             out DDRB, r16

             sbi PORTD, 2
             ldi r17, 0x08
             out spl, r16

             out sph, r17

             ldi r18, 0x01

             out EIMSK, r18
             ldi r19, 0x0F

             sts EICRA, r19
inicio:
    ldi r21, 0x00
    out PORTB, r21
    rjmp loop

S1:
    sbi PORTD, 5
    rcall delay
    cbi PORTD, 5
    rcall delay
    sbi PORTD, 7
    rcall delay
    cbi PORTD, 7
    rcall delay
    sbi PORTB, 0
    rcall delay
    cbi PORTB, 0
    rcall delay
    sbi PORTB, 1
    rcall delay
    cbi PORTB, 1
    rcall delay
    sbi PORTB, 2
    rcall delay
    cbi PORTB, 2
    rcall delay
    sbi PORTB, 3
    rcall delay
    cbi PORTB, 3
    rcall delay
    sbi PORTB, 4
    rcall delay
    cbi PORTB, 4
    rcall delay
    sbi PORTB, 5
    rcall delay
    cbi PORTB, 5
    rcall delay
    rjmp loop

loop:
    cpi r21, 0
    breq S1
    cpi r21, 1
    breq S2
    cpi r21, 2
    breq S3
    cpi r21, 3
    breq inicio
    rjmp loop


S2:
    sbi PORTD, 5
    rcall delay
    sbi PORTD, 7
    rcall delay
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
    cbi PORTD, 5
    rcall delay
    cbi PORTD, 7
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
    rjmp loop
S3:
    ldi r17, 0b00100000
    out PORTB, r17
    ldi r17, 0b00100000
    out PORTD, r17
    rcall delay
    ldi r17, 0b00110000
    out PORTB, r17
    ldi r17, 0b10100000
    out PORTD, r17
    rcall delay
    ldi r17, 0b00111001
    out PORTB, r17
    rcall delay
    ldi r17, 0b00111111
    out PORTB, r17
    rcall delay
    ldi r17, 0b00011111
    out PORTB, r17
    ldi r17, 0b10000000
    out PORTD, r17
    rcall delay
    ldi r17, 0b00001111
    out PORTB, r17
    ldi r17, 0b00000000
    out PORTD, r17
    rcall delay
    ldi r17, 0b00000110
    out PORTB, r17
    rcall delay
    ldi r17, 0b00000000
    out PORTB, r17
    rcall delay
    rjmp loop

RSI_0:
    inc r21
    cbi EIMSK, INT0
    rcall delay
    sbi EIMSK, INT0
    reti


delay:
    ldi r18, 52
L1: ldi r19, 255
L2: ldi r20, 200
L3: dec r20
    brne L3
    dec r19
    brne L2
    dec r18
    brne L1
    ret
