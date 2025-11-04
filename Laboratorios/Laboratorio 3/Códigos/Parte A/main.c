#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define clk_x PB3
#define dir_x PB4
#define en_x PB5
#define clk_y PC3
#define dir_y PC4
#define en_y PC5
#define SOL PC0



void pwm_motor_x() {
	DDRB |= (1 << clk_x) | (1 << dir_x) | (1 << en_x);

	TCCR2A |= (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << WGM22);
	TCCR2B |= (1 << CS21);

	OCR2A = 30;
}

void motor_x_velocidad(uint8_t velocidad) {
	OCR2A = velocidad;
}

void pwm_motor_y() {
	DDRC |= (1 << clk_y) | (1 << dir_y) | (1 << en_y) | (1 << SOL);

	TCCR1A = 0;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);

	OCR1A = 30;

	TIMSK1 |= (1 << OCIE1A);
	
}



void motor_y_velocidad(uint8_t velocidad) {
	OCR1A = velocidad;
	sei();
}

ISR(TIMER1_COMPA_vect) {
	PORTC ^= (1 << clk_y);
}

void mover_abajo() {
	PORTB |= (1 << dir_x) | (1 << en_x);
}

void mover_arriba() {
	PORTB |= (1 << en_x);
	PORTB &= ~(1 << dir_x);
}

void detener() {
	PORTB &= ~((1 << en_x) | (1 << dir_x));
	PORTC &= ~((1 << en_y) | (1 << dir_y));
}

void mover_izquierda() {
	PORTC |= (1 << en_y);
	PORTC &= ~(1 << dir_y);
}

void mover_derecha() {
	PORTC |= (1 << dir_y) | (1 << en_y);
}

void subir_lapiz() {
	PORTC |= (1 << SOL);
}

void bajar_lapiz() {
	PORTC &= ~(1 << SOL);
}

void cruz() {
	bajar_lapiz();
	mover_derecha();
	_delay_ms(1000);
	mover_izquierda();
	_delay_ms(500);
	detener();
	mover_arriba();
	_delay_ms(600);
	mover_abajo();
	_delay_ms(1200);
	detener();
	subir_lapiz();
}

void triangulo() {
	bajar_lapiz();
	mover_derecha();
	_delay_ms(1000);
	mover_izquierda();
	mover_arriba();
	_delay_ms(500);
	mover_abajo();
	_delay_ms(500);
	detener();
	subir_lapiz();
	
}

void circulo() {
	bajar_lapiz();
	mover_izquierda();
	_delay_ms(250);
	detener();
	for (int i = 0;i<5;i++) {
		mover_izquierda();
		_delay_ms(50);
		detener();
		mover_abajo();
		_delay_ms(30);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_izquierda();
		_delay_ms(25);
		detener();
		mover_abajo();
		_delay_ms(30);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_izquierda();
		_delay_ms(25);
		detener();
		mover_abajo();
		_delay_ms(60);
		detener();
	}
	mover_abajo();
	_delay_ms(200);
	detener();
	for (int i = 0;i<5;i++) {
		mover_abajo();
		_delay_ms(60);
		detener();
		mover_derecha();
		_delay_ms(25);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_abajo();
		_delay_ms(30);
		detener();
		mover_derecha();
		_delay_ms(25);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_abajo();
		_delay_ms(30);
		detener();
		mover_derecha();
		_delay_ms(50);
		detener();
	}
	mover_derecha();
	_delay_ms(200);
	detener();
	for (int i = 0;i<5;i++) {
		mover_arriba();
		_delay_ms(30);
		detener();
		mover_derecha();
		_delay_ms(50);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_arriba();
		_delay_ms(30);
		detener();
		mover_derecha();
		_delay_ms(25);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_arriba();
		_delay_ms(60);
		detener();
		mover_derecha();
		_delay_ms(25);
		detener();
	}
	mover_arriba();
	_delay_ms(200);
	detener();
	for (int i = 0;i<5;i++) {
		mover_izquierda();
		_delay_ms(25);
		detener();
		mover_arriba();
		_delay_ms(60);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_arriba();
		_delay_ms(30);
		detener();
		mover_izquierda();
		_delay_ms(25);
		detener();
	}
	for (int i = 0;i<5;i++) {
		mover_izquierda();
		_delay_ms(50);
		detener();
		mover_arriba();
		_delay_ms(30);
		detener();
	}
	subir_lapiz();
}

void rana() {
	detener();
	subir_lapiz();
}
	
void flor() {
	detener();
	subir_lapiz();
}

int main(void) {
	subir_lapiz();
	pwm_motor_x();
	pwm_motor_y();
	motor_x_velocidad(90);
	motor_y_velocidad(90);
	//cruz();
	//triangulo();
	circulo();
	

	while (1) {
		motor_x_velocidad(90);
		motor_y_velocidad(90);
		
	}
	
	return 0;
}