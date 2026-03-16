#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pRxCharacteristic;
bool deviceConnected = false;

// Callbacks para eventos de conexión
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Dispositivo conectado por BLE");
    }
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Dispositivo desconectado");
      pServer->getAdvertising()->start();
    }
};

// Callbacks para recibir datos
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("Iniciando BLE Serial Bridge en ESP32-S3");
  
  // Inicializar BLE
  BLEDevice::init("ESP32-S3-Bridge");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Crear servicio
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Característica TX (envía datos al teléfono)
  pTxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pTxCharacteristic->addDescriptor(new BLE2902());
  
  // Característica RX (recibe datos del teléfono)
  pRxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  
  // Iniciar servicio
  pService->start();
  
  // Configurar y empezar advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  
  Serial.println("Esperando conexión BLE...");
  Serial.println("Nombre: ESP32-S3-Bridge");
}

void loop() {
  // Enviar datos del Serial Monitor al BLE
  if (Serial.available() && deviceConnected) {
    char c = Serial.read();
    char str[2] = {c, '\0'};
    pTxCharacteristic->setValue((uint8_t*)str, 1);
    pTxCharacteristic->notify();
  }
  
  delay(20);
}