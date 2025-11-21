#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define LED PD6
#define boton PD7
#define matriz 8

#define MPU_ADDR 0x68
#define ZONA_MUERTA_RAW 2000

uint8_t pos_x = 4;
uint8_t pos_y = 4;
uint8_t colorR = 255, colorG = 0, colorB = 0;

/* ---------------- UART ---------------- */
void UART_init(unsigned int baud) {
	unsigned int ubrr = F_CPU/16/baud - 1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0); // habilitar transmisión
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); // 8 bits de datos
}

void UART_transmit(char data) {
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void UART_print(const char* str) {
	while (*str) {
		UART_transmit(*str++);
	}
}

void UART_print_int(int16_t val) {
	char buffer[10];
	itoa(val, buffer, 10);
	UART_print(buffer);
}

/* ---------------- WS2812 ---------------- */
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

uint16_t xy_to_index(uint8_t x, uint8_t y) {
	return y * matriz + x;
}

void mostrar_led(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
	uint16_t total = matriz * matriz;
	uint16_t target = xy_to_index(x, y);
	for (uint16_t i = 0; i < total; i++) {
		if (i == target) ws2812_send_color(r, g, b);
		else ws2812_send_color(0, 0, 0);
	}
	ws2812_reset();
}

/* ---------------- I2C ---------------- */
void twi_init(void) {
	TWSR = 0x00;
	TWBR = 72;
	TWCR = (1<<TWEN);
}

void twi_start(void) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void twi_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	_delay_us(10);
}

void twi_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

uint8_t twi_read_ack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t twi_read_nack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void twi_write_register(uint8_t devaddr, uint8_t reg, uint8_t data) {
	twi_start();
	twi_write((devaddr << 1) | 0); // write
	twi_write(reg);
	twi_write(data);
	twi_stop();
}

void twi_read_regs(uint8_t devaddr, uint8_t reg, uint8_t *buf, uint8_t len) {
	twi_start();
	twi_write((devaddr << 1) | 0);
	twi_write(reg);
	twi_start();
	twi_write((devaddr << 1) | 1);
	for (uint8_t i = 0; i < len; i++) {
		if (i == (len - 1)) buf[i] = twi_read_nack();
		else buf[i] = twi_read_ack();
	}
	twi_stop();
}

/* ---------------- MPU6050 ---------------- */
void mpu6050_init(void) {
	twi_write_register(MPU_ADDR, 0x6B, 0x00);
	_delay_ms(10);
}

void mpu6050_read_accel(int16_t *ax, int16_t *ay) {
	uint8_t buf[6];
	twi_read_regs(MPU_ADDR, 0x3B, buf, 6);
	*ax = (int16_t)((buf[0] << 8) | buf[1]);
	*ay = (int16_t)((buf[2] << 8) | buf[3]);
}

/* ---------------- Lógica ---------------- */
void cambiar_color(void) {
	colorR = rand() % 256;
	colorG = rand() % 256;
	colorB = rand() % 256;
}

void leer_mpu_y_mover(void) {
	int16_t ax_raw = 0, ay_raw = 0;
	static uint8_t puedeMover = 1;

	mpu6050_read_accel(&ax_raw, &ay_raw);

	// Imprimir por UART los valores
	UART_print("ax: "); UART_print_int(ax_raw);
	UART_print(" | ay: "); UART_print_int(ay_raw);
	UART_print(" | pos_x: "); UART_print_int(pos_x);
	UART_print(" | pos_y: "); UART_print_int(pos_y);
	UART_print("\r\n");

	if (puedeMover) {
		if (ax_raw > ZONA_MUERTA_RAW) {
			if (pos_x < (matriz - 1)) pos_x++;
			puedeMover = 0;
			} else if (ax_raw < -ZONA_MUERTA_RAW) {
			if (pos_x > 0) pos_x--;
			puedeMover = 0;
		}

		if (ay_raw > ZONA_MUERTA_RAW) {
			if (pos_y < (matriz - 1)) pos_y++;
			puedeMover = 0;
			} else if (ay_raw < -ZONA_MUERTA_RAW) {
			if (pos_y > 0) pos_y--;
			puedeMover = 0;
		}
	}
	
	if ((ax_raw < ZONA_MUERTA_RAW) && (ax_raw > -ZONA_MUERTA_RAW) &&
	(ay_raw < ZONA_MUERTA_RAW) && (ay_raw > -ZONA_MUERTA_RAW)) {
		puedeMover = 1;
	}
}

/* ---------------- Main ---------------- */
int main(void) {
	DDRD |= (1 << LED);
	DDRD &= ~(1 << boton);
	PORTD |= (1 << boton);

	twi_init();
	mpu6050_init();
	UART_init(9600);

	srand( (uint16_t) ( (uint16_t) (TCNT1) ^ (uint16_t) (TIFR0) ) );

	_delay_ms(200);
	mostrar_led(pos_x, pos_y, colorR, colorG, colorB);

	UART_print("MPU6050 iniciado correctamente.\r\n");

	while (1) {
		leer_mpu_y_mover();

		if (!(PIND & (1 << boton))) {
			cambiar_color();
			_delay_ms(200);
		}

		mostrar_led(pos_x, pos_y, colorR, colorG, colorB);
		_delay_ms(100);
	}

	return 0;
}
