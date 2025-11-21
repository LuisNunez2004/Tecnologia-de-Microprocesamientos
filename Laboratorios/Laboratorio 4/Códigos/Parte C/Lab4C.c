#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define F_CPU 16000000UL
#define LED PD6
#define NUM_LEDS 64
#define MATRIX_SIZE 8

uint8_t leds[NUM_LEDS][3];
volatile uint8_t current_mode = 1;

const uint8_t sunrise_lut[64][3] = {
    {0, 0, 0}, {5, 12, 18}, {10, 24, 35}, {15, 36, 52},
    {20, 48, 69}, {25, 60, 86}, {30, 72, 103}, {35, 84, 120},
    {40, 96, 137}, {45, 108, 154}, {50, 120, 171}, {55, 132, 188},
    {60, 144, 205}, {65, 156, 222}, {70, 168, 239}, {75, 180, 255},
    {80, 186, 255}, {85, 192, 255}, {90, 198, 255}, {95, 204, 255},
    {100, 210, 255}, {105, 215, 255}, {110, 220, 255}, {115, 225, 255},
    {125, 230, 255}, {135, 235, 255}, {145, 240, 255}, {155, 245, 255}, 
    {165, 250, 255}, {170, 255, 255}, 
    {170, 255, 255}, {170, 255, 255}, {170, 255, 255}, {170, 255, 255},
    {165, 250, 255}, {155, 245, 255}, {145, 240, 255}, {135, 235, 255}, 
    {125, 230, 255}, {115, 225, 255}, {105, 220, 255}, {95, 215, 255}, 
    {85, 205, 255}, {75, 195, 255}, {65, 185, 255}, {55, 175, 255}, 
    {45, 165, 255}, {35, 155, 255}, {25, 145, 255}, {15, 135, 255}, 
    {12, 115, 200}, {9, 95, 150}, {6, 75, 100}, {3, 55, 50}, 
    {2, 40, 35}, {1, 25, 20}, {0, 10, 10}, {0, 5, 5},
    {0, 3, 3}, {0, 2, 2}, {0, 1, 1}, {0, 0, 1}, 
    {0, 0, 0}, {0, 0, 0}
};

void uart_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

char uart_read_nonblocking() {
    if (UCSR0A & (1 << RXC0)) {
        return UDR0;
    }
    return 0;
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
        ws2812_send_bit(byte & (1 << (7 - i)));
    }
}

void show(uint8_t (*colors)[3]) {
    cli();
    for (int i = 0; i < NUM_LEDS; i++) {
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

void animation_motorcycle(uint8_t frame_offset) {
    const uint8_t ROAD_COLOR_R = 10;
    const uint8_t ROAD_COLOR_G = 10;
    const uint8_t ROAD_COLOR_B = 10;
    
    const uint8_t LINE_COLOR_R = 200;
    const uint8_t LINE_COLOR_G = 200;
    const uint8_t LINE_COLOR_B = 200;
    
    const uint8_t GRASS_COLOR_R = 0;
    const uint8_t GRASS_COLOR_G = 60;
    const uint8_t GRASS_COLOR_B = 0;
    
    const uint8_t MOTORCYCLE_COLOR_R = 255;
    const uint8_t MOTORCYCLE_COLOR_G = 0;
    const uint8_t MOTORCYCLE_COLOR_B = 0;

    const int MOTO_ROW_START = 6;
    const int MOTO_ROW_END = 7;
    const int MOTO_COL_START = 3;
    const int MOTO_COL_END = 3;

    uint8_t line_offset = frame_offset % MATRIX_SIZE;

    for (int i = 0; i < NUM_LEDS; i++) {
        int x = i % MATRIX_SIZE;
        int y = i / MATRIX_SIZE;

        if (y >= MOTO_ROW_START && y <= MOTO_ROW_END && x >= MOTO_COL_START && x <= MOTO_COL_END) {
            setLedRGB(leds, i, MOTORCYCLE_COLOR_R, MOTORCYCLE_COLOR_G, MOTORCYCLE_COLOR_B);
        }
        else if (x == 0 || x == MATRIX_SIZE - 1) {
            setLedRGB(leds, i, GRASS_COLOR_R, GRASS_COLOR_G, GRASS_COLOR_B);
        }
        else {
            setLedRGB(leds, i, ROAD_COLOR_R, ROAD_COLOR_G, ROAD_COLOR_B);
            if (x == MATRIX_SIZE / 2 - 1 || x == MATRIX_SIZE / 2) {
                if (((y + line_offset) % 4) < 2) {
                    setLedRGB(leds, i, LINE_COLOR_R, LINE_COLOR_G, LINE_COLOR_B);
                }
            }
        }
    }
}

void animation_sunrise(uint8_t frame_offset) {
    uint16_t anim_step = frame_offset % (NUM_LEDS * 4); 

    uint8_t sky_index;
    if (anim_step >= 254) {
      sky_index = 64;
      } else {
        sky_index  = anim_step / 4;
      };

    uint8_t sky_r = sunrise_lut[sky_index][0];
    uint8_t sky_g = sunrise_lut[sky_index][1];
    uint8_t sky_b = sunrise_lut[sky_index][2];
    uint8_t sun_brightness = (sky_r + sky_g + sky_b) / 3;
    uint8_t sun_center = 3 - 3*8;
    sun_center += 8*(anim_step/21);
    uint8_t sun_location[12] = {
        sun_center - 8, sun_center - 8 + 1,
        sun_center - 1, sun_center, sun_center + 1, sun_center + 2,
        sun_center - 1 + 8, sun_center + 8, sun_center + 1 + 8, sun_center + 2 + 8,
        sun_center + 16, sun_center + 16 + 1
      }; 
    
    for (int i = 0; i < NUM_LEDS; i++) {
      for (int s = 0; s < 12; s++) {
        if ( i == sun_location[s]) {
          setLedRGB(leds, i, sun_brightness, sun_brightness, 0);
          break;
        } else {
            setLedRGB(leds, i, sky_r, sky_g, sky_b);  
        }
      }
    }
}

int main(void) {
    DDRD |= (1 << LED);
    uart_init(MYUBRR);

    uint8_t frame_counter = 0;
    char received_char;

    while (1) {
        received_char = uart_read_nonblocking();
        if (received_char == '1') current_mode = 1;
        if (received_char == '2') current_mode = 2;

        if (current_mode == 1) {
            animation_motorcycle(frame_counter);
            _delay_ms(80);
        } else {
            animation_sunrise(frame_counter);
            _delay_ms(50);
        }

        show(leds);
        frame_counter++; 
    }
    return 0;
}
