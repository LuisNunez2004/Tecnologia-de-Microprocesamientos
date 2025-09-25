.cseg
.org 0x0000
rjmp inicio

inicio:
    sei
    ldi r16, low(RAMEND)
    out SPL, r16
    ldi r16, high(RAMEND)
    out SPH, r16

    ; Columnas
    ldi r16, 0b11111100   ; PD2-PD7 salida
    out DDRD, r16
    ldi r16, 0b00001100   ; PC2-PC3 salida
    out DDRC, r16

    ; Filas
    ldi r16, 0b00111111   ; PB0-PB5 salida
    out DDRB, r16
    in r17, DDRC
    ori r17, 0b00000011   ; PC0-PC1 salida
    out DDRC, r17

    rjmp loop_main

retardo:
    ldi r18, 50
    ldi r19, 255
ret1:
    dec r19
    brne ret1
    dec r18
    brne ret1
    ret

MostrarJ:
    ldi ZH, high(patrones_J)
    ldi ZL, low(patrones_J)
    ldi r20, 0

loop_filas:
    ld r16, Z+        ; PD2-PD7
    ld r17, Z+        ; PC2-PC3

    ; Columnas
    out PORTD, r16
    in r21, PORTC
    andi r21, 0b11110011
    or r21, r17
    out PORTC, r21

    ; Limpiar todas las filas (poner en 1)
    ldi r22, 0b00111111
    out PORTB, r22
    in r23, PORTC
    ori r23, 0b00000011
    out PORTC, r23

    ; Activar fila (poner en 0)
    cpi r20, 0
    breq fila0
    cpi r20, 1
    breq fila1
    cpi r20, 2
    breq fila2
    cpi r20, 3
    breq fila3
    cpi r20, 4
    breq fila4
    cpi r20, 5
    breq fila5
    cpi r20, 6
    breq fila6
    cpi r20, 7
    breq fila7

fila0:
    cbi PORTB, 0
    rjmp fila_listo
fila1:
    cbi PORTB, 1
    rjmp fila_listo
fila2:
    cbi PORTB, 2
    rjmp fila_listo
fila3:
    cbi PORTB, 3
    rjmp fila_listo
fila4:
    cbi PORTB, 4
    rjmp fila_listo
fila5:
    cbi PORTB, 5
    rjmp fila_listo
fila6:
    cbi PORTC, 0
    rjmp fila_listo
fila7:
    cbi PORTC, 1

fila_listo:
    rcall retardo
    inc r20
    cpi r20, 8
    brne loop_filas
    ret

patrones_J:
    .db 0b00011100, 0b00001100
    .db 0b00011100, 0b00001100
    .db 0b00011100, 0b00001100
    .db 0b00011100, 0b00001100
    .db 0b00011100, 0b00001100
    .db 0b01111100, 0b00001100
    .db 0b01110000, 0b00001100
    .db 0b00100000, 0b00001100

loop_main:
    rcall MostrarJ
    rjmp loop_main
