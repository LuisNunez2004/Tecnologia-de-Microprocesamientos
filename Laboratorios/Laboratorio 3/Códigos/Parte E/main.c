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

uint8_t UID_guardada[5];
uint8_t card_uid[10] = {0};
uint8_t mismo_UID = 1;

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

void guardar_UID(uint8_t *uid){
	for (uint8_t i = 0; i < 5; i++){
		EEPROM_write(i, uid[i]);
	}
}

void leer_UID(uint8_t *buffer){
	for (uint8_t i =  0; i < 5; i++){
		buffer[i] = EEPROM_read(i);
	}	
}

void comparar_UID(uint8_t *uid1, uint8_t *uid2){
	for (uint8_t i = 0; i < 5; i++){
		if (uid1[i] != uid2[i]){
			mismo_UID = 0;
			return;
		}
	}
	mismo_UID = 1;
}

void borrar_tarjeta(void){
	for (uint8_t i = 0; i < 5; i++){
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
	uint8_t nueva_UID[5];
	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);
	_delay_ms(2);
	twi_lcd_cmd(0x80);
	twi_lcd_msg(" Acerque nueva");
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("    tarjeta");
	uart_print("Esperando nueva tarjeta...\n");
	
	while(1){
		mfrc522_standard(nueva_UID);
		_delay_ms(500);
		if (nueva_UID[0] != 0){
			uart_print_hex_array(nueva_UID, 5);
			uart_print("NUeva tarjeta detectada\n");
			guardar_UID(nueva_UID);
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg(" Nueva tarjeta");
			twi_lcd_cmd(0xC0);
			twi_lcd_msg("   registrada");
			uart_print("Nueva tarjeta registrada\n");
			_delay_ms(2000);
			break;
		}
	}
	
}

int main(void) {
	DDRD &= ~0x3C;
	PORTD |= 0x3C;


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
  
	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);
	_delay_ms(2);
	twi_lcd_cmd(0x80);
	twi_lcd_msg("   Acerque su");
	twi_lcd_cmd(0xC0);
	twi_lcd_msg("    tarjeta");
	uart_print("Acerque su tarjeta\n");
   
    while (1) {
		
		mfrc522_standard(card_uid);
		_delay_ms(500);
		
		uart_print("Lectura realizada\n");
		
		if(card_uid[0] != 0){
			uart_print_hex_array(card_uid, 5);
			uart_print("Tarjeta detectada\n");
			_delay_ms(500);
			
			leer_UID(UID_guardada);
			comparar_UID(card_uid, UID_guardada);
			
			if (mismo_UID){
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
				twi_lcd_clear();
				_delay_ms(2);
				twi_lcd_cmd(0x02);
				_delay_ms(2);
				twi_lcd_cmd(0x80);
				twi_lcd_msg("Acesso denegado");
				uart_print("Acceso denegado\n");
				_delay_ms(2000);
				PORTD &= ~(1<<LED_ROJO);
			}
			
			if (!(PIND & (1<<BORRAR))){
				borrar_tarjeta();
				_delay_ms(500);
			}
			
			if (!(PIND & (1<<ACTUALIZAR))){
				actualizar_tarjeta();
				_delay_ms(500);
			}
			  
		}
    }
}

