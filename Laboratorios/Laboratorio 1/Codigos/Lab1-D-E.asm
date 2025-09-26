.org    0x0000             ; Dirección de inicio del programa
     rjmp inicializacion   ; Salta a la rutina de inicialización
.equ	F_CPU = 16000000    ; Frecuencia del microcontrolador (16 MHz)
.equ	baud = 9600         ; Velocidad de transmisión UART
.equ	bps = (F_CPU/16/baud) - 1 ; Cálculo del divisor para baud rate

; ---------------- INICIALIZACIÓN DEL STACK Y UART ----------------
inicializacion:
	 ldi r16, high(ramend) ; Carga el byte alto de la RAM
	 out sph, r16          ; Configura el stack pointer (parte alta)
	 ldi r16, low(ramend)  ; Carga el byte bajo de la RAM
	 out spl, r16          ; Configura el stack pointer (parte baja)

	 ldi r16, low(bps)     ; Parte baja del divisor de baudios
	 ldi r17, high(bps)    ; Parte alta del divisor de baudios
	 rcall initUART        ; Inicializa UART
	 ldi r16, 0xFE         
	 out DDRD, r16         ; Configura puerto D como salida (excepto PD0)
	 rjmp msgInicial       ; Salta a enviar mensaje inicial

; ---------------- CONFIGURACIÓN UART ----------------
initUART:
   sts UBRR0L, r16         ; Configura parte baja del baud rate
   sts UBRR0H, r17         ; Configura parte alta del baud rate
   ldi r16, (1<<RXEN0)|(1<<TXEN0) ; Habilita RX y TX
   sts UCSR0B, r16
   ret

; ---------------- MENSAJE INICIAL ----------------
msgInicial:
   ; "1-Triangulo"
   ldi r16, 0x31           ; "1"
   rcall putc
   ldi r16, 0x2D           ; "-"
   rcall putc
   ldi r16, 0x54           ; "T"
   rcall putc
   ldi r16, 0x72           ; "r"
   rcall putc
   ldi r16, 0x69           ; "i"
   rcall putc
   ldi r16, 0x61           ; "a"
   rcall putc
   ldi r16, 0x6E           ; "n"
   rcall putc
   ldi r16, 0x67           ; "g"
   rcall putc
   ldi r16, 0x75           ; "u"
   rcall putc
   ldi r16, 0x6C           ; "l"
   rcall putc
   ldi r16, 0x6F           ; "o"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   ; "2-Circulo"
   ldi r16, 0x32           ; "2"
   rcall putc
   ldi r16, 0x2D           ; "-"
   rcall putc
   ldi r16, 0x43           ; "C"
   rcall putc
   ldi r16, 0x69           ; "i"
   rcall putc
   ldi r16, 0x72           ; "r"
   rcall putc
   ldi r16, 0x63           ; "c"
   rcall putc
   ldi r16, 0x75           ; "u"
   rcall putc
   ldi r16, 0x6C           ; "l"
   rcall putc
   ldi r16, 0x6F           ; "o"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   ; "3-Cruz"
   ldi r16, 0x33           ; "3"
   rcall putc
   ldi r16, 0x2D           ; "-"
   rcall putc
   ldi r16, 0x43           ; "C"
   rcall putc
   ldi r16, 0x72           ; "r"
   rcall putc
   ldi r16, 0x75           ; "u"
   rcall putc
   ldi r16, 0x7A           ; "z"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   ; "T-Todo"
   ldi r16, 0x54           ; "T"
   rcall putc
   ldi r16, 0x2D           ; "-"
   rcall putc
   ldi r16, 0x54           ; "T"
   rcall putc
   ldi r16, 0x6F           ; "o"
   rcall putc
   ldi r16, 0x64           ; "d"
   rcall putc
   ldi r16, 0x6F           ; "o"
   rcall putc
   ldi r16, 0x0A           ; Salto de línea
   rcall putc

   rjmp wait               ; Salta a la espera de entrada

; ---------------- ENVÍO DE CARÁCTER UART ----------------
putc:
   lds r17, UCSR0A         ; Lee estado UART
   sbrs r17, UDRE0         ; Verifica si buffer está listo
   rjmp putc               ; Si no, espera
   sts UDR0, r16           ; Envía el dato
   ldi r16, 0
   ret

; ---------------- RECEPCIÓN DE CARÁCTER UART ----------------
getc:
   lds r17, UCSR0A         ; Lee estado UART
   sbrs r17, RXC0          ; Verifica si hay dato recibido
   rjmp getc               ; Espera hasta recibirlo
   lds r16, UDR0           ; Guarda dato recibido en r16
   ret

; ---------------- OPCIÓN "TODO" ----------------
toda:
	ldi r22, 1             ; Marca bandera
	cpi r22, 1
	breq triangulo         ; Salta a dibujar triángulo primero
	rjmp msgInicial

; ---------------- ESPERA DE ENTRADA POR UART ----------------
wait:
   ldi r22, 0              ; Reinicia bandera
   rcall getc              ; Espera entrada
   cpi r16, 0x31           ; "1"
   breq triangulo
   cpi r16, 0x33           ; "3"
   breq cruz
   cpi r16, 0x54           ; "T"
   breq toda
   rjmp wait

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

; ---------------- FIGURA: TRIÁNGULO ----------------
triangulo:
	rcall centrar
	sbi portd, 2           ; Enciende PD2
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
	cpi r22, 1             ; Si se llamó desde "toda"
	breq cruzdib           ; Continuar con cruz
	rcall descentrar
	rjmp msgInicial

; ---------------- FIGURA: CRUZ ----------------
cruz:
	rcall centrar
cruzdib:
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
	rcall descentrar
	rjmp msgInicial

; ---------------- EFECTO DE CENTRADO ----------------
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

; ---------------- EFECTO DE DESCENTRADO ----------------
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
