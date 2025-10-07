#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>

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
   while (1) {
      unsigned char recieved = USART_Recieve();
      if (recieved == '0') {
   break;
      }
   }
}

void ModoReproductor(void) {
   USART_SendString("Modo Reproductor:\r\n");
   USART_SendString("0-Salir.\r\n");
   USART_SendString("1-Reproducir: Ya Nadie Va a Escuchar Tu Remera - Patricio Rey y Sus Redonditos de Ricota.\r\n");
   USART_SendString("2-Reproducir: Avril - Los Estomagos.\r\n");
   USART_SendString("0-Salir.\r\n");
   while (1) {
      unsigned char recieved = USART_Recieve();
      if (recieved == '0') {
   break;
      }
   }
}

int main(void) {
   USART_init(MYUBRR);
   while (1) {
      menu();
   }
}
