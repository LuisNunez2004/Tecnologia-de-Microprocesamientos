#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define LDR 0
#define SERVO PB1
#define LED PD6
#define NUM_LEDS 64

uint8_t leds[NUM_LEDS][3];

void uart_init(unsigned int ubrr) {
   UBRR0H = (unsigned char)(ubrr >> 8);
   UBRR0L = (unsigned char)(ubrr);
   UCSR0B = (1 << TXEN0);
   UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(char data) {
   while (!(UCSR0A & (1 << UDRE0)));
   UDR0 = data;
}

void uart_print(const char *s) {
   while (*s) uart_transmit(*s++);
}

void adc_init(void) {
   ADMUX = (1 << REFS0);
   ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t adc_read(uint8_t channel) {
   ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
   ADCSRA |= (1 << ADSC);
   while (ADCSRA & (1 << ADSC));
   return ADC;
}

void servo_init(void) {
   DDRB |= (1 << SERVO);
   TCCR1A = (1 << COM1A1) | (1 << WGM11);
   TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
   ICR1 = 40000; //Periodo 20ms (16MHz / 8 / 40000)
}

void servo_set_angle(uint8_t angle) {
   uint16_t duty = 1000 + ((angle * 1000) / 100);
   OCR1A = (duty * 2);
}

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
      ws2812_send_bit(byte & (1 << (7-i)));
   }
}

void show(uint8_t (*colors)[3]){
  cli();
  for (int i=0; i < NUM_LEDS; i++) {
      ws2812_send_byte(colors[i][1]);
      ws2812_send_byte(colors[i][0]);
      ws2812_send_byte(colors[i][2]);
    }
  sei();
  _delay_us(60); 
  }

void setLedRGB(uint8_t (*leds)[3], int ledIndex, uint8_t r, uint8_t g, uint8_t b) {
    if (ledIndex < 0 || ledIndex >= NUM_LEDS) return;
    leds[ledIndex][0] = r;
    leds[ledIndex][1] = g;
    leds[ledIndex][2] = b;
  }

int main(void) {
   uart_init(MYUBRR);
   adc_init();
   servo_init();
   DDRD |= (1 << LED);
   uart_print('Programa inicializado\r\n');
   
   sei();
   
   
   uint16_t valor;
   uint8_t r, g, b, angle;
   char buffer[64];
   while (1) {
      valor = adc_read(LDR);
      
      if (valor < 200) { r=255; g=0; b=0; angle=0; uart_print("Color: Rojo\r\n");}
      else if (valor < 400) { r=0; g=255; b=0; angle=105; uart_print("Color: Verde\r\n");}
      else if (valor < 600) { r=0; g=0; b=255; angle=145; uart_print("Color: Celeste\r\n");}
      else if (valor < 800) { r=255; g=255; b=0; angle=45; uart_print("Color: Amarillo\r\n");}
      else { r=255; g=255; b=255; angle=0; uart_print("Color: Blanco\r\n");}
   
   servo_set_angle(angle);
   for (int i = 0; i < 256; i++) {
      setLedRGB(leds, i, r, g, b);
    }
   
   show(leds);
   
   snprintf(buffer, sizeof(buffer), "Lectura ADC: %u\r\n", valor);
   uart_print(buffer);
   
   _delay_ms(500);
   }
}