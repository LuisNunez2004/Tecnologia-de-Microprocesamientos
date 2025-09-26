.equ f_cpu = 16000000
.equ baud = 9600
.equ bps = (f_cpu/16/baud) - 1

.dseg
pos: .byte 1
modo: .byte 1

.cseg
.org 0x00
    rjmp inicio

inicio:
    ldi r16, low(ramend)
    out spl, r16
    ldi r16, high(ramend)
    out sph, r16

    ldi r16, 0xfc
    out ddrd, r16
    ldi r16, 0x00
    out portd, r16
    ldi r16, 0x3f
    out ddrb, r16
    ldi r16, 0x3c
    out portb, r16
    ldi r16, 0x0f
    out ddrc, r16
    out portc, r16

    clr r17
    sts pos, r17

    ldi r16, low(bps)
    sts ubrr0l, r16
    ldi r16, high(bps)
    sts ubrr0h, r16

    ldi r16, (1<<rxen0)|(1<<txen0)
    sts ucsr0b, r16
    ldi r16, (1<<ucsz01)|(1<<ucsz00)
    sts ucsr0c, r16

    ldi zh, high(secuencia*2)
    ldi zl, low(secuencia*2)

enviar_bucle:
    lpm r16, z+
    cpi r16, 0
    breq enviar_secuencia
    rcall transmitir_uart
    rjmp enviar_bucle

menu:
main_loop:
    lds r18, ucsr0a
    sbrs r18, rxc0
    rjmp no_uart
    lds r16, udr0
    sts modo, r16

no_uart:
    lds r16, modo
    cpi r16, 'm'
    breq enviar_secuencia
    cpi r16, '1'
    breq figura_1
    cpi r16, '2'
    breq figura_2
    cpi r16, '3'
    breq figura_3
    cpi r16, '4'
    breq figura_4
    cpi r16, '5'
    breq figura_5
    rjmp main_loop

enviar_secuencia:
    clr r17
    sts pos, r17

secuencia_loop:
    ldi r24, 200

secuencia_wait_loop:
    rcall actualizar
    dec r24
    brne secuencia_wait_loop
    lds r17, pos
    inc r17
    cpi r17, tamano_secuencia
    brlo no_fin
    rcall limpiar_display
    sts pos, r17
    clr r16
    sts modo, r16
    rjmp menu

no_fin:
    sts pos, r17
    rjmp secuencia_loop

figura_1:
    ldi zh, high(cara_feliz*2)
    ldi zl, low(cara_feliz*2)
    rcall mostrar_display
    rjmp main_loop

figura_2:
    ldi zh, high(cara_triste*2)
    ldi zl, low(cara_triste*2)
    rcall mostrar_display
    rjmp main_loop

figura_3:
    ldi zh, high(corazon*2)
    ldi zl, low(corazon*2)
    rcall mostrar_display
    rjmp main_loop

figura_4:
    ldi zh, high(rombo*2)
    ldi zl, low(rombo*2)
    rcall mostrar_display
    rjmp main_loop

figura_5:
    ldi zh, high(alien*2)
    ldi zl, low(alien*2)
    rcall mostrar_display
    rjmp main_loop

transmitir_uart:
    lds r18, ucsr0a
    sbrs r18, udre0
    rjmp transmitir_uart
    sts udr0, r16
    ret

actualizar:
    push r16
    push r17
    push r18
    push r19
    push r20
    lds r17, pos
    ldi zh, high(secuencia*2)
    ldi zl, low(secuencia*2)
    add zl, r17
    adc zh, r1
    ldi r20, 0
    rjmp refrescar_columnas

mostrar_display:
    push r16
    push r17
    push r18
    push r19
    push r20
    ldi r20, 0

refrescar_columnas:
    lpm r19, z+
    in r18, portd
    andi r18, 0b00000011
    out portd, r18
    in r18, portb
    andi r18, 0b11111100
    out portb, r18
    mov r16, r19
    andi r16, 0x0f
    com r16
    andi r16, 0x0f
    lsl r16
    lsl r16
    in r18, portb
    andi r18, 0b11000011
    or r18, r16
    out portb, r18
    mov r16, r19
    swap r16
    andi r16, 0x0f
    com r16
    andi r16, 0x0f
    in r18, portc
    andi r18, 0b11110000
    or r18, r16
    out portc, r18
    rcall elegir_columna
    ldi r16, 200

elegir_columna:
    cpi r20, 0
    breq sc0
    cpi r20, 1
    breq sc1
    cpi r20, 2
    breq sc2
    cpi r20, 3
    breq sc3
    cpi r20, 4
    breq sc4
    cpi r20, 5
    breq sc5
    cpi r20, 6
    breq sc6
    sbi portb, pb1
    ret

sc0: sbi portd, pd2
    ret
sc1: sbi portd, pd3
    ret
sc2: sbi portd, pd4
    ret
sc3: sbi portd, pd5
    ret
sc4: sbi portd, pd6
    ret
sc5: sbi portd, pd7
    ret
sc6: sbi portb, pb0
    ret

delay:
    dec r16
    brne delay
    inc r20
    cpi r20, 8
    brlo refrescar_columnas
    pop r20
    pop r19
    pop r18
    pop r17
    pop r16
    ret

limpiar_display:
    in r16, portd
    andi r16, 0b00000011
    out portd, r16
    in r16, portb
    andi r16, 0b11111100
    out portb, r16
    in r16, portb
    ori r16, 0b00111100
    out portb, r16
    in r16, portc
    ori r16, 0b00001111
    out portc, r16
    ret

secuencia:
.db 0b11111111,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011  ; J
.db 0x00
.db 0b11111111,0b11111111,0b11011011,0b11011011,0b11111111,0b11011011,0b11011011,0b11111111  ; E
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; S
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; S
.db 0x00
.db 0b11111111,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011  ; U
.db 0x00
.db 0b11111111,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011  ; S
.db 0x00
.db 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000  ; espacio
.db 0x00
.db 0b11111111,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011,0b11011011  ; M
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11011011,0b11111111,0b11011011,0b11111111,0b11111111  ; O
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; L
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; E
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; S
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; T
.db 0x00
.db 0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11000011,0b11111111,0b11111111  ; O
.db 0x00

.equ tamano_mensaje = 171

cara_feliz:
.db 0b00000000,0b00110000,0b01000110,0b01000000,0b01000000,0b01000110,0b00110000,0b00000000

cara_triste:
.db 0b00000000,0b01100000,0b00010110,0b00010000,0b00010000,0b00010110,0b01100000,0b00000000

corazon:
.db 0b00000000,0b00001100,0b00010010,0b00100100,0b00010010,0b00001100,0b00000000,0b00000000

rombo:
.db 0b00001000,0b00010100,0b00100010,0b01000001,0b00100010,0b00010100,0b00001000,0b00000000

alien:
.db 0b00110000,0b00011000,0b01110101,0b00111110,0b00111110,0b01110101,0b00011000,0b00110000
