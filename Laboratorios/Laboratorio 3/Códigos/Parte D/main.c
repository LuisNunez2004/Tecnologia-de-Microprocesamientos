#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LED PD6
#define boton PD7
#define mov_x 0
#define mov_y 1
#define matriz 8
#define zona_muerta 100

uint8_t pos_x = 4;
uint8_t pos_y = 4;
uint8_t colorR = 255, colorG = 0, colorB = 0;

void ws2812_send_bit(uint8_t bitVal) {
	if (bitVal) {
		PORTD |= (1 << LED);
		asm volatile (
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t");
		PORTD &= ~(1 << LED);
		asm volatile (
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t");
		} else {
		PORTD |= (1 << LED);
		asm volatile("nop\n\t""nop\n\t""nop\n\t");
		PORTD &= ~(1 << LED);
		asm volatile(
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
	}
}

void ws2812_send_byte(uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++) {
		ws2812_send_bit(byte & (1 << (7 - i)));
	}
}

void ws2812_send_color(uint8_t r, uint8_t g, uint8_t b) {
	ws2812_send_byte(g);
	ws2812_send_byte(r);
	ws2812_send_byte(b);
}

void ws2812_reset(void) {
	PORTD &= ~(1 << LED);
	_delay_us(80);
}

void iniciar_ADC(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t leer_ADC(uint8_t canal) {
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void cambiar_color(void) {
	colorR = rand() % 256;
	colorG = rand() % 256;
	colorB = rand() % 256;
}

uint16_t xy_to_index(uint8_t x, uint8_t y) {
	return y * matriz + x; 
}

void mostrar_led(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	uint16_t total = matriz * matriz;
	uint16_t target = xy_to_index(x, y);
	for (uint16_t i = 0; i < total; i++) {
		if (i == target)
		ws2812_send_color(r, g, b);
		else
		ws2812_send_color(0, 0, 0);
	}
	ws2812_reset();
}

void leer_joystick(void) {
	uint16_t x = leer_ADC(mov_x);
	uint16_t y = leer_ADC(mov_y);
	static uint8_t puedeMover = 1;

	if (puedeMover) {
		if (x > (512 + zona_muerta)) {
			if (pos_x < matriz - 1) pos_x++;
			puedeMover = 0;
			} else if (x < (512 - zona_muerta)) {
			if (pos_x > 0) pos_x--;
			puedeMover = 0;
		}
		if (y > (512 + zona_muerta)) {
			if (pos_y < matriz - 1) pos_y++;
			puedeMover = 0;
			} else if (y < (512 - zona_muerta)) {
			if (pos_y > 0) pos_y--;
			puedeMover = 0;
		}
	}
	if ((x < (512 + zona_muerta)) && (x > (512 - zona_muerta)) &&
	(y < (512 + zona_muerta)) && (y > (512 - zona_muerta))) {
		puedeMover = 1;
	}
}

int main(void) {
	DDRD |= (1 << LED);
	DDRD &= ~(1 << boton);
	PORTD |= (1 << boton);

	iniciar_ADC();
	srand(leer_ADC(0));

	_delay_ms(200);
	mostrar_led(pos_x, pos_y, colorR, colorG, colorB);

	while (1) {
		leer_joystick();

		if (!(PIND & (1 << boton))) {
			cambiar_color();
			_delay_ms(200);
		}

		mostrar_led(pos_x, pos_y, colorR, colorG, colorB);
		_delay_ms(100);
	}
}
