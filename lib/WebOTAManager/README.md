# Actualizaciones OTA

Las actualizaciones OTA (Over-the-Air) son un tipo de actualización de software que se realiza de forma remota, sin necesidad de que el dispositivo sea retirado de servicio. Este tipo de actualización es especialmente importante para los dispositivos IoT, que suelen estar distribuidos en grandes cantidades y en lugares remotos.

Las actualizaciones OTA ofrecen una serie de ventajas para los dispositivos IoT, entre las que se incluyen:

* Facilidad de implementación: Las actualizaciones OTA se pueden implementar de forma remota, lo que ahorra tiempo y costes.
* Eficiencia: Las actualizaciones OTA pueden realizarse sin interrumpir el funcionamiento del dispositivo.
* Seguridad: Las actualizaciones OTA pueden utilizarse para corregir vulnerabilidades de seguridad.

Es muy importante las actualizaciones OTA, ya que bajo esta base es como podemos actualizar nuestro código, considerando que ya habrán clientes y usuarios que tengan nuestro dispositivo IoT comprado e instalado.

Imaginemos que una empresa de redes como TP-Link o Cisco, que tiene millones de ventas en routers y switches, pero de repente, encuentra una vulnerabilidad que permite a usuarios no admin cambiar la contraseña. Esto claramente es un fix que hay que realizar para poder cubrir este caso borde. Sin actualizaciones OTA, la vulnerabilidad se quedaría activa dentro de los equipo permanentemente.

## Ejemplos de Actualizaciones OTA
A continuación, se describen algunos ejemplos de cómo se pueden utilizar las actualizaciones OTA para dispositivos IoT:

* Actualización de seguridad: Un fabricante de dispositivos de seguridad puede utilizar las actualizaciones OTA para corregir vulnerabilidades de seguridad que puedan ser explotadas por los piratas informáticos.
* Mejora de las funciones: Un fabricante de dispositivos de automatización del hogar puede utilizar las actualizaciones OTA para añadir nuevas funciones a sus productos, como la compatibilidad con nuevos dispositivos o la integración con nuevos servicios.
* Optimización del rendimiento: Un operador de una red de sensores industriales puede utilizar las actualizaciones OTA para optimizar el rendimiento de los sensores, reduciendo el consumo de energía o mejorando la precisión de las mediciones.
* Nuevas funcionalidades dentro del código: Funcionalidades que no sean sensores extras, circuitos extras, es decir, que NO requieran modificación física del equipo. Tiene que ser funcionalidades que puedan ser manejadas dentro del código del dispositivo.

## Consideraciones para desarrollar OTA

* El código tiene que estar adaptado desde su primera versión para recibir actualizaciones OTA.
* Todas las versiones nuevas, deben estar adaptadas para recibir las siguientes versiones actualizadas. De lo contrario, la versión subida sería la última en mantenerse activa, por lo que no se podría recibir nuevas.
* Debe haber una interfaz o medio por el cual el usuario pueda realizar una actualización. Ejemplo: Via Bluetooth (ejemplo: los reloj inteligentes de Cubitt), Via Web Server del microcontrolador (ejemplo: routers)



# Crear OTA con un Web Server en ESP8266/ESP32

Para poder crear una actualización OTA, primero asegurate haber leido la guía de Web Server React en ESP. Esto es un requisito para poder entender de primera mano como es crear un web server, ya que será la base desde donde partiremos.

## React App

En la aplicación de React debe haber un `form` que haga un POST hacia el endpoint correspondiente para hacer la petición de actualizar

```html
<p>
Update <code>ESP8266 Firmware</code> via OTA
</p>
<form method="POST" action="/update" enctype="multipart/form-data">
    <input type="file" name="update" /><input type="submit" value="Update" />
</form>
```

## Endpoints en el Web Server

Al momento de definir los endpoints del web server del microcontrolador, vamos a poseer un cuarto parametro que nos va a permitir hacer el manejo de archivos. Es aqui donde indicamos `onUploadBinary` que sera una funcion que va a manejar 

```cpp

server.on(
      "/update", HTTP_POST, [](AsyncWebServerRequest *request)
{
    log("POST /update " + request->url(), "Server.update");

request->send(400, "text/plain", "Update"); 
},
onUploadBinary);
```

La función de onUploadBinary será la encargada de manejar los archivos .

```cpp
void onUploadBinary(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.println("Update");
    content_len = request->contentLength();
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;
#ifdef ESP8266
    Update.runAsync(true);
    if (!Update.begin(content_len, cmd))
    {
#else
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
    {
#endif
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
#ifdef ESP8266
  }
  else
  {
    Serial.printf("Progress: %d%%\n", (Update.progress() * 100) / Update.size());
#endif
  }

  if (final)
  {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
    response->addHeader("Refresh", "20");
    response->addHeader("Location", "/");
    request->send(response);
    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      Serial.println("Update complete");
      Serial.flush();
      ESP.restart();
    }
  }
}
```

Esta función realiza no solo la actualización de la memoria flash, que es donde se almacena el código del microcontrolador, sino que también realiza la actualización del sistema de archivos si es que el nombre del archivo es `spiffs.bin`

Una vez realizado para poder subir y actualizar nuestro firmware o nuestro spiffs, tenemos que realizar el build de nuestro código o de nuestro file system y que se genere el archivo binario `.bin`, este es el que mandaremos en nuestra petición
