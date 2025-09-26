.cseg
.org 0x0000
    rjmp inicio

.org  0x0012
    rjmp RSI

inicio:
	sei
    ldi r16, HIGH(RAMEND)
    out SPH, r16
    ldi r16, LOW(RAMEND)
    out SPL, r16
    clr r1

    ldi r16, 0b11111111
    out DDRD, r16
    clr r16
    out PORTD, r16

    clr r18

    ldi r16, 0
    sts TCCR2A, r16
    ldi r16, (0<<CS12)|(1<<CS10)
    sts TCCR2B, r16

    ldi r16, (1<<OCIE2A)
    sts TIMSK2, r16

main:
    rjmp main

RSI:
    push r16
    in   r16, SREG
    push r16

    ldi ZL, low(LUT<<1)
    ldi ZH, high(LUT<<1)
    add ZL, r18
    adc ZH, r1
    lpm r16, Z

    out PORTD, r16

    inc r18

	ldi r16, low(Final<<1)
	ldi r17, high(Final<<1)

	cp ZL, r16
	cpc ZH, r17
	brsh RESET_Z
	rjmp ISR_END

	RESET_Z:
	clr r18

	ISR_END:
 
    pop r16
    out SREG, r16
    pop r16
    reti

.org 0x200
LUT:

.db 112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112
.db 112,113,113,113,113,113,113,113,113,113,113,114,114,114,114,114
.db 114,114,114,114,115,115,115,115,115,115,115,115,116,116,116,116
.db 116,116,117,117,117,117,117,118,118,118,118,118,119,119,119,119
.db 120,120,120,120,121,121,121,122,122,122,123,123,123,124,124,125
.db 125,125,126,126,127,127,128,128,129,129,130,130,131,131,132,132
.db 133,133,134,135,135,136,137,137,138,139,140,141,141,142,143,144
.db 145,146,147,148,150,151,152,154,155,157,159,160,163,165,168,112
Final:
