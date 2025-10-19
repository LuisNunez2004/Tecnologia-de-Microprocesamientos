#define F_CPU 16000000U
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

uint8_t bajar_lapiz = 0b00000100;
uint8_t subir_lapiz = 0b00001000;
uint8_t abajo = 0b00010000;
uint8_t arriba = 0b00100000;
uint8_t izquierda = 0b01000000;
uint8_t derecha = 0b10000000;

#define PUNTOS 250

int16_t x_tab[PUNTOS + 1];
int16_t y_tab[PUNTOS + 1];

void circulo_LUT(uint16_t RADIO) {
	for (uint16_t i = 0; i < PUNTOS; i++) {
		double ang = (2.0 * M_PI * i) / PUNTOS;
		x_tab[i] = (int16_t)(RADIO * cos(ang));
		y_tab[i] = (int16_t)(RADIO * sin(ang));
	}
	x_tab[PUNTOS] = x_tab[0];
	y_tab[PUNTOS] = y_tab[0];
}

void cruz(void){
	PORTD = bajar_lapiz;
	_delay_ms(100);
	PORTD = abajo;
	_delay_ms(2000);
	PORTD = arriba;
	_delay_ms(1000);
	PORTD = izquierda;
	_delay_ms(1000);
	PORTD = derecha;
	_delay_ms(2000);
	PORTD = subir_lapiz;
	_delay_ms(100);
	
	PORTD = 0;
}

void triangulo(void){
	PORTD = bajar_lapiz;
	_delay_ms(100);
	PORTD = 0b01010000;
	_delay_ms(2000);
	PORTD = derecha;
	_delay_ms(2000);
	PORTD = arriba;
	_delay_ms(2000);
	PORTD = subir_lapiz;
	_delay_ms(100);
	
	PORTD = 0;
}

void circulo(uint8_t RADIO) {
	PORTD = bajar_lapiz;
	_delay_ms(100);
	circulo_LUT(RADIO);

	for (uint16_t i = 0; i < PUNTOS; i++) {
		int8_t dx = x_tab[i + 1] - x_tab[i];
		int8_t dy = y_tab[i + 1] - y_tab[i];

		uint8_t dir = 0;
		if (dy > 0) dir |= arriba;
		else if (dy < 0) dir |= abajo;

		if (dx > 0) dir |= derecha;
		else if (dx < 0) dir |= izquierda;

		PORTD = dir ;
		_delay_ms(80);  
	}
	PORTD = derecha;
	_delay_ms(50);
	PORTD = arriba;
	_delay_ms(100);
	PORTD = subir_lapiz;
	_delay_ms(100);
	PORTD = 0;
}

void semicirculo_orientado(uint8_t RADIO, double ang_offset) {
	

	for (uint16_t i = 0; i < PUNTOS / 2; i++) { 
		double ang = ang_offset + (M_PI * i) / (PUNTOS / 2);
		int8_t dx = (int8_t)(RADIO * cos(ang + M_PI / PUNTOS)) - (int8_t)(RADIO * cos(ang));
		int8_t dy = (int8_t)(RADIO * sin(ang + M_PI / PUNTOS)) - (int8_t)(RADIO * sin(ang));

		uint8_t dir = 0;
		if (dy > 0) dir |= arriba;
		else if (dy < 0) dir |= abajo;
		if (dx > 0) dir |= derecha;
		else if (dx < 0) dir |= izquierda;

		PORTD = dir;
		_delay_ms(60);
	}

	_delay_ms(100);
	PORTD = 0;
}

void flor(void) {
	PORTD = bajar_lapiz;
	_delay_ms(100);
	circulo(20);
	PORTD = subir_lapiz;
	_delay_ms(100);
	PORTD = izquierda;
	_delay_ms(3550);
	PORTD = arriba;
	_delay_ms(1650);
	PORTD = bajar_lapiz;
	_delay_ms(100);
	semicirculo_orientado(60, M_PI_2);
	semicirculo_orientado(60, M_PI);
	semicirculo_orientado(60, 3*M_PI_2);
	semicirculo_orientado(60, 0);
	
	PORTD = izquierda | arriba;
	_delay_ms(5000);
	PORTD = abajo;
	_delay_ms(450);
	PORTD = derecha | abajo;
	_delay_ms(2500);
	semicirculo_orientado(30, M_PI_2);
	PORTD = arriba;
	_delay_ms(1900);
	PORTD = derecha | abajo;
	_delay_ms(2350);

	PORTD = subir_lapiz;
	_delay_ms(100);
	PORTD = 0;
}

void pie_rana(void){
	PORTD = izquierda | abajo;
	_delay_ms(1000);
	PORTD = derecha;
	_delay_ms(500);
	PORTD = derecha | arriba;
	_delay_ms(750);
	PORTD = abajo;
	_delay_ms(750);
	PORTD = derecha;
	_delay_ms(500);
	PORTD = arriba;
	_delay_ms(750);
	PORTD = derecha | abajo;
	_delay_ms(750);
	PORTD = derecha;
	_delay_ms(500);
	PORTD = izquierda | arriba;
	_delay_ms(1000);
}

void rana(void) {
	PORTD = bajar_lapiz;
	_delay_ms(100);
	
	// Cabeza
	semicirculo_orientado(55, M_PI_2);
	PORTD = derecha;
	_delay_ms(5000);
	semicirculo_orientado(55, 3*M_PI_2);
	
	// Ojo derecho
	semicirculo_orientado(40, 0);
	PORTD = izquierda;
	_delay_ms(400);
	
	// Ojo izquierdo
	semicirculo_orientado(40, 0);

	PORTD = subir_lapiz;
	_delay_ms(100);

	// Bajar al cuerpo
	PORTD = derecha;
	_delay_ms(5800);
	PORTD = abajo;
	_delay_ms(6000);

	// Cuerpo
	PORTD = bajar_lapiz;
	_delay_ms(100);
	semicirculo_orientado(150, 0);
	semicirculo_orientado(30, M_PI_2);
	pie_rana();
	PORTD = derecha;
	_delay_ms(4150);
	pie_rana();
	semicirculo_orientado(30, 3*M_PI_2);
	PORTD = subir_lapiz;
	_delay_ms(100);
	PORTD = arriba;
	_delay_ms(6600);
	PORTD = izquierda;
	_delay_ms(1000);
	circulo(8);
	PORTD = izquierda;
	_delay_ms(2850);
	PORTD = abajo;
	_delay_ms(250);
	circulo(8);
	PORTD = abajo;
	_delay_ms(2450);
	PORTD = bajar_lapiz;
	_delay_ms(100);
	semicirculo_orientado(20, M_PI);

	// Terminar
	PORTD = subir_lapiz;
	_delay_ms(100);
	PORTD = 0;
}



void iniciar(void){
	PORTD = derecha;
	_delay_ms(10000);
	PORTD = abajo;
	_delay_ms(5000);
}

void regresar(void){
	PORTD = izquierda;
	_delay_ms(10000);
	PORTD = arriba;
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
		UART_sendString("(6) Todo junto\r\n");

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
			PORTD = derecha;
			_delay_ms(6000);
			iniciar();
			PORTD = abajo;
			_delay_ms(3000);
			flor();
			regresar();
			} else if (received == '6'){
			PORTD = derecha;
			_delay_ms(17000);
			PORTD = abajo;
			_delay_ms(1000);
			triangulo();
			PORTD = izquierda;
			_delay_ms(7000);
			cruz();
			PORTD = izquierda;
			_delay_ms(3500);
			PORTD = abajo;
			_delay_ms(1000);
			circulo(40);
			PORTD =  abajo;
			_delay_ms(4000);
			rana();
			PORTD = derecha;
			_delay_ms(8000);
			PORTD = abajo;
			_delay_ms(3000);
			flor();
			} else {
			UART_sendString("Opción inválida\r\n");
		}
	}

	return 0;
}



