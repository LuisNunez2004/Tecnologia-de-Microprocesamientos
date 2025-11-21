/*
 * MAESTRO: DHT11 (Humedad/Temp) + Potenciómetro + Botón
 * Controla al Esclavo via I2C
 */
#define PCF8574 0x27
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// --- CONFIGURACIÓN I2C LCD ---
#include "twi.h" 
#include "twi_lcd.h" 

// --- CONFIGURACIÓN DHT11 ---
#define DHT_PORT PORTD
#define DHT_DDR  DDRD
#define DHT_PIN  PIND
#define DHT_BIT  PD4    // Nota: Usar PD4 es más estándar que PORTD4 para bit positions

// --- DIRECCIONES Y UMBRALES ---
#define SLAVE_ADDR 0x10 
#define UMBRAL_HUMEDAD 70 // Encender LED si humedad > 70%

// Comandos
#define CMD_ALERTA_ON   'A' 
#define CMD_ALERTA_OFF  'B'
#define CMD_MOTOR_ON    'C'
#define CMD_MOTOR_OFF   'D' 
#define CMD_BUZZER      'E'

uint8_t dht_hum_int, dht_hum_dec, dht_temp_int, dht_temp_dec, dht_checksum;

// --- FUNCIONES DEL DHT11 ---

// Envía señal de inicio al sensor
void dht11_request() {
    DHT_DDR |= (1<<DHT_BIT);   // Pin como Salida
    DHT_PORT &= ~(1<<DHT_BIT); // Pin en BAJO
    _delay_ms(20);             // Esperar 20ms (min 18ms)
    DHT_PORT |= (1<<DHT_BIT);  // Pin en ALTO
    _delay_us(30);             // Esperar un poco
    DHT_DDR &= ~(1<<DHT_BIT);  // Pin como Entrada (dejar que el sensor responda)
}

// Recibe la respuesta del sensor
uint8_t dht11_response() {
    uint8_t respuesta = 0;
    _delay_us(40);
    if (!(DHT_PIN & (1<<DHT_BIT))) { // Si el sensor puso el pin en BAJO
        _delay_us(80);
        if (DHT_PIN & (1<<DHT_BIT)) { // Si el sensor puso el pin en ALTO
            respuesta = 1;
        }
        _delay_us(40); // Esperar a que termine la señal de respuesta
    }
    return respuesta;
}

// Lee un byte de datos
uint8_t dht11_receive_data() {
    uint8_t c = 0;
    for (int q=0; q<8; q++) {
        while(!(DHT_PIN & (1<<DHT_BIT))); // Esperar a que el pin suba (inicio de bit)
        _delay_us(30); // Diferenciar entre 0 y 1: '0' dura ~28us, '1' dura ~70us
        
        if (DHT_PIN & (1<<DHT_BIT)) { // Si sigue alto después de 30us, es un 1
            c = (c<<1)|(0x01); 
        } else {                      // Si bajó, es un 0
            c = (c<<1);
        }
        while(DHT_PIN & (1<<DHT_BIT)); // Esperar a que el pin baje para el prox bit
    }
    return c;
}

// Función principal de lectura
uint8_t leer_dht11() {
    dht11_request(); // Pedir datos
    if (dht11_response()) { // Si el sensor responde
        dht_hum_int = dht11_receive_data();
        dht_hum_dec = dht11_receive_data();
        dht_temp_int = dht11_receive_data();
        dht_temp_dec = dht11_receive_data();
        dht_checksum = dht11_receive_data();
        
        // Verificar integridad de datos
        if ((dht_hum_int + dht_hum_dec + dht_temp_int + dht_temp_dec) == dht_checksum) {
            return 1; // Lectura exitosa
        }
    }
    return 0; // Error
}

// --- FUNCIONES AUXILIARES ---
void adc_init() {
    ADMUX = (1<<REFS0); 
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
}

uint16_t adc_read(uint8_t ch) {
    ch &= 0b00000111;
    ADMUX = (ADMUX & 0xF8) | ch;
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return (ADC);
}

void enviar_comando(uint8_t comando) {
    twi_start();
    twi_write_cmd((SLAVE_ADDR << 1) | 0); 
    twi_write_dwr(comando);
    twi_stop();
}

// --- MAIN ---
int main(void) {
    // <--- ARREGLO 1 (PARTE A): Espera inicial de seguridad
    // Esto permite que el controlador del LCD arranque antes de recibir datos
    _delay_ms(500); 
    // ---------------------------------------------------

    // Botón en D2
    DDRD &= ~(1<<PD2); 
    PORTD |= (1<<PD2); 
    
    adc_init();
    twi_init();      
    twi_lcd_init(); 
    
    // <--- ARREGLO 1 (PARTE B): Limpieza forzada
    twi_lcd_cmd(0x01); // Comando CLEAR DISPLAY explícito
    _delay_ms(20);     // El borrado toma tiempo (>1.53ms), damos 20ms por seguridad
    twi_lcd_cmd(0x0C); // Comando DISPLAY ON, CURSOR OFF explícito
    // ---------------------------------------------------
    
    char buffer[16];
    uint16_t pot_val = 0;
    
    twi_lcd_msg("Iniciando...");
    _delay_ms(1000);
    twi_lcd_clear(); // Aquí ya podemos usar la función normal

    while(1) {
        // 1. LEER DHT11
        leer_dht11(); 
        
        // 2. LEER POTENCIÓMETRO Y BOTÓN
        pot_val = adc_read(0);
        uint8_t boton = !(PIND & (1<<PD2));

        // 3. MOSTRAR EN LCD
        twi_lcd_cmd(0x80);
        // Mostramos Humedad (H) y Temperatura (T)
        sprintf(buffer, "H:%d%% T:%dC      ", dht_hum_int, dht_temp_int);
        twi_lcd_msg(buffer);

        // 4. LÓGICA DE CONTROL (Humedad -> LED)
        if (dht_hum_int >= UMBRAL_HUMEDAD) {
            enviar_comando(CMD_ALERTA_ON);
            twi_lcd_cmd(0xC0);
            twi_lcd_msg("ALERTA: HUMEDAD!");
        } else {
            enviar_comando(CMD_ALERTA_OFF);
            // Limpiar linea si no hay alertas
            if(!boton && pot_val <= 512) {
                twi_lcd_cmd(0xC0);
                sprintf(buffer, "Pot: %d          ", pot_val);
                twi_lcd_msg(buffer);
            }
        }

        // Lógica Potenciómetro -> Motor
        _delay_ms(5); 
        if (pot_val > 512) enviar_comando(CMD_MOTOR_ON);
        else enviar_comando(CMD_MOTOR_OFF);

        // Lógica Botón -> Buzzer
        _delay_ms(5);
        if (boton) enviar_comando(CMD_BUZZER);

        _delay_ms(1000); // El DHT11 es lento, leer cada 1 o 2 segundos máximo
    }
}