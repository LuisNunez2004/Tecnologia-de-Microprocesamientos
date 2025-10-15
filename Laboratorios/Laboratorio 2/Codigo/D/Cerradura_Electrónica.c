#define F_CPU 16000000UL
#define PCF8574 0x27
#include <avr/io.h>
#include <util/delay.h>
#include <LCD/twi.h>
#include <LCD/twi_lcd.h>

#define MAX_CONTRA 6
#define EEPROM_ADDR 0x20

#define LED_VERDE PB5
#define LED_ROJO PB4
#define BUZZER PD4

char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};


char read_keypad(void){
	for (uint8_t row = 0; row < 4; row++){
		PORTB |= 0x0F;
		PORTB &= ~(1 << row);  
		_delay_us(5);

		for (uint8_t col = 0; col < 4; col++){
			if (!(PIND & (1 << col))){   
				_delay_ms(20);            
				while (!(PIND & (1 << col))); 
				PORTB |= 0x0F;
				return keypad[row][col];
			}
		}
	}
	PORTB |= 0x0F;
	return 0;
}


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

uint8_t ingresar_contra(char *buffer, uint8_t max_len){
	uint8_t indice = 0;
	char clave;

	twi_lcd_cmd(0xC0); 
	 while (1) {
		 clave = read_keypad();
		 if (clave != 0) {
			 if (clave == '#') {
				 break; 
			 }
			 else if (clave == '*') {
				 if (indice > 0) {
					 indice--;                
					 buffer[indice] = '\0';   
					 
					 twi_lcd_cmd(0x10);       
					 twi_lcd_msg(" ");        
					 twi_lcd_cmd(0x10);       
				 }
			 }
			  else if(indice < max_len) {
				  buffer[indice++] = clave;

				  char msg[2] = {clave, '\0'};
				  twi_lcd_msg(msg);
				  _delay_ms(300); 

				  twi_lcd_cmd(0x10);
				  twi_lcd_msg("*");
			  }
		 }
	 }
	return indice;
}

void guardar_contra(char *buffer, uint8_t longitud) {
	EEPROM_write(EEPROM_ADDR, longitud);
	for (uint8_t i = 0; i < longitud; i++) {
		EEPROM_write(EEPROM_ADDR + 1 + i, buffer[i]);
	}
}

uint8_t revisar_contra(char *buffer, uint8_t longitud) {
	uint8_t longitud_guardada = EEPROM_read(EEPROM_ADDR); 
	
	if (longitud != longitud_guardada) {
		return 0; 
	}

	for (uint8_t i = 0; i < longitud_guardada; i++) {
		if (buffer[i] != EEPROM_read(EEPROM_ADDR + 1 + i)) {
			return 0; 
		}
	}
	return 1;
}

void restablecer_contra(void) {
	char contra_inicial[] = "1234";
	guardar_contra(contra_inicial, 4);
}

int main(void){
	DDRB |= 0x0F;    
	PORTB |= 0x0F;   

	DDRD &= ~0x0F;  
	PORTD |= 0x0F; 

	DDRD |= (1 << BUZZER);
	PORTD &= ~(1 << BUZZER);

	EEPROM_write(0x10, 42);
	uint8_t read_value = EEPROM_read(0x10);
	(void)read_value;

	twi_init();
	twi_lcd_init();
	
	if (EEPROM_read(EEPROM_ADDR) == 0xFF){ 
		char contra_inicial[] = "1234";
		guardar_contra(contra_inicial, 4);
	}
	
	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);   
	_delay_ms(2);
	twi_lcd_cmd(0x80);   
	twi_lcd_msg("BIENVENIDO :)");
	_delay_ms(3000);
	twi_lcd_clear();
	_delay_ms(2);
	twi_lcd_cmd(0x02);
	_delay_ms(2);  
	twi_lcd_msg("Elija una opción");
	_delay_ms(2000);
	
	char opcion;
	char contra_buffer[MAX_CONTRA];

	while(1){
		twi_lcd_clear();
		_delay_ms(2);
		twi_lcd_cmd(0x02);
		_delay_ms(2);
		twi_lcd_cmd(0x80);
		twi_lcd_msg("(1) Ingresar");
		twi_lcd_cmd(0xC0);
		twi_lcd_msg("(2) Cambiar");
	
		opcion = 0;
		while (opcion != '1' && opcion != '2'){
			opcion = read_keypad();
	}
	
	if(opcion == '1'){
		uint8_t intentos = 0;
		uint8_t valido = 0;
		
		while(intentos  < 3 && !valido){
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Ingrese:");
			twi_lcd_cmd(0xC0);
			uint8_t len = ingresar_contra(contra_buffer, MAX_CONTRA);
			
			 if(revisar_contra(contra_buffer, len)){
				 PORTB |= (1<<LED_VERDE);
				 twi_lcd_clear();
				 _delay_ms(2);
				 twi_lcd_cmd(0x02);
				 _delay_ms(2);
				 twi_lcd_cmd(0x80);
				 twi_lcd_msg("ACCESO CONCEDIDO");
				 _delay_ms(2000);
				 PORTB &= ~(1<<LED_VERDE);
				 valido = 1;
				 } else {
				 PORTB |= (1<<LED_ROJO);
				twi_lcd_clear();
				_delay_ms(2);
				twi_lcd_cmd(0x02);
				_delay_ms(2);
				twi_lcd_cmd(0x80);
				 twi_lcd_msg("Clave incorrecta");
				 _delay_ms(1000);
				 PORTB &= ~(1<<LED_ROJO);
				 intentos++;
				 
				 if (intentos == 3) {
					 twi_lcd_clear();
					 _delay_ms(2);
					 twi_lcd_cmd(0x02);
					 _delay_ms(2);
					 twi_lcd_cmd(0x80);
					 twi_lcd_msg("BLOQUEADO :v");
					  _delay_ms(1000);
					 
					 for (uint8_t i = 0; i < 5; i++) {
						 PORTD |= (1 << BUZZER);
						 _delay_ms(2000);
						 PORTD &= ~(1 << BUZZER);
						 _delay_ms(2000);
					 }
				 }
			 }
		   }
		} else {
		twi_lcd_clear();
		_delay_ms(2);
		twi_lcd_cmd(0x02);
		_delay_ms(2);
		twi_lcd_cmd(0x80);
		twi_lcd_msg("Actual:");
		uint8_t len = ingresar_contra(contra_buffer, MAX_CONTRA);

		if(revisar_contra(contra_buffer, len)){
			PORTB |= (1<<LED_VERDE);
			_delay_ms(1000);
			PORTB &= ~(1<<LED_VERDE);
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Nueva:");
			uint8_t nueva_len = ingresar_contra(contra_buffer, MAX_CONTRA);
			guardar_contra(contra_buffer, nueva_len);

			PORTB |= (1<<LED_VERDE);
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Guardada!");
			_delay_ms(1000);
			PORTB &= ~(1<<LED_VERDE);
		}
		else {
			PORTB |= (1<<LED_ROJO);
			twi_lcd_clear();
			_delay_ms(2);
			twi_lcd_cmd(0x02);
			_delay_ms(2);
			twi_lcd_cmd(0x80);
			twi_lcd_msg("Clave incorrecta");
			_delay_ms(1000);
			PORTB &= ~(1<<LED_ROJO);
			}
		}

	}
	
}
