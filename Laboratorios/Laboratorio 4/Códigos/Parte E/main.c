#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

#define IN1 PD2
#define IN2 PD3
#define IN3 PD4
#define IN4 PD5

#define SERVO_PIN PB1

#define ANGULO_REPOSO 2000
#define ANGULO_GOLPE_DER  700
#define ANGULO_GOLPE_IZQ 3000

void init_servo(void) {
	DDRB |= (1 << SERVO_PIN);
	
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;
	
	OCR1A = ANGULO_REPOSO;
}

void patearDER(void) {
	uart_print("Golpe!\r\n");
	
	OCR1A = ANGULO_GOLPE_DER;
	_delay_ms(350);
	OCR1A = ANGULO_REPOSO;
	_delay_ms(150);
}

void patearIZQ(void){
	uart_print("Golpe!\r\n");
	
	OCR1A = ANGULO_GOLPE_IZQ;
	_delay_ms(350);
	OCR1A = ANGULO_REPOSO;
	_delay_ms(150);
}

void adelante(void){
	PORTD |= (1 << IN1);
	PORTD &= ~(1 << IN2);
	PORTD |= (1 << IN3);
	PORTD &= ~(1 << IN4);
}

void atras(void){
	PORTD |= (1 << IN2);
	PORTD &= ~(1 << IN1);
	PORTD |= (1 << IN4);
	PORTD &= ~(1 << IN3);
}

void parar(void){
	PORTD &= ~(1 << IN1);
	PORTD &= ~(1 << IN2);
	PORTD &= ~(1 << IN3);
	PORTD &= ~(1 << IN4);
}

void derecha(void){
	PORTD |= (1 << IN3);
	PORTD &= ~(1 << IN2);
	PORTD &= ~(1 << IN1);
	PORTD &= ~(1 << IN4);
}

void izquierda(void){
	PORTD |= (1 << IN1);
	PORTD &= ~(1 << IN3);
	PORTD &= ~(1 << IN2);
	PORTD &= ~(1 << IN4);
}

int main(void) {
	DDRD |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4);
	
	uart_init(103); 
	uart_print("Sistema Listo. Esperando Bluetooth...\r\n");
	
	init_servo();

	while (1) {
		if (UCSR0A & (1 << RXC0)) {

			char c = uart_receive();
			
			switch (c) {
				case 'W': case 'w': adelante();  break;
				case 'S': case 's': atras();     break;
				case 'D': case 'd': derecha();   break;
				case 'A': case 'a': izquierda(); break;
				case 'P': case 'p': parar();     break;

				case 'X': case 'x': patearDER(); break;
				case 'Y': case 'y': patearIZQ(); break;
			}
		}
	}
}

