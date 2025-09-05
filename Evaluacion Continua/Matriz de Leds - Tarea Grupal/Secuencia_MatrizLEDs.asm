.org 0x0000
rjmp inicio
.org 0x0002
rjmp RSI_0

inicio:
	   sei
       ldi r16, low(RAMEND)
       out SPL, r16
       ldi r16, high(RAMEND)
       out SPH, r16

       ldi r16, 0b00111111
       out DDRB, r16
	   ldi r17, 0b00001111
	   out DDRC, r17
	   ldi r18, 0b11110011
       out DDRD, r18

	   cbi DDRD, 2       
       sbi PORTD, 2      

       ldi r16, (1<<ISC01)    
       sts EICRA, r16
       ldi r16, (1<<INT0)
       out EIMSK, r16

	   ldi r21, 0x00
	   rjmp loop

RSI_0:
	cbi EIMSK, INT0
	inc r21
	rcall delay
	sbi EIMSK, INT0
	reti

delay:
	ldi r18, 55
	L7: ldi r19, 255
	L2: ldi r20, 200
	L3: dec r20
		brne L3
		dec r19
		brne L2
		dec r18
		brne L7
		ret

loop:
    cpi r21, 0
    brne check1
	rcall cara_feliz
    rjmp loop

check1:
    cpi r21, 1
    brne check2
	rcall cara_triste
    rjmp loop

check2:
    cpi r21, 2
    brne check3
	rcall corazon
    rjmp loop

check3:
    cpi r21, 3
    brne check4
	rcall rombo
    rjmp loop

check4:
    cpi r21, 4
    brne check5
    rcall space_invaders
	rjmp loop

check5:
    cpi r21, 5
    brne loop
    ldi r21, 0x00
	rjmp loop

cara_feliz:
			ldi r16, 0b01111110
			ldi r17, 0b00000011
            ldi r18, 0b00000000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
            rcall retardo2ms

			ldi r16, 0b01111101
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01111011
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01110111
			ldi r17, 0b00000011
            ldi r18, 0b00000000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01101111
			ldi r17, 0b00000011
            ldi r18, 0b00000000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01011111
			ldi r17, 0b00000011
            ldi r18, 0b01000010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00001110
            ldi r18, 0b00110000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00000001
            ldi r18, 0b00000000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ret

cara_triste:
			ldi r16, 0b01111110
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
            rcall retardo2ms

			ldi r16, 0b01111101
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01111011
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01110111
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01101111
			ldi r17, 0b00000011
            ldi r18, 0b00000000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01011111
			ldi r17, 0b00001111
            ldi r18, 0b00110000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00000010
            ldi r18, 0b01000010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00000001
            ldi r18, 0b10000001
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ret
corazon:
			ldi r16, 0b01111110
			ldi r17, 0b00000111
            ldi r18, 0b01100010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
            rcall retardo2ms

			ldi r16, 0b01111101
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01111011
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01110111
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01101111
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01011111
			ldi r17, 0b00001111
            ldi r18, 0b01110010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00001110
            ldi r18, 0b00110000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00001001
            ldi r18, 0b00010000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ret
rombo:
		ldi r16, 0b01111110
			ldi r17, 0b00001011
            ldi r18, 0b00010000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
            rcall retardo2ms

			ldi r16, 0b01111101
			ldi r17, 0b00001111
            ldi r18, 0b00110000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01111011
			ldi r17, 0b00001111
            ldi r18, 0b01110010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01110111
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01101111
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01011111
			ldi r17, 0b00001111
            ldi r18, 0b01110010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00001110
            ldi r18, 0b00110000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00001001
            ldi r18, 0b00010000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ret

space_invaders:
			ldi r16, 0b01111110
			ldi r17, 0b00000111
            ldi r18, 0b00100000
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
            rcall retardo2ms

			ldi r16, 0b01111101
			ldi r17, 0b00001111
            ldi r18, 0b01110010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01111011
			ldi r17, 0b00001011
            ldi r18, 0b11010011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01110111
			ldi r17, 0b00001011
            ldi r18, 0b11010011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01101111
			ldi r17, 0b00001111
            ldi r18, 0b11110011
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b01011111
			ldi r17, 0b00001011
            ldi r18, 0b01010010
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00000110
            ldi r18, 0b10100001
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ldi r16, 0b00111111
			ldi r17, 0b00000001
            ldi r18, 0b10000001
			out PORTB, r16
			out PORTC, r17
			out PORTD, r18
			rcall retardo2ms

			ret

retardo2ms:
           ldi r19, 54
		   ldi r20, 200
	   L1: dec r20
		   brne L1
		   dec r19
		   brne L1
		   ret	

		   