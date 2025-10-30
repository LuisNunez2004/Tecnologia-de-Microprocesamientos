#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define mov_x PC0 
#define mov_y PC1
#define boton PD7
#define LED PD6

#define matriz 8
#define zona_muerta 100

uint8_t pos_x = 4;
uint8_t pos_y = 4;
uint8_t colorR = 255, colorG = 255, colorB = 255;

void ws2812_send_bit(uint8_t bitVal) {
	if (bitVal) {
		PORTD |= (1 << LED);
		asm volatile (
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		);
		PORTD &= ~(1 << LED);
		asm volatile (
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		);
		} else {
		PORTD |= (1 << LED);
		asm volatile(
		"nop\n\t""nop\n\t""nop\n\t"
		);
		PORTD &= ~(1 << LED);
		asm volatile(
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		"nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
		);
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
	_delay_us(50); 
}

void iniciar_ADC(void){
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t leer_ADC(uint8_t canal){
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

void leer_joystick(void) {
	uint16_t x = leer_ADC(0);
	uint16_t y = leer_ADC(1);
	
	static uint8_t puedeMover = 1;
	
	if (puedeMover) {
		if (x > (512 + zona_muerta)) { 
			pos_x = (pos_x + 1) % matriz;
			puedeMover = 0;
			} else if (x < (512 - zona_muerta)) { 
			pos_x = (pos_x == 0) ? (matriz - 1) : (pos_x - 1);
			puedeMover = 0;
		}
		
		if (y > (512 + zona_muerta)) {
			pos_y = (pos_y + 1) % matriz;
			puedeMover = 0;
			} else if (y < (512 - zona_muerta)) { 
			pos_y = (pos_y == 0) ? (matriz - 1) : (pos_y - 1);
			puedeMover = 0;
		}
	}
	
	if ((x < (512 + zona_muerta)) && (x > (512 - zona_muerta)) &&
	(y < (512 + zona_muerta)) && (y > (512 - zona_muerta))) {
		puedeMover = 1;
	}
}

int main(void)
{
	DDRD |= (1 << LED);	
	DDRD &= ~(1 << boton);   
    PORTD |= (1 << boton);
	
	iniciar_ADC();
	srand(leer_ADC(0));

    while (1) {
		leer_joystick();
		
		if (!(PIND &(1 << boton))){
			cambiar_color();
			_delay_ms(200);
		}
		
		ws2812_send_color(colorR, colorG, colorB);
		_delay_ms(100);
    }
}

