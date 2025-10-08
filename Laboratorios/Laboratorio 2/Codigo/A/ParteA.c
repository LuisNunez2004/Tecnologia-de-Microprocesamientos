#define F_CPU 16000000U
#include <avr/io.h>
#include <util/delay.h>

void cruz(void){
	PORTD = 0b00000100;
	_delay_ms(1000);
	PORTD = 0b01000000;
	_delay_ms(500);
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
	PORTD = 0b00001000;
	_delay_ms(1000);
	
	PORTD = 0b00000000;
}

void triangulo(void){
	PORTD = 0b00000100;
	_delay_ms(1000);
	PORTD = 0b01010000;
	_delay_ms(1000);
	PORTD = 0b10000000;
	_delay_ms(1000);
	PORTD = 0b00100000;
	_delay_ms(1000);
	PORTD = 0b00001000;
	_delay_ms(1000);
	
	PORTD = 0b00000000;
}

void circulo(uint8_t tam) {
	uint8_t pen_down = 0b00000100;
	uint8_t pen_up   = 0b00001000;
	uint8_t down     = 0b00010000;
	uint8_t up       = 0b00100000;
	uint8_t left     = 0b01000000;
	uint8_t right    = 0b10000000;

	uint16_t d = 200;  // delay base ajustado para círculo más grande

	PORTD = pen_down;
	_delay_ms(100);

	// 1?? Primer cuadrante (derecha -> arriba)
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = right; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  right | up; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/14; i++) { PORTD =  up ; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  right | up; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = up; _delay_ms(d); }

	// 2?? Segundo cuadrante (arriba -> izquierda)
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = up; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  up | left; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/14; i++) { PORTD = left; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  up | left; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = left; _delay_ms(d); }

	// 3?? Tercer cuadrante (izquierda -> abajo)
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = left; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  left | down; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/14; i++) { PORTD =  left; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  left | down; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = down; _delay_ms(d); }

	// 4?? Cuarto cuadrante (abajo -> derecha)
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = down; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  down | right; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/14; i++) { PORTD =  down; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/8; i++) { PORTD =  down | right; _delay_ms(d); }
	for (uint8_t i = 0; i < tam/10; i++) { PORTD = right; _delay_ms(d); }
		
		PORTD = right;
		_delay_ms(300);

	PORTD = pen_up;
	_delay_ms(200);
	PORTD = 0;
}



void flor(void){
	circulo(28);
	_delay_ms(500);

	PORTD = 0b00100000; _delay_ms(1500);
	circulo(14);

	PORTD = 0b00010000; _delay_ms(3000);
	PORTD = 0b10000000; _delay_ms(500);
	circulo(14);

	PORTD = 0b01000000; _delay_ms(3000);
	circulo(14);

	PORTD = 0b00100000; _delay_ms(3000);
	PORTD = 0b01000000; _delay_ms(1000);
	circulo(14);

	PORTD = 0b10000000; _delay_ms(2000);
	circulo(14);

	PORTD = 0b00001000;
	_delay_ms(500);
	PORTD = 0;
}

void rana(void){
	circulo(20);

	PORTD = 0b00010000; _delay_ms(3000);
	circulo(10);

	PORTD = 0b10000000; _delay_ms(1000);
	circulo(5);
	PORTD = 0b01000000; _delay_ms(2000);
	circulo(5);

	PORTD = 0b00001000; _delay_ms(300);
	PORTD = 0;
}


void iniciar(void){
	PORTD = 0b10000000;
	_delay_ms(10000);
	PORTD = 0b00010000;
	_delay_ms(5000);
}

void regresar(void){
	PORTD = 0b01000000;
	_delay_ms(10000);
	PORTD = 0b00100000;
	_delay_ms(5000);
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
			circulo(20);
			regresar();
			} else if (received == '4') {
			iniciar();
			rana();
			regresar();
			} else if (received == '5') {
			regresar();
			flor();
			regresar();
			} else {
			UART_sendString("Opción inválida\r\n");
		}
	}

	return 0;
}

