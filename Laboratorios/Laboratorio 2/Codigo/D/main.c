#define F_CPU 16000000UL
#define PCF8574 0x27
#include <avr/io.h>
#include <util/delay.h>
#include <LCD/twi.h>
#include <LCD/twi_lcd.h>

void EEPROM_write(uint16_t addres, uint8_t data){
	while (EECR & (1<<EEPE));
	EEAR = addres;
	EEDR = data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

uint8_t EEPROM_read(uint16_t address){
	while (EECR & (1<<EEPE));
	EEAR = address;
	EECR |= (1<<EERE);
	return EEDR;
}

char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

char read_keypad(void){
	for (uint8_t row = 0; row < 4; row ++){
		PORTB = ~(1 << row);
		for (uint8_t col = 0; col < 4; col ++){
			if (!(PIND & (1 << col))){
				return keypad[row][col];
			}
		}
	}
	return 0;
}

int main(void){
	EEPROM_write(0x10, 42);
	uint8_t read_value = EEPROM_read(0x10);

	twi_init();
	twi_lcd_init();

	twi_lcd_msg("Hola mundo!");
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("Funciona LCD :)");

	while(1){
		_delay_ms(1000);
		twi_lcd_clear();
		twi_lcd_msg("Nuevo texto");
		_delay_ms(1000);
		twi_lcd_clear();
		twi_lcd_msg("LCD por I2C");
	}
}
