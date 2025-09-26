.org    0x0000             ; Dirección de inicio del programa en la memoria de programa
     rjmp inicializacion   ; Salta a la rutina de inicialización
.equ	F_CPU = 16000000    ; Frecuencia del reloj del microcontrolador (16 MHz)
.equ	baud = 9600         ; Velocidad de transmisión UART (9600 baudios)
.equ	bps = (F_CPU/16/baud) - 1 ; Valor para configurar el baud rate en UBRR

; ---------------- INICIALIZACIÓN DEL STACK Y PERIFÉRICOS ----------------
inicializacion:
	 ldi r16, high(ramend) ; Carga en r16 el byte alto del final de la RAM
	 out sph, r16          ; Configura el puntero de pila (parte alta)
	 ldi r16, low(ramend)  ; Carga en r16 el byte bajo del final de la RAM
	 out spl, r16          ; Configura el puntero de pila (parte baja)

	 ldi r16, low(bps)     ; Carga parte baja del divisor de baudios
	 ldi r17, high(bps)    ; Carga parte alta del divisor de baudios
	 rcall initUART        ; Llama a rutina de inicialización UART
	 ldi r16, 0x00         
	 out DDRC, r16         ; Puerto C como entrada
	 ldi r16, 0x7A         
	 out DDRD, r16         ; Configura DDRD como salida en ciertos pines
	 sbi PORTC, 0          ; Activa resistencia pull-up en PC0
	 rjmp msgInicial       ; Salta a enviar mensaje inicial

; ---------------- INICIALIZACIÓN DEL UART ----------------
initUART:
   sts UBRR0L, r16         ; Configura parte baja del baud rate
   sts UBRR0H, r17         ; Configura parte alta del baud rate
   ldi r16, (1<<RXEN0)|(1<<TXEN0) ; Habilita recepción y transmisión
   sts UCSR0B, r16         ; Guarda configuración en registro de control
   ret

; ---------------- MENSAJE INICIAL POR UART ----------------
msgInicial:
   ldi r16, 0x4C           ; "L"
   rcall putc
   ldi r16, 0x20           ; " "
   rcall putc
   ldi r16, 0x2D           ; "-"
   rcall putc
   ldi r16, 0x3E           ; ">"
   rcall putc
   ldi r16, 0x20           ; " "
   rcall putc
   ldi r16, 0x4C           ; "L"
   rcall putc
   ldi r16, 0x69           ; "i"
   rcall putc
   ldi r16, 0x67           ; "g"
   rcall putc
   ldi r16, 0x65           ; "e"
   rcall putc
   ldi r16, 0x72           ; "r"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   ; --- Mensaje "M -> Mediana" ---
   ldi r16, 0x4D           ; "M"
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x3E
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x4D           ; "M"
   rcall putc
   ldi r16, 0x65           ; "e"
   rcall putc
   ldi r16, 0x64           ; "d"
   rcall putc
   ldi r16, 0x69           ; "i"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   ldi r16, 0x6E           ; "n"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   ; --- Mensaje "P -> Pesada" ---
   ldi r16, 0x50           ; "P"
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x2D
   rcall putc
   ldi r16, 0x3E
   rcall putc
   ldi r16, 0x20
   rcall putc
   ldi r16, 0x50           ; "P"
   rcall putc
   ldi r16, 0x65           ; "e"
   rcall putc
   ldi r16, 0x73           ; "s"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   ldi r16, 0x64           ; "d"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   rjmp wait               ; Espera entrada del usuario

; ---------------- TRANSMISIÓN UART ----------------
putc:
   lds r17, UCSR0A         ; Lee estado UART
   sbrs r17, UDRE0         ; Verifica si buffer está listo
   rjmp putc               ; Espera hasta que se pueda escribir
   sts UDR0, r16           ; Envía el dato
   ldi r16, 0
   ret

; ---------------- RECEPCIÓN UART ----------------
getc:
   lds r17, UCSR0A         ; Lee estado UART
   sbrs r17, RXC0          ; Verifica si hay dato recibido
   rjmp getc               ; Espera hasta que llegue dato
   lds r16, UDR0           ; Carga el dato recibido en r16
   ret

; ---------------- PATRÓN "PESADA" ----------------
pesada:
	sbi PORTD, 3           ; Enciende PD3
	cbi portd, 5           ; Apaga PD5
	rcall s1Delay          ; Delay de 1 segundo
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	cbi portd, 3           ; Apaga PD3
	rcall s1Delay
	rcall s1Delay
	rcall s1Delay
	sbi portd, 6           ; Enciende PD6
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

; ---------------- ESPERA DE COMANDOS POR UART ----------------
wait:
   rcall getc              ; Espera caracter por UART
   cpi r16, 0x4C           ; "L"
   breq ligera
   cpi r16, 0x4D           ; "M"
   breq mediana
   cpi r16, 0x50           ; "P"
   breq pesada
   rjmp wait

; ---------------- PATRÓN "LIGERA" ----------------
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

; ---------------- DELAY DE 1 SEGUNDO ----------------
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

; ---------------- DELAY DE ~200 ms ----------------
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

; ---------------- PATRÓN "MEDIANA" ----------------
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
