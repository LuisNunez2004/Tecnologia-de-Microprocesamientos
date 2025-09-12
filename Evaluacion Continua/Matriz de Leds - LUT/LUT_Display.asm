.org 0x0000
rjmp inicio
.org 0x0002
rjmp RSI_0
.org 0x0004
rjmp RSI_1

inicio:
       sei
       ldi r16, low(RAMEND)
       out spl, r16
       ldi r16, high(RAMEND)
       out sph, r16

	   ldi r20, 0b00111111
	   out DDRB, r20
	   
	   sbi DDRD, 4

       cbi DDRD, 2
       sbi PORTD, 2
       cbi DDRD, 3
       sbi PORTD, 3

       ldi r16, (1<<ISC01)|(1<<ISC11)
       sts EICRA, r16
       ldi r16, (1<<INT0)|(1<<INT1)
       out EIMSK, r16

       rcall guardar_codigos

       ldi r21, 0x00
       ldi r22, 0x00
       rjmp loop

RSI_0:
      ldi r22, 1
      reti

RSI_1:
      clr r22
      reti

delay:
	  ldi r17, 2
	  L4: ldi r18, 53
      L1: ldi r19, 252
      L2: ldi r20, 200
      L3: dec r20
          brne L3
          dec r19
          brne L2
          dec r18
          brne L1
		  dec r17
		  brne L4
          ret

loop:
     mov r16, r21
     rcall get_u
     rcall set_7seg_u

     cpi r22, 1
     brne loop

     rcall delay
     inc r21
     cpi r21, 10
     brlo loop
     ldi r21, 0
     rjmp loop

get_u:
	mov		r20, r16
	andi	r20, 0x0f
	mov		r1, r20
	ret

set_7seg_u:
	mov r0, r1
    rcall get_7seg_code  

    mov r17, r20        

    mov r18, r17
    andi r18, 0b00111111
    out PORTB, r18

    mov r19, r17
    andi r19, 0b01000000 
    lsr r19                
    lsr r19               

    in  r20, PORTD
    andi r20, 0b11101111  
    or  r20, r19
    out PORTD, r20

    ret

get_7seg_code:
	ldi r28,0x00 ;LOW(0x0100)
	ldi r29,0x01 ;HIGH(0x0100)
	add r28,r0
	ld r20, Y
	ret

guardar_codigos:
	ldi r28, 0x00 ;LOW(0x0100)
	ldi r29, 0x01 ;HIGH(0x0100)
	ldi r20, 0b00111111 ;cargamos el 0
	ST Y+, r20
	ldi r20, 0b00000110 ;cargamos el 1
	ST Y+, r20
	ldi r20, 0b01011011 ;cargamos el 2
	ST Y+, r20
	ldi r20, 0b01001111 ;cargamos el 3
	ST Y+, r20
	ldi r20, 0b01100110 ;cargamos el 4
	ST Y+, r20
	ldi r20, 0b01101101 ;cargamos el 5
	ST Y+, r20
	ldi r20, 0b01111101 ;cargamos el 6
	ST Y+, r20
	ldi r20, 0b00000111 ;cargamos el 7
	ST Y+, r20
	ldi r20, 0b01111111 ;cargamos el 8
	ST Y+, r20
	ldi r20, 0b01101111 ;cargamos el 9
	ST Y+, r20
	ret