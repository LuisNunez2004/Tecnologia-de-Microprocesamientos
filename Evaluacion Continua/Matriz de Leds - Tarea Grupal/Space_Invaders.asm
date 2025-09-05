.org 0x00

jmp inicio

inicio:
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

			rcall space_invaders	

retardo2ms:
           ldi r19, 54
		   ldi r20, 200
	   L1: dec r20
		   brne L1
		   dec r19
		   brne L1
		   ret
		  		
