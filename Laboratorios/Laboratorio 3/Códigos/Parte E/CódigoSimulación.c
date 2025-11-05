#define F_CPU 16000000UL
#define PCF8574 0x27
#include <avr/io.h>
#include <util/delay.h>

#include "twi.h"
#include "twi_lcd.h"
#include "SPI.h"
#include "UART.h"
#include "RC522.h"

#define EEPROM_ADDR 0x20
#define LED_VERDE PD2
#define LED_ROJO PD3
#define ACTUALIZAR PD4
#define BORRAR PD5
#define BUZZER PD6

uint8_t UID_guardada[5];
uint8_t card_uid[10] = {0};
uint8_t mismo_UID = 1;


void EEPROM_write(uint16_t addres, uint8_t data) {
	while (EECR & (1<<EEPE));
	EEAR = addres;
	EEDR = data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

uint8_t EEPROM_read(uint16_t address) {
	while (EECR & (1<<EEPE));
	EEAR = address;
	EECR |= (1<<EERE);
	return EEDR;
}

void guardar_UID(uint8_t *uid) {
	for (uint8_t i = 0; i < 5; i++) {
		EEPROM_write(i, uid[i]);
	}
}

void leer_UID(uint8_t *buffer) {
	for (uint8_t i = 0; i < 5; i++) {
		buffer[i] = EEPROM_read(i);
	}
}

void comparar_UID(uint8_t *uid1, uint8_t *uid2) {
	for (uint8_t i = 0; i < 5; i++) {
		if (uid1[i] != uid2[i]) {
			mismo_UID = 0;
			return;
		}
	}
	mismo_UID = 1;
}

void alarma(void) {
	PORTD |= (1<<BUZZER);
	_delay_ms(700);
	PORTD &= ~(1<<BUZZER);
}

void borrar_tarjeta(void){
	for(uint8_t i = 0; i < 5; i++){
		EEPROM_write(i, 0x00);
	}
	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);
	_delay_ms(2);
	twi_lcd_cmd(0x80);
	twi_lcd_msg("Tarjeta borrada");
	uart_print("Tarjeta borrada\n");
}

void actualizar_tarjeta(void){
	uint8_t nueva_UID[5] = {0};
		
		twi_lcd_clear();
		_delay_ms(2);
		twi_lcd_cmd(0x02);
		_delay_ms(2);
		twi_lcd_cmd(0x80);
		twi_lcd_msg("Acerque nueva");
		twi_lcd_cmd(0xC0);
		twi_lcd_msg("   tarjeta");
		uart_print("Esperando nueva tarjeta...\n");

		do {
			mfrc522_standard_2(nueva_UID);
			_delay_ms(500);
		} while(nueva_UID[0] == 0);

		guardar_UID(nueva_UID);
		PORTD |= (1<<LED_VERDE);
		twi_lcd_clear();
		_delay_ms(2);
		twi_lcd_cmd(0x02);
		_delay_ms(2);
		twi_lcd_cmd(0x80);
		twi_lcd_msg("Nueva tarjeta");
		twi_lcd_cmd(0xC0);
		twi_lcd_msg(" registrada");
		uart_print("Nueva tarjeta registrada\n");
		_delay_ms(1000);
		PORTD &= ~(1<<LED_VERDE);
		_delay_ms(2000);
		
		for (uint8_t i = 0; i < 5; i++) nueva_UID[i] = 0;
	}
	
int main(void) {
	
	DDRD |= (1<<LED_VERDE) | (1<<LED_ROJO) | (1<<BUZZER);
	DDRD &= ~((1<<ACTUALIZAR) | (1<<BORRAR));
	PORTD |= (1<<ACTUALIZAR) | (1<<BORRAR);
	
	twi_init();
	twi_lcd_init();
	uart_init(103);
	spi_init();
	mfrc522_resetPinInit();
	mfrc522_init();
	_delay_ms(100);

	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);
	_delay_ms(2);
	twi_lcd_cmd(0x80);
	twi_lcd_msg(" Bienvenidos al");
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("  sistema RFID");
	uart_print("Bienvenidos al sistema RFID\n");
	_delay_ms(3000);

	while (1) {
		twi_lcd_clear();
		_delay_ms(2);
		twi_lcd_cmd(0x02);
		_delay_ms(2);
		twi_lcd_cmd(0x80);
		twi_lcd_msg(" Pasar tarjeta");
		twi_lcd_cmd(0xC0);
		twi_lcd_msg("(1) A      (2) B");
		uart_print("Pasar tarjeta\n(1)Actualizar\n(2)Borrar");
		
		mfrc522_standard_2(card_uid);
		_delay_ms(500);
		
		 if (!(PIND & (1<<ACTUALIZAR))){
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Actualizar");
			uart_print("Actualizar tarjeta\n");
			actualizar_tarjeta();
			_delay_ms(2000);
			
		} else if (!(PIND & (1<<BORRAR))){
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Borrar");
			uart_print("Borrar tarjeta\n");
		    borrar_tarjeta();
		    _delay_ms(2000);
			
	} else	if(card_uid[0] != 0) {
			uart_print_hex_array(card_uid, 5);
			uart_print("Tarjeta detectada\n");
			
			_delay_ms(500);

			leer_UID(UID_guardada);
			comparar_UID(card_uid, UID_guardada);

			if (mismo_UID) {
				
				PORTD |= (1<<LED_VERDE);
				twi_lcd_clear();
				_delay_ms(2);
				twi_lcd_cmd(0x02);
				_delay_ms(2);
				twi_lcd_cmd(0x80);
				twi_lcd_msg("Acceso permitido");
				uart_print("Acceso permitido\n");
				_delay_ms(2000);
				PORTD &= ~(1<<LED_VERDE);
				
				} else {
				PORTD |= (1<<LED_ROJO);
				alarma();
				twi_lcd_clear();
				_delay_ms(2);
				twi_lcd_cmd(0x02);
				_delay_ms(2);
				twi_lcd_cmd(0x80);
				twi_lcd_msg("Acceso denegado");
				uart_print("Acceso denegado\n");
				_delay_ms(2000);
				PORTD &= ~(1<<LED_ROJO);	
			}
			 for (uint8_t i = 0; i < 10; i++) card_uid[i] = 0;
		}
	}
}