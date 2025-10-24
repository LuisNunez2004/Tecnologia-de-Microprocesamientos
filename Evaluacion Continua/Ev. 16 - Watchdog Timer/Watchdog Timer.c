#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

volatile uint8_t contador = 0;  

ISR(WDT_vect) {
	contador++;
}

void setup_watchdog(void) {
	cli(); 
	wdt_reset();
	WDTCSR = (1 << WDCE) | (1 << WDE);          
	WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);
	sei(); 
}

int main(void) {
	DDRB |= 0b00011111; 
	PORTB &= ~0b00011111;

	setup_watchdog(); 

	while (1) {
		PORTB |= 0b00011111; 
		_delay_ms(5000);     
		PORTB &= ~0b00011111; 

		contador = 0;
		
		set_sleep_mode(SLEEP_MODE_IDLE);
		
		while (contador < 10) {
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
		contador = 0;
		set_sleep_mode(SLEEP_MODE_ADC);
		while (contador < 10) {
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
		contador = 0;
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		while (contador < 10) {
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
	}
}
