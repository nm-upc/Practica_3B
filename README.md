# ESP32-S3 BLE Serial Bridge

Este proyecto implementa un **puente de comunicación serie usando Bluetooth Low Energy (BLE)** en una placa **ESP32-S3**.  
Permite enviar datos desde el **Serial Monitor del ordenador al móvil** y recibir datos desde el **móvil hacia el monitor serie** mediante BLE.

El ESP32 actúa como **servidor BLE**, creando un servicio con dos características:

- **TX** → envía datos al dispositivo conectado
- **RX** → recibe datos desde el dispositivo conectado

Esto permite utilizar el ESP32 como un **terminal serie inalámbrico BLE**.

---

# Características

- Comunicación **Bluetooth Low Energy (BLE)**
- Compatible con **ESP32-S3**
- Comunicación **bidireccional**
- Uso de **callbacks BLE**
- Notificaciones BLE para enviar datos en tiempo real
- Reconexión automática cuando el dispositivo se desconecta

---

# Hardware necesario

- ESP32-S3 DevKit
- Cable USB
- Smartphone con aplicación BLE

Aplicaciones recomendadas:

- **nRF Connect**
- **Serial Bluetooth Terminal**

---

# Librerías utilizadas

El proyecto utiliza las librerías BLE incluidas en el framework de ESP32:

```cpp
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
