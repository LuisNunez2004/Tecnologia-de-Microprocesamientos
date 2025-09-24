.include "m328pdef.inc"

.org    0x0000
     rjmp inicializacion
inicializacion:
	 ldi r16, 0x00
	 out DDRC, r16
   ldi r16, 0xFF
	 out DDRD, r16
	 sbi PORTC, 0
	 rjmp inicio
inicio:
	 sbi portd, 3
	 cbi portd, 5
	 rcall ms200delay
	 cbi portd, 3
	 rcall ms200Delay
	 sbi portd, 6
	 rcall ms200Delay
	 rjmp inicio
ligera:
	sbi PORTD, 3
	sbic PORTC, 0
	rjmp inicio
	rjmp ligera
ms200Delay:
    ldi  r21, 17
    ldi  r22, 60
    ldi  r23, 204
L1: dec  r23
    brne L1
    dec  r22
    brne L1
    dec  r21
    brne L1
    ret
