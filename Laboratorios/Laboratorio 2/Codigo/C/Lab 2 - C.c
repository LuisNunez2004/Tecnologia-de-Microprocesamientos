#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>

const float notas[8] = {262, 296, 330, 369, 392, 440, 494, 523};
const uint8_t btns_portd[6] = {2, 3, 4, 5, 6, 7};
const uint8_t btns_portc[2] = {0, 1};

void USART_init(unsigned int ubrr) {
   UBRR0H = (unsigned char) (ubrr >> 8);
   UBRR0L = (unsigned char) (ubrr);
   
   UCSR0B = (1 << RXEN0) | (1 << TXEN0);
   UCSR0C = (1 << UCSZ01) | (1<< UCSZ00);
}

void USART_Transmit(unsigned char data) {
   while (!(UCSR0A & (1 << UDRE0)));
   UDR0 = data;
}

unsigned char USART_Recieve(void) {
   while (!(UCSR0A & (1 << RXC0)));
   return UDR0;
}

void USART_SendString(const char *str) {
   while (*str) {
      USART_Transmit(*str++);
   }
}
void Tone(int freq) {
   DDRB |= (1 << PB1);
   TCCR1A = (1 << COM1A0);
   TCCR1B = (1 << WGM12) | (1 << CS10);
   
   OCR1A = (uint16_t)((F_CPU / (2 * freq)) - 1);
}

void noTone() {
   TCCR1A = 0;
   TCCR1B = 0;
   PORTB &= ~(1 << PB1);
}

void menu(void) {
   USART_SendString("1-Modo Piano\r\n");
   USART_SendString("2-Reproducir Musica\r\n");
   while (1) {
      unsigned char recieved = USART_Recieve();
      if (recieved == '1') {
   ModoPiano();
   break;
      }
      else if (recieved == '2') {
   ModoReproductor();
   break;
      } else {
   USART_SendString("Opcion Invalida, intente nuevamente.\r\n");
      }
   }
}

void ModoPiano(void) {
    USART_SendString("Modo Piano:\r\n");
    USART_SendString("Para salir presione 0.\r\n");

    // Configurar botones como entrada con pull-ups activados
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
    DDRC &= ~((1 << PC0) | (1 << PC1));
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTC |= (1 << PC0) | (1 << PC1);

    while (1) {
        uint8_t btns_D = PIND;
        uint8_t btns_C = PINC;
        uint8_t notaTocada = 255;

        // Leer botones de PORTD (6 botones)
        for (uint8_t i = 0; i < 6; i++) {
            if (!(btns_D & (1 << btns_portd[i]))) {  // Activo en LOW
                notaTocada = i;
                break;
            }
        }

        // Leer botones de PORTC (2 botones adicionales)
        if (notaTocada == 255) {
            for (uint8_t i = 0; i < 2; i++) {
                if (!(btns_C & (1 << btns_portc[i]))) {  // Activo en LOW
                    notaTocada = i + 6;  // notas[6] y notas[7]
                    break;
                }
            }
        }

        // Reproducir nota o silencio
        if (notaTocada != 255) {
            Tone(notas[notaTocada]);
        } else {
            noTone();
        }

        // Si el usuario presiona '0' en el monitor serie, salir
        if (UCSR0A & (1 << RXC0)) {  // Verifica si hay datos disponibles
            unsigned char recieved = UDR0;
            if (recieved == '0') {
                noTone();
                break;
            }
        }

        _delay_ms(20); // Pequeño retardo para estabilidad
    }
}


void ModoReproductor(void) {
   USART_SendString("Modo Reproductor:\r\n");
   USART_SendString("0-Salir.\r\n");
   USART_SendString("1-Reproducir: JIJIJI - Solo - Patricio Rey y Sus Redonditos de Ricota.\r\n");
   USART_SendString("2-Reproducir: Avril - Los Estomagos.\r\n");
   USART_SendString("0-Salir.\r\n");
   while (1) {
      unsigned char recieved = USART_Recieve();
      if (recieved == '0') {
   break;
      } else if (recieved == '1') {
   Cancion1();
   break;
      } else if (recieved == '2') {
   Cancion2();
   break;
      }
   }
}

void Delay_ms_variable(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1);
    }
}

void Cancion1(void) {
    const float melodia[] = {220, 294, 330, 349, 392, 440, 440, 440, 440, 392, 392, 349, //12
    349, 330, 294, 294, 294, 330, 294, 262, 262, 294, 330, 330, 392, 349, 330, 392, 349, //17
    330, 392, 349, 330, 392, 349, 330, 392, 349, 330, 392, 349, 330, 294, 262, 294, 330, //46
    349, 330, 294, 262, 294, 349, 392, 349, 330, 349, 330, 262, 262, 294, 262, 262, 294, //63
    262, 294, 294, 349, 392, 440, 587, 523, 587, 659, 698, 784, 698, 784, 698, 659, 698, //80
    659, 523, 523, 587, 523, 294, 349, 392, 415, 440, 392, 349, 294, 349, 392, 415, 440, //97
    587, 523, 440, 440, 392, 349, 294, 262, 349, 294, 880, 932, 880, 784, 698, 784, 880, //114
    932, 880, 784, 698, 784, 880, 784, 880, 784, 698, 587, 698, 1175, 1245, 1109};
    const int duraciones[] = {175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 
    175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 351, 175, 175, 175, 175, 175, 
    175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175,
    175, 175, 175, 175, 351, 1755, 351, 351, 175, 175, 175, 351, 175, 175, 877, 175, 175,
    175, 351, 175, 175, 175, 351, 351, 351, 351, 2106, 88, 88, 1930, 526, 351, 175, 175,
    175, 351, 175, 175, 1579, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175,
    175, 175, 175, 351, 88, 88, 175, 175, 175, 351, 1930, 1666, 88, 351, 175, 175, 175, 
    1753, 175, 351, 175, 175, 88, 88, 88, 88, 702, 351, 702, 175, 175, 702, 351, 702};
    
    for (int i = 0; i < 2; i++) {
        for (int i = 0; i < 7; i++) {
          Tone(melodia[i]);
          Delay_ms_variable(duraciones[i]); 
        }
        noTone();
        Delay_ms_variable(175);
        for (int i = 7; i < 14; i++) {
              Tone(melodia[i]);
              Delay_ms_variable(duraciones[i]); 
          }
        noTone();
        Delay_ms_variable(175);
        Tone(melodia[14]);
        Delay_ms_variable(175);
        noTone();
        Delay_ms_variable(175);
        for (int i = 15; i < 20; i++) {
              Tone(melodia[i]);
              Delay_ms_variable(duraciones[i]); 
          }
        noTone();
        Delay_ms_variable(175);
        Tone(melodia[20]);
        Delay_ms_variable(175);
        noTone();
        Delay_ms_variable(175);
        Tone(melodia[21]);
        Delay_ms_variable(175);
        noTone();
        Delay_ms_variable(175);
        Tone(melodia[22]);
        Delay_ms_variable(175);
        Tone(melodia[23]);
        Delay_ms_variable(351);
        noTone();
        Delay_ms_variable(526);
        for (int i = 24; i < 44; i++) {
              Tone(melodia[i]);
              Delay_ms_variable(duraciones[i]);
          }
        noTone();
        Delay_ms_variable(175);
        for (int i = 44; i < 51; i++) {
              Tone(melodia[i]);
              Delay_ms_variable(duraciones[i]);
          }
        noTone();
        Delay_ms_variable(175);
    }
    
    Delay_ms_variable(877);
    for (int i = 51; i < 61; i++) {
        Tone(melodia[i]);
        Delay_ms_variable(duraciones[i]);
    }
    noTone();
    Delay_ms_variable(175);
    for (int i = 61; i < 73; i++) {
        Tone(melodia[i]);
        Delay_ms_variable(duraciones[i]);
    }
    noTone();
    Delay_ms_variable(175);
    for (int i = 73; i < 108; i++) {
        Tone(melodia[i]);
        Delay_ms_variable(duraciones[i]);
    }
    noTone();
    Delay_ms_variable(351);
    for (int i = 108; i < 129; i++) {
        Tone(melodia[i]);
        Delay_ms_variable(duraciones[i]);
    }
    noTone();
    Delay_ms_variable(702);
}



void Cancion2(void) {
   
}

int main(void) {
   USART_init(MYUBRR);
   while (1) {
      menu();
   }
}