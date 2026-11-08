# 📡 TPI 02 - Supervisión Ambiental con ESP32 e IoT

Este proyecto es un sistema embebido basado en **ESP32** para la supervisión ambiental y actuación remota. Desarrollado en **PlatformIO** y simulado en **Wokwi**, el sistema integra adquisición de datos locales, reportes periódicos en la nube (**ThingSpeak**) y un bot interactivo de **Telegram** para control y recepción de alertas automáticas.

---

## 🛠️ Hardware y Pinout

El proyecto está diseñado para ejecutarse tanto en el simulador Wokwi como en la placa de evaluación UTN FRC.

| Componente | Pin ESP32 | Descripción |
| :--- | :--- | :--- |
| **DHT22** | `GPIO 33` | Sensor de Temperatura y Humedad Relativa. |
| **Potenciómetro** | `GPIO 32` (ADC) | Simula un sensor analógico (Rango de 0 a 3.3V). |
| **LED Verde** | `GPIO 23` | Actuador principal / Relé temporizado. |
| **LED Azul** | `GPIO 2` | Indicador de estado de conexión Wi-Fi. |
| **OLED SSD1306**| `GPIO 21` (SDA), `GPIO 22` (SCL) | Display I2C (Dirección `0x3C`). |

---

## 🚀 Características Principales

*   **Multitarea Real:** La comunicación web (ThingSpeak y Telegram) corre en una tarea de FreeRTOS aislada en el Core 0, permitiendo que la lectura de sensores y el control de hardware en el `loop()` no se bloqueen jamás.
*   **Temporización Segura:** El apagado del actuador se gestiona por hardware mediante comparaciones lógicas (`millis()`), sin el uso de retardos bloqueantes.
*   **Alertas Autónomas:** El sistema evalúa constantemente el voltaje del ADC. Si supera el umbral crítico de **2.50V**, dispara una alerta automática directa al chat de Telegram del administrador, gestionando la histéresis para evitar repeticiones innecesarias (rearme en **2.30V**).
*   **Manejo de Fallos:** Si el sensor DHT22 falla o se desconecta, el sistema retiene la última lectura válida, informa la antigüedad del dato y evita subir lecturas erróneas a la nube.

---

## ⚙️ Configuración e Instalación

### 1. Clonar el repositorio y abrir en VS Code
Asegúrese de tener instalada la extensión **PlatformIO IDE** en Visual Studio Code.

### 2. Configurar las Credenciales (¡Importante!)
Por seguridad, las claves de red y de las APIs **no se suben al repositorio**.
1. En la carpeta `include/`, haga una copia del archivo `secrets.example.h` y renómbrela a `secrets.h`.
2. Edite `include/secrets.h` con sus datos reales:

```cpp
#pragma once
#define WIFI_SSID "TU_WIFI_AQUI" // Usar "Wokwi-GUEST" para simulador
#define WIFI_PASSWORD "TU_CLAVE" // Dejar vacío "" para Wokwi
#define BOT_TOKEN "TOKEN_DE_BOTFATHER"
#define ALLOWED_CHAT_ID "TU_ID_NUMERICO_DE_TELEGRAM"
#define THINGSPEAK_WRITE_KEY "TU_WRITE_API_KEY"
```

### 3. Ejecución
*   **En Simulador:** Ejecutar el comando `Wokwi: Start Simulator` desde la paleta de comandos (`F1`).
*   **En Placa Física:** Conectar la placa por USB y ejecutar `pio run -t upload` en la terminal de PlatformIO.

---

## 📱 Comandos del Bot de Telegram

El bot solo responderá al usuario definido en `ALLOWED_CHAT_ID`.

*   `/start` - Despliega el menú interactivo con botones (Teclado Inline).
*   `/clima` - Devuelve la temperatura y humedad actual, indicando la antigüedad del dato.
*   `/releon <segundos>` - Enciende el LED verde/actuador durante el tiempo especificado (1 a 3600 segundos) y lo apaga de forma automática.
*   `/releoff` - Apaga inmediatamente el actuador, cancelando cualquier temporización activa.
*   `/thingup` - Encola el valor actual del potenciómetro para ser enviado a ThingSpeak (Field 3) respetando los tiempos de la API.
*   `/display<cmd>` - Muestra información en la pantalla OLED. Ejemplos válidos:
    *   `/displayled` (Estado del actuador)
    *   `/displaypot` (Voltaje actual)
    *   `/displayclima` o `/displaydht22` (Temperatura y Humedad)

---

## 📂 Estructura del Código

El código está modularizado usando librerías propias en la carpeta `lib/` para facilitar el mantenimiento y la evaluación:

*   `lib/Control/`: Lógica de tiempo, temporizadores de relé, y alarmas con histéresis.
*   `lib/Device/`: Abstracción del hardware local (Sensores, Pines, Pantalla OLED).
*   `lib/Cloud/`: Cliente HTTP y gestión de reconexión Wi-Fi hacia ThingSpeak.
*   `lib/Bot/`: Cliente seguro de Telegram y parseo de comandos entrantes.

---

## 👥 Equipo de Trabajo

*   Integrante 1 - Legajo
*   Integrante 2 - Legajo
*   Integrante 3 - Legajo
*   Integrante 4 - Legajo
*   Integrante 5 - Legajo
*   Integrante 6 - Legajo

*(Completar con los datos reales del grupo antes de la entrega)*