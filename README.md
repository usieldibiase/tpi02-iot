# TPI 02 - Supervision ambiental ESP32
Proyecto PlatformIO + Wokwi. Fuente principal: src/main.ino.
Copiar include/secrets.example.h a include/secrets.h y completar datos.
Compilar: pio run. Simular: Wokwi: Start Simulator.
Placa fisica: configurar Wi-Fi de 2,4 GHz; pio run -t upload.
GPIO: DHT22 33, potenciometro 32, LED verde/actuador 23, azul 2,
OLED SDA 21 y SCL 22, SSD1306 128x64 direccion 0x3C.
Decisiones: LED verde representa el actuador principal. Confirmar con catedra.
Si exigen rele fisico, confirmar pin, polaridad y conexion antes de adaptar.
Umbral elegido: 2,50 V; rearme por debajo de 2,30 V.
Voltaje: estimacion didactica ADC * 3,3 / 4095, no instrumento calibrado.
La pantalla y placa UTN deben verificarse fisicamente antes de entregar.
Integrantes: COMPLETAR antes del primer commit.
