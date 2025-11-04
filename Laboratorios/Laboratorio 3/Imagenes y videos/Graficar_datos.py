import pandas as pd
import matplotlib.pyplot as plt

# === Cargar los datos del archivo CSV ===
archivo = "datos_motor.csv"  # debe estar en el mismo directorio
df = pd.read_csv(archivo)

# === Mostrar las primeras filas para verificar ===
print(df.head())

# === Crear la figura ===
plt.figure(figsize=(10, 6))
plt.plot(df["Pot1"], label="Potenciómetro Referencia (Pot1)", linewidth=2)
plt.plot(df["Pot2"], label="Potenciómetro Motor (Pot2)", linewidth=2)
plt.plot(df["PWM"] * 4, label="PWM (Escalado x4)", linestyle="--", linewidth=1.5)

plt.xlabel("Muestras")
plt.ylabel("Valor ADC / PWM")
plt.title("Control de Motor - Potenciómetros y PWM")
plt.legend(loc="upper right")
plt.grid(True, alpha=0.3)

# === Guardar y mostrar ===
plt.tight_layout()
plt.savefig("grafico_motor.png", dpi=300)
plt.show()

print("✅ Gráfico guardado como 'grafico_motor.png'")
