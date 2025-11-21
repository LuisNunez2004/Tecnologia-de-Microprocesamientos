/*
 * ESCLAVO: Recibe órdenes del Maestro y activa actuadores
 * Dirección I2C: 0x10
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// --- CONFIGURACIÓN DE DIRECCIÓN ---
#define SLAVE_ADDR 0x10 

// --- PINOUT (ATmega328P / Arduino Uno) ---
// LED de Alerta: Puerto B, Pin 0 (Digital 8)
#define PIN_LED     PB0 
// Motor: Puerto D, Pin 7 (Digital 7)
#define PIN_MOTOR   PD7 
// Buzzer: Puerto D, Pin 6 (Digital 6)
#define PIN_BUZZER  PD6 

// Variable volátil para intercambiar datos entre la interrupción y el main
volatile uint8_t comando_recibido = 0;
volatile uint8_t flag_buzzer = 0;

// Inicialización del I2C en modo Esclavo
void I2C_Slave_Init(uint8_t address) {
    // Cargar la dirección en el registro TWAR (los 7 bits superiores)
    TWAR = (address << 1); 
    
    // Configurar TWI: 
    // TWEN: Habilitar TWI
    // TWEA: Habilitar Acknowledge (responder "sí" cuando el maestro nos llame)
    // TWINT: Limpiar bandera de interrupción
    // TWIE: Habilitar Interrupción de TWI
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT) | (1<<TWIE);
    
    // Configurar pines como salida
    DDRB |= (1<<PIN_LED);
    DDRD |= (1<<PIN_MOTOR) | (1<<PIN_BUZZER);
    
    // Estado inicial: Todo apagado
    PORTB &= ~(1<<PIN_LED);
    PORTD &= ~((1<<PIN_MOTOR) | (1<<PIN_BUZZER));
}

int main(void) {
    // Inicializar I2C
    I2C_Slave_Init(SLAVE_ADDR); 
    
    // Habilitar interrupciones globales
    sei(); 

    while(1) {
        // El bucle principal revisa si hay tareas pendientes que no queremos
        // hacer dentro de la interrupción (como delays largos).
        
        if (flag_buzzer) {
            // Hacer sonar el buzzer
            PORTD |= (1<<PIN_BUZZER);
            _delay_ms(100); // Duración del beep
            PORTD &= ~(1<<PIN_BUZZER);
            
            flag_buzzer = 0; // Limpiar bandera
        }
    }
}

// --- RUTINA DE INTERRUPCIÓN (Se ejecuta cuando llega un dato I2C) ---
ISR(TWI_vect) {
    // Leer el estado del bus I2C (enmascarando los bits de prescaler)
    uint8_t status = TWSR & 0xF8;
    
    // Caso 0x80: Dato recibido anteriormente, ACK ha sido devuelto
    // Caso 0x82: Dato recibido (llamada general), ACK devuelto
    if (status == 0x80 || status == 0x82) {
        
        comando_recibido = TWDR; // Leer el dato que llegó
        
        // Procesar el comando inmediatamente
        switch(comando_recibido) {
            case 'A': // COMANDO: ALERTA ON (Humedad alta)
                PORTB |= (1<<PIN_LED);
                break;
                
            case 'B': // COMANDO: ALERTA OFF
                PORTB &= ~(1<<PIN_LED);
                break;
                
            case 'C': // COMANDO: MOTOR ON
                PORTD |= (1<<PIN_MOTOR);
                break;
                
            case 'D': // COMANDO: MOTOR OFF
                PORTD &= ~(1<<PIN_MOTOR);
                break;
                
            case 'E': // COMANDO: BUZZER
                // No usamos delay aquí para no bloquear el bus I2C.
                // Levantamos una bandera y el main se encarga de sonar.
                flag_buzzer = 1; 
                break;
        }
    }
    
    // Al final, limpiamos la bandera de interrupción (TWINT) y 
    // volvemos a habilitar el ACK (TWEA) para seguir escuchando.
    TWCR |= (1<<TWINT) | (1<<TWEA);
}