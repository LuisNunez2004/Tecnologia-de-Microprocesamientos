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
	 ldi r16, 0x00
	 out DDRC, r16
	 ldi r16, 0x7A
	 out DDRD, r16
	 sbi PORTC, 0
	 rjmp msgInicial
initUART:
   sts UBRR0L, r16
   sts UBRR0H, r17
   ldi r16, (1<<RXEN0)|(1<<TXEN0)
   sts UCSR0B, r16
   ret
msgInicial:
   ldi r16, 0x4C
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x3E
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x4C
   rcall putc
   ldi r16, 0x69
   rcall putc
   ldi r16, 0x67
   rcall putc
   ldi r16, 0x65
   rcall putc
   ldi r16, 0x72
   rcall putc
   ldi r16, 0x61
   rcall putc
   ldi r16, 0x0A
   rcall putc
   ldi r16, 0x4D
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x3E
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x4D
   rcall putc
   ldi r16, 0x65
   rcall putc
   ldi r16, 0x64
   rcall putc
   ldi r16, 0x69
   rcall putc
   ldi r16, 0x61
   rcall putc
   ldi r16, 0x6E
   rcall putc
   ldi r16, 0x61
   rcall putc
   ldi r16, 0x0A
   rcall putc
   ldi r16, 0x50
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x3E
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x50
   rcall putc
   ldi r16, 0x65
   rcall putc
   ldi r16, 0x73
   rcall putc
   ldi r16, 0x61
   rcall putc
   ldi r16, 0x64
   rcall putc
   ldi r16, 0x61
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
pesada:
	sbi PORTD, 3
	cbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 3
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	sbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 5
	rjmp msgInicial
wait:
   rcall getc
   cpi r16, 0x4C
   breq ligera
   cpi r16, 0x4D
   breq mediana
   cpi r16, 0x50
   breq pesada
   rjmp wait
ligera:
	sbi PORTD, 3
	cbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 3
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	sbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 5
	rjmp msgInicial
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
ms200Delay:
    ldi  r21, 12
    ldi  r22, 255
    ldi  r23, 255
m1: dec  r23
    brne m1
    dec  r22
    brne m1
    dec  r21
    brne m1
    ret
mediana:
	sbi PORTD, 3
	cbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 3
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6
	rcall ms200Delay
	cbi portd, 6
	sbi portd, 5
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 5
	rjmp msgInicial

