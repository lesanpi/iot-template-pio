# Arquitectura de Desarrollo.

Esta arquitectura se basa en una estructura modular con diferentes capas y componentes que interactúan entre sí. Esta estructura permite dividir la funcionalidad del sistema en partes más pequeñas y manejables, mejorando la organización, legibilidad y mantenibilidad del código y del hardware.

## Componentes principales:

* Managers: Clases que encapsulan la lógica de negocio y la interacción con dispositivos específicos o recursos del sistema. Cada "Manager" se encarga de una funcionalidad específica, proporcionando una interfaz sencilla para el resto del sistema.
* Use Cases: Casos de uso que definen las diferentes funcionalidades que el sistema puede ofrecer a los usuarios. Cada caso de uso describe una secuencia de pasos que se deben seguir para completar una tarea específica.
* Capa de Interfaz: Se encarga de la interacción con el usuario, mostrando información y recibiendo comandos a través de interfaces físicas como LEDs, botones, pantallas, etc. Realmente esta es la capa que menos control tenemos ya que son componentes electrónicos y/o circuitos.

## Ventajas:

* Modularidad: Facilita la organización y el mantenimiento del código y del hardware al dividirlo en módulos independientes.
* Reutilización: Los "Managers" y casos de uso pueden ser reutilizados en diferentes proyectos o componentes del sistema.
* Encapsulamiento: Oculta la complejidad interna de los "Managers" y casos de uso, simplificando el código.
* Escalabilidad: Permite añadir nuevas funcionalidades de forma sencilla al crear nuevos "Managers" y casos de uso.
* Testabilidad: Facilita la creación de pruebas unitarias para cada "Manager" y caso de uso.


# Managers 

Los "Managers" en la arquitectura son clases que encapsulan la lógica de negocio y la interacción con dispositivos específicos o recursos del sistema. Cada "Manager" se encarga de una funcionalidad específica, proporcionando una interfaz sencilla para el resto del sistema.

## Beneficios:

* Modularidad: Permiten dividir el código en módulos independientes, mejorando la organización y legibilidad.
* Reutilización: Los "Managers" pueden ser reutilizados en diferentes proyectos o componentes del sistema.
* Encapsulamiento: Ocultan la complejidad interna de la interacción con dispositivos o recursos, simplificando el código.
* Mantenimiento: Facilitan la detección y corrección de errores al tener una responsabilidad acotada.

## Managers en tu código:

* OutputManager: Controla 3 LEDs, actualizando su estado según el estado del dispositivo.
* MemoryManager: Gestiona la memoria persistente (EEPROM) para almacenar y recuperar información clave como la distancia total recorrida e identificador del vehículo.
* InputManager: Detecta pulsaciones largas en un botón de reinicio, notificando al sistema la necesidad de reiniciar.
* GPSManager: Se encarga de la comunicación con el módulo GPS, calculando la distancia recorrida y gestionando actualizaciones de posición.
* BLEManager: Establece un servicio BLE, transmitiendo los kilómetros recorridos y el identificador del vehículo, permitiendo la comunicación con dispositivos externos.

## Interacción entre Managers:

Los "`Managers`" pueden interactuar entre sí para compartir información o realizar tareas conjuntas.

### Ejemplo:

El `GPSManager` puede notificar al `MemoryManager` la distancia recorrida para actualizar la información almacenada.
El `OutputManager` puede mostrar diferentes patrones de iluminación en los LEDs según el estado del dispositivo, información proporcionada por el `DeviceState`.

Para poder interactuar entre ello utilizamos los `UseCases`

# Use Cases / Casos de Uso

## ¿Qué son?

Los casos de uso son descripciones de cómo los usuarios interactúan con el dispositivo para lograr objetivos específicos. Son una herramienta esencial para comprender las necesidades del usuario y para asegurar que el dispositivo se desarrolle de manera que las satisfaga y ejecute todas las acciones que involucran ese objetivo.

## ¿Por qué son importantes?

Los casos de uso son importantes porque:

* Mejoran la comunicación entre los diferentes módulos que trabajan en el desarrollo del dispositivo IoT.
* Ayudan a asegurar que el desarrollo sea pensando en el usuario y los casos que se aplican.
* Facilitan la planificación y el desarrollo de las funcionalidades.
* Permiten una mejor gestión de los requisitos.
* Sirven como base para las pruebas.

## ¿Como definimos un Use Case?

### 1. Definir una tarea, funcionalidad u objetivo.

Por ejemplo: Configurar el dispositivo.

### 2. Dado esta funcionalidad, definir que manager o módulos estaran involucrados y por que. Partiendo del ejemplo anterior "Configurar el disposito".

* ¿Como sabemos que no esta configurado? Por qué no encontramos datos en la memoría del dispositivo o datos vacios.
* ¿Si queremos implementar un botón de restaurar de fábrica, que involucra? Esto involucra el manager encargado de las entradas del dispositivo (en este caso el botón de reset) y el manager de memoría. Una vez que se deje presionado este botón por 5 segundos, los datos en memoría deben ser limpiados.
* ¿Si recibo una escritura vía bluetooth, que debería ocurrir? Si la escritura me indica que el dispositivo tiene un nuevo ID, debo borrar los datos guardados en memoría. Si indica que ese nuevo ID es un String vacio, implica que ya no estoy configurado, ya que no tengo ID, por lo que debería limpiar memoría y volver a mi estado de no configurado.

### 3. Desarrollar funciones que integren los diferentes managers.

El UseCase debe implementar las funciones que integren los diferentes managers según sea el objetivo. En el caso de ConfigurationUseCase, para la función de botón de reset sería algo así:

```cpp
void ConfigurationUseCase::loop()
{
    inputManager->loop();
    if (inputManager->mustRestartDevice())
    {
        log("🚨 Restarting to factory configuration", "ConfigurationUseCase.mustRestartDevice()");
        /// Clean memory
        memoryManager->writeVehicleID("");
        memoryManager->writeDistance(0);
        /// Restart distance traveled counter
        gpsManager->restartDistanceTraveled();
        /// Update characteristic data
        bleManager->updateKilometers(0);
        bleManager->updateVehicleId("");
        /// Restart reset button
        inputManager->restarted();
        /// Change output state
        this->outputManager->setState(DeviceState::NotConfigured);
    }
}

```

En este caso cuando el `InputManager` detecte que el botón de reset fue presionado por más de n segundos y debe ejecutarse el reseteo, se ejecutaran las siguientes acciones:
* Se debe limpiar los datos en memoría, a través del `MemoryManager`
* El `GPSManager` debe limpiar los kilometros recorridos, ya que tenemos un dispositivo sin configurarse.
* Se debe encender la luz que indique que el dispositivo no esta configurado a través del `OutputManager`
* Se debe actualizar la información que se transmite vía Bluetooth LE, a través del `BLEManager`.
* Se debe indicar al `InputManager` que la acción de reseteo ya se ejecuto, por lo que reiniciara el flag que indica que se presiono el botón de reset.


# Glosario

## Modo configuración

El modo configuración es un estado en el que definí como "En espera a ser configurado por el usuario", básicamente el dispositivo espera todos los datos iniciales para poder empezar a funcionar.

En el archivo `DeviceState.h`, definí los siguientes estados:

```cpp
enum class DeviceState
{
    NotConfigured,
    Running,
    Updating
};
```

#### ¿En que refleja esto en el código?
Básicamente se refleja en la NO ejecutación de funcionalides. Aquí un ejemplo en donde en el caso de uso se válida si está configurado el equipo.

```cpp
    bool isConfigured = memoryManager->isConfigured();
    if (isConfigured)
        gpsManager->loop();
    if (gpsManager->isUpdated() && isConfigured)
    {
        /// GPS Distance traveled counter
        double distanceTraveledUpdated = gpsManager->getDistanceTraveled();
        log("🚗 Distance traveled... " + String(distanceTraveledUpdated) + " km", "MileageMeterUseCase.loop()");

        /// Memory distance traveled
        double currentTotalDistanceTraveled = memoryManager->getTotalDistanceTraveled();
        double totalDistanceTraveledUpdated = currentTotalDistanceTraveled + distanceTraveledUpdated;
        log("🛣️ Total distance traveled... " + String(totalDistanceTraveledUpdated) + " km", "MileageMeterUseCase.loop()");

        /// Restart GPS Manager counter
        gpsManager->restartDistanceTraveled();
        /// Update memory distance distanceTraveled
        memoryManager->writeDistance(totalDistanceTraveledUpdated);
        /// Send data via BLE
        bleManager->updateKilometers(totalDistanceTraveledUpdated);
    }

```

En este caso de ejemplo, el equipo no continua midiendo la distancia recorrida a través del módulo GPS, ya que no se encuentra configurado.



