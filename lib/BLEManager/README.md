# BLE Manager
BLE significa Bluetooth Low Energy (Bluetooth de baja energía). Es una tecnología de red inalámbrica de área personal diseñada y comercializada por el Bluetooth Special Interest Group (Bluetooth SIG) para aplicaciones novedosas en las industrias de la salud, el fitness, las balizas, la seguridad y el entretenimiento doméstico.

 BLE se diferencia del Bluetooth clásico en que está diseñado para consumir mucha menos energía. Esto lo hace ideal para dispositivos que necesitan funcionar con baterías durante largos períodos de tiempo, como sensores, dispositivos portátiles y balizas. 

## Funcionamiento

 En Bluetooth Low Energy (BLE), los UUIDs (Identificadores Únicos Universales) son valores de 128 bits que se utilizan para identificar de forma única diferentes aspectos de un dispositivo BLE, como servicios, características y descripciones.

### 1. UUIDs de Servicio:

Un UUID de servicio identifica un tipo específico de servicio que ofrece un dispositivo BLE. Por ejemplo, un dispositivo que ofrece un servicio de sensor de temperatura podría tener un UUID de servicio específico para ese tipo de servicio.
Los UUIDs de servicio se utilizan durante el proceso de descubrimiento de servicios, en el que un dispositivo BLE busca qué servicios ofrece otro dispositivo BLE.

### 2. UUIDs de Característica:

Un UUID de característica identifica una propiedad específica dentro de un servicio BLE. Por ejemplo, un servicio de sensor de temperatura podría tener una característica para la temperatura actual, otra para la temperatura máxima y otra para la temperatura mínima.
Los UUIDs de característica se utilizan para leer y escribir datos en una característica específica.

### 3. UUIDs de Descripción:

Un UUID de descripción proporciona información adicional sobre una característica BLE. Por ejemplo, una descripción podría proporcionar el nombre de la característica, las unidades de medida utilizadas o el rango de valores válidos.
Los UUIDs de descripción no son esenciales para el funcionamiento de BLE, pero pueden proporcionar información útil para los desarrolladores que están trabajando con dispositivos BLE.

### Ejemplo.

Un dispositivo BLE que ofrece un servicio de sensor de temperatura podría tener los siguientes UUIDs:

* UUID de servicio: 0x0000ffe0-0000-1000-8000-00805f9b34fb
* UUID de característica para la temperatura actual: 0x0000ffe1-0000-1000-8000-00805f9b34fb
* UUID de característica para la temperatura máxima: 0x0000ffe2-0000-1000-8000-00805f9b34fb
* UUID de característica para la temperatura mínima: 0x0000ffe3-0000-1000-8000-00805f9b34fb
* UUID de descripción para la característica de temperatura actual: 0x00002901-0000-1000-8000-00805f9b34fb

## Introducción

### 1. Requisitos
* BLEServer: Biblioteca para crear un servidor de BLE

### 2. Importación
```cpp
#include <BLEServer.h>
#include <Arduino.h>
```

### 3. Constructor
En el constructor se debe pasar por parametro el nombre del dispositivo BLE que se estará publicando y los UUIDs para los servicios, las características y las descripciones (si es requerido)

```cpp
BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *vehicleUUID);
```
### Implmentacion

En la implementación del constructor se deben definir los valores de los UUIDs, crear el servidor, los servicios y las caracteristicas. A continuación un ejemplo de ello.
```cpp
BLEManager::BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *vehicleUUID)
{
    this->serviceUUID = serviceUUID;
    this->kilometersCharacteristicUUID = kilometersUUID;
    this->vehicleCharacteristicUUID = vehicleUUID;
    // Initialize BLE
    BLEDevice::init(deviceName);

    // Create server and service
    pServer = BLEDevice::createServer();
    pService = pServer->createService(this->serviceUUID);

    // Create and add characteristic
    kilometersCharacteristic = pService->createCharacteristic(this->kilometersCharacteristicUUID, BLECharacteristic::PROPERTY_READ);
    vehicleCharacteristic = pService->createCharacteristic(this->vehicleCharacteristicUUID, BLECharacteristic::PROPERTY_READ |
   BLECharacteristic::PROPERTY_WRITE);
}
```

### 4. Begin
En la funcion begin, se definen los callbacks del server, se inician los servicios, se agregan los UUIDS  y se empieza el advertising de los mismos. A continuación un ejemplo de ello.

```cpp
void BLEManager::begin()
{
    // Set server callbacks
    pServer->setCallbacks(new MyServerCallbacks());

    // Start service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(this->serviceUUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}
```
### 5. Server callbacks

Los server callbacks, permiten ejecutar acciones al momento de una conexión o desconexión del servidor. Existen una especie de `"bug"` que hace que el server BLE deje de compartir información al momento que ocurre una conexión, es por ello que siempre se llama a la función `pServer->startAdvertising();`

```cpp
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        pServer->startAdvertising(); // restart advertising
    };

    void onDisconnect(BLEServer *pServer)
    {
        pServer->startAdvertising(); // restart advertising
    }
};
```

## BLE Manager y sus funciones
El BLE Manager debe implementar toda la información y las funciones relacionadas con Bluetooth Low Energy (BLE).

* Debe almacenar los UUIDs del servicio, characteristicas y descripciones BLE
* Debe implementar la funcion begin, para iniciar el manejador de BLE
* Debe implementar funciones que actualicen la información que se transmite via BLE
* Debe tener funciones que definan los callbacks de las características, de forma que se ejecute una función al momento de escritura o lectura de la característica. Ejemplo: `void setVehicleCharacteristicCallback(BLECharacteristicCallbacks *pCallbacks);`



## Ejemplo BLEManager


```cpp
class BLEManager
{
public:
    BLEManager(std::__cxx11::string deviceName, const char *serviceUUID, const char *kilometersUUID, const char *vehicleUUID);
    void begin();
    void updateKilometers(int kilometers);
    void updateVehicleId(String vehicleId);
    void setVehicleCharacteristicCallback(BLECharacteristicCallbacks *pCallbacks);

private:
    const char *serviceUUID;
    const char *kilometersCharacteristicUUID;
    const char *vehicleCharacteristicUUID;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *kilometersCharacteristic;
    BLECharacteristic *vehicleCharacteristic;
};
```

Como se puede ver en el ejemplo: 
* El BLEManager guarda los UUIDs del servicio y las características
* El BLEManager tiene BLEServer, BLEService y BLECharacteristic's
* Implementa la funciones `update` de las características que se transmiten.
* Implementa la función `begin`, para empezar el servicio de BLE.
* Posee la función `setCharacteristicCallback`, que define los callbacks de una característica específica.
