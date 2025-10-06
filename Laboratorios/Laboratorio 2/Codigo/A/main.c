#define F_CPU 16000000U
#include <avr/io.h>
#include <util/delay.h>

void iniciar(void);
void regresar(void);
void cruz(void);
void triangulo(void);
void circulo(void);

void cruz(void){
	PORTD = 0b01000000;
	_delay_ms(5000);
	PORTD = 0b00000100;
	_delay_ms(500);
	PORTD = 0b00010000;
	_delay_ms(1000);
	PORTD = 0b00100000;
	_delay_ms(500);
	PORTD = 0b01000000;
	_delay_ms(500);
	PORTD = 0b10000000;
	_delay_ms(1000);
	PORTD = 0b00001000;
	_delay_ms(500);
	
	PORTD = 0;
}

void triangulo(void){	
	PORTD = 0b10000000;
	_delay_ms(7000);
	PORTD = 0b00010000;
	_delay_ms(5000);
	PORTD = 0b00000100;
	_delay_ms(500);
	PORTD = 0b01010000;
	_delay_ms(1000);
	PORTD = 0b10000000;
	_delay_ms(1000);
	PORTD = 0b00100000;
	_delay_ms(1000);
	PORTD = 0b00001000;
	_delay_ms(500);	
	
	PORTD = 0;
}

void circulo(uint8_t tamaño){
	PORTD = 0b00000100;   
	_delay_ms(100);

	// Primer cuadrante
	for (uint8_t i = 0; i < tamaño; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/2; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/3; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);

	// Segundo cuadrante
	for (uint8_t i = 0; i < tamaño; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/2; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/3; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00010000; _delay_ms(20);

	// Tercer cuadrante
	for (uint8_t i = 0; i < tamaño; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/2; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/3; i++) { PORTD = 0b01000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);

	// Cuarto cuadrante
	for (uint8_t i = 0; i < tamaño; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/2; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);
	for (uint8_t i = 0; i < tamaño/3; i++) { PORTD = 0b10000000; _delay_ms(20); }
	PORTD = 0b00100000; _delay_ms(20);

	PORTD = 0b00001000;
	_delay_ms(100);

	PORTD = 0;
}


void rana(void){
	
}

void flor(void){
	circulo(14);
	PORTD
	_delay_ms(2500);
	circulo(7);
}


void iniciar(void){
	PORTD = 0b10000000;
	_delay_ms(10000);
	PORTD = 0b00010000;
	_delay_ms(5000);
}

void regresar(void){
	PORTD = 0b0010000;
	_delay_ms(5000);
	PORTD = 0b0100000;
	_delay_ms(10000);
}

void UART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar(char data){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

char UART_receiveChar(void){
	while (!(UCSR0A &(1 << RXC0)));
	return UDR0;
}
void UART_sendString(const char *str){
	while (*str){
		UART_sendChar(*str++);
	}
}

int main(void)
{
	DDRD |= 0b11111100; 

	UART_init(103); 
	
	while (1) {
		UART_sendString("\r\nSeleccione una figura:\r\n");
		UART_sendString("(1) Triangulo\r\n");
		UART_sendString("(2) Cruz\r\n");
		UART_sendString("(3) Circulo\r\n");
		UART_sendString("(4) Rana\r\n");
		UART_sendString("(5) Flor\r\n");

		char received = UART_receiveChar(); 
		UART_sendChar(received);            
		UART_sendString("\r\n");            

		if (received == '1') {
			iniciar();
			triangulo();
			regresar();
			} else if (received == '2') {
			iniciar();
			cruz();
			regresar();
			} else if (received == '3') {
			iniciar();
			circulo(14);
			regresar();
			} else if (received == '4') {
			iniciar();
			rana();
			regresar();	
		    } else if (received == '5') {
			regresar();
			flor;	
			regresar();
			} else {
			  UART_sendString("Opcion invalida\r\n");
		   }
	}

	return 0;
}

