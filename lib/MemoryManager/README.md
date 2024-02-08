
# Memory Manager

## Memoria EEPROM en proyectos de IoT

La memoria EEPROM (Electrically Erasable Programmable Read-Only Memory) es un tipo de memoria no volátil que permite almacenar datos de forma permanente en dispositivos IoT, incluso cuando se desconectan de la alimentación. Esto la convierte en una herramienta valiosa para proyectos que requieren preservar información como configuraciones, datos de usuario o registros de eventos.

## Introducción

### 1. Requisitos
* ArduinoJson: Biblioteca para trabajar con datos en formato JSON.
* StreamUtils: Biblioteca que permite crear streams a partir de la EEPROM.


### 2. Importación
```cpp
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <StreamUtils.h>

```
### 3. Lectura de un valor específico

```cpp
return EEPROM.readDouble(EEPROM_ADDR_DOUBLE);
return EEPROM.readString(EEPROM_ADDR_STRING);
```

### 4. Escritura de un valor específico
```cpp
EEPROM.writeString(getAddrOfVehicle(), vehicleID);
EEPROM.commit();
```

### 5. Lectura JSON

El doc es inicializado con el tamaño máximo del JSON, en este caso 1024.


```cpp
DynamicJsonDocument doc(1024);
JsonObject json;
EepromStream eepromStream(0, 1024);
deserializeJson(doc, eepromStream);
json = doc.as<JsonObject>();
name = String(json["name"]);
```
### 6. Escritura JSON
El doc es inicializado con el tamaño máximo de capacidad del JSON.

El EepromStream se define desde la dirección inicial de memoria hasta que la final. En este caso desde la dirección 0, hasta la 1024.

```cpp
DynamicJsonDocument doc(capacity); 
doc["id"] = "my-special-id";
EepromStream eepromStream(0, 1024);
serializeJson(doc, eepromStream);
EEPROM.commit();
```

## ¿Cúal usar?

Sinceramente todo depende del proyecto. Puede que un proyecto requiera guardar pocos datos en memoría, por lo tanto es más óptimo guardar solo dos valores en direcciones específicas.

Por otro lado, si un proyecto requiere guardar muchos campos, es preferible usar json para almacenarlo.

## Memory Manager

Cuando queremos utilizar la memoría del microcontrolador, la mejor forma de tener desarrollado cada una de las funciones es a través de la POO, ya que agruparemos en una sola clase todas las responsabilidades relacionadas al manejo de memoría. A continuación se explica a profundidad que se encarga.

* La función begin o de inicialización: Permitir inicializar el módulo o manager, con los datos que iniciales requeridos para comenzar a funcionar con normalidad. 
* Lectura de los datos en memoría: Puede ser utilizando la estragía con JSON o de direcciones de memoría especificas (solo un campo) o una combinación de ambas.
* Escritura de valores en memoría: Almacenar la información  en un espacio de memoría, puede ser un String, double, int, etc...


## Consideraciones del uso de memoría.
* La memoría EEPROM tiene un tiempo de vida útil que va relacionado a la cantidad de escrituras realizadas sobre ella. Mientrás más seguido hagas escrituras, la memoría se va acordtando.
* Precaución con los espacios de memoría donde se almacena los datos. Ya que si asignamos en la dirección de memoría 0 un campo, y en el espacio de memoría 1 otro campo, hay que asegurar que no ocurra un desbordamiento, en el sentido que el primer campo ocupe mayor espacio de memoría del estipulado.

## Ejemplo Memory Manager.

```cpp
class MemoryManager
{
public:
    MemoryManager(size_t size);
    void begin();
    void writeDistance(double distance);
    double readDistance();
    void writeVehicleID(String vehicleID);
    String readVehicleID();

    String getVehicleId();
    double getTotalDistanceTraveled();
    bool isConfigured()
    {
        String vehicle = getVehicleId();
        return !vehicle.isEmpty();
    }
}
```

Como se puede observar:
* Se definen las funciones de escritura y lectura de cada campo.
* Se define la función begin, al iniciar el manager.
