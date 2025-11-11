#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define MOTOR_DIR_PIN1 PB2
#define MOTOR_DIR_PIN2 PB3
#define MOTOR_PWM_PIN PB1

void UART_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_sendString(const char *str) {
	while (*str) UART_sendChar(*str++);
}

void setupADC() {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t readADC(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void setupPWM() {
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
	ICR1 = 150; // Rango 0-255
	DDRB |= (1 << MOTOR_PWM_PIN) | (1 << MOTOR_DIR_PIN1) | (1 << MOTOR_DIR_PIN2);
}

void setPWM(uint8_t value) {
	OCR1A = value;
}

int main(void) {
	uint16_t valor_pot1, valor_pot2;
	uint8_t valor_pwm;
	uint8_t direccion;
	char buffer[64];

	UART_init(103);
	setupADC();
	setupPWM();

	while (1) {
		valor_pot1 = readADC(0);
		valor_pot2 = readADC(1);

		int16_t diferencia = (int16_t)valor_pot1 - (int16_t)valor_pot2;
		uint16_t abs_diff = abs(diferencia);

		// === ZONA MUERTA (para evitar oscilaciones pequeñas) ===
		if (abs_diff < 10) {
			direccion = 2;  // detenido
			valor_pwm = 0;
			PORTB &= ~((1 << MOTOR_DIR_PIN1) | (1 << MOTOR_DIR_PIN2));
		}
		else {
			// === Dirección ===
			if (diferencia > 0) {
				direccion = 1; // Horario
				PORTB &= ~(1 << MOTOR_DIR_PIN1);
				PORTB |= (1 << MOTOR_DIR_PIN2);
				} else {
				direccion = 0; // Antihorario
				PORTB |= (1 << MOTOR_DIR_PIN1);
				PORTB &= ~(1 << MOTOR_DIR_PIN2);
			}

			// === Control proporcional con compensación mínima ===
			valor_pwm = (abs_diff * 200) / 1023; // proporcional pero limitado
			if (valor_pwm < 50) valor_pwm = 50;  // mínimo torque para vencer inercia
			if (valor_pwm > 255) valor_pwm = 255; // saturación
		}

		setPWM(valor_pwm);

		snprintf(buffer, sizeof(buffer),
		"%u,%u,%u,%s\r\n",
		valor_pot1, valor_pot2, valor_pwm,
		direccion == 1 ? "Horario" : (direccion == 0 ? "Antihorario" : "Detenido"));
		UART_sendString(buffer);

		_delay_ms(50);
	}
}
