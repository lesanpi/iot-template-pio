# Use Cases / Casos de Uso

## ¿Qué son?

Los casos de uso son descripciones de cómo los usuarios interactúan con el software para lograr objetivos específicos. Son una herramienta esencial para comprender las necesidades del usuario y para asegurar que el software se desarrolle de manera que las satisfaga.

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
