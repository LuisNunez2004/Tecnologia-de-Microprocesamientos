#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define TRIG_PIN PD2
#define ECHO_PIN PD3

void init_hcsr04(void);
uint16_t measure_distance(void);
void init_uart(uint16_t baudrate);
void uart_send_char(char c);
void uart_send_string(const char *str);
void uart_send_uint(uint16_t data);
void init_pwm(void);
void pwm_set_duty_cycle(uint16_t distance);
void lcd_init(void);
void lcd_send_command(unsigned char cmd);
void lcd_send_data(unsigned char data);
void display_distance_lcd(uint16_t distance);

void init_hcsr04() {
    DDRD |= (1 << TRIG_PIN);
    DDRD &= ~(1 << ECHO_PIN);
}

uint16_t measure_distance() {
    uint16_t time;
    PORTD &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTD |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN);
    while (!(PIND & (1 << ECHO_PIN)));
    TCCR1A = 0;
    TCCR1B = (1 << CS11);
    TCNT1 = 0;
    while (PIND & (1 << ECHO_PIN));
    TCCR1B = 0;
    time = TCNT1;
    return (time / 58);
}

void init_uart(uint16_t baudrate) {
    uint16_t ubrr = F_CPU / 16 / baudrate - 1;
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

void uart_send_uint(uint16_t data) {
    char buffer[10];
    itoa(data, buffer, 10);
    uart_send_string(buffer);
    uart_send_string(" cm\r\n");
}

void init_pwm() {
    DDRB |= (1 << PB1);
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);
    ICR1 = 39999;
}

void pwm_set_duty_cycle(uint16_t distance) {
    if (distance > 100) distance = 100;
    OCR1A = (distance * ICR1) / 100;
}

int main(void) {
    uint16_t distance;
    init_hcsr04();
    init_uart(9600);
    init_pwm();
    while (1) {
        distance = measure_distance() / 1.99;
        uart_send_uint(distance);
        pwm_set_duty_cycle(distance);
        _delay_ms(500);
    }
    return 0;
}
