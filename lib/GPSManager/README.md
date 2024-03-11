# GPS Manager

## GPS en proyectos de IoT


Esta plantilla te ofrece una base sólida para integrar la funcionalidad GPS en tus proyectos de IoT. Se basa en la biblioteca TinyGPS++ para decodificar las frases NMEA de un módulo GPS conectado mediante comunicación serial.

### 1. Requisitos
* TinyGPS++: Biblioteca para manejo de la información GPS https://github.com/mikalhart/TinyGPS++
* HardwareSerial: Biblioteca para la comunicación serial con el modulo GPS.


### 2. Conexión del módulo GPS

La conexión del módulo GPS a tu placa Arduino dependerá del modelo específico que estés utilizando. Sin embargo, la mayoría de los módulos GPS comparten conexiones comunes:

* Vcc: Alimentación (5V)
* GND: Tierra
* Tx: Transmisión serial
* Rx: Recepción serial


## GPS Manager

### Parámetros:

* rxPin: Pin de recepción serial del módulo GPS.
* txPin: Pin de transmisión serial del módulo GPS.
* useMock (opcional): Indica si se debe utilizar un simulador de GPS (por defecto, false).
 
 
### Métodos:

* `begin()`: Inicia la comunicación con el módulo GPS.
* `loop()`: Procesa los datos GPS de forma continua.
* `logGPS()`: Registra la información GPS en un formato específico (a definir).
* `restartDistanceTraveled()`: Reinicia la distancia recorrida acumulada.
* `restart()`: Reinicia las variables internas del GPS.
* `getDistanceTraveled()`: Obtiene la distancia total recorrida.
* `isUpdated()`: Indica si se han recibido nuevos datos GPS.

### Atributos

* `GPSBaud`: Constante que define la velocidad en baudios por defecto del GPS (9600).
Funciones Privadas

* `calculate()`: Realiza cálculos internos relacionados con la información GPS.
* `calculateSatellites()`: Actualiza la cantidad de satélites visibles.
* `calculateHdop()`: Actualiza el valor de HDOP (Dilución de Precisión Horizontal).
* `calculateSpeed()`: Calcula la velocidad en base a los datos GPS.
* `isMoving()`: Determina si el dispositivo está en movimiento (basado en la velocidad).

### Variables:

* `ss`: Objeto HardwareSerial para la comunicación con el módulo GPS.
* `useMock`: Indica si se está utilizando un simulador de GPS.
* `initialized`: Indica si el GPS ha sido inicializado.
* `lastLatitud, lastLongitud`: Coordenadas GPS anteriores.
* `satellites`: Número de satélites en la vista actual.
* `speedKpmh`: Velocidad actual en kilómetros por hora.
* `hdop`: Valor actual de HDOP.
* `distanceTraveled`: Distancia total recorrida.
* `rxPin, txPin`: Pines de recepción y transmisión para el módulo GPS.
* `updated`: Indica si se han recibido nuevos datos GPS.
* `gps`: Objeto TinyGPS++ para analizar los datos GPS.
* `hdopWeightFactor, satelliteWeightFactor,  hdopThreshold, minimumSatellites`: Parámetros para evaluar la calidad de la señal GPS.
* `minimumSpeedKmph, maximumSpeedKmph`: Rango de velocidad para detectar movimiento.

## Código de tu proyecto

Cada proyecto es un mundo, ten la libertad de agregar y quitar código (si es necesario), crear nuevas funciones y algoritmos a partir de este template. Aquí un resumen:

### Componentes Clave:

* `Constructor (GPSManager(int8_t rxPin, int8_t txPin, bool useMock))`:
Inicializa la clase, especificando los pines seriales y un indicador useMock opcional para simular datos GPS.
* `begin():` Inicia la comunicación serial con el módulo GPS.
* `loop():` Procesa continuamente los datos GPS, calculando satélites, HDOP, velocidad y distancia.
* `logGPS():` Registra la información GPS (implementa tu formato de registro).
* `restart():` Reinicia los valores relacionados con el GPS.
* `restartDistanceTraveled():` Reinicia la distancia total recorrida acumulada.
* `getDistanceTraveled():` Obtiene la distancia total recorrida calculada.
* `isUpdated():` Indica si se han recibido nuevos datos GPS.
### Metodos Auxiliares.
* `calculateSatellites():` Actualiza la cantidad de satélites visibles.
* `calculateHdop():` Actualiza el valor HDOP (Dilución de Precisión Horizontal).
* `calculateSpeed():` Calcula la velocidad actual en base a los datos GPS.
* `isMoving():` Determina si el dispositivo está en movimiento según los umbrales de velocidad.
* `calculate():` Realiza cálculos internos para la distancia y las actualizaciones según la validez del GPS y las condiciones de movimiento.
## Consideraciones

* Implementar la función logGPS() para registrar la información GPS en el formato deseado.
Especificar el formato de salida de la función logGPS().
* Ajustar los parámetros de calidad de la señal GPS (hdopWeightFactor, satelliteWeightFactor, hdopThreshold, minimumSatellites) según las necesidades del proyecto.
* Adaptar el rango de velocidad para detectar movimiento (minimumSpeedKmph, maximumSpeedKmph) a la aplicación específica, si es que es una aplicación que detecte movimiento
* Considera el manejo de errores y la resolución de problemas para posibles fallos del GPS.
* Explora las funciones avanzadas de TinyGPS++ para necesidades específicas de extracción de datos.
* Adapta la plantilla al uso específico del GPS y a los requisitos de datos de tu proyecto.


## Ejemplo 
```cpp
#include "GPSManager.h"

void setup() {
  Serial.begin(9600);
  GPSManager gpsManager(2, 3);
  gpsManager.begin();
}

void loop() {
  gpsManager.loop();
  if (gpsManager.isUpdated()) {
    // Procesar la información GPS actualizada
  }
}
```

## Glosario

###  Distancia entre coordinadas.
Es la distancia que hay entre dos puntos geográficos, por medio de latitud y longitud. 

### HDOP
1. [geodesy] Acrónimo de Horizontal Dilution of Precision (dilución de la precisión horizontal). Una medida de la calidad geométrica de la configuración del satélite GPS en el cielo. HDOP es un factor a la hora de determinar la precisión relativa de una posición horizontal. Cuanto más pequeño es el número de DOP, mejor es la geometría.

El HDOP (Dilución de Precisión Horizontal) puede estar en un rango de valores entre 0 y ∞. Sin embargo, en la práctica, los valores de HDOP normalmente oscilan entre 0.5 y 4.
- Un HDOP de 0 indicaría una precisión perfecta, lo que significa que la posición horizontal se conoce con una precisión absoluta.
- Un HDOP de 1 se considera un valor muy bueno, lo que indica una precisión horizontal muy alta.
- Un HDOP de 2 se considera un valor bueno, lo que indica una precisión horizontal alta.
- Un HDOP de 4 se considera un valor límite, lo que indica una precisión horizontal aceptable.
- Un HDOP superior a 4 se considera un valor malo, lo que indica una precisión horizontal baja.

Valores típicos de HDOP:
- 1-2: Excelente precisión.
- 2-4: Buena precisión.
- 4-8: Precisión moderada.
- 8-16: Baja precisión.
- mayor que 16: Precisión muy baja.

### Número de Satélites
Una muestra de GPS puede tomar en cuenta uno o varios satélites, por lo que una muestra puede ser más fidedigna cuando tiene más satelites.


## Schematic Circuit

El siguiente circuito representa lo que sería el modulo GPS, el cual sera necesario para cuando se requiera diseñar un esquematico completo del proyecto IoT.

![Schematic](pcb/GPS-ATGM336H_schematic.png?raw=true "Schematic")

[Schematic](GPS-ATGM336H_schematic.pdf)
