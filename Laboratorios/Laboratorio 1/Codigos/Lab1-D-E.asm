.org    0x0000
     rjmp inicializacion
.equ	F_CPU = 16000000
.equ	baud = 9600
.equ	bps = (F_CPU/16/baud) - 1
inicializacion:
	 ldi r16, high(ramend)
	 out sph, r16
	 ldi r16, low(ramend)
	 out spl, r16

	 ldi r16, low(bps)
	 ldi r17, high(bps)
	 rcall initUART
	 ldi r16, 0xFE
	 out DDRD, r16
	 rjmp msgInicial
initUART:
   sts UBRR0L, r16
   sts UBRR0H, r17
   ldi r16, (1<<RXEN0)|(1<<TXEN0)
   sts UCSR0B, r16
   ret
msgInicial:
   ldi r16, 0x31
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x54
   rcall putc
   ldi r16, 0x72
   rcall putc
   ldi r16, 0x69
   rcall putc
   ldi r16, 0x61
   rcall putc
   ldi r16, 0x6E
   rcall putc
   ldi r16, 0x67
   rcall putc
   ldi r16, 0x75
   rcall putc
   ldi r16, 0x6C
   rcall putc
   ldi r16, 0x6F
   rcall putc
   ldi r16, 0x0A
   rcall putc
   ldi r16, 0x32
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x43
   rcall putc
   ldi r16, 0x69
   rcall putc
   ldi r16, 0x72
   rcall putc
   ldi r16, 0x63
   rcall putc
   ldi r16, 0x75
   rcall putc
   ldi r16, 0x6C
   rcall putc
   ldi r16, 0x6F
   rcall putc
   ldi r16, 0x0A
   rcall putc
   ldi r16, 0x33
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x43
   rcall putc
   ldi r16, 0x72
   rcall putc
   ldi r16, 0x75
   rcall putc
   ldi r16, 0x7A
   rcall putc
   ldi r16, 0x0A
   rcall putc
   ldi r16, 0x54
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x54
   rcall putc
   ldi r16, 0x6F
   rcall putc
   ldi r16, 0x64
   rcall putc
   ldi r16, 0x6F
   rcall putc
   ldi r16, 0x0A
   rcall putc
   rjmp wait
putc:
   lds r17, UCSR0A
   sbrs r17, UDRE0
   rjmp putc
   sts UDR0, r16
   ldi r16, 0
   ret
getc:
   lds r17, UCSR0A
   sbrs r17, RXC0
   rjmp getc
   lds r16, UDR0
   ret
wait:
   rcall getc
   cpi r16, 0x31
   breq triangulo
   cpi r16, 0x33
   breq cruz
   rjmp wait
s1Delay:
    ldi r18, 199
L1: ldi r19, 199
L2: ldi r20, 134
L3: dec r20
    brne L3
    dec r19
    brne L2
    dec r18
    brne L1
    ret
triangulo:
	rcall centrar
	sbi portd, 2
	rcall s1delay
	cbi portd, 2
	sbi portd, 5
	rcall s1delay
	rcall s1delay
	cbi portd, 5
	sbi portd, 4
	sbi portd, 6
	rcall s1delay
	cbi portd, 6
	sbi portd, 7
	rcall s1delay
	cbi portd, 7
	cbi portd, 4
	sbi portd, 3
	rcall s1delay
	cbi portd, 3
	rcall descentrar
	rjmp msgInicial
centrar:
	sbi portd, 7
	sbi portd, 4
	ldi r21, 5
cenloop1:
	rcall s1Delay
	dec r21
	cpi r21, 0
	brne cenloop1
	cbi portd, 4
	ldi r21, 5
cenloop2:
	rcall s1Delay
	dec r21
	cpi r21, 0
	brne cenloop2
	cbi portd, 7
	ret
descentrar:
	sbi portd, 6
	sbi portd, 5
	ldi r21, 5
dcenloop1:
	rcall s1Delay
	dec r21
	cpi r21, 0
	brne dcenloop1
	cbi portd, 5
	ldi r21, 5
dcenloop2:
	rcall s1Delay
	dec r21
	cpi r21, 0
	brne dcenloop2
	cbi portd, 6
	ret
cruz:
	rcall centrar
	sbi portd, 2
	rcall s1delay
	cbi portd, 2
	sbi portd, 4
	rcall s1delay
	rcall s1delay
	cbi portd, 4
	sbi portd, 5
	rcall s1delay
	cbi portd, 5
	sbi portd, 7
	rcall s1delay
	cbi portd, 7
	sbi portd, 6
	rcall s1delay
	rcall s1delay
	cbi portd, 6
	sbi portd, 7
	rcall s1delay
	cbi portd, 7
	sbi portd, 3
	rcall s1delay
	cbi portd, 3
	rjmp msgInicial
