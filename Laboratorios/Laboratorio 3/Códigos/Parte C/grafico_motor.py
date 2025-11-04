import serial
import matplotlib.pyplot as plt
from collections import deque
import csv
import time

# === CONFIGURACIÓN SERIAL ===
puerto = 'COM6'   # ⚠️ Cambia esto según tu puerto (ej: 'COM4' o '/dev/ttyUSB0')
baudrate = 9600
ventana = 100  # Cantidad de puntos visibles en el gráfico

# === CONEXIÓN SERIAL ===
ser = serial.Serial(puerto, baudrate, timeout=1)
print(f"Conectado a {puerto} a {baudrate} baudios")

# === PREPARAR ARCHIVO CSV ===
archivo_csv = open("datos_motor.csv", "w", newline='')
csv_writer = csv.writer(archivo_csv)
csv_writer.writerow(["Pot1", "Pot2", "PWM", "Direccion"])

# === BUFFERS DE DATOS ===
pot1_data = deque(maxlen=ventana)
pot2_data = deque(maxlen=ventana)
pwm_data  = deque(maxlen=ventana)

# === CONFIGURACIÓN DEL GRÁFICO ===
plt.ion()
fig, ax = plt.subplots()
line1, = ax.plot([], [], label='POT1 (Referencia)', linewidth=2)
line2, = ax.plot([], [], label='POT2 (Motor)', linewidth=2)
line3, = ax.plot([], [], label='PWM (Escalado)', linestyle='--', linewidth=1.5)
ax.set_xlim(0, ventana)
ax.set_ylim(0, 1023)
ax.set_xlabel("Muestras")
ax.set_ylabel("Valor ADC / PWM")
ax.legend(loc="upper right")
plt.title("Control de Motor - Lecturas en Tiempo Real")
fig.canvas.draw()
fig.show()

print("Presiona Ctrl + C para detener y guardar los datos...")

last_update = time.time()
refresh_rate = 0.02  # segundos entre actualizaciones (más bajo = más rápido)

try:
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        if not line:
            continue

        partes = line.split(',')
        if len(partes) < 4:
            continue

        try:
            pot1 = int(partes[0])
            pot2 = int(partes[1])
            pwm  = int(partes[2])
            dir_str = partes[3]
        except ValueError:
            continue

        csv_writer.writerow([pot1, pot2, pwm, dir_str])

        pot1_data.append(pot1)
        pot2_data.append(pot2)
        pwm_data.append(pwm * 4)

        # === Actualizar más rápido ===
        if time.time() - last_update > refresh_rate:
            line1.set_data(range(len(pot1_data)), pot1_data)
            line2.set_data(range(len(pot2_data)), pot2_data)
            line3.set_data(range(len(pwm_data)), pwm_data)
            ax.set_xlim(0, len(pot1_data))

            # Dibujo eficiente (sin redibujar todo)
            ax.draw_artist(ax.patch)
            ax.draw_artist(line1)
            ax.draw_artist(line2)
            ax.draw_artist(line3)
            fig.canvas.blit(ax.bbox)
            fig.canvas.flush_events()

            last_update = time.time()

except KeyboardInterrupt:
    print("\nFinalizando...")

finally:
    ser.close()
    archivo_csv.close()
    print("✅ Archivo guardado como 'datos_motor.csv'")
    plt.ioff()
    plt.savefig("grafico_motor.png", dpi=300)
    print("📸 Gráfico guardado como 'grafico_motor.png'")
    plt.show()
