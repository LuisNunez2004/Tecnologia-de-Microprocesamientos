#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ---------- LCD CONFIG ----------
#define LCD_RS PB0
#define LCD_RW PB1
#define LCD_EN PB2
#define LCD_DATA_PORT PORTD  // PD4–PD7
#define LCD_CTRL_PORT PORTB
#define LCD_DATA_DDR DDRD
#define LCD_CTRL_DDR DDRB

void lcd_init_pins(void) {
	LCD_DATA_DDR |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
	LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);
}

void lcd_pulse_enable(void) {
	LCD_CTRL_PORT |= (1 << LCD_EN);
	_delay_us(1);
	LCD_CTRL_PORT &= ~(1 << LCD_EN);
	_delay_us(100);
}

void lcd_send_nibble(uint8_t nibble) {
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (nibble & 0xF0);
	lcd_pulse_enable();
}

void lcd_command(uint8_t cmd) {
	LCD_CTRL_PORT &= ~(1 << LCD_RS);
	LCD_CTRL_PORT &= ~(1 << LCD_RW);
	lcd_send_nibble(cmd);
	lcd_send_nibble(cmd << 4);
	_delay_ms(2);
}

void lcd_data(uint8_t data) {
	LCD_CTRL_PORT |= (1 << LCD_RS);
	LCD_CTRL_PORT &= ~(1 << LCD_RW);
	lcd_send_nibble(data);
	lcd_send_nibble(data << 4);
	_delay_ms(2);
}

void lcd_init(void) {
	_delay_ms(50);
	lcd_command(0x30);
	_delay_ms(5);
	lcd_command(0x30);
	_delay_us(100);
	lcd_command(0x32);
	_delay_ms(5);
	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x01);
	_delay_ms(2);
	lcd_command(0x06);
}

void lcd_write_line1(const char *message) {
	lcd_command(0x80);
	while (*message) lcd_data(*message++);
}

void lcd_write_line2(const char *message) {
	lcd_command(0xC0);
	while (*message) lcd_data(*message++);
}

const char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void keypad_init(void) {
	DDRC |= 0x0F;      
	PORTC |= 0x0F;     
	DDRB &= ~0xF0;     
	PORTB |= 0xF0;     
}

char keypad_read(void) {
	for (uint8_t row = 0; row < 4; row++) {
		PORTC |= 0x0F;          
		PORTC &= ~(1 << row);   
		_delay_us(5);

		for (uint8_t col = 0; col < 4; col++) {
			if (!(PINB & (1 << (col + 4)))) {  
				_delay_ms(20); 
				while (!(PINB & (1 << (col + 4)))); 
				return keypad[row][col];
			}
		}
	}
	return 0;
}

int main(void) {
	lcd_init_pins();
	lcd_init();
	keypad_init();

	lcd_write_line1("Presione tecla:");
	lcd_write_line2("                ");

	while (1) {
		char key = keypad_read();
		if (key) {
			char msg[17] = "Tecla: ";
			msg[7] = key;
			msg[8] = '\0';
			lcd_write_line2(msg);
		}
	}
}
