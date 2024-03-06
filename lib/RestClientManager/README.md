# REST Client Manager

## Conexión a una API REST en proyectos de IoT
El presente gestor de cliente REST para Arduino te permite interactuar con APIs REST de forma fácil y eficiente en tus proyectos de IoT. Te facilita el envío y recepción de datos hacia y desde la nube o cualquier otro dispositivo conectado.

## Con esta librería podrás:

* Realizar peticiones HTTP GET, POST, PUT y PATCH a APIs REST.
Agregar headers personalizados a tus peticiones.
* Manejar diferentes códigos de estado HTTP.
* Obtener el cuerpo de la respuesta en formato JSON o XML.
* Conectarte a una amplia variedad de APIs REST.

## ¿Para qué usar esta librería en tus proyectos de IoT?

* Controlar dispositivos remotamente: Puedes enviar comandos a dispositivos conectados a la red, como encender o apagar luces, abrir o cerrar puertas, etc.
* Monitorizar datos de sensores: Puedes recopilar datos de sensores en tiempo real y enviarlos a una API para su análisis y almacenamiento.
* Almacenar datos en la nube: Puedes enviar datos a una base de datos en la nube para su posterior procesamiento y visualización.
* Interactuar con servicios web: Puedes consumir APIs REST de terceros para obtener información o realizar acciones, como enviar notificaciones por correo electrónico o SMS.
## Librerías utilizadas

* <Arduino.h>: Proporciona funcionalidades básicas para la programación de Arduino.
"Development.h": Probablemente es un archivo de encabezado personalizado que contiene definiciones relevantes para el desarrollo de su proyecto.
* <WiFi.h>: Proporciona funciones para la comunicación WiFi en Arduino.
* <HTTPClient.h>: Ofrece funcionalidades para realizar peticiones HTTP utilizando Arduino.
## Enum

`HttpMethod`: Esta enumeración define los diferentes métodos HTTP admitidos por la clase: `GET`, `POST`, `PUT`, `PATCH` y `DELETE`.

## Definiciones de Clase

`HttpResponse`: Esta clase representa la respuesta recibida de una llamada a la API REST. Tiene dos miembros públicos:

`statusCode`: Un entero que representa el código de estado HTTP de la respuesta (ej., 200 para éxito, 404 para no encontrado).
body: Una cadena que contiene el cuerpo de la respuesta recibida de la API.

`RestClientManager`: Esta es la clase principal responsable de administrar las solicitudes de la API REST. Tiene varios miembros públicos y privados:

## Miembros públicos:
* Constructores: La clase tiene dos constructores, uno que toma una referencia a un objeto WiFiClient y otro que toma referencias a WiFiClient y HTTPClient.
* setWifiClient: Este método establece el objeto WiFiClient utilizado para la comunicación.
* getWifiClient: Este método devuelve una referencia al objeto WiFiClient establecido actualmente.
* addHeader: Este método agrega un par nombre-valor de encabezado a la solicitud HTTP.
* request: Este es el método más importante. Toma la URL, el método HTTP y los datos opcionales como argumentos. Luego usa el objeto HTTPClient proporcionado para realizar la solicitud según el método elegido y devuelve un objeto HttpResponse que contiene el código de estado de la respuesta y el cuerpo.

## Miembros privados:
* client: Un puntero al objeto WiFiClient utilizado para la comunicación.
* http: Un puntero al objeto HTTPClient utilizado para realizar solicitudes HTTP.

## Explicación del método request

Este método es la funcionalidad central de la clase. Toma los siguientes argumentos:

* url: La URL del punto final de la API REST.
method: El método HTTP (GET, POST, PUT, PATCH) que se utilizará para la solicitud.
* data (opcional): Los datos que se enviarán con la solicitud en caso de los métodos POST, PUT o PATCH.

El método primero inicia la solicitud HTTP utilizando la url y el objeto client proporcionados. Luego, usa una instrucción switch para manejar diferentes métodos HTTP:

Para cada método compatible (GET, POST, PUT, PATCH), llama al método correspondiente (GET(), POST(), PUT(), o PATCH()) en el objeto http con cualquier data proporcionado.
Si el método es DELETE, registra un mensaje que indica que el método DELETE no es compatible con la librería HTTPClient.
Después de realizar la solicitud, el código verifica el código de estado de la respuesta.

Si el código de estado es positivo, lee el cuerpo de la respuesta usando getString() y devuelve un objeto HttpResponse con el código de estado y el cuerpo.
Si el código de estado es negativo, registra un mensaje de error con el código recibido y devuelve un objeto HttpResponse con un código de estado de 500 (error interno del servidor) y un cuerpo vacío.

En general, este fragmento de código proporciona un marco básico para realizar solicitudes a una API REST utilizando Arduino y manejar las respuestas. Demuestra funcionalidades esenciales como establecer encabezados, elegir métodos HTTP, manejar diferentes códigos de estado y devolver información de respuesta.